
//
//  Created by Anastasia Taran on 02.09.2020.
//  Copyright © 2020 Anastasia Taran. All rights reserved.

#include <cstdlib>
#include <vector>
#include <fstream>
#include <sstream>
#include <GLUT/GLUT.h> /* подключаем библиотеку GLUT */

float translate_x = 0.0f;
float translate_y = 0.0f;
float translate_z = 0.0f;

class XY    //класс, который имеет координаты точок
{
    public:
        GLfloat x, y;
        XY(float x, float y)
        {
            this->x = static_cast<GLfloat>(x);
            this->y = static_cast<GLfloat>(y);
        }
};

class Element // класс, который рисует примитивы
{
public:
    std::vector<XY> points;
    void render(){
        glColor3ub(255, 165, 00);    //цвет фигуры
        
        glBegin(GL_POLYGON);
        for(auto point : this->points) {
            glVertex2f(point.x, point.y);
        }
        glEnd();
        
        glColor3f(0.0, 0.0, 0.0);  //цвет обводки
        glLineWidth(2);     //толщина обводки
        
        glBegin(GL_LINES); //начинаем рисовать линии
        for (int i = 0; i < this->points.size()-1; ++i)
        {
            glVertex2f((this->points)[i].x, (this->points)[i].y);
            glVertex2f((this->points)[i+1].x, (this->points)[i+1].y);
        }
        glVertex2f((this->points)[this->points.size()-1].x, (this->points)[this->points.size()-1].y);
        glVertex2f((this->points)[0].x, (this->points)[0].y);
        glEnd();
    }
    void add_point(XY point){
        this->points.push_back(point);
    }
};

class ElementSet{ //считывание данных с файла
public:
    std::vector<Element> element_set;
    void readFromFile(const std::string& filepath){
        std::ifstream infile;
        infile.open(filepath);
        std::string line;
        Element elem = Element();
        while (std::getline(infile, line))
        {
            std::istringstream iss(line);
            float a, b;
            if (!(iss >> a >> b)) {
                    this->element_set.push_back(elem);
                    elem = Element();
            } else {
                elem.add_point(XY(a, b));
            }
        }
        infile.close();
    }
    void render(){
        gluOrtho2D(-1.0, 2.0, -2.0, 1.0); //положение фигуры в окне
        glTranslatef(translate_x, translate_y, translate_z);
        glColor3f(0, 0, 1);
        for(auto element : this->element_set) {
            element.render();
        }
    }
};
ElementSet elem_set = ElementSet();

/* Функция обрабатывает сообщения от клавиатуры */
void keyPress(int key, int x, int y)
{
#define ESCAPE '\033'
    switch (key) {
        case GLUT_KEY_UP:
            translate_x += 0.5f;
            translate_y += 1.0f;
            break;
        case GLUT_KEY_DOWN:
            translate_x -= 0.5f;
            translate_y -= 1.0f;
            break;
        case ESCAPE:
            exit(0);
        default:
            break;
    }
    glutPostRedisplay();//ф-ция которая заставляет систему перерисовать текущее окно
}

void Reshape(int w, int h) {
/* установлюємо розміри області відображення */
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION); //Switch to setting the camera perspective
    //Set the camera perspective
    glLoadIdentity(); //Reset the camera
}

void drawScene() // рисуем картинку
{
    glClearColor(0, 0, 1, 1); //цвет фона
    glClear(GL_COLOR_BUFFER_BIT);//Очистка RGBA mode window в нужный цвет
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();// Сброс просмотра
    elem_set.render();
    glutSwapBuffers();//отображая окно целиком после того, как оно уже сформировано
}

int main(int argc, char** argv)
{
    elem_set.readFromFile("/Users/anastasia_tarannn/Downloads/3курс/графика/lab1/file.txt");
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB);
    glutInitWindowSize(500, 500); // size окна
    glutInitWindowPosition(500, 0); // положение окна
    glutCreateWindow("lab_1_Taran");
    glutDisplayFunc(drawScene);
    glutSpecialFunc(keyPress);
    glutReshapeFunc(Reshape);
    glutMainLoop();

    return 0;
}
