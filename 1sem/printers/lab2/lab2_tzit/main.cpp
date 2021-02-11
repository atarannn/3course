//
//  main.cpp
//  lab2_tzit
//
//  Created by Anastasia Taran on 07.10.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.

#include <iostream>
#include <fstream>
#include "data.h"
using namespace std;
const char esc = 27;

void setDefaultFontHead(ofstream& file);
void setDefaultFontText(ofstream& file);
void printInformation(ofstream& file, string infotype);

int main()
{    
    int thing = 0b11111111;
    
    ofstream myfile;
    myfile.open("/Users/anastasia_tarannn/Downloads/3курс/принтера/lab2/test.pcl", std::ios::out | std::ios::binary);
    myfile << esc << "E";
    myfile << esc << "%0B";     // enter HP-GL/2 mode
    myfile << "IN";             // initialize
    myfile << "SP1";            // selects pen number 1, (black)
    
    //рисуем карточку
      myfile << "PA0,10000";        // plot absolute (using absolute coordinates)
      myfile << "EA7800,7000;";     // edge rectangle absolute (; is the end of command)
      
    //рисуем карточку (маленькая внутри)
      myfile << "PA500,9600";       // откуда рисуем
      myfile << "EA2200,7400; ";    // куда рисуем
      myfile << esc << "%0A ";      // enter the PCL mode

      //текст карточки
      myfile << esc << "*p900x250Y"; // позиция текста
      printInformation(myfile, "1");
      
      myfile << esc << "*p900x390Y";
      printInformation(myfile, "2");
    
      myfile << esc << "*p900x520Y";
      printInformation(myfile, "3");
      
      myfile << esc << "*p900x660Y";
      printInformation(myfile, "4");
      
      myfile << esc << "*p900x800Y";
      printInformation(myfile, "5");
    
// myfile << esc << "E";
        myfile << esc << "*p300x400Y";  // позиция картинки
        myfile <<esc<<"*t75R";
        myfile <<esc<<"*r1A";
    
// счётчик
    for (int i = 0; i < y; i++){

        myfile << esc << "*b" << x << "W";      // инициализация растровой печати
        for (int j = 0; j < x; j++) {
            thing = r[i][j];
            myfile << (char)thing;
        }
    }
    myfile <<esc<<"*rC"; // закончить графический процесс
    myfile.close();
    return 0;
}
//шрифт общего текста
void setDefaultFontHead(ofstream& file)
{
     file << esc << "(s1p";     // spacing //интервалы
          file << "14v";        // height //размер шрифта
          file << "0s";         // style //курсив/некурсив
          file << "3b";         // stroke weight //толщина шрифта
          file << "4168T";      // typeface family //сам шрифт
}
//шрифт текста информации личной
void setDefaultFontText(ofstream& file)
{
     file << esc << "(s1p";     // spacing //интервалы
       file << "14v";           // height //размер шрифта
       file << "0s";            // style //курсив/некурсив
       file << "0b";            // stroke weight //толщина шрифта
       file << "4197T";         // typeface family //сам шрифт
}
void printInformation(ofstream& file, string infotype)
{
    switch (infotype[0]) {
        case '1':
            setDefaultFontHead(file);
            file << "Taran Anastasia";
            break;
        case '2':
            setDefaultFontHead(file);
            file << "Born in: ";
            setDefaultFontText(file);
            file << "Kyiv";
            break;
        case '3':
            setDefaultFontHead(file);
            file << "Birth date: ";
            setDefaultFontText(file);
            file << "October 28, 2000";
            break;
        case '4':
            setDefaultFontHead(file);
            file << "E-mail: ";
            setDefaultFontText(file);
            file << "anastasia_tarannn@icloud.com";
            break;
        case '5':
            setDefaultFontHead(file);
            file << "Institute: ";
            setDefaultFontText(file);
            file << "KPI";
            break;
    }
}
