//
//  main.cpp
//  lab2
//
//  Created by Anastasia Taran on 21.10.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#ifdef __APPLE__
#define GL_SILENCE_DEPRECATION
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include "DrawableFacade/DrawableFacade.cpp"
#else
#include <GL/gl.h>
#include <GL/glut.h>
#include "DrawableFacade/DrawableFacade.cpp"
#endif

#define PI 3.14159265
#define ESCAPE '\033'
#include <cmath>
#include <iostream>

float varray[300][3];
float x = 0.5f, y = 0.5f, z;
static double cx = 0., cy = 0., cz = 0.;
double angleH = 45.0, angleV = 45.0;
double rotate_x, rotate_y, rotate_z = 0.;
double lightSpeed = 0.2, lightCoord = 0.0;

void ChangeCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    cx = cos(angleV * PI / 180.0) * cos(angleH * PI / 180.0);
    cz = cos(angleV * PI / 180.0) * sin(angleH * PI / 180.0);
    cy = sin(angleV * PI / 180.0);
    gluLookAt(cx, cy, cz, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void light() {
        GLfloat light_ambient[] = {0, 0, 1, 1.0};
        GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat light_specular[] = {1.0, 1.0, 1.0, 1.0};
        GLfloat lightPosition[] = {(float) cos(lightCoord) * 100, (float) sin(lightCoord) * 100, 0.0, 0.0};

        /* устанавливаем параметры источника света */
        glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);   //Інтенсивність фонового світла
        glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);   //Інтенсивність дифузного світла
        glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular); //Інтенсивність дзеркального світла
        glLightfv(GL_LIGHT0, GL_POSITION, lightPosition); //Положення джерела світла (x, y, z, w)
        /* включаем z-буфер */
        glEnable(GL_DEPTH_TEST);
        /* включаем освещение и источник света */
        glEnable(GL_ALPHA_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_NORMALIZE);
        lightCoord += lightSpeed;
}

void rotate() {
    GLdouble rotation_matrix_X[] = {
            1, 0, 0, 0,
            0, cos(rotate_x), sin(rotate_x), 0,
            0, -sin(rotate_x), cos(rotate_x), 0,
            0, 0, 0, 1};
    glMultMatrixd(rotation_matrix_X);

    GLdouble rotation_matrix_Y[] = {
            cos(rotate_y), 0, -sin(rotate_y), 0,
            0, 1, 0, 0,
            sin(rotate_y), 0, cos(rotate_y), 0,
            0, 0, 0, 1};
    glMultMatrixd(rotation_matrix_Y);

    GLdouble rotation_matrix_Z[] = {
            cos(rotate_z), sin(rotate_z), 0, 0,
            -sin(rotate_z), cos(rotate_z), 0, 0,
            0, 0, 1, 0,
            0, 0, 0, 1};
    glMultMatrixd(rotation_matrix_Z);
}

void display(void) {
    
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    ChangeCamera();
    rotate();
    light();
    
    DrawableFacade facade;
    facade.drawCube();
    facade.drawCones();
    
    GLint list_of_lists[] = {1, 2, 3};
    glCallLists(3, GL_INT, list_of_lists);
    glutSwapBuffers();
    glFinish();

    facade.drawGraph();
}

void reshape(GLint w, GLint h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 4, w / 4, -h / 4, h / 4, -10000, 10000);
}

void specialKey(int key, int x, int y) {
    
    if (key == GLUT_KEY_RIGHT)
    {
        angleH += 5.0f;
    }
    if (key == GLUT_KEY_LEFT)
    {
        angleH -= 5.0f;
    }
    if (key == GLUT_KEY_UP)
    {
        if (angleV < 90.0)
            angleV += 5.0f;
    }
    if (key == GLUT_KEY_DOWN)
    {
        if (angleV > -90.0)
            angleV -= 5.0f;
    }
    glutPostRedisplay();
}

void update (int value){
    angleH += value;
    if (angleH> 360) {
        angleH -=360;
    }
    glutPostRedisplay();
    glutTimerFunc(40, update, 8);
}

int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitWindowSize(1200, 1200);
    glutCreateWindow("3_lab");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(specialKey);
    glutTimerFunc(40, update, 8);
    glutMainLoop();
}
