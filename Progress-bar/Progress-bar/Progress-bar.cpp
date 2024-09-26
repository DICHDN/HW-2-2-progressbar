/*
    Задание 2
    Прогресс-бар
    Создайте консольное приложение для имитации многопоточного расчёта.

    Количество потоков, длина расчёта должны быть заданы переменными.
    В консоль во время работы программы должны построчно для каждого потока выводиться:
    номер потока по порядку;
    идентификатор потока;
    заполняющийся индикатор наподобие прогресс-бара, визуализирующий процесс «расчёта»;
    после завершения работы каждого потока в соответствующей строке суммарное время, затраченное на работу потока.
    Строки прогресс-баров каждого потока должны выводиться одновремено. 
    Время появления каждого нового символа в строке прогресс-бара подберите так, чтобы процесс заполнения строки был виден. 

    Дополнение к заданию 2*
    Во время очередной итерации «расчёта» сымитируйте со случайной вероятностью возникновение ошибки (exception), которая не должна приводить к прекращению работы потока или программы.
    При этом этот факт должен визуализироваться отдельным цветом на прогресс-баре.

*/


#include<chrono>
#include<thread>
#include<execution>
#include <iostream>
#include <windows.h>
#include<vector>
#include<mutex>

std::mutex m;


void drawProgressBar(int x, int y) 
{
    size_t hash = std::hash<std::thread::id>{}(std::this_thread::get_id());
    int u=0;
    bool flag = true;
    int k = 50; // количество символов в прогрессбаре
    for (int i = 0; i < k; ++i)
    {
        if (flag)
        {
            //const int barWidth = i;
            // блок для вывода id потока под своими параметрами вывода
            m.lock();
            COORD id_thred;
            id_thred.X = x-19;
            id_thred.Y = y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), id_thred);
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 15);
            std::cout <<"  ID THREAD: " << std::this_thread::get_id();
            // блок рисования прогресса
            COORD bar;
            bar.X = x;
            bar.Y = y;
            SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), bar);            
            SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), BACKGROUND_GREEN);

            int pos = (i * i) / 100; // условный прогресс до 100%            
            for (int j = 0; j < i; ++j)
            {
                if (flag)
                {
                    std::cout << " ";
                    
                    if (u > 895) // условие иметации ошибки
                    {
                        bar.X =x+i+1;
                        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), bar);
                        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 4);
                        std::cout << "ERROR!!!";
                        flag = false;
                    }
                }
                if (j == k-2)
                {
                    bar.X = x + i + 1;
                    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), bar);
                    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 2);
                    std::cout << "COMPLITE!!!";
                }
            }
        }
        m.unlock();
        u = (rand() *hash) % 900;
        Sleep(u);
        if (!flag) break;
    }
}

int main() {
    std::vector<std::thread> threads;
    int k = 5;
    for (int i = 0; i < k; i++)
    {
        threads.push_back(std::thread(drawProgressBar, 19, 1+i));
    };
    for (auto& t : threads)
    {
        t.join();
    };        
    COORD bar;
    bar.X = 0;
    bar.Y = k+2;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), bar);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),15);
    return 0;
}

