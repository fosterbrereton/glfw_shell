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

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
float camX{0};
float camY{0};
float camZ{0};


float camRotateX{-260};
float camRotateY{-50};


float DegreesToRads(float Degrees){
    return Degrees/180*3.14159;
    
    
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
        glfwSetWindowShouldClose(window, GL_TRUE);
        }
    else if (key == GLFW_KEY_R && action == GLFW_PRESS)
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
        }
    else if (key == GLFW_KEY_D && action == GLFW_PRESS)
    {
        
        camY += std::cos(DegreesToRads(camRotateY-90));
        camX += std::sin(DegreesToRads(camRotateY-90));

    }
    else if (key == GLFW_KEY_A && action == GLFW_PRESS)
    {
        camY += std::cos(DegreesToRads(camRotateY+90));
        camX += std::sin(DegreesToRads(camRotateY+90));

    }
    else if (key == GLFW_KEY_S && action == GLFW_PRESS)
    {
        camY -= std::cos(DegreesToRads(camRotateY));
        camX -= std::sin(DegreesToRads(camRotateY));

    }
    else if (key == GLFW_KEY_W && action == GLFW_PRESS)
    {
        camY += std::cos(DegreesToRads(camRotateY));
        camX += std::sin(DegreesToRads(camRotateY));
    }
    else if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
    {
        camRotateX += 10;
    }
    else if (key == GLFW_KEY_UP && action == GLFW_PRESS)
    {
        camRotateX -= 10;
    }
    else if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        camRotateY += 10;
    }
    else if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        camRotateY -= 10;
    }
    else if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
    {
        camZ += 0.5;
    }
    else if (key == GLFW_KEY_LEFT_SHIFT && action == GLFW_PRESS)
    {
        camZ -= 0.5;
    }
    
    
  

    
    
    
    
    else
        {
        std::string actionString;

        if (action == GLFW_PRESS)
            actionString = "DOWN";
        else
            actionString = "UP";

        std::cout << "You pressed " << actionString << " on the key " << key << '\n';
        }
    std::cout << "Camera X = " << camX << '\n';
    std::cout << "Camera Y = " << camY << '\n';
    std::cout << "Camera Z = " << camZ << '\n';
    
    std::cout << "Camera Rotate X =" << camRotateX << '\n';
    std::cout << "Camera Rotate Y =" << camRotateY << '\n';
}
void p1(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p2(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p3(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p4(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 0.8431372549f, 0.f); // yellow
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p5(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p6(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p7(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p8(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 0.8431372549f, 0.f);
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}



void p2_1(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.3f, 0.3f, 0.3f); // red
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p2_2(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.3f, 0.3f, 0.3f); // blue
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p2_3(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.3f, 0.3f, 0.3f); // gray
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p2_4(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.3f, 0.3f, 0.3f); // yellow
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p2_5(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.3f, 0.3f, 0.3f); // orange
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p2_6(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 1.0f, 1.0f); // pink
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p2_7(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 1.0f, 1.0f); // tan-ish
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p2_8(float x, float y,float z, float h, float w, float d)
{
    glColor3f(1.0f, 1.0f, 1.0f); // white
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}


void p3_1(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.4f, 0.4f, 0.0f); // red
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p3_2(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.4f, 0.4f, 0.0f); // blue
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p3_3(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.4f, 0.4f, 0.0f); // gray
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p3_4(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.4f, 0.4f, 0.0f); // yellow
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p3_5(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.4f, 0.4f, 0.0f); // orange
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p3_6(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.4f, 0.4f, 0.0f); // pink
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p3_7(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.4f, 0.4f, 0.0f); // tan-ish
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p3_8(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.4f, 0.4f, 0.0f); // white
    glVertex3f(h/2+x, -w/2+y, d/2+z);
}




//Color = black
void p4_1(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(-h/2+x, -w/2+y, -d/2+z);
}
void p4_2(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(-h/2+x, w/2+y, -d/2+z);
}
void p4_3(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(h/2+x, w/2+y, -d/2+z);
}
void p4_4(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(h/2+x, -w/2+y, -d/2+z);
}

void p4_5(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(-h/2+x, -w/2+y, d/2+z);
}
void p4_6(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(-h/2+x, w/2+y, d/2+z);
}
void p4_7(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.f, 0.f, 0.0f);
    glVertex3f(h/2+x, w/2+y, d/2+z);
}
void p4_8(float x, float y,float z, float h, float w, float d)
{
    glColor3f(0.f, 0.f, 0.0f);
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


void cube(float x, float y, float z, float h, float w, float d){
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
    p1(x,y,z,h,w,d);
    p2(x,y,z,h,w,d);
    p3(x,y,z,h,w,d);
    p4(x,y,z,h,w,d);
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
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p2_5(x,y,z,h,w,d);
    p2_8(x,y,z,h,w,d);
    p2_7(x,y,z,h,w,d);
    p2_6(x,y,z,h,w,d);
    
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p2_8(x,y,z,h,w,d);
    p2_4(x,y,z,h,w,d);
    p2_3(x,y,z,h,w,d);
    p2_7(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p2_5(x,y,z,h,w,d);
    p2_6(x,y,z,h,w,d);
    p2_2(x,y,z,h,w,d);
    p2_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p2_6(x,y,z,h,w,d);
    p2_7(x,y,z,h,w,d);
    p2_3(x,y,z,h,w,d);
    p2_2(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p2_4(x,y,z,h,w,d);
    p2_8(x,y,z,h,w,d);
    p2_5(x,y,z,h,w,d);
    p2_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p2_1(x,y,z,h,w,d);
    p2_2(x,y,z,h,w,d);
    p2_3(x,y,z,h,w,d);
    p2_4(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
}

void cube3(float x, float y, float z, float h, float w, float d){
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
    p3_1(x,y,z,h,w,d);
    p3_2(x,y,z,h,w,d);
    p3_3(x,y,z,h,w,d);
    p3_4(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
}

void cube4(float x, float y, float z, float h, float w, float d){
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p4_5(x,y,z,h,w,d);
    p4_8(x,y,z,h,w,d);
    p4_7(x,y,z,h,w,d);
    p4_6(x,y,z,h,w,d);
    
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p4_8(x,y,z,h,w,d);
    p4_4(x,y,z,h,w,d);
    p4_3(x,y,z,h,w,d);
    p4_7(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p4_5(x,y,z,h,w,d);
    p4_6(x,y,z,h,w,d);
    p4_2(x,y,z,h,w,d);
    p4_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p4_6(x,y,z,h,w,d);
    p4_7(x,y,z,h,w,d);
    p4_3(x,y,z,h,w,d);
    p4_2(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p4_4(x,y,z,h,w,d);
    p4_8(x,y,z,h,w,d);
    p4_5(x,y,z,h,w,d);
    p4_1(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p4_1(x,y,z,h,w,d);
    p4_2(x,y,z,h,w,d);
    p4_3(x,y,z,h,w,d);
    p4_4(x,y,z,h,w,d);
    glEnd(); // All OpenGL drawing ends with a glEnd.
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

    // Set some OpenGL world options.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

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
        gluPerspective(90, ratio, 1, 1000);
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

        glTranslatef(camX+2,camY-2.5,camZ);
        
      

        cube2(0,0,0,0.125,0.125,1);
        cube2(0.5,0,0,0.125,0.125,1);
        cube2(0,0.5,0,0.125,0.125,1);
        cube2(0.5,0.5,0,0.125,0.125,1);
        
        cube2(0.25,0.25,-0.5,1,1,0.5);
        
        cube3(0.5,0.5,1.75,100,100,0.5);
        
        cube2(0,0,1,0.5,0.5,1);
        cube2(0.5,0,1,0.5,0.5,1);
        cube2(0,0.5,1,0.5,0.5,1);
        cube2(0.5,0.5,1,0.5,0.5,1);
        
        triangle(1,2,1,1,1,1);
        triangle(0,2,1,1,1,1);
        
        cube4(5,1,1.49,6,50,0.000001);
        
        cube(5,1,1.48,0.5,50,0.000001);
        
        //door
        cube4(10.49,1,1,0.2,2,4);
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
