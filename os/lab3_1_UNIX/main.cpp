//
//  main.cpp
//  lab3_1_UNIX
//
//  Created by Anastasia Taran on 11.12.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <string>
#include <limits.h>
#include <vector>
#include <map>
#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h> // заголовок в библиотеке  POSIX для языка программирования Cи, который содержит конструкции, облегчающие перемещение по каталогам
#include <unistd.h> // имя файла заголовка, обеспечивающего доступ к API операционной системы POSIX
#include <math.h>

int size_step = 1024; //Размер шага по умолчанию
int number_of_stars = 80;  //Максимальное количество звездочек, 80 - стандартная ширина терминального окна
std::map<long, int> statistic;  //Структура <Размер файла, Количество файлов> (Другими словами хеш-таблица или словарь)

void read_directory (std::string dirName) { // Функция дл чтения директории
    DIR* dir = NULL;
    struct dirent entry;
    struct dirent *entryPointer = NULL;
    char pathName[PATH_MAX];

    dir = opendir(dirName.c_str());  //открываем директорию
    if (dir == NULL){
        printf("Error opening %s\n", dirName.c_str()); //Ошибка если не удалось
    }

    readdir_r(dir, &entry, &entryPointer); //Считываем информацию о Файле (папке) в &entry и &entryPointer
    while (entryPointer != NULL) {  //пока рекурсивно не обойдем всю директорию
        
        struct stat entryInfo;

        int compare1 = strncmp(entry.d_name, ".", PATH_MAX);
        int compare2 = strncmp(entry.d_name, "..", PATH_MAX);
        if (compare1 == 0 || compare2 == 0) { // Игонрируем папки . и .. (. - текущая папка, .. - папка на уровень выше)
            readdir_r(dir, &entry, &entryPointer); //Если . или .. - переходим к следующей папке
            continue;
        }
        
        // конструируем путь к файлу
        strncpy(pathName, dirName.c_str(), PATH_MAX); // копируем название директории в pathName
        strncat(pathName, "/", PATH_MAX); // объединяем / и pathName
        strncat(pathName, entry.d_name, PATH_MAX); // объединяем entry.d_name и pathName

        if (lstat(pathName, &entryInfo) == 0) { // Считываем информацию об текущем файле в &entryInfo
            if (S_ISDIR(entryInfo.st_mode)){ // Проверка на то, папка ли это
                std::string dirName(pathName);
                read_directory(dirName);       //Рекурсивно заходим в нее
            }else if (S_ISREG(entryInfo.st_mode)) { //А если это файл
                if (statistic.find((int)entryInfo.st_size / size_step) != statistic.end()) { //если текущий размер файла содержиться внашем словаре
                    statistic[(int)entryInfo.st_size /size_step]++; //добавляем к количеству таких файлов 1
                }else {
                    statistic.insert(std::pair<long, int>((int)entryInfo.st_size / size_step, 1));  // Если не соержиться -- вставляем первый файл такого размера
                }
            }
        }else { //Если не удалось считать инфу -> выводим ошибку
            printf("Error statting %s", pathName);
        }
        // Переходим к следующему файлу (папке), и так пока рекурсивно не обойдем всю директорию
        readdir_r(dir, &entry, &entryPointer);
    }
    closedir(dir); //закрываем директорию
}

int main(int argc, char* argv[]){

    std::string my_directory = "/Users/anastasia_tarannn/Downloads";  //тестовая директория
    std::cout << "Enter size step" << std::endl; // Вводим размер шага
    std::cin >> size_step;  //Размер шага

    read_directory(my_directory); //Рекурсивно читаем директорию и записываем информациб о ней в словарь statistic

    auto x = std::max_element(statistic.begin(), statistic.end(),
                              [](const std::pair<int, int>& p1, const std::pair<int, int>& p2) {
                                  return p1.second < p2.second; });  //Выбираем наибольшую групу файлов в нашем словаре

    std::map<long, int>::iterator at = statistic.begin(); //получаем итератор

    for (; at != statistic.end(); ++at) {  //Итерируемся по итератору (for присваевает at следующий элемент на каждой итерации)
        if (at->second == 0){  //Если файлов текущей группы -- 0, пропускаем ее
            continue;
        }
        printf("%ld - %ld\n", at->first * size_step, (at->first + 1) * size_step); //выводим диапазон размеров файлов, который покрывает наша группа
        printf("["); // Начинаем печать звездочек
        double count_of_stars = double(number_of_stars) * at->second / x->second; //считаем текущие звездочки, с учетом максимума
        count_of_stars = ceil(count_of_stars); //Окургляем до целого
        for (int j = 0; j < count_of_stars; j += 1) { //Печатаем
            printf("*");
        }
        printf("]\n");  //Конец звездочек
    }

    return 0;
}
