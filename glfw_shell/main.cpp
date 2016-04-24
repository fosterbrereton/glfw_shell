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

#include <GLFW/glfw3.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <OpenGL/glu.h>
#include <cmath>
#include <iomanip>
#include <unistd.h>

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
bool CarSprint{false};
bool placeCube{false};
bool fall{false};

bool MouseOut{false};

float camRotateX{-260};
float camRotateY{0};
time_t  timev;
float DecreaseClimbRate{0.1};
float IncreaseFallRate{0.05};

struct texture_t {
    explicit texture_t(const std::string& name) : name_m(name) {
    }
    
    void load() {
        id_m = SOIL_load_OGL_texture(("textures/" + name_m + ".tga").c_str(),
                                     SOIL_LOAD_AUTO,
                                     SOIL_CREATE_NEW_ID,
                                     SOIL_FLAG_POWER_OF_TWO |
                                     SOIL_FLAG_MIPMAPS |
                                     SOIL_FLAG_DDS_LOAD_DIRECT);

        if (id_m == 0) {
            std::cout << "error loading texture " + name_m + "\n";
        }
    }

    void activate() {
        glBindTexture(GL_TEXTURE_2D, id_m);
    }

private:
    std::string name_m;
    GLuint      id_m{0};
};

texture_t gTextureSteel{"steel_floor"};
texture_t gTexture{"grasstex"};
texture_t gTextureRoad{"road"};
texture_t gTextureRoadY{"yellowline"};

texture_t gTextureBall{"soccerball"};
texture_t gTextureWhite{"white"};
texture_t gTextureWood{"wood"};
texture_t gTextureLeaves{"leaves"};
texture_t gTextureClear{"clear"};

inline float DegreesToRads(float degrees){
    return degrees/180*3.14159;
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
    else if (key == GLFW_MOUSE_BUTTON_1)
    {
        placeCube=true;
    }
    else if (key == GLFW_KEY_LEFT_SHIFT)
    {
        Sprint=action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    else if (key == GLFW_KEY_E)
    {
        CarSprint=action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    else if (key == GLFW_KEY_Q)
    {
        AdminSprint=action == GLFW_PRESS || action == GLFW_REPEAT;
    }
}
void p1(float x, float y,float z, float h, float w, float d, float r, float g, float b)
{
    glColor3f(r/255,g/255,b/255);
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p2(float x, float y,float z, float h, float w, float d,float r, float g, float b)
{
    glColor3f(r/255,g/255,b/255);
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p3(float x, float y,float z, float h, float w, float d,float r, float g, float b)
{
    glColor3f(r/255,g/255,b/255);
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p4(float x, float y,float z, float h, float w, float d,float r, float g, float b)
{
    glColor3f(r/255,g/255,b/255);
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p5(float x, float y,float z, float h, float w, float d,float r, float g, float b)
{
    glColor3f(r/255,g/255,b/255);
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p6(float x, float y,float z, float h, float w, float d,float r, float g, float b)
{
    glColor3f(r/255,g/255,b/255);
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p7(float x, float y,float z, float h, float w, float d,float r, float g, float b)
{
    glColor3f(r/255,g/255,b/255);
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p8(float x, float y,float z, float h, float w, float d,float r, float g, float b)
{
    glColor3f(r/255,g/255,b/255);
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}



void p2_1(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.5f, 0.5f, 0.5f); // red
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p2_2(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.5f, 0.5f, 0.5f); // blue
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p2_3(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.5f, 0.5f, 0.5f); // gray
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p2_4(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.5f, 0.5f, 0.5f); // yellow
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p2_5(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.5f, 0.5f, 0.5f); // orange
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p2_6(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.5f, 0.5f, 0.5f); // pink
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p2_7(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.5f, 0.5f, 0.5f); // tan-ish
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p2_8(float x, float y,float z, float h, float w, float d)
{
    //glColor3f(0.5f, 0.5f, 0.5f); // white
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

void cube(float x, float y, float z, float h, float w, float d, float r, float g, float b, texture_t texFront, texture_t texRight, texture_t texLeft, texture_t texTop, texture_t texBottom, texture_t texBack){
    
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    texFront.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p5(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 1);p8(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 0);p7(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(0, 0);p6(x,y,z,h,w,d,r,g,b);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    texRight.activate();
    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p8(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 1);p4(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 0);p3(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(0, 0);p7(x,y,z,h,w,d,r,g,b);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    texLeft.activate();
    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p5(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 1);p6(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 0);p2(x,y,z,h,w,d,r,g,b);
   glTexCoord2f(0, 0); p1(x,y,z,h,w,d,r,g,b);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    texTop.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p6(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 1);p7(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 0);p3(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(0, 0);p2(x,y,z,h,w,d,r,g,b);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    texBottom.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p4(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 1);p8(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 0);p5(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(0, 0);p1(x,y,z,h,w,d,r,g,b);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    texBack.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p1(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 1); p2(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(1, 0); p3(x,y,z,h,w,d,r,g,b);
    glTexCoord2f(0, 0); p4(x,y,z,h,w,d,r,g,b);
    glEnd(); // All OpenGL drawing ends with a glEnd.
}

void cube(float x, float y, float z, float h, float w, float d, float r, float g, float b, texture_t tex){
    cube(x,y,z,h,w,d,r,g,b,tex,tex,tex,tex,tex,tex);
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


    void cube2(float x, float y, float z, float h, float w, float d,texture_t texFront, texture_t texRight, texture_t texLeft, texture_t texTop, texture_t texBottom, texture_t texBack){
        texFront.activate();
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
        texRight.activate();
        glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
        glTexCoord2f(0, 30); p4_8(x,y,z,h,w,d);
        glTexCoord2f(30, 30); p4_4(x,y,z,h,w,d);
        glTexCoord2f(30, 0); p4_3(x,y,z,h,w,d);
        glTexCoord2f(0, 0); p4_7(x,y,z,h,w,d);
        glEnd(); // All OpenGL drawing ends with a glEnd.

        // Left face
        texLeft.activate();
        glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
        glTexCoord2f(0, 30); p4_5(x,y,z,h,w,d);
        glTexCoord2f(30, 30); p4_6(x,y,z,h,w,d);
        glTexCoord2f(30, 0); p4_2(x,y,z,h,w,d);
        glTexCoord2f(0, 0); p4_1(x,y,z,h,w,d);
        glEnd(); // All OpenGL drawing ends with a glEnd.
        // Top face
        texTop.activate();
        glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
        glTexCoord2f(0, 30); p4_6(x,y,z,h,w,d);
        glTexCoord2f(30, 30); p4_7(x,y,z,h,w,d);
        glTexCoord2f(30, 0); p4_3(x,y,z,h,w,d);
        glTexCoord2f(0, 0); p4_2(x,y,z,h,w,d);
        glEnd(); // All OpenGL drawing ends with a glEnd.
        // Bottom face
        texBottom.activate();
        glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
        glTexCoord2f(0, 30); p4_4(x,y,z,h,w,d);
        glTexCoord2f(30, 30); p4_8(x,y,z,h,w,d);
        glTexCoord2f(30, 0); p4_5(x,y,z,h,w,d);
        glTexCoord2f(0, 0); p4_1(x,y,z,h,w,d);
        glEnd(); // All OpenGL drawing ends with a glEnd.
        // Back face
        texBack.activate();
        glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.

        glTexCoord2f(0, 30); p4_1(x,y,z,h,w,d);
        glTexCoord2f(30, 30); p4_2(x,y,z,h,w,d);
        glTexCoord2f(30, 0); p4_3(x,y,z,h,w,d);
        glTexCoord2f(0, 0); p4_4(x,y,z,h,w,d);
        glEnd(); // All OpenGL drawing ends with a glEnd.
    }


void cube2(float x, float y, float z, float h, float w, float d, texture_t tex){
    cube2(x,y,z,h,w,d,tex,tex,tex,tex,tex,tex);
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
void clyinder(float x,float y,float z,float size, float base, float top, float height){
    //glMatrixMode(GL_MODELVIEW);
    glPushMatrix();
    //glLoadIdentity();
    glTranslatef(-x, -y, -z);
    
    GLUquadricObj*quad=gluNewQuadric();
    gluQuadricTexture( quad, GL_TRUE);
    gluCylinder(quad, base, top , height , 15, 15);
    gluDeleteQuadric(quad);
    glPopMatrix();
    
}
void tree(float x, float y, float z){
    glColor3f(1, 1, 1);
    gTextureWood.activate();
    clyinder(-x,-y,z+5,5, 0.8, 0.8, 20);
    cube(x,y,z+-4,4,0.000001,4,0,128,0,gTextureLeaves);
    cube(x,y,z+-4,4,4,0.000001,0,128,0,gTextureLeaves);
    cube(x,y,z+-4,0.000001,4,4,0,128,0,gTextureLeaves);
    cube(x,y,z+-5,4,4,0.000001,0,128,0,gTextureLeaves);
    cube(x,y,z+-4,3.8,3.8,3.8,0,128,0,gTextureLeaves);
    
}
struct tree_t {
    double x_m; // This is a member variable of the class.
    double y_m; // we say "_m" to be able to see the member variables more easily.
    double z_m;
    
    void draw();
};


// This is a member function of the class.
void tree_t::draw() {
    gTextureWood.activate();
    clyinder(-x_m,-y_m,z_m+5,5, 0.8, 0.8, 20);
    cube(x_m,y_m,z_m+-4,4,0.000001,4,0,0,0,gTextureLeaves);
    cube(x_m,y_m,z_m+-4,4,4,0.000001,0,0,0,gTextureLeaves);
    cube(x_m,y_m,z_m+-4,0.000001,4,4,0,0,0,gTextureLeaves);
    cube(x_m,y_m,z_m+-5,4,4,0.000001,0,0,0,gTextureLeaves);
    cube(x_m,y_m,z_m+-4,3.8,3.8,3.8,0,0,0,gTextureLeaves);
}

struct cubeD_D {
    double x_m; // This is a member variable of the class.
    double y_m; // we say "_m" to be able to see the member variables more easily.
    double z_m;
    double h_m;
    double w_m;
    double d_m;
    
    double r_m;
    double g_m;
    double b_m;
    texture_t tex1;
    texture_t tex2;
    texture_t tex3;
    texture_t tex4;
    texture_t tex5;
    texture_t tex6;
    
    void draw();
};

void cubeD_D::draw() {
   
    tex1.activate();
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p5(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1); p8(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0); p7(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0); p6(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    tex2.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p8(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1);p4(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0);p3(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0);p7(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    tex3.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p5(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1);p6(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0);p2(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0);p1(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    tex4.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p6(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1);p7(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0);p3(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0);p2(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    tex5.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p4(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1); p8(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0); p5(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0); p1(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    tex6.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p1(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1); p2(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0); p3(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0); p4(x_m,y_m,z_m,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    
}




void officePlant(float x, float y, float z){
    cube(x,y,1.2+z,0.5,0.5,0.5,0,0,0,gTextureSteel);
    
    cube(x,y,1.2+z,0.3,0.3,1,0,0,0,gTextureLeaves);
    cube(x,y,0.9+z,0.3,0.3,1,0,0,0,gTextureLeaves);
    cube(x,y,0.6+z,0.3,0.3,1,0,0,0,gTextureLeaves);
    cube(x,y,0.3+z,0.3,0.3,1,0,0,0,gTextureLeaves);
}
void officeCube(float x, float y, float z){
    //table 1
    //legs
    //cube3(30,-6,1,0.1,0.1,1);
    //cube3(29,-6,1,0.1,0.1,1);
    
    //top
    cube(29.5+x,-5+y,0.5+z,1.5,3,0.1,255,255,255,gTextureSteel);
    
    //table 2
    //legs
    cube(30+1.3+x,-6+1.2+y,1+z,0.1,0.1,1,0,0,0,gTexture);
    cube(30+1.3+x,-5+1.2+y,1+z,0.1,0.1,1,0,0,0,gTexture);
    cube(29+1+x,-6+1.2+y,1,0.1+z,0.1,1,0,0,0,gTexture);
    //top
    cube(29+1.3+x,-5.5+1.2+y,0.5+z,3,1.5,0.1,255,255,255,gTextureSteel);
    
    //table 3
    //legs
    cube(30+1.3+x,-8.9+1.2+y,1+z,0.1,0.1,1,0,0,0,gTexture);
    cube(30+1.3+x,-7.9+1.2+y,1+z,0.1,0.1,1,0,0,0,gTexture);
    cube(29+1+x,-8+1.2+y,1,0.1+z,0.1,1,0,0,0,gTexture);
    //top
    cube(29+1.3+x,-8.4+1.2+y,0.5+z,3,1.5,0.1,255,255,255,gTextureSteel);
    
    
    //walls
    cube(28.8+x,-5.8+y,0.5+z,0.1,4.5,3,0,0,0,gTextureSteel);
    cube(30.8+x,-3.5+y,0.5+z,4,0.1,3,0,0,0,gTextureSteel);
    cube(30.8+x,-8+y,0.5+z,4,0.1,3,0,0,0,gTextureSteel);
    
    //computer
    cube(29.1+x,-6+y,0+z,0.1,1,0.7,0,0,0,gTexture);
    cube(29.5+x,-6+y,0.4+z,0.7,1,0.1,255,255,255,gTextureSteel);
    
    cube(29.5+x,-6.8+y,0.4+z,0.3,0.2,0.1,255,255,255,gTextureSteel);
    
    //paper
    cube(29.5+x,-4+y,0.4+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.41+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.42+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.43+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.44+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.45+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.46+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.47+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.48+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.49+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.5+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.51+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.52+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.53+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.54+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    cube(29.5+x,-4+y,0.55+z,0.7,1,0.00001,255,255,255,gTextureSteel);
    
    officePlant(32.5+x,-7.5+y,0+z);
}

void officeFloor(float x, float y, float z){
    officeCube(-68.3+x-5,-3.5+y-20,0+z);
    officeCube(-68.3+x-5,1+y-20,0);
    officeCube(-68.3+x-5,5.5+y-20,0+z);
    officeCube(-68.3+x-5,10+y-20,0+z);
    officeCube(-68.3+x-5,14.5+y-20,0+z);
    
    officeCube(-68.3+7+x-5,-3.5+y-20,0+z);
    officeCube(-68.3+7+x-5,1+y-20,0+z);
    officeCube(-68.3+7+x-5,5.5+y-20,0+z);
    officeCube(-68.3+7+x-5,10+y-20,0+z);
    
    
    officeCube(-68.3+14+x-5,-3.5+y-20,0+z);
    officeCube(-68.3+14+x-5,1+y-20,0+z);
    officeCube(-68.3+14+x-5,5.5+y-20,0+z);
    officeCube(-68.3+14+x-5,10+y-20,0+z);
}

void officeB(float x,float y,float z){
    //office building
    //walls
    cube(-20+x,-20+y,0+z,1,25,70,255,255,255,gTextureSteel);
    cube(-32+x,-32+y,0+z,25,1,70,255,255,255,gTextureSteel);
    cube(-45+x,-20+y,0+z,1,25,70,255,255,255,gTextureSteel);
    cube(-32+x,-7+y,0+z,25,1,70,255,255,255,gTextureSteel);
    //roof
    cube(-32.5+x,-19.5+y,-30+z,25,25,1,255,255,255,gTextureSteel);
    cube(-32.5+x,-19.5+y,-5+z,25,25,1,255,255,255,gTextureSteel);
    //floor
    cube(-32.5+x,-19.5+y,1.9+z,25,25,1,255,255,255,gTextureSteel);
    
    //door
    cube(-19.49+x,-10+y,1+z,0.2,2,4,255,255,255,gTexture);
    cube(-19.3+x,-10.7+y,0.1+z,0.2,0.2,0.2,255,255,255,gTextureSteel);
    
    cube(-20.53+x,-10+y,1+z,0.2,2,4,255,255,255,gTexture);
    cube(-20.99+x,-10.7+y,0.1+z,0.2,0.2,0.2,255,255,255,gTextureSteel);
    
    
    
    tree(-10.8+x-5,12.8+y-20,0+z);
    tree(-10.8+x-5,-12.8+y-20,0+z);
    
    
    cube(-10+x-5,0+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,2+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,-2+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,4+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,-4+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,6+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,-6+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,8+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,-8+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,-10+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,-12+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    cube(-10+x-5,12+y-20,1+z,0.5,2,1,255,255,255,gTextureLeaves);
    
    cube(-10.8+x-5,-12.8+y-20,1+z,2,0.5,1,255,255,255,gTextureLeaves);
    cube(-12.8+x-5,-12.8+y-20,1+z,2,0.5,1,255,255,255,gTextureLeaves);
    cube(-14.8+x-5,-12.8+y-20,1+z,2,0.5,1,255,255,255,gTextureLeaves);
    
    cube(-10.8+x-5,12.8+y-20,1+z,2,0.5,1,255,255,255,gTextureLeaves);
    cube(-12.8+x-5,12.8+y-20,1+z,2,0.5,1,255,255,255,gTextureLeaves);
    cube(-14.8+x-5,12.8+y-20,1+z,2,0.5,1,255,255,255,gTextureLeaves);
    
    
}

void roadH(float x, float y, float z){
    cube2(0.5+x,222+y,1.49+z,450,6,0.000001,gTextureRoad);
    cube(0.5+x,222+y,1.48+z,450,0.5,0.000001,255,255,0,gTextureRoadY);
}
void roadV(float x, float y, float z){
    cube2(5+x,1+y,1.49+z,6,450,0.000001,gTextureRoad);
    cube(5+x,1+y,1.48+z,0.5,450,0.000001,255,255,0,gTextureRoadY);
}





int main(void)
{
    chdir(getenv("HOME"));
    std::srand(std::time(NULL));
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

         //comment this out to go to normal colors
    
        glEnable(GL_TEXTURE_2D);
        //glBindTexture(GL_TEXTURE_2D, gTextureRoadY);
    
    
    gTextureSteel.load();
    gTexture.load();
    gTextureWood.load();
    gTextureLeaves.load();
    gTextureRoad.load();
    gTextureRoadY.load();
    gTextureWhite.load();
    
    gTextureBall.load();
    gTextureClear.load();
    
cubeD_D myCube{0,0,0,1,1,1,255,255,255,gTextureBall,gTextureBall,gTextureBall,gTextureBall,gTextureBall,gTextureBall};
    
    


    
    float carSpeed1{100};
    float carSpeed2{100};
    float carSpeed3{100};
    float carSpeed4{100};
    
    float carSpeed6{-100};
    
    float carStop{1};
    float carStop2{1.12};
    float carStop3{0.6};
    
    float car4x{0};
    float car6x{0};
    float car1x{0};
    std::vector<tree_t> tree_vector;
    for (std::size_t i(0); i < 50; ++i) {
        tree_vector.push_back({rand()%440-220.,rand()%440-220.,0});
    }
    
    //std::vector<cubeD_D> cubeD_vector;
    
    //tree_t tree1{rand()%100+0.,rand()%100+0.,0};

    static const float simulation_start_k = glfwGetTime();
    static const float real_min_per_game_day_k = 0.5; // CHANGE ONLY HERE TO AFFECT DAY/NIGHT SPEED
    static const float real_sec_per_game_day_k = real_min_per_game_day_k * 60;
    static const float real_sec_per_game_hrs_k = real_sec_per_game_day_k / 24;
    static const float real_sec_per_game_min_k = real_sec_per_game_hrs_k / 60;
    static const float game_min_per_real_sec_k = 1 / real_sec_per_game_min_k;
    static const float min_per_day_k = 24 * 60;
   

    
    // This is the main processing loop that draws the spinning rectangle.
    while (!glfwWindowShouldClose(window)) // this will loop until the window should close.
    {
        float elapsed_real_sec = glfwGetTime() - simulation_start_k;
        float elapsed_game_min = game_min_per_real_sec_k * elapsed_real_sec;
        float elapsed_game_hrs = elapsed_game_min / 60;
        float percent_of_day = (static_cast<int>(elapsed_game_min) % static_cast<int>(min_per_day_k)) / min_per_day_k;
        float sky_cycle = std::sin(percent_of_day * M_PI);
        float sky = 0 * (1-sky_cycle) + 0.9803921569 * sky_cycle;
        int game_hrs_mil = static_cast<int>(elapsed_game_hrs) % 24; // military hours

        std::cout.width(2);
        std::cout.fill('0');
        std::cout << game_hrs_mil << ":";
        std::cout.width(2);
        std::cout << (static_cast<int>(elapsed_game_min)%60) << '\n';

        // These are variable spaces.
        float ratio; // this is a floating point number
        int width, height; // these variables store the dimensions of the window

        glfwGetFramebufferSize(window, &width, &height); // Get the height and width of the window from GLFW.
        ratio = width / (float) height; // compute the aspect ratio of the window, which we need below.

        glViewport(0, 0, width, height); // This tells OpenGL how big the window is,
        glClearColor(0.5294117648+sky-0.9803921569, 0.8078431373+sky-0.9803921569, sky, 0);                              // and OpenGL goes off and creates a space
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

    

        //cube5(10,10,-5,10,4,4);
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
        if(Sprint && CarSprint){
            camY += std::cos(DegreesToRads(camRotateY))*-0.55;
            camX += std::sin(DegreesToRads(camRotateY))*-0.55;
        }
        if(CarSprint){
            camY += std::cos(DegreesToRads(camRotateY))*1;
            camX += std::sin(DegreesToRads(camRotateY))*1;
        }
        if(AdminSprint){
            camY += std::cos(DegreesToRads(camRotateY))*2.5;
            camX += std::sin(DegreesToRads(camRotateY))*2.5;
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

        gTexture.activate();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.

        glTexCoord2f(0, 450); glVertex3f(-450, 450, 1.5);
        glTexCoord2f(450, 450); glVertex3f(450, 450, 1.5);
        glTexCoord2f(450, 0); glVertex3f(450, -450, 1.5);
        glTexCoord2f(0, 0); glVertex3f(-450, -450, 1.5);

        glEnd(); // All OpenGL drawing ends with a glEnd.

        //If you would like to make a custom make change this to true v
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        bool customMap{true};
        if(customMap){
            //change this float to change the size of your world!
            float worldSize{1};
            //put all your code in here!
            //there are several built in structures
            //1 is officeB which places an offic building
            //2 is roadH and roadV which are vertical and horizontal roads
            //officePlant a small splant
            //officeFloor a floor of office cubicals
            //officeCube one office cubical
            //the other shapes include
            //sphere
            //cube1, cube2, cube3, cube4,vcube5,vand cube6
    
                //to use non-shape built in function put the name first and then x y z
            //for example
            //        x  y  z
            //officeB(10,28,3);
    
                //to use the cube you have to do x y z and height, width, and depth
            //for example
            //      x  y  z  h  w d
            //cube1(38,39,10,1,30,1);
            //for spheres there is x y z and size
            //for example
            //       x  y  z  size
            //sphere(13,34,4,  5 );
            // now go program your world!
    

                //grass
            myCube.draw();
            officeB(0,0,0);
            officeFloor(0, 0, 0);
    
                if(worldSize==1){
        
                    }
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
