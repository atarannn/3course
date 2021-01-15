//
//  main.cpp
//  lab1_3ReadersAndWriters
//
//  Created by Anastasia Taran on 17.11.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#include <iostream>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
using namespace std;
#define COUNT_OF_READERS 5 //количество читателей
#define COUNT_OF_WRITERS 2 //количество писателей
unsigned int notes = 0;
unsigned int readCount = 0;

bool flag = false;
sem_t doorForReader, doorForWriter, semReader, semWriter;

//читатель
void *reader(void *prm)
{
    int idReader = *(int*)prm; //номер читателя
    
    while(true) //бесконечный цикл
    {

         if(notes == 0 || flag) continue; // если флаг установлен происходит переход но новую итерацию цикла

        //вход
        sem_wait(&doorForReader); //читатель приходит, блокируем семафор
            readCount++; //увеличиваем кол-во читателей
            
            if(readCount == 1) //если ко-во читателей = 1 (то бишь он пришел первым)
                sem_wait(&semWriter); // блокировка для того, чтобы писатели не зашли (читатель и писатель одновременно быть не могут)
                //следующие читатели просто увеличивают readCount
        
        sem_post(&doorForReader); //разблокировали семафор

        //вывод 
        printf("Читач ID = %d читає. readCount = %d\n", idReader, readCount);
        sleep(1 + rand()%4); //пауза
        printf("Читач ID = %d закінчив читати.\n", idReader);

        //выход
        sem_wait(&doorForReader); //блокируем семафор
            readCount--; //уменьшаем кол-во читателей
        
            if(readCount == 0) //если же кол-во читателей = 0
                sem_post(&semWriter); // разблокировали семафор(писатель может зайти)
        
        sem_post(&doorForReader); //разблокировали семафор
        
        sleep(1); //пауза
    }
}


//писатель
void *writer(void *prm)
{
    int idWriter = *(int*)prm; //номер писателя
    
    while(true) //бесконечный цикл
    {
        sem_wait(&doorForWriter); // блокировка семафора
            flag = true; // новые читатели не зайдут, если флаг установлен, это флаг приоритетности писателя
        sem_post(&doorForWriter); //разблокировали семафор
   
        sem_wait(&semWriter); //блокируем семафор

            sem_wait(&doorForWriter);
                flag = false; // флаг скинут
            sem_post(&doorForWriter);
        
            //вывод
            printf("\tПисьменник ID = %d пише.\n", idWriter);
            notes++; //добавили запись
            sleep(1 + rand()%4); //пауза
            printf("\tПисьменник ID = %d закінчив писати. Кількість нотаток: %d\n", idWriter, notes);

        sem_post(&semWriter); //разбокировали семафор

        sleep(1); //пауза
    }
}

int main()
{
    pthread_t ThreadOfReaders[COUNT_OF_READERS];
    pthread_t ThreadOfWriters[COUNT_OF_WRITERS];
    
    //инициализируем семафоры
    sem_init(&doorForReader, 0, 1);
    sem_init(&doorForWriter, 0, 1);
    sem_init(&semReader, COUNT_OF_READERS, COUNT_OF_READERS);
    sem_init(&semWriter, 0, 1);
    
    //создаем потоки читателей
    for (int i = 0; i < COUNT_OF_READERS; i++){
        pthread_create(&(ThreadOfReaders[i]), NULL, reader, (void*)&i);
        usleep(50000); //приостановка работы потока
    }
    
    //создаем потоки писателей
    for (int i = 0; i < COUNT_OF_WRITERS; i++){
        pthread_create(&(ThreadOfWriters[i]), NULL, writer, (void*)&i);
        usleep(50000); //приостановка работы потока
    }

    for (int i = 0; i < COUNT_OF_READERS; i++){
        pthread_join(ThreadOfReaders[i], NULL); //блокировка вызывающего потока, до момента пока указанный поток не завершится
    }
    
    for (int i = 0; i < COUNT_OF_WRITERS; i++){
        pthread_join(ThreadOfWriters[i], NULL);//блокировка вызывающего потока, до момента пока указанный поток не завершится
    }
    
    //удаление семафоров
    sem_destroy(&doorForReader);
    sem_destroy(&doorForWriter);
    sem_destroy(&semReader);
    sem_destroy(&semWriter);
    
    return 0;
}
