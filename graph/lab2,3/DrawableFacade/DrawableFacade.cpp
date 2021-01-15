//
//  DrawableFacade.cpp
//  lab2
//
//  Created by Anastasia Taran on 21.10.2020.
//  Copyright © 2020 AnastasiaTaran. All rights reserved.
//

#define GL_SILENCE_DEPRECATION
#include "DrawableFacade.hpp"
#include "Drawable.hpp"
#include <OpenGL/gl.h>
#include <GLUT/glut.h>
#include <cmath>

class DrawableCube: Drawable {
public:
    virtual void draw()
    {
        glNewList(1, GL_COMPILE);
        glPushMatrix();
        
        glTranslatef(0, 0, 100);
        glutSolidCube(50);
        
        glPopMatrix();
        glEndList();
    }
};

class DrawableCone: Drawable {
public:
    virtual void draw()
    {
        glNewList(2, GL_COMPILE);
        glPushMatrix();
        
        glTranslatef(0, 0, 200);
        glutSolidCone(30, 70, 20, 20);
        
        glPopMatrix();
        glEndList();
    }
};

class DrawableGraph: Drawable {
public:
    virtual void draw()
    {
        GLdouble points[300];
        GLint count = 0;
        for (GLint y = -5; y < 5; y++) {
            for (GLint x = -5; x < 5; x++) {
                points[count++] = x * 5;
                points[count++] = y * 5;
                points[count++] = sin(x) + cos(y) * 5;
            }
        }
    
        glEnableClientState(GL_VERTEX_ARRAY);
        
        glVertexPointer(3, GL_DOUBLE, 0, points);
    
        glDrawArrays(GL_POINTS, 0, 100);
    
        glDisableClientState(GL_VERTEX_ARRAY);
        glutSwapBuffers();
        glFinish();
    }
};

class DrawableFacade
{
    DrawableCube one;
    DrawableCone two;
    DrawableGraph third;
    public:
        DrawableFacade()
        {
        }
        
        void drawCube() {
            one.draw();
        }
        
        void drawCones() {
            two.draw();
        }
        
        void drawGraph() {
            third.draw();
        }
};
