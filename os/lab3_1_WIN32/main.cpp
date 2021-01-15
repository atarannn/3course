//
//  main.cpp
//  lab3_1_WIN32
//
//  Created by Anastasia Taran on 23.12.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#ifndef UNICODE
#define UNICODE
#define UNICODE_WAS_UNDEFINED
#endif
#include <iostream>
#include <deque>
#include <map>
#include <string>
#include <comdef.h>
#include <comutil.h>
#include <windows.h>
#include <strsafe.h>
#include <stdio.h>
#include <cmath>

unsigned getHistogramInterval(unsigned long long fileSize, unsigned long step) { //интервал гистограммы
    return fileSize / step;
}

ULONGLONG dwordToUnsigneLongLong(DWORD high, DWORD low) {  //конвертации разных типов строк
    return static_cast<__int64>(high) << 32 | low;
}

std::wstring StringToWString(const std::string& s) {
    std::wstring temp(s.length(), L' ');
    std::copy(s.begin(), s.end(), temp.begin());
    return temp;
}

std::string WStringToString(const std::wstring& s) {
    std::string temp(s.length(), ' ');
    std::copy(s.begin(), s.end(), temp.begin());
    return temp;
}

class DirectoryTraversation { // перемещение по каталогам
private:
    unsigned long step; // шаг
    std::map<unsigned, unsigned long> histMap; // структура <Размер файла, Количество файлов> (Другими словами хеш-таблица или словарь)
    std::string rootDir;


    void addFileInfo(WIN32_FIND_DATAA& data) { // добавление информации о файле
        auto fileSize = dwordToUnsigneLongLong(data.nFileSizeHigh, data.nFileSizeLow); // размер файла
        auto interval = getHistogramInterval(fileSize, step); // интервал
        auto val = histMap.find(interval); //Ищем размер файла в словаре
        if (val == histMap.end()) {
            histMap.insert({ interval, 1 });
        }
        else { //записываем или добавляем
            val->second += 1;
        }
    }

    void searchDirectory(std::string dir) { //проходим по директории
        std::string directoryPath = dir;
        directoryPath += "\\*"; // путь директории
        const CHAR* directory = directoryPath.c_str();
        CHAR* curDir = new CHAR[MAX_PATH]{ '0' };
        StringCchCopy((STRSAFE_LPWSTR)curDir, MAX_PATH, (STRSAFE_LPWSTR)directory);

        WIN32_FIND_DATAA file_data;
        HANDLE h_file = FindFirstFileA(curDir, &file_data); // ищем в директории файл, чье имя совпадает с указанным

        if (INVALID_HANDLE_VALUE == h_file) { // в случае неудачи
            std::cout << "ERROR OCCURED WHEN USEING FINDFIRSTFILEA" << std::endl;
            return;
        }

        do {
            if (file_data.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) { // проверка на то, папка ли это
                _bstr_t fName(file_data.cFileName);
                char* fileName = fName;

                if (strcmp(fileName, ".") && strcmp(fileName, "..")) { // Игонрируем папки . и ..
                    std::string next = dir + '\\' + fileName;  // создаем путь к папке
                    searchDirectory(next); //рекурсивно заходим в нее
                }
            }
            else {
                addFileInfo(file_data); // получаем информацию о файле
            }

        } while (FindNextFileA(h_file, &file_data) != 0); // продолжаем поиск файла
    }
public:
    DirectoryTraversation() {
        step = 1024 * 1024;
        rootDir = "C:\\ironpython-stubs-master";
    }
    DirectoryTraversation(std::string rootDir, unsigned long step) {
        this->rootDir = rootDir;
        this->step = step;
    }
    void run() {
        searchDirectory(rootDir);
    }
    std::map<unsigned, unsigned long> getMap() {
        return histMap;
    }
};


int main() {
    std::string rootDir = "C:\\ironpython-stubs-master";
    unsigned long step = 1024; //размер шага
    DirectoryTraversation dt = DirectoryTraversation(rootDir, step); //Получаем директорию
    dt.run();
    std::map<unsigned, unsigned long> histMap = dt.getMap(); //проходимся по ней и записываем все в словарь

    //ограничитель на длину
    const int diagLen = 80;

    unsigned long maxFilesCountInInterval = 0;
    for (auto& interval : histMap) { //находим интервал с максимальным количеством файлов
        if (interval.second > maxFilesCountInInterval) {
            maxFilesCountInInterval = interval.second;
        }
    }

    double scale = maxFilesCountInInterval > diagLen ? (double)diagLen / (double)maxFilesCountInInterval : 1.0;
    std::cout << "TABLE: " << std::endl; //печатаем
    for (auto& interval : histMap) {
        auto intSymb = std::ceil(interval.second * scale); // округляем до целого
        printf("[%d-%d]->%s\n", interval.first, interval.first + 1, std::string(intSymb, '#').c_str()); //выводим диапазон размеров файлов, который покрывает наша группа
    }
    system("pause");
    return 0;
}
