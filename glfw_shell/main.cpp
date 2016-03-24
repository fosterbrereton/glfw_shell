//========================================================================
// Simple GLFW example
// Copyright (c) Camilla Berglund <elmindreda@elmindreda.org>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================
//! [code]

#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <OpenGL/glu.h>
#include <cmath>

#include "SOIL.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
float camX{0};
float camY{0};
float camZ{0};

bool MoveForward{false};
bool MoveBackward{false};
bool MoveLeft{false};
bool MoveRight{false};
bool MoveUp{false};
bool Sprint{false};
bool AdminSprint{false};

bool fall{false};

bool MouseOut{false};

float camRotateX{-260};
float camRotateY{0};

float DecreaseClimbRate{0.1};
float IncreaseFallRate{0.05};

GLuint gTexture{0};
GLuint gTextureSteel{0};
GLuint gTextureRoad{0};
GLuint gTextureRoadY{0};
GLuint gTextureBlank{0};
GLuint gTextureBall{0};
GLuint gTextureWhite{0};

float DegreesToRads(float Degrees){
    return Degrees/180*3.14159;
    
    
}
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    static double oldx{xpos};
    static double oldy{ypos};
    
    double deltaX{xpos-oldx};
    double deltaY{ypos-oldy};
    
    // std::cout << deltaX << "," << deltaY << "\n";
    camRotateY-=deltaX;
    camRotateX+=deltaY;
    oldx = xpos;
    oldy = ypos;
    
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
        glfwSetWindowShouldClose(window, GL_TRUE);
        }
    /*else if (key == GLFW_KEY_R && action == GLFW_PRESS)
        {
        // These values are floating point from 0 to 1.
        // These are in RGB order.
        glClearColor(1.0, 0.0, 0.0, 1.0); // red
        }
    else if (key == GLFW_KEY_K && action == GLFW_PRESS)
        {
        // These values are floating point from 0 to 1.
        // These are in RGB order.
        glClearColor(0.0, 0.0, 0.0, 1.0); // black
        }*/
    else if (key == GLFW_KEY_P && action == GLFW_PRESS)
    {
        MouseOut^=true;
        
        
    }
    else if (key == GLFW_KEY_D)
    {
        MoveRight = action == GLFW_PRESS || action == GLFW_REPEAT;
        

    }
    else if (key == GLFW_KEY_A)
    {
        MoveLeft = action == GLFW_PRESS || action == GLFW_REPEAT;
        

    }
    else if (key == GLFW_KEY_S)
    {
        MoveBackward = action == GLFW_PRESS || action == GLFW_REPEAT;
        

    }
    else if (key == GLFW_KEY_W)
    {
        MoveForward=action == GLFW_PRESS || action == GLFW_REPEAT;
        
        
    }

    else if (key == GLFW_KEY_SPACE)
    {
        
        MoveUp=true;
        
        
        
    }
    else if (key == GLFW_KEY_LEFT_SHIFT)
    {
        Sprint=action == GLFW_PRESS || action == GLFW_REPEAT;
        
    }
    else if (key == GLFW_KEY_E)
    {
        AdminSprint=action == GLFW_PRESS || action == GLFW_REPEAT;
        
    }
    
    
  

    
    
    
    
    else
        {
        std::string actionString;

        if (action == GLFW_PRESS)
            actionString = "DOWN";
        else
            actionString = "UP";

        // std::cout << "You pressed " << actionString << " on the key " << key << '\n';
        }
    std::cout << "Camera X = " << camX << '\n';
    std::cout << "Camera Y = " << camY << '\n';
    std::cout << "Camera Z = " << camZ << '\n';
    
    // std::cout << "Camera Rotate X =" << camRotateX << '\n';
    // std::cout << "Camera Rotate Y =" << camRotateY << '\n';
}
void p1(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p2(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p3(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p4(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(1.0f, 0.8431372549f, 0.f); // yellow
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p5(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p6(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p7(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p8(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}



void p2_1(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.5f, 0.5f, 0.5f); // red
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p2_2(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.5f, 0.5f, 0.5f); // blue
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p2_3(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.5f, 0.5f, 0.5f); // gray
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p2_4(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.5f, 0.5f, 0.5f); // yellow
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p2_5(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.5f, 0.5f, 0.5f); // orange
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p2_6(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.5f, 0.5f, 0.5f); // pink
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p2_7(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.5f, 0.5f, 0.5f); // tan-ish
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p2_8(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.5f, 0.5f, 0.5f); // white
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}


void p3_1(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // red
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p3_2(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // blue
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p3_3(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // gray
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p3_4(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // yellow
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p3_5(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // orange
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p3_6(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // pink
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p3_7(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // tan-ish
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p3_8(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // white
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}




//Color = black
void p4_1(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p4_2(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p4_3(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p4_4(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p4_5(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p4_6(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p4_7(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p4_8(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}
//rainbow colors


void p5_1(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 0.f, 0.f); // red
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p5_2(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.f, 0.f, 1.0f); // blue
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p5_3(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.5f, 0.5f, 0.5f); // gray
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p5_4(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 1.0f, 0.f); // yellow
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p5_5(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 0.4666666667f, 0.f); // orange
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p5_6(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 0.f, 0.6352941176f); // pink
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p5_7(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 0.8f, 0.6f); // tan-ish
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p5_8(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 1.0f, 1.0f); // white
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}

void p6_1(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // red
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p6_2(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // blue
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p6_3(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // gray
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p6_4(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // yellow
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p6_5(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // orange
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p6_6(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // pink
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p6_7(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // tan-ish
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p6_8(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.f, 1.f, 1.f); // white
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}

void cube(float x, float y, float z, float h, float w, float d){
    glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p5(x,y,z,h,w,d);
    p8(x,y,z,h,w,d);
    p7(x,y,z,h,w,d);
    p6(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p8(x,y,z,h,w,d);
    p4(x,y,z,h,w,d);
    p3(x,y,z,h,w,d);
    p7(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p5(x,y,z,h,w,d);
    p6(x,y,z,h,w,d);
    p2(x,y,z,h,w,d);
    p1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p6(x,y,z,h,w,d);
    p7(x,y,z,h,w,d);
    p3(x,y,z,h,w,d);
    p2(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p4(x,y,z,h,w,d);
    p8(x,y,z,h,w,d);
    p5(x,y,z,h,w,d);
    p1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p1(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p2(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p4(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
}
void triangle(float x, float y, float z, float h, float w, float d){
    
    glBegin(GL_TRIANGLES);
    p5_7(x,y,z,h,w,d);
    p5_8(x,y,z,h,w,d);
    p5_4(x,y,z,h,w,d);
    glEnd();
    
    glBegin(GL_QUADS);
    p5_6(x,y,z,h,w,d);
    p5_7(x,y,z,h,w,d);
    p5_4(x,y,z,h,w,d);
    p5_1(x,y,z,h,w,d);
    glEnd();
    
    glBegin(GL_TRIANGLES);
    p5_1(x,y,z,h,w,d);
    p5_5(x,y,z,h,w,d);
    p5_6(x,y,z,h,w,d);
    glEnd();
    
    glBegin(GL_QUADS);
    p5_1(x,y,z,h,w,d);
    p5_4(x,y,z,h,w,d);
    p5_8(x,y,z,h,w,d);
    p5_5(x,y,z,h,w,d);
    glEnd();
    
    glBegin(GL_QUADS);
    p5_6(x,y,z,h,w,d);
    p5_5(x,y,z,h,w,d);
    p5_8(x,y,z,h,w,d);
    p5_7(x,y,z,h,w,d);
    glEnd();
    
   
}

void cube2(float x, float y, float z, float h, float w, float d){
    glBindTexture(GL_TEXTURE_2D, gTextureSteel);
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p2_5(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p2_8(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p2_7(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p2_6(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p2_8(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p2_4(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p2_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p2_7(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p2_5(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p2_6(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p2_2(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p2_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p2_6(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p2_7(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p2_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p2_2(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p2_4(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p2_8(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p2_5(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p2_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p2_1(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p2_2(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p2_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p2_4(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
}

void cube3(float x, float y, float z, float h, float w, float d){
    glBindTexture(GL_TEXTURE_2D, gTexture);
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p3_5(x,y,z,h,w,d);
    p3_8(x,y,z,h,w,d);
    p3_7(x,y,z,h,w,d);
    p3_6(x,y,z,h,w,d);
    
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p3_8(x,y,z,h,w,d);
    p3_4(x,y,z,h,w,d);
    p3_3(x,y,z,h,w,d);
    p3_7(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p3_5(x,y,z,h,w,d);
    p3_6(x,y,z,h,w,d);
    p3_2(x,y,z,h,w,d);
    p3_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p3_6(x,y,z,h,w,d);
    p3_7(x,y,z,h,w,d);
    p3_3(x,y,z,h,w,d);
    p3_2(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p3_4(x,y,z,h,w,d);
    p3_8(x,y,z,h,w,d);
    p3_5(x,y,z,h,w,d);
    p3_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p3_1(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p3_2(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p3_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p3_4(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
}

void cube4(float x, float y, float z, float h, float w, float d){
    glBindTexture(GL_TEXTURE_2D, gTextureRoad);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 30); p4_5(x,y,z,h,w,d);
    glTexCoord2f(30, 30); p4_8(x,y,z,h,w,d);
    glTexCoord2f(30, 0); p4_7(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p4_6(x,y,z,h,w,d);
    
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 30); p4_8(x,y,z,h,w,d);
    glTexCoord2f(30, 30); p4_4(x,y,z,h,w,d);
    glTexCoord2f(30, 0); p4_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p4_7(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 30); p4_5(x,y,z,h,w,d);
    glTexCoord2f(30, 30); p4_6(x,y,z,h,w,d);
    glTexCoord2f(30, 0); p4_2(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p4_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 30); p4_6(x,y,z,h,w,d);
    glTexCoord2f(30, 30); p4_7(x,y,z,h,w,d);
    glTexCoord2f(30, 0); p4_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p4_2(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 30); p4_4(x,y,z,h,w,d);
    glTexCoord2f(30, 30); p4_8(x,y,z,h,w,d);
    glTexCoord2f(30, 0); p4_5(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p4_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    
    glTexCoord2f(0, 30); p4_1(x,y,z,h,w,d);
    glTexCoord2f(30, 30); p4_2(x,y,z,h,w,d);
    glTexCoord2f(30, 0); p4_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p4_4(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
}

void cube5(float x, float y, float z, float h, float w, float d){
    glBindTexture(GL_TEXTURE_2D, gTextureSteel);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p6_5(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p6_8(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p6_7(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p6_6(x,y,z,h,w,d);
    
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p6_8(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p6_4(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p6_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p6_7(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p6_5(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p6_6(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p6_2(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p6_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p6_6(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p6_7(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p6_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p6_2(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p6_4(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p6_8(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p6_5(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p6_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    
    glTexCoord2f(0, 1); p6_1(x,y,z,h,w,d);
    glTexCoord2f(1, 1); p6_2(x,y,z,h,w,d);
    glTexCoord2f(1, 0); p6_3(x,y,z,h,w,d);
    glTexCoord2f(0, 0); p6_4(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
}

void sphere(float x,float y,float z,float size){
    //glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    //glLoadIdentity();
    glTranslatef(-x, -y, -z);
    
    GLUquadricObj*quad=gluNewQuadric();
    gluQuadricTexture( quad, GL_TRUE);
    gluSphere(quad, size, 15, 15);
    gluDeleteQuadric(quad);
    glPopMatrix();

}
int main(void)
{
    // In JavaScript, this would be "var window;"
    GLFWwindow* window; // This creates a variable to store the GLFW window

    glfwSetErrorCallback(error_callback); // Gives GLFW a function to call when there's an error.

    if (!glfwInit()) // Allows GLFW to do some initial setup and initialization.
        exit(EXIT_FAILURE); // If initialization fails, we can't continue with the program.

    // Builds a new GLFW window and saves the result in the variable above.
    // If there's an error here, window will be set to 0.
    // 640x480 is the initial size, and "Simple example" is the name of the window.
    window = glfwCreateWindow(640, 480, "Simple example", NULL, NULL);

    // If window == 0, this will be true, and we've hit an error.
    if (!window /*same as saying `window == 0`*/)
    {
        glfwTerminate(); // This is the opposite of glfwInit, and allows GLFW to close up shop.
        exit(EXIT_FAILURE); // This kills the application.
    }

    glfwMakeContextCurrent(window); // Tells GLFW which window is going to be drawn to.
    glfwSwapInterval(1); // Tells GLFW how often the window should be redrawn.

    // key_callback is the function that GLFW should call when the user hits
    // a key on the keyboard. So we give that function to GLFW with this routine.
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    
    // Set some OpenGL world options.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    gTexture = SOIL_load_OGL_texture("../../textures/grasstex.tga",
                                     SOIL_LOAD_AUTO,
                                     SOIL_CREATE_NEW_ID,
                                     SOIL_FLAG_POWER_OF_TWO |
                                     SOIL_FLAG_MIPMAPS |
                                     SOIL_FLAG_DDS_LOAD_DIRECT);
    


    if (gTexture == 0)
    {
        std::cout << "error loading texture grass\n";
    }
    else
    {   //comment this out to go to normal colors
    
        glEnable(GL_TEXTURE_2D);
        //glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
    }
    
    
    gTextureSteel = SOIL_load_OGL_texture("../../textures/steel_floor.tga",
                                          SOIL_LOAD_AUTO,
                                          SOIL_CREATE_NEW_ID,
                                          SOIL_FLAG_POWER_OF_TWO |
                                          SOIL_FLAG_MIPMAPS |
                                          SOIL_FLAG_DDS_LOAD_DIRECT);
    
    if (gTextureSteel == 0)
    {
        std::cout << "error loading texture steel\n";
    }
    
    
    gTextureRoad = SOIL_load_OGL_texture("../../textures/road.tga",
                                          SOIL_LOAD_AUTO,
                                          SOIL_CREATE_NEW_ID,
                                          SOIL_FLAG_POWER_OF_TWO |
                                          SOIL_FLAG_MIPMAPS |
                                          SOIL_FLAG_DDS_LOAD_DIRECT);
    
    if (gTextureRoad == 0)
    {
        std::cout << "error loading texture road\n";
    }
    
    gTextureBall = SOIL_load_OGL_texture("../../textures/soccerball.tga",
                                         SOIL_LOAD_AUTO,
                                         SOIL_CREATE_NEW_ID,
                                         SOIL_FLAG_POWER_OF_TWO |
                                         SOIL_FLAG_MIPMAPS |
                                         SOIL_FLAG_DDS_LOAD_DIRECT);
    
    if (gTextureBall == 0)
    {
        std::cout << "error loading texture road\n";
    }

    
    
    gTextureRoadY = SOIL_load_OGL_texture("../../textures/yellowline.tga",
                                         SOIL_LOAD_AUTO,
                                         SOIL_CREATE_NEW_ID,
                                         SOIL_FLAG_POWER_OF_TWO |
                                         SOIL_FLAG_MIPMAPS |
                                         SOIL_FLAG_DDS_LOAD_DIRECT);
    
    if (gTextureRoadY == 0)
    {
        std::cout << "error loading texture yellow line\n";
    }

    
    gTextureWhite = SOIL_load_OGL_texture("../../textures/white.tga",
                                         SOIL_LOAD_AUTO,
                                         SOIL_CREATE_NEW_ID,
                                         SOIL_FLAG_POWER_OF_TWO |
                                         SOIL_FLAG_MIPMAPS |
                                         SOIL_FLAG_DDS_LOAD_DIRECT);
    
    if (gTextureWhite == 0)
    {
        std::cout << "error loading texture white\n";
    }
    

    


    
    float carSpeed1=100;
    float carSpeed2=100;
    float carSpeed3=100;
    float carSpeed4=100;
    
    float carSpeed6=-100;
    
    
    // This is the main processing loop that draws the spinning rectangle.
    while (!glfwWindowShouldClose(window)) // this will loop until the window should close.
    {
    
        // These are variable spaces.
        float ratio; // this is a floating point number
        int width, height; // these variables store the dimensions of the window

        glfwGetFramebufferSize(window, &width, &height); // Get the height and width of the window from GLFW.
        ratio = width / (float) height; // compute the aspect ratio of the window, which we need below.
        
        glViewport(0, 0, width, height); // This tells OpenGL how big the window is,
        glClearColor(0.5294117647, 0.8078431373, 0.9803921569, 0);                              // and OpenGL goes off and creates a space
                                         // for drawing.
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // This asks OpenGL to wipe clean the drawing space.
                                      // The default color is black. If you want it to be
                                      // another color, you have to call glClearColor with
                                      // the new color values before making this call.

        /*
            These operations tell OpenGL how to convert the 3D world we are about
            to create into a 2D image that can be displayed on the computer screen.
        */
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(60, ratio, 1, 1000);
        //glOrtho(-ratio, ratio, -1.f, 1.f, 50.f, -50.f);
        glMatrixMode(GL_MODELVIEW);
       

        /*
            These operations tell OpenGL that we want to rotate the 3D world that
            we are drawing. The call to glfwGetTime is constantly changing as time
            marches on, and the * 50 determines how fast the rotation should be.
        */
        glLoadIdentity();
        //glRotatef((float) glfwGetTime() * 8.f, 0.f, 0.f, 1.f);
        //glRotatef((float) glfwGetTime() * 4.f, 0.f, 1.f, 0.f);
        //glRotatef((float) glfwGetTime() * 5.f, 1.f, 0.f, 0.f);
        
        glRotatef(camRotateX, 1.f, 0.f, 0.f);
        
        glRotatef(camRotateY, 0.f, 0.f, 1.f);
        if(MoveForward){
            
            camY += std::cos(DegreesToRads(camRotateY))*0.1;
            camX += std::sin(DegreesToRads(camRotateY))*0.1;
        }
        if(MoveRight){
            camY += std::cos(DegreesToRads(camRotateY-90))*0.1;
            camX += std::sin(DegreesToRads(camRotateY-90))*0.1;
        }
        if(MoveLeft){
            camY += std::cos(DegreesToRads(camRotateY+90))*0.1;
            camX += std::sin(DegreesToRads(camRotateY+90))*0.1;
        }
        if(MoveBackward){
            camY -= std::cos(DegreesToRads(camRotateY))*0.1;
            camX -= std::sin(DegreesToRads(camRotateY))*0.1;
        }
        
        
        if(MoveUp && fall==false){
            camZ += DecreaseClimbRate;
            DecreaseClimbRate-=0.0077;
            
        }
        if(Sprint){
            camY += std::cos(DegreesToRads(camRotateY))*0.15;
            camX += std::sin(DegreesToRads(camRotateY))*0.15;
        }
        if(AdminSprint){
            camY += std::cos(DegreesToRads(camRotateY))*1;
            camX += std::sin(DegreesToRads(camRotateY))*1;
        }
       
        if(camZ<=0){
            camZ += 0.1;
            fall=false;
            DecreaseClimbRate=0.2;
            MoveUp=false;
            
        }
        if(MouseOut==true){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        }
        if(MouseOut==false){
            glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            
        }
        if(MouseOut==true){
        glfwSetCursorPosCallback(window, 0);
        
        }
        if(MouseOut==false){
            glfwSetCursorPosCallback(window, cursor_pos_callback);
        }
       
        
       
        
        glTranslatef(camX+2,camY-2.5,camZ);
        
      
        //water well
        cube2(0,0,0,0.125,0.125,1);
        cube2(0.5,0,0,0.125,0.125,1);
        cube2(0,0.5,0,0.125,0.125,1);
        cube2(0.5,0.5,0,0.125,0.125,1);
        
        cube2(0.25,0.25,-0.5,1,1,0.5);
         
        cube2(0,0,1,0.5,0.5,1);
         cube2(0.5,0,1,0.5,0.5,1);
         cube2(0,0.5,1,0.5,0.5,1);
         cube2(0.5,0.5,1,0.5,0.5,1);
        //water well
        
        //grass
        cube3(0.5,0.5,1.75,450,450,0.5);
        
        
        //rainbow ramps
        triangle(1,2,1,1,1,1);
        triangle(0,2,1,1,1,1);
        
        
        //road
        cube4(5,1,1.49,6,450,0.000001);
        cube(5,1,1.48,0.5,450,0.000001);
        //road
        //road2
        cube4(-50,1,1.49,6,450,0.000001);
        cube(-50,1,1.48,0.5,450,0.000001);
        //road2
        //road3
        cube4(0.5,101,1.49,450,6,0.000001);
        cube(0.5,101,1.48,450,0.5,0.000001);
        //road3
        //road4
        cube4(0.5,-99,1.49,450,6,0.000001);
        cube(0.5,-99,1.48,450,0.5,0.000001);
        //road4
        //road5
        cube4(0.5,51,1.49,450,6,0.000001);
        cube(0.5,51,1.48,450,0.5,0.000001);
        //road5
        //road6
        cube4(0.5,-49,1.49,450,6,0.000001);
        cube(0.5,-49,1.48,450,0.5,0.000001);
        //road6
        //road7
        cube4(0.5,222,1.49,450,6,0.000001);
        cube(0.5,222,1.48,450,0.5,0.000001);
        //road7
        //road8
        cube4(0.5,-222,1.49,450,6,0.000001);
        cube(0.5,-222,1.48,450,0.5,0.000001);
        //road8
        //road9
        cube4(222,0.5,1.49,6,450,0.000001);
        cube(222,0.5,1.48,0.5,450,0.000001);
        //road9
        //road10
        cube4(-222,0.5,1.49,6,450,0.000001);
        cube(-222,0.5,1.48,0.5,450,0.000001);
        //road10



        
     
        //warehouse
        //door
        cube3(10.49,1,1,0.2,2,4);
        cube2(10.3,1.7,0.1,0.2,0.2,0.2);
        //walls
        cube2(10,-5,1,1,10,10);
        cube2(18,-1,1,10,1,10);
        cube2(11.5,-1,-2.5,3,1,3);
        cube2(23,-5,1,1,10,10);
        cube2(17,-10,1,14,1,10);
        //floor
        cube2(17,-5,1.49,13,9,0.000001);
        //roof
        cube2(17,-5,-4.25,13,9,0.5);
        
        //speed
        carSpeed1=carSpeed1-0.6;
        //car
        
        //bottom
        cube5(6.5,101+carSpeed1,0.5,2.5,6,0.5);
        //roof
        cube5(6.5,101.5+carSpeed1,-1.6,2.5,5,0.5);
        //side
        cube5(5.5,101.5+carSpeed1,-0.6,0.5,5,1.65);
        //side
        cube5(7.5,101.5+carSpeed1,-0.6,0.5,5,1.65);
        //back
        cube5(6.5,104+carSpeed1,-0.6,2.5,0.3,1.65);
        //front
        cube5(6.5,98.7+carSpeed1,-0.25,2.5,0.6,1);
        //front window
        cube3(6.5,99+carSpeed1,-1.25,2.5,0.05,1);
        //wheels
        cube3(7.5,103+carSpeed1,1,0.3,1,1);
        cube3(5.5,103+carSpeed1,1,0.3,1,1);
        cube3(7.5,99+carSpeed1,1,0.3,1,1);
        cube3(5.5,99+carSpeed1,1,0.3,1,1);
        glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
        sphere(-5.5,-98.4-carSpeed1,0.5,0.2);
        sphere(-7.5, -98.4-carSpeed1,0.5,0.2);
        
        
        //car2
        carSpeed2=carSpeed2-0.45;
        //bottom
        cube2(3.5,-101-carSpeed2,0.5,2.5,6,0.5);
        //roof
        cube2(3.5,-101.5-carSpeed2,-1.6,2.5,5,0.5);
        //side
        cube2(2.5,-101.5-carSpeed2,-0.6,0.5,5,1.65);
        //side
        cube2(4.5,-101.5-carSpeed2,-0.6,0.5,5,1.65);
        //back
        cube2(3.5,-104-carSpeed2,-0.6,2.5,0.3,1.65);
        //front
        cube2(3.5,-98.7-carSpeed2,-0.25,2.5,0.6,1);
        //front window
        cube3(3.5,-99-carSpeed2,-1.25,2.5,0.05,1);
        //wheels
        cube3(4.5,-103-carSpeed2,1,0.3,1,1);
        cube3(2.5,-103-carSpeed2,1,0.3,1,1);
        cube3(4.5,-99-carSpeed2,1,0.3,1,1);
        cube3(2.5,-99-carSpeed2,1,0.3,1,1);


        glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
        sphere(-2.5,98.4+carSpeed2,0.5,0.2);
        sphere(-4.5, 98.4+carSpeed2,0.5,0.2);
        
        
        //car3
        carSpeed3=carSpeed3-0.4;
        //bottom
        cube2(3.5,-51-carSpeed3,0.5,2.5,6,0.5);
        //roof
        cube2(3.5,-51.5-carSpeed3,-1.6,2.5,5,0.5);
        //side
        cube2(2.5,-51.5-carSpeed3,-0.6,0.5,5,1.65);
        //side
        cube2(4.5,-51.5-carSpeed3,-0.6,0.5,5,1.65);
        //back
        cube2(3.5,-54-carSpeed3,-0.6,2.5,0.3,1.65);
        //front
        cube2(3.5,-48.7-carSpeed3,-0.25,2.5,0.6,1);
        //front window
        cube3(3.5,-49-carSpeed3,-1.25,2.5,0.05,1);
        //wheels
        cube3(4.5,-53-carSpeed3,1,0.3,1,1);
        cube3(2.5,-53-carSpeed3,1,0.3,1,1);
        cube3(4.5,-49-carSpeed3,1,0.3,1,1);
        cube3(2.5,-49-carSpeed3,1,0.3,1,1);
        glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
        sphere(-2.5,48.4+carSpeed3,0.5,0.2);
        sphere(-4.5, 48.4+carSpeed3,0.5,0.2);
        
        //car4
        //car 4-5 speed
        carSpeed4=carSpeed4-1;
        //bottom
        cube4(-51.5,-51-carSpeed4,0.5,2.5,6,0.5);
        //roof
        cube4(-51.5,-51.5-carSpeed4,-1.6,2.5,5,0.5);
        //side
        cube4(-52.5,-51.5-carSpeed4,-0.6,0.5,5,1.65);
        //side
        cube4(-50.5,-51.5-carSpeed4,-0.6,0.5,5,1.65);
        //back
        cube4(-51.5,-54-carSpeed4,-0.6,2.5,0.3,1.65);
        //front
        cube4(-51.5,-48.7-carSpeed4,-0.25,2.5,0.6,1);
        //front window
        cube3(-51.5,-49-carSpeed4,-1.25,2.5,0.05,1);
        //wheels
        cube3(-50.5,-53-carSpeed4,1,0.3,1,1);
        cube3(-52.5,-53-carSpeed4,1,0.3,1,1);
        cube3(-50.5,-49-carSpeed4,1,0.3,1,1);
        cube3(-52.5,-49-carSpeed4,1,0.3,1,1);
        
        
        glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
        sphere(52.5,48.4+carSpeed4,0.5,0.2);
        sphere(50.5, 48.4+carSpeed4,0.5,0.2);
        
        //car5
        
        //bottom
        cube4(-51.5,-51-32-carSpeed4,0.5,2.5,6,0.5);
        //roof
        cube4(-51.5,-51.5-32-carSpeed4,-1.6,2.5,5,0.5);
        //side
        cube4(-52.5,-51.5-32-carSpeed4,-0.6,0.5,5,1.65);
        //side
        cube4(-50.5,-51.5-32-carSpeed4,-0.6,0.5,5,1.65);
        //back
        cube4(-51.5,-54-32-carSpeed4,-0.6,2.5,0.3,1.65);
        //front
        cube4(-51.5,-48.7-32-carSpeed4,-0.25,2.5,0.6,1);
        //front window
        cube3(-51.5,-49-32-carSpeed4,-1.25,2.5,0.05,1);
        //wheels
        cube3(-50.5,-53-32-carSpeed4,1,0.3,1,1);
        cube3(-52.5,-53-32-carSpeed4,1,0.3,1,1);
        cube3(-50.5,-49-32-carSpeed4,1,0.3,1,1);
        cube3(-52.5,-49-32-carSpeed4,1,0.3,1,1);
        
        
        glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
        sphere(52.5,48.4+32+carSpeed4,0.5,0.2);
        sphere(50.5, 48.4+32+carSpeed4,0.5,0.2);

        
        
        /*//car6
        carSpeed6=carSpeed6-1.12;
        //bottom
        cube4(-51.5,-51-88-carSpeed6,0.5,2.5,6,0.5);
        //roof
        cube4(-51.5,-51.5-88-carSpeed6,-1.6,2.5,5,0.5);
        //side
        cube4(-52.5,-51.5-88-carSpeed6,-0.6,0.5,5,1.65);
        //side
        cube4(-50.5,-51.5-88-carSpeed6,-0.6,0.5,5,1.65);
        //back
        cube4(-51.5,-54-88-carSpeed6,-0.6,2.5,0.3,1.65);
        //front
        cube4(-51.5,-48.7-88-carSpeed6,-0.25,2.5,0.6,1);
        //front window
        cube3(-51.5,-49-88-carSpeed6,-1.25,2.5,0.05,1);
        //wheels
        cube3(-50.5,-53-88-carSpeed6,1,0.3,1,1);
        cube3(-52.5,-53-88-carSpeed6,1,0.3,1,1);
        cube3(-50.5,-49-88-carSpeed6,1,0.3,1,1);
        cube3(-52.5,-49-88-carSpeed6,1,0.3,1,1);
        
        
        glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
        sphere(52.5,48.4+88+carSpeed6,0.5,0.2);
        sphere(50.5, 48.4+88+carSpeed6,0.5,0.2);*/
        
        //car6
        carSpeed6=carSpeed6-1.12;
        //bottom
        cube4(-48.5,51-88+carSpeed6,0.5,2.5,6,0.5);
        //roof
        cube4(-48.5,51.5-88+carSpeed6,-1.6,2.5,5,0.5);
        //side
        cube4(-49.5,51.5-88+carSpeed6,-0.6,0.5,5,1.65);
        //side
        cube4(-47.5,51.5-88+carSpeed6,-0.6,0.5,5,1.65);
        //back
        cube4(-48.5,54-88+carSpeed6,-0.6,2.5,0.3,1.65);
        //front
        cube4(-48.5,48.7-88+carSpeed6,-0.25,2.5,0.6,1);
        //front window
        cube3(-48.5,49-88+carSpeed6,-1.25,2.5,0.05,1);
        //wheels
        cube3(-47.5,53-88+carSpeed6,1,0.3,1,1);
        cube3(-49.5,53-88+carSpeed6,1,0.3,1,1);
        cube3(-47.5,49-88+carSpeed6,1,0.3,1,1);
        cube3(-49.5,49-88+carSpeed6,1,0.3,1,1);
        
        
        glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
        sphere(49.5,-48.4+88-carSpeed6,0.5,0.2);
        sphere(47.5, -48.4+88-carSpeed6,0.5,0.2);




        
        glBindTexture(GL_TEXTURE_2D, gTextureBall);
        sphere(2,2,-1.21,0.3);
        
        sphere(-20,6,-1.2,0.3);
        if(carSpeed1<-350){
            carSpeed1=100;
        }
        if(carSpeed2<-330){
            carSpeed2=100;
        }
        if(carSpeed3<-280){
            carSpeed3=100;
        }
        if(carSpeed4<-330){
            carSpeed4=100;
        }
        if(carSpeed6<-150){
            carSpeed6=300;
        }
        if(camY>228){
            camY=-218;
        }
        if(camY<-228){
            camY=218;
        }
        
        if(camX>228){
            camX=-218;
        }
        if(camX<-228){
            camX=218;
        }
        


        // SwapBuffers causes the background drawing to get slapped onto the
        // display for the user to see.
        glfwSwapBuffers(window);

        // This lets GLFW monitor event queues like keyboard and mouse events.
        // It's at this time GLFW will call your callbacks to let you handle
        // the events any way you would like.
        glfwPollEvents();
    } // end of the while loop - do it all again!

    // At this point the window should be destroyed. This is the opposite routine
    // for glfwCreateWindow.
    glfwDestroyWindow(window);

    // This is the opposite of glfwInit - do some final cleanup before quitting.
    glfwTerminate();

    // Quit the program.
    exit(EXIT_SUCCESS);
}

//! [code]
