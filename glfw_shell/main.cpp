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

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
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
    else
        {
        std::string actionString;

        if (action == GLFW_PRESS)
            actionString = "DOWN";
        else
            actionString = "UP";

        std::cout << "You pressed " << actionString << " on the key " << key << '\n';
        }
}
void p1(float x, float y,float z)
{
    glColor3f(1.f, 0.f, 0.f); // red
    glVertex3f(-0.5+x, -0.5+y, -0.5+z);
}
void p2(float x, float y,float z)
{
    glColor3f(0.f, 0.f, 1.0f); // blue
    glVertex3f(-0.5+x, 0.5+y, -0.5+z);
}
void p3(float x, float y,float z)
{
    glColor3f(0.5f, 0.5f, 0.5f); // gray
    glVertex3f(0.5+x, 0.5+y, -0.5+z);
}
void p4(float x, float y,float z)
{
    glColor3f(1.0f, 1.0f, 0.f); // yellow
    glVertex3f(0.5+x, -0.5+y, -0.5+z);
}

void p5(float x, float y,float z)
{
    glColor3f(1.f, 0.4666666667f, 0.f); // orange
    glVertex3f(-0.5+x, -0.5+y, 0.5+z);
}
void p6(float x, float y,float z)
{
    glColor3f(1.f, 0.f, 0.6352941176f); // pink
    glVertex3f(-0.5+x, 0.5+y, 0.5+z);
}
void p7(float x, float y,float z)
{
    glColor3f(1.f, 0.8f, 0.6f); // tan-ish
    glVertex3f(0.5+x, 0.5+y, 0.5+z);
}
void p8(float x, float y,float z)
{
    glColor3f(1.0f, 1.0f, 1.0f); // white
    glVertex3f(0.5+x, -0.5+y, 0.5+z);
}
void cube(float x, float y, float z){
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p5(x,y,z);
    p8(x,y,z);
    p7(x,y,z);
    p6(x,y,z);
    
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p8(x,y,z);
    p4(x,y,z);
    p3(x,y,z);
    p7(x,y,z);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p5(x,y,z);
    p6(x,y,z);
    p2(x,y,z);
    p1(x,y,z);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Top face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p6(x,y,z);
    p7(x,y,z);
    p3(x,y,z);
    p2(x,y,z);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Bottom face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p4(x,y,z);
    p8(x,y,z);
    p5(x,y,z);
    p1(x,y,z);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    // Back face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    p1(x,y,z);
    p2(x,y,z);
    p3(x,y,z);
    p4(x,y,z);
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
    glEnable(GL_CULL_FACE);
    //glClearDepth(0.0f);
    //glDepthFunc(GL_LESS);
    glEnable(GL_DEPTH_TEST);
    // This is the main processing loop that draws the spinning rectangle.
    while (!glfwWindowShouldClose(window)) // this will loop until the window should close.
    {
        // These are variable spaces.
        float ratio; // this is a floating point number
        int width, height; // these variables store the dimensions of the window

        glfwGetFramebufferSize(window, &width, &height); // Get the height and width of the window from GLFW.
        ratio = width / (float) height; // compute the aspect ratio of the window, which we need below.
        
        glViewport(0, 0, width, height); // This tells OpenGL how big the window is,
                                         // and OpenGL goes off and creates a space
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
        glOrtho(-ratio, ratio, -1.f, 1.f, 20.f, -20.f);
        glMatrixMode(GL_MODELVIEW);

        /*
            These operations tell OpenGL that we want to rotate the 3D world that
            we are drawing. The call to glfwGetTime is constantly changing as time
            marches on, and the * 50 determines how fast the rotation should be.
        */
        glLoadIdentity();
        glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
        glRotatef((float) glfwGetTime() * 30.f, 0.f, 1.f, 0.f);
        glRotatef((float) glfwGetTime() * 60.f, 1.f, 0.f, 0.f);

      

        cube(0,0,1);
        cube(1,0,0);

        
        

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
