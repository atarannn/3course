//
//  main.cpp
//  lab1_1
//
//  Created by Anastasia Taran on 22.09.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#include <iostream>
#include <fstream>
using namespace std;

const char esc = 27;

void printText(ofstream& file, string text);
void setDefaultFontHead(ofstream& file);
void printInformation(ofstream& file, string infotype);

int main()
{
    ofstream myfile;
    myfile.open("/Users/anastasia_tarannn/Downloads/3курс/принтера/lab1/lab_1.pcl", std::ios::out | std::ios::binary);
    myfile << esc << "E";
    myfile << esc << "%0B";
    myfile << "IN";
    myfile << "SP1";
    
    myfile << esc << "%0A ";
    
    //Font 1
    myfile << esc << "*p0x200Y";
    myfile << esc << "(s0p";
    myfile << "8v";
    myfile << "0s";
    myfile << "0b";
    myfile << "4099T";
    myfile << "Taran Anastasia";

    //Font 2
    myfile << esc << "*p0x300Y";
    myfile << esc << "(s1p";
    myfile << "12v";
    myfile << "1s";
    myfile << "0b";
    myfile << "4101T";
    myfile << "Taran Anastasia";

    //Font 3
    myfile << esc << "*p0x400Y";
    myfile << esc << "(s1p";
    myfile << "16v";
    myfile << "1s";
    myfile << "0b";
    myfile << "4116T";
    myfile << "Taran Anastasia";
    
    //Font 4
    myfile << esc << "*p0x500Y";
    myfile << esc << "(s1p";
    myfile << "20v";
    myfile << "0s";
    myfile << "3b";
    myfile << "4168T";
    myfile << "Taran Anastasia";

    //Font 5
    myfile << esc << "*p0x600Y";
    myfile << esc << "(s1p";
    myfile << "24v";
    myfile << "0s";
    myfile << "3b";
    myfile << "24580T";
    myfile << "Taran Anastasia";
    
    // Дата
    myfile << esc << "*p500x750Y";
    myfile << esc << "(s0p";
    myfile << "26v";
    myfile << "0s";
    myfile << "0b";
    myfile << "0T";
    myfile << "29.09.2020";
    myfile.close();
    return 0;
}
