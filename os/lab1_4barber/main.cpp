#include<cstdio>
#include<cstdlib>
#include<unistd.h>
#include<pthread.h>
#include<cerrno>
#include<sys/ipc.h>
#include <dispatch/dispatch.h>
#include <iostream>

#define N 6 // клиенты
#define M 5
using namespace std;

int counter = 0; //  Количество клиентов, ожидающих стрижки

pthread_cond_t barber_awake, client_wait, client_got_haircut;
pthread_mutex_t mp, output;

// дискриптор потока
pthread_t barber_thread;
pthread_t customer_thread[N];

[[noreturn]] void *barber(void *arg);
[[noreturn]] void *customer(void *arg);

int main(int argc, char *argv[]) {
    /* инициализация мьютекса. значением по умолчанию */
    pthread_mutex_init(&mp, nullptr);
    pthread_mutex_init(&output, nullptr);

    // инициализация условных переменных
    pthread_cond_init(&barber_awake, nullptr);
    pthread_cond_init(&client_wait, nullptr);
    pthread_cond_init(&client_got_haircut, nullptr);

    // Создание потока Barber_thread
    pthread_create(&barber_thread, nullptr, barber, &counter);
    
    // Создание потока Customer_thread
    for (int i = 0; i < N; i++) {
        pthread_create(&customer_thread[i], nullptr, customer, &counter);
        sleep(1); // задержка
    }

    pthread_join(barber_thread, nullptr); // ожидание завершения потока
    for (pthread_t i : customer_thread) {
        pthread_join(i, nullptr);
    }
    exit(0);
}

/*Barber*/
[[noreturn]] void *barber(void *arg)
{
    for (;;) {
        pthread_mutex_lock(&output); // блокировка мьютекса (захват мьютекса)
        if (counter == 0) {  // если очередь пустая
            printf("Barber fall asleep\n"); // барбер засыпает
            pthread_mutex_unlock(&output); // разблокировали мьютекс (освобождение мьютекса)
            
            pthread_mutex_lock(&mp);
            pthread_cond_wait(&barber_awake, &mp); //блокировка потока
            pthread_mutex_unlock(&mp);
        }

        pthread_cond_signal(&client_wait); //пробуждение заблокированого потока
        pthread_cond_signal(&client_got_haircut);

        counter--;
        printf("Barber:cut hair, count is:%d.\n", counter);
        pthread_mutex_unlock(&output); // разблокировали мьютекс

        sleep(rand() % 3 + 2); // задержка
    }
}

/*Customers*/
[[noreturn]] void *customer(void *arg)
{
    for (;;) {
        pthread_mutex_lock(&output); // блокируем мьютекс
        if (counter < M) { // если места есть
            if (counter == 0) { // если клиент первый -> будим барбера
                pthread_cond_signal(&barber_awake); // пробуждение потока
            }
            counter++;
            printf("Customer:add count,count is:%d\n", counter);
            pthread_mutex_unlock(&output);
            
            pthread_mutex_lock(&mp);
            pthread_cond_wait(&client_wait, &mp); // блокировка потока
            pthread_mutex_unlock(&mp);
            
            pthread_mutex_lock(&mp);
            pthread_cond_wait(&client_got_haircut, &mp); // блокируем поток
            pthread_mutex_unlock(&mp);
        }
        else { // если же мест нет
            pthread_mutex_lock(&output);
            printf("Customer is leaving\n");
            pthread_mutex_unlock(&output);
        }
        sleep(rand() % 3 + 1); // задержка
    }
}
