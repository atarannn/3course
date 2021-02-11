//
//  main.cpp
//  lab3_3_UNIX
//
//  Created by Anastasia Taran on 13.12.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#include <string.h>
#include <vector>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

struct Info{ // создаем струкутуру
    std::string name; //имя
    mode_t mode; // режим
    std::string uid; // пользователь
    std::string gid; // группа
    off_t size; // размер
    time_t mtim; // время
    
    void Print(){
        printf((S_ISDIR(mode)) ? "d" : "-"); // каталог
        /* 3 bits for user  */
        printf((mode & S_IRUSR) ? "r" : "-"); // пользователь имеет права на чтение файла
        printf((mode & S_IWUSR) ? "w" : "-"); // пользователь имеет права на запись информации в файл
        printf((mode & S_IXUSR) ? "x" : "-"); // пользователь имеет права на выполнение файла
        /* 3 bits for group */
        printf((mode & S_IRGRP) ? "r" : "-"); // группа имеет права на чтение файла
        printf((mode & S_IWGRP) ? "w" : "-"); // группа имеет права на запись информации в файл
        printf((mode & S_IXGRP) ? "x" : "-"); // группа имеет права на выполнение файла
        /* 3 bits for other */
        printf((mode & S_IROTH) ? "r" : "-"); // все остальные имеют права на чтение файла
        printf((mode & S_IWOTH) ? "w" : "-"); // все остальные имеют права на запись информации в файл
        printf((mode & S_IXOTH) ? "x" : "-"); // все остальные имеют права на выполнение файла
        
        printf(" %7s %7s ", uid.c_str(), gid.c_str()); // пользоватеель и группа
        
        printf(" %7d ", (int)size); // выводим размер файла
        printf(" %30s ", name.c_str()); // выводим название файла
        printf (" %s " , ctime (&mtim) ); // выводим время модификации
    }
};

void FindFiles(char dir[256], std::vector<Info>& files) { // функция для поиска файлов
    DIR *cdir;
    dirent *direntry; // struct
    struct stat fileinfo; // структура типа stat
    chdir(dir); // меняем текущую директорию на dir
    cdir=opendir(dir); // открываем ее
    if(cdir!=NULL){ // если каталог открылся
        while((direntry=readdir(cdir))!=NULL){ // перехоим к следующему файлу в папке, пока не дошли до конца
            if(lstat(direntry->d_name,&fileinfo)!=0){ // если у файла нет имени -- пропускаем
                continue;
            }
            Info tempDir; // создаем временный объект с инфо о файле
            tempDir.name = direntry->d_name;  // записываем имя
            tempDir.mode = fileinfo.st_mode; // записываем права достпа файла
            struct passwd *pw = getpwuid(fileinfo.st_uid); //*
            tempDir.uid = std::string(pw->pw_name);        //* - записываем информацию о пользователе, которому принадлежит файл
            struct group  *gr = getgrgid(fileinfo.st_gid);  //*
            tempDir.gid = std::string(gr->gr_name);         //* - записываем информацию о группе
            tempDir.size = fileinfo.st_size;  // записываем информацию о размере файла
            tempDir.mtim = fileinfo.st_mtime;  // записываем информацию о дате и времени последнего изменения
            files.push_back(tempDir);  // добавляем наш временный объект в конец вектора информаций о файлах
        }
    }
}

void PrintAll(std::vector<Info>& files){ // функция для вывода всех файлов
    std::vector<Info>::iterator at = files.begin(); // получаем итератор по нашему вектору
    for (; at != files.end(); at++){ // проходимся по итератору
        at->Print(); //вызвываем функцию Print() для каждого обьекта в вакторе files
    }
}

int main(int argc, char* argv[]){
    std::vector<Info> names; // вектор объектов с информацией о файле
    std::vector<std::string> folders;
    char directory[256] = "/Users/anastasia_tarannn/Downloads/you-film-test";
    FindFiles((char *)directory, names); // читаем все вайлы в папке
    PrintAll(names); // выводим информацию о них
    return 0;
}
