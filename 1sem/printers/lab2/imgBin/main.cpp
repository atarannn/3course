//
//  main.cpp
//  imgBin
//
//  Created by Anastasia Taran on 09.10.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main() {
    string one = "";
    int xsize = 50, ysize = 50, repeater = 0, x = 0;
    
    string someString[ysize];
    ifstream imageFile("/Users/anastasia_tarannn/Downloads/3курс/принтера/lab2/imgBin/image.txt");
    
    if (imageFile.is_open()) {
        while (imageFile.good()) {
            getline (imageFile,someString[x]);
            cout << someString[x] << endl;
            x++;
        }
        imageFile.close();
    }
    else cout << "Unable to open file";
    imageFile.close();
    cout << "{";
    for (int i = 0; i < ysize; i++) {
        cout << "{0b";
        for (int j = 0; j < xsize; j++) {
            if (repeater == 8) {
                cout << ", 0b";
                repeater = 0;
            }
            cout << someString[i][j];
            repeater++;
        }
        repeater = 0;
        cout << "}, ";
    }
    cout << "}";
    return  0;
}
