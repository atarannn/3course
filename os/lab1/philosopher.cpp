//
//  main.cpp
//  lab1_2philosophers
//
//  Created by Anastasia Taran on 13.11.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#include <windows.h>
#include <iostream>
using namespace std;
#define NUMBER_OF_P 7   //кол-во философов

HANDLE Threads[NUMBER_OF_P]; //дескриптор потока
CRITICAL_SECTION cs[NUMBER_OF_P];
CRITICAL_SECTION print;

//функция потока
DWORD WINAPI test(LPVOID lpParam) {

    while (true) { //бесконечный цикл
        
        int left = ((int) lpParam); //соответствует левой вилке философа
        int right = ((int) lpParam + 1) % NUMBER_OF_P; //соответствует правой вилке философа

        Sleep(1000); //пауза
        
        if (TryEnterCriticalSection(&cs[left]))  //проверка КС на занятость
            //если занято то следующая часть кода пропускается и мы ждем, пока КС освободится и только тогда, пойдем дальше
        {
            if (TryEnterCriticalSection(&cs[right])) //опять проверка на занятость уже правой вилки
            {

                EnterCriticalSection(&print); //запрос на использование в КС, вход в КС
                cout << "PhiloАкsopher " << left << " is eating" << endl; //вывод того, что философ есть
                //lest - из-за того, что номер философа = номеру его левой вилки
                LeaveCriticalSection(&print); //освобождение КС, выход с КС
               
                Sleep(1000); //пауза

                EnterCriticalSection(&print);
                cout << "Philosopher " << left << " ended" << endl; //выводим, что философ закончил есть
                LeaveCriticalSection(&print);

                LeaveCriticalSection(&cs[right]); //выход с КС, которая отвечает правой вилке
                //освобождаем ее, что-бы другой поток мог использовать ее
            }
            LeaveCriticalSection(&cs[left]); //выход с КС, которая отвечает левой вилке
        }
    }
    return 0;
}

//главная функция
int main() {

    InitializeCriticalSection(&print); //инициализация критической секции
    for (int i = 0; i < NUMBER_OF_P; i++) {
        InitializeCriticalSection(&cs[i]);
    }
    
    //создаем потоки философов
    for (int i = 0; i < NUMBER_OF_P; i++) {
        //Sleep(700);
        Threads[i] = CreateThread(nullptr, 0, &test, (LPVOID) i, 0, nullptr); //создание потоков
    }
    
    WaitForMultipleObjects(7, Threads, TRUE, INFINITE); //поток ждет освобождения или нескольких объектов синхронизации, или одного из них.

}
