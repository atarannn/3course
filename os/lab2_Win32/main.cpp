//
//  main.cpp
//  лаб_2_Win32_Таран
//
//  Created by Anastasia Taran on 07.12.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#include <windows.h> // содержит множество определений типов данных, макросов, прототипов функций, констант и т. д.
#include <stdio.h> // стандратная библиотека ввода/вывода
#include <tchar.h>
#include <psapi.h> // стандартная WinAPI-библиотека
#include <iostream> // iostream — заголовочный файл с классами, функциями и переменными для организации ввода-вывода
#include <pdh.h>
#pragma comment(lib,"pdh.lib") // директива pragma заставляет компоновщик искать в исходном файле библиотеку pdh.lib во время компоновки
#define DIV 1024 // Используем для преобразования байтов в МБ
#define WIDTH 7 // ширина поля для печати чисел
using namespace std; // содержит стандартные функции и переменные C++

class ProcInfo {
public:
    TCHAR processName[MAX_PATH] = TEXT("<unknown>");
    DWORD pid;
    //  FILETIME - 64-битовое значение, представляющее число интервалов по 100 наносекунд с 1 января 1601 (универсальное глобальное время (UTC)
    FILETIME ftKernelStart, // запуск ядра
        ftUserStart, // запуск пользователя
        ftSysKernelStart, // системный запуск ядра
        ftSysUserStart, // системный запуск пользователя
        ftKernelEnd, // конец ядра
        ftUserEnd, // конец пользователя
        ftSysKernelEnd, // системное окончание ядра
        ftSysUserEnd; // системное окончание пользователя
    PROCESS_MEMORY_COUNTERS pmc;

    void printProcInfo() { //вывод информации о процессах
        cout<<endl << " " << pid << "\t"; //id  процесса
        cout <<getCpuUsage()<<" %    \t\t" ; // использование ЦП
        cout << pmc.PeakWorkingSetSize / (DIV * DIV) << " mb \t\t\t"; // максимальный размер рабочего места
        cout << pmc.WorkingSetSize / (DIV * DIV) << " mb\t\t"; // размер рабочего места, который используется
        cout << pmc.PagefileUsage / (DIV * DIV) << " mb \t\t"; // использование файла подкачки
        cout << pmc.PeakPagefileUsage / (DIV * DIV) << " mb \t\t"; // максимальное использование файла подкачки
        wprintf(processName); // имя процесса
    }
    static void printProcHead() { // вывод "шапки" под которой будет выводиться информация о процессах
        cout << " pid | cpu usage | peak working set size |";
        cout << " working set size | page file usage | peak page file usage | name" << endl;
    }

private:
    int getCpuUsage() { // использование cpu
        ULONGLONG sysTime = ((convert(ftSysKernelEnd) + convert(ftSysUserEnd))
            - (convert(ftSysKernelStart) + convert(ftSysUserStart)));
        ULONGLONG procTime = convert(ftKernelEnd) - convert(ftKernelStart) + convert(ftUserEnd) - convert(ftUserStart);
        return  ((200 * procTime) / sysTime);
    }
    ULONGLONG convert(FILETIME ft) {
        return ((((ULONGLONG)ft.dwHighDateTime) << 32) + ft.dwLowDateTime);
    }

};
void printMemoryInfo() { // функция вывода информация о памяти
    MEMORYSTATUSEX statex; // структура
    statex.dwLength = sizeof(statex); // длина структуры
    GlobalMemoryStatusEx(&statex); // заполняем структуру информацией о физической памяти
    cout << "Memory in use: " << statex.dwMemoryLoad << "%" << endl; // память в использовании
    cout << "Total memory: " << statex.ullTotalPhys / (DIV * DIV) << "Mb" << endl; // вся память
    cout << "Available memory: " << statex.ullAvailPhys / (DIV * DIV) << "Mb" << endl; // доступная память
    cout << "Total paging file: " << statex.ullTotalPageFile / (DIV * DIV) << "Mb" << endl;// файл подкачки
    cout << "Available paging file: " << statex.ullAvailExtendedVirtual / (DIV * DIV) << "Mb" << endl; // доступный файл подкачки
}

void ProcessInfo(DWORD processID) // функция получения информация о процессах
{
    ProcInfo proc_info;
    proc_info.pid = processID;
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, processID);
    FILETIME ftCreation,
        ftExit,
        ftSysIdle;
    
    // если не получили информацию об использовании памяти процессом, то
    if (!GetProcessMemoryInfo(hProcess, &proc_info.pmc, sizeof(proc_info.pmc))) {
        proc_info.pmc.PeakWorkingSetSize = 0;
        proc_info.pmc.WorkingSetSize = 0;
        proc_info.pmc.PagefileUsage = 0;
        proc_info.pmc.PeakPagefileUsage = 0;
    }
    
    // если получили информацию о выполнении процессов, то
    if (GetProcessTimes(hProcess, &ftCreation, &ftExit, &proc_info.ftKernelStart, &proc_info.ftUserStart)) {
        // GetSystemTimes - функция возвращает указатели на 3 значения, время простоя CPU, время нахождения CPU в режиме kernel и время нахождения CPU в пользовательском режиме
        GetSystemTimes(&ftSysIdle, &proc_info.ftSysKernelStart, &proc_info.ftSysUserStart);
        // задержка
        Sleep(2000);
        //получение информации о выполнении процессов
        GetProcessTimes(hProcess, &ftCreation, &ftExit, &proc_info.ftKernelEnd, &proc_info.ftUserEnd);
        // опять вызываем функцию GetSystemTimes
        GetSystemTimes(&ftSysIdle, &proc_info.ftSysKernelEnd, &proc_info.ftSysUserEnd);
        HMODULE hMod;
        DWORD cbNeeded;
        EnumProcessModules(hProcess, &hMod, sizeof(hMod),
            &cbNeeded); // перечисляем все модули процесса
        GetModuleBaseName(hProcess, hMod, proc_info.processName,
            sizeof(proc_info.processName) / sizeof(TCHAR)); // возвращаем имя модуля
        proc_info.printProcInfo();
    }
    CloseHandle(hProcess); // уничтожаем дескриптор
}

int main(void)
{
    DWORD processes_arr[1024], processes_bytes_num, num_processes;
    unsigned int i;
    EnumProcesses(processes_arr, sizeof(processes_arr), &processes_bytes_num); // The EnumProcesses function retrieves the process identifier for each process object in the system.
    num_processes = processes_bytes_num / sizeof(DWORD);
    printMemoryInfo(); // выводим информацию о памяти
    ProcInfo::printProcHead(); // выводим шапку и процессы
    for (i = 0; i < num_processes; i++){
        ProcessInfo(processes_arr[i]);
    }
    return 0;
}
