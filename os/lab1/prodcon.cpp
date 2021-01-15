#include <stdio.h>
#include <Windows.h>
#include <time.h>
#include <queue>
#include <iostream>

#define N 4 //производитель
#define M 5 //потребитель
using namespace std;

int maxs = 5;
std::queue<int> buffer; //очередь
// переменая события HANDLE
HANDLE hEvent1;
HANDLE hEvent2;
HANDLE hEvent_buff_full;
HANDLE hEvent_buff_empty;
HANDLE hEvent_cout;
HANDLE pThread[N]; //поток производитель
HANDLE cThread[M]; //поток потребитель

//tread 1 // производитель
DWORD Producer(LPVOID lpdwThreadParam)
{
    int pn = *(int*)lpdwThreadParam;
    
    for (;;) // бесконечный цикл
    {
        WaitForSingleObject(hEvent1, INFINITE);  // ждем пока hEvent1 не окажется в сигнальном состоянии
        
        if (buffer.size() >= maxs) { // если буфер полный

            WaitForSingleObject(hEvent_cout, INFINITE); // ждем уст. в  сигнальное состояние
            cout << "Queue is full" << endl; // выводим, что очередь полная
            SetEvent(hEvent_cout); // устанавливаем указанный объект события в сигнальное состояние
            
            WaitForSingleObject(hEvent_buff_full, INFINITE); // ждет пока потребитель заберет элемент
        }
        else {  // доступ к буферу
        
            buffer.push(pn);
            
            SetEvent(hEvent_buff_empty);// устанавливает ивент пустой буфер в сигнальное состояние
            
            WaitForSingleObject(hEvent_cout, INFINITE); // ждем уст. в  сигнальное состояние
            cout << "Queue size is " << buffer.size() << endl;
            SetEvent(hEvent_cout); // устанавливаем указанный объект события в сигнальное состояние
        }
        
        SetEvent(hEvent1); // пускаем следующего производителя
        
        Sleep(5000 + rand() % 5000); // задержка
    }
    return 0;
}

//thread 2 // потребитель
DWORD Consumer(LPVOID lpdwThreadParam)
{
    int cn = *(int*)lpdwThreadParam;
    
    for (;;)
    {
        WaitForSingleObject(hEvent2, INFINITE); // ждем устанавление события потребитель в сигнальное состояние
        
        // если очередь пустая
        if (buffer.empty()) {
            
            WaitForSingleObject(hEvent_cout, INFINITE); // ждем уст. в  сигнальное состояние
            cout << "Queue is empty"<< endl;
            SetEvent(hEvent_cout); // устанавливаем указанный объект события в сигнальное состояние
            
            WaitForSingleObject(hEvent_buff_empty, INFINITE); // ждет пока производитель добавит элемент
        }
        else {
            buffer.pop();
            
            WaitForSingleObject(hEvent_cout, INFINITE); // ждем уст. в  сигнальное состояние
            cout << "Q size is " << buffer.size() << endl;
            SetEvent(hEvent_cout); // устанавливаем указанный объект события в сигнальное состояние
            
            SetEvent(hEvent_buff_full);// устанавливает ивент  полный буфер в сигнальное состояни
        }
        
        SetEvent(hEvent2); // пускаем следующего потребителя
        
        Sleep(rand() % 500 + 100); // блокировка потока
    }
    
    return 0;
}

int createthreads() {
    
    srand(time(NULL));
    // создаем обьекты события
    /* событие создаётся в сигнальном состоянии , с автоматическим сбросом*/
    hEvent1 = CreateEvent(NULL, FALSE, TRUE, NULL);
    hEvent2 = CreateEvent(NULL, FALSE, TRUE, NULL);
    hEvent_buff_full = CreateEvent(NULL, FALSE, TRUE, NULL);
    hEvent_buff_empty = CreateEvent(NULL, FALSE, TRUE, NULL);
    hEvent_cout = CreateEvent(NULL, FALSE, TRUE, NULL);
    
    for (int i = 0; i < N; ++i) {
        int* pn = new int(i);
        pThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Producer, pn, 0, NULL);
    }
    for (int i = 0; i < M; ++i) {
        int* cn = new int(i);
        cThread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Consumer, cn, 0, NULL);
    }
    return 0;
}
int main() {
    createthreads();// создаем потоки производителей и потребителей
    SetEvent(hEvent1);// устанавливаем событие производителей в сигнальное состояние
    getchar();
    }
