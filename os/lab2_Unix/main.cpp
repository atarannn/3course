//
//  main.cpp
//  lab2
//
//  Created by Anastasia Taran on 01.12.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#include <errno.h> // номера системных ошибок
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <sys/types.h> // различные типы данных
#include <sys/sysctl.h> // для MacOS
#include <pwd.h> // назначение структуры типа passwd, содержащую разложенную на поля строку файла паролей
#include <sys/param.h> // системные параметры
#include <sys/mount.h> // файловая система
#include <sys/vmmeter.h> // системная широкая статистическая информация
#include <libproc.h> // интерфейс для взятия информации о процессах
#include <mach/vm_statistics.h>
#include <mach/mach_types.h>
#include <mach/mach_init.h>
#include <mach/mach_host.h>
#include <mach/mach.h>
#include <mach/mach_error.h>
#include <mach/vm_map.h>
#include <unistd.h> // различные основные функции и константы POSIX

using namespace std;
static unsigned long long TotalTicks = 0; // тики
static unsigned long long IdleTicks = 0; // тики бездействия

float CalcCPULoad(unsigned long long idleTicks, unsigned long long totalTicks) // передаем в функцию текущие тики и текущие тики бездейтвия
{
    unsigned long long totalTicksSinceLastTime = totalTicks-TotalTicks; // отнимает от них предидущие для нахождения разницы
    unsigned long long idleTicksSinceLastTime  = idleTicks-IdleTicks;
    float ret = 1.0f-((totalTicksSinceLastTime > 0) ? ((float)idleTicksSinceLastTime)/totalTicksSinceLastTime : 0); // если разница больше нуля то находим процент загрузки
    TotalTicks = totalTicks; // записываем текущие как предыдущие
    IdleTicks  = idleTicks; //записываем текущие как предыдущие (id)
    return ret;
}

float GetCPULoad()
{
    host_cpu_load_info_data_t cpuinfo;  // переменную в которою будет возвращен результат
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT; // присваеваем переменной значение константы отвечающей за возврат загрузки цпу
    if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuinfo, &count) == KERN_SUCCESS)  // функция host_statistics записывает в переменную cpuinfo запрошенную статистику хоста, статистику по тикам процессора
    {   // если результат получен успешно -- заходим в иф
        unsigned long long totalTicks = 0;
        for(int i=0; i<CPU_STATE_MAX; i++) totalTicks += cpuinfo.cpu_ticks[i];  // проходим по всем состояниям ЦПУ и складываем их чтоб получить общее количество тиков
        // состояний соществоет много, не только "простой" и "работа", их количество храниться в константе  CPU_STATE_MAX
        return CalcCPULoad(cpuinfo.cpu_ticks[CPU_STATE_SYSTEM], totalTicks);  // вызываем нашу функцию и везвращаем ее результат
    }
    else return -1.0f; // если не удалось получить инфу возвращаем -1, что укажет на ошибку
}

size_t getMemorySize()
{
    int i[2];
    i[0] = CTL_HW;
    i[1] = HW_MEMSIZE; // размер памяти
    
    int64_t size = 0; // int64_t - знаковый целочисленный тип с 64 битами
    size_t len = sizeof( size ); // size_t -  базовый беззнаковый целочисленный тип
    if (sysctl(i, 2, &size, &len, NULL, 0) == 0)
        return (size_t)size; // sysctl -  утилита, предназначенная для управления параметрами ядра на лету. Позволяет читать и изменять параметры ядра.
    return 0L;
}

int main(void) {
    int err = 0; // error
    struct kinfo_proc *process_list = NULL; // структура, которая содержит информации о процессе, включая идентификатор процесса и имя процесса
    size_t length = 0; // длинна proc_list
    static const int name[] = { CTL_KERN, KERN_PROC, KERN_PROC_ALL, 0 }; // массив, который содержит высокие ограничения ядра, выбранную информацию о конкретных запущенных процессах, выбранную информацию обо всех запущенных процессах и null
    
    // Вызов sysctl с буфером NULL, чтобы получить правильную длину
    err = sysctl((int *)name, (sizeof(name) / sizeof(*name)) - 1, NULL, &length, NULL, 0);
    if (err) // если ошибка
    {
        perror(NULL); // вывод сообщения ошибки
        free(process_list); // очистить нашу структуру proc_list
        return EXIT_FAILURE; // неудачное выполнение программы
    }
    
    // Выделение буфер
    process_list = (kinfo_proc *)malloc(length*sizeof(kinfo_proc));
    if (!process_list) // если выделение не удалось
    {
        perror(NULL); // вывод сообщения ошибки
        free(process_list); // очищаем нашу структуру proc_list
        return EXIT_FAILURE; // неудачное выполнение программы
    }
    
    // Получаем актуальный список процессов
    err = sysctl((int *)name, (sizeof(name) / sizeof(*name)) - 1, process_list, &length, NULL, 0);
    if (err) // если  не удалось
    {
        perror(NULL); // выводим сообщение ошибки
        free(process_list); // очищаем нашу структуру proc_list
        return EXIT_FAILURE; // неудачное выполнение программы
    }
    
    int process_count = int(length / sizeof(struct kinfo_proc)); // количество процессов
    
        for (int i = 0; i < process_count; i++)
        { // цикл по запущенным процессам
            uid_t uid = process_list[i].kp_eproc.e_ucred.cr_uid; // личность владельца (пользователя) текущего процесса
            struct passwd *user = getpwuid(uid); // информация о пользователе по его личности
            const char *username = user ? user->pw_name : "user name not found"; // имя пользователя не найдено
            uid_t user_uid = user ? user->pw_uid : -100; // id пользователя
            
            cout << "Process ID: \t" << process_list[i].kp_proc.p_pid << endl; // id процесса
            cout << "Process name: \t" << process_list[i].kp_proc.p_comm << endl; // имя процесса
            cout << "Username: \t" << username << endl; // имя пользователя
            cout << "User id: \t" << user_uid << endl; // id пользователя
            cout << "Parent Process ID: \t" << process_list[i].kp_eproc.e_ppid << endl; // id  родителя процесса
            cout << "Process status: \t" << int(process_list[i].kp_proc.p_stat) << endl; // статус процесса
            cout << "Process priority: \t" << int(process_list[i].kp_proc.p_priority) << endl; // приоритет процесса
            cout << "CPU usage: \t" << int(process_list[i].kp_proc.p_cpticks) << endl; // такты времени центрального процессора
            cout << "RAM usage: \t" << int(process_list[i].kp_eproc.e_xsize) << endl; // такты использования оперативной памяти
            cout << "CPU: \t" << int(process_list[i].kp_proc.p_estcpu) << endl; // cpu
            cout << "ID process group: \t" << process_list[i].kp_eproc.e_pgid << endl; // id группы процессов
            cout << endl;
    }
    
    free(process_list); // очищаем нашу структуру proc_list
    cout << endl << "Physical memory: " << getMemorySize()/1048576 << " Mb" << endl; // физическая память ноута
    
    int i1[2]; // информация о машине
    i1[0] = CTL_HW;
    i1[1] = HW_NCPU; // количество процессоров
    int cpu_numbers;
    size_t len1 = sizeof(cpu_numbers);
    if (sysctl(i1, 2, &cpu_numbers, &len1, NULL, 0) == 0)
        cout << "CPU number: " << cpu_numbers << endl; // номер процессора
    
    int i2[2];
    i2[0] = CTL_HW;
    i2[1] = HW_PAGESIZE; // размер страницы
    int page_size;
    size_t len2 = sizeof(page_size);
    if (sysctl(i2, 2, &page_size, &len2, NULL, 0) == 0)
        cout << "Software page size: " << page_size << endl; // размер страницы программного обеспечения
    
    int i3[2];
    i3[0] = CTL_KERN;
    i3[1] = KERN_CLOCKRATE; // частоты
    clockinfo smth;
    size_t len3 = sizeof(smth);
    if (sysctl(i3, 2, &smth, &len3, NULL, 0) == 0)
        cout << "Periodicity: " << smth.hz << endl; // частоты ядра
    
    int i4[2];
    i4[0] = CTL_KERN;
    i4[1] = KERN_MAXPROC; // максимальное количество одновременных процессов, разрешенных системой
    int maxproc;
    size_t len4 = sizeof(maxproc);
    if (sysctl(i4, 2, &maxproc, &len4, NULL, 0) == 0)
        cout << "MAX Processes number: " << maxproc << endl; // максимальное кол-во процессов
    
    int i5[2];
    i5[0] = CTL_KERN;
    i5[1] = KERN_OSTYPE; // строка системного типа
    string OSType;
    size_t len5 = sizeof(OSType);
    if (sysctl(i5, 2, &OSType, &len5, NULL, 0) == 0)
        cout << "OS Type: " << OSType << endl; //тип операционной системы
    
    vm_size_t page_size_1;
    mach_port_t mach_port;
    mach_msg_type_number_t count;
    vm_statistics64_data_t vm_stats;
    
    mach_port = mach_host_self(); // текущий хост компа
    count = sizeof(vm_stats) / sizeof(natural_t);
    if (KERN_SUCCESS == host_page_size(mach_port, &page_size_1) &&
        KERN_SUCCESS == host_statistics64(mach_port, HOST_VM_INFO,
                                          (host_info64_t)&vm_stats, &count))
        // функция получение размера страницы памяти и функция для получения всех размеров разных памятей
    {
        long long free_memory = (int64_t)vm_stats.free_count * (int64_t)page_size; // свободная память
        long long used_memory = ((int64_t)vm_stats.active_count +
                                 (int64_t)vm_stats.inactive_count +
                                 (int64_t)vm_stats.wire_count) *  (int64_t)page_size; // занятая память
        cout<< "Free memory: " << free_memory << endl;
        cout<< "Used memory: " << used_memory << endl;
    }
    cout<< "CPU load: " << GetCPULoad() << endl;
    return EXIT_SUCCESS;
}

