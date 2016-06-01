// Seth's Simulator!

// stdc+++
#include <cmath>
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <vector>

// OpenGL
#include <OpenGL/glu.h>

// soil
// #include "SOIL.h"

// GLFW
#include <GLFW/glfw3.h>

// ODE
#include "ode/ode.h"

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}

float gCamRotX{-70};
float gCamRotY{140};

// ODE global variables
static dWorldID      gODEWorld;
static dSpaceID      gODESpace;
static dJointGroupID gODEContactGroup;

static void ODEContactCallback (void *data, dGeomID o1, dGeomID o2)
{
    dContact contact[32];

    int numc = dCollide(o1, o2, 32, &(contact[0].geom), sizeof(dContact));

    for (int i(0); i < numc; ++i) {
        contact[i].surface.slip1 = 0.7;
        contact[i].surface.slip2 = 0.7;
        contact[i].surface.mode = dContactSoftERP | dContactSoftCFM | dContactApprox1 | dContactSlip1 | dContactSlip2;
        contact[i].surface.mu = 50.0; // was: dInfinity
        contact[i].surface.soft_erp = 0.96;
        contact[i].surface.soft_cfm = 0.04;

        dJointID c = dJointCreateContact (gODEWorld, gODEContactGroup, &contact[i]);

        dJointAttach(c, dGeomGetBody(contact[i].geom.g1), dGeomGetBody(contact[i].geom.g2));
    }
}

inline float DegreesToRads(float degrees){
    return degrees / 180 * M_PI;
}

static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    static double oldx{xpos};
    static double oldy{ypos};
    
    double deltaX{xpos-oldx};
    double deltaY{ypos-oldy};
    
    gCamRotY += deltaX;
    gCamRotX += deltaY;

    oldx = xpos;
    oldy = ypos;
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
}

inline void c1(float halfx, float halfy, float halfz)
{
    glVertex3f(-halfx, -halfy, -halfz);
}

inline void c2(float halfx, float halfy, float halfz)
{
    glVertex3f(-halfx, halfy, -halfz);
}

inline void c3(float halfx, float halfy, float halfz)
{
    glVertex3f(halfx, halfy, -halfz);
}

inline void c4(float halfx, float halfy, float halfz)
{
    glVertex3f(halfx, -halfy, -halfz);
}

inline void c5(float halfx, float halfy, float halfz)
{
    glVertex3f(-halfx, -halfy, halfz);
}

inline void c6(float halfx, float halfy, float halfz)
{
    glVertex3f(-halfx, halfy, halfz);
}

inline void c7(float halfx, float halfy, float halfz)
{
    glVertex3f(halfx, halfy, halfz);
}

inline void c8(float halfx, float halfy, float halfz)
{
    glVertex3f(halfx, -halfy, halfz);
}

void draw_axis(float x, float y, float z) {
    glDisable(GL_TEXTURE_2D);
    glBegin(GL_LINES);
        glColor3f(1, 0, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(x, 0, 0);

        glColor3f(0, 1, 0);
        glVertex3f(0, 0, 0);
        glVertex3f(0, y, 0);

        glColor3f(0, 0, 1);
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, z);
    glEnd(); // GL_LINES
    glEnable(GL_TEXTURE_2D);
}

inline void draw_axis(float size) {
    draw_axis(size, size, size);
}

void orient_body_in_opengl(dBodyID body) {
    const dReal* pos = dBodyGetPosition(body);
    const dReal* rot = dBodyGetRotation(body);

    GLfloat matrix[16] = {
        rot[0], rot[1], rot[2],  0,
        rot[4], rot[5], rot[6],  0,
        rot[8], rot[9], rot[10], 0,
        pos[0], pos[1], pos[2],  1
    };

    glMultMatrixf(matrix);
}

struct cube_t {
    explicit cube_t(float x, float y, float z) {
        body_m = dBodyCreate(gODEWorld);

        dBodySetPosition(body_m, x, y, z);

        dMatrix3 R;

        dRFromAxisAndAngle(R, dRandReal() * 2.0 - 1.0,
                              dRandReal() * 2.0 - 1.0,
                              dRandReal() * 2.0 - 1.0,
                              dRandReal() * 10.0 - 5.0);

        dBodySetRotation(body_m, R);

        geom_m = dCreateBox(gODESpace, 1, 1, 1);

        dGeomSetBody(geom_m, body_m);
    }

    void draw();

    float   red_m{0};
    float   green_m{0};
    float   blue_m{1};
    dBodyID body_m;
    dGeomID geom_m;
};

void cube_t::draw() {
    glPushMatrix();

    orient_body_in_opengl(body_m);

    float halfx = 0.5;
    float halfy = 0.5;
    float halfz = 0.5;

    draw_axis(1, 1, 1);

    glColor3f(red_m, green_m, blue_m);

    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    c5(halfx, halfy, halfz);
    c8(halfx, halfy, halfz);
    c7(halfx, halfy, halfz);
    c6(halfx, halfy, halfz);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    // Right face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    c8(halfx, halfy, halfz);
    c4(halfx, halfy, halfz);
    c3(halfx, halfy, halfz);
    c7(halfx, halfy, halfz);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    // Left face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    c5(halfx, halfy, halfz);
    c6(halfx, halfy, halfz);
    c2(halfx, halfy, halfz);
    c1(halfx, halfy, halfz);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    // Top face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    c6(halfx, halfy, halfz);
    c7(halfx, halfy, halfz);
    c3(halfx, halfy, halfz);
    c2(halfx, halfy, halfz);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    // Bottom face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    c4(halfx, halfy, halfz);
    c8(halfx, halfy, halfz);
    c5(halfx, halfy, halfz);
    c1(halfx, halfy, halfz);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    // Back face
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    c1(halfx, halfy, halfz);
    c2(halfx, halfy, halfz);
    c3(halfx, halfy, halfz);
    c4(halfx, halfy, halfz);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    glPopMatrix();
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

    // ODE initialization
    dInitODE();
    gODEWorld = dWorldCreate();
    gODESpace = dHashSpaceCreate(0);
    dWorldSetGravity(gODEWorld, 0, 0, -0.2);
    dWorldSetERP(gODEWorld, 0.2);
    dWorldSetCFM(gODEWorld, 1e-5);
    dCreatePlane(gODESpace, 0, 0, 1, 0); // create the base plane
    gODEContactGroup = dJointGroupCreate (0);

    // Builds a new GLFW window and saves the result in the variable above.
    // If there's an error here, window will be set to 0.
    // 640x480 is the initial size, and "Simple example" is the name of the window.
    window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);

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
    
    // Set some OpenGL gODEWorld options.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    // Set up a basic ODE object
    cube_t cube1(0, 0, 3);

    cube_t cube2(0, 0, 5);
    cube2.red_m = 1;
    cube2.blue_m = 0;

    cube_t cube3(0, 0, 7);
    cube3.green_m = 1;
    cube3.blue_m = 0;

    // This is the main processing loop that draws the spinning rectangle.
    while (!glfwWindowShouldClose(window)) // this will loop until the window should close.
    {
        // Simulate the physics engine
        // find collisions and add contact joints
        dSpaceCollide (gODESpace, 0, &ODEContactCallback);
        // step the simulation
        dWorldQuickStep (gODEWorld, 0.1);  
        // remove all contact joints
        dJointGroupEmpty (gODEContactGroup);

        // These are variable declarations.
        int width, height; // these variables store the dimensions of the window

        glfwGetFramebufferSize(window, &width, &height); // Get the height and width of the window from GLFW.
        float ratio = width / (float) height; // compute the aspect ratio of the window, which we need below.

        glViewport(0, 0, width, height); // This tells OpenGL how big the window is,
        glClearColor(0, 1, 0, 0); // and OpenGL goes off and creates a space for drawing.
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

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glRotatef(gCamRotX, 1.f, 0.f, 0.f);
        glRotatef(gCamRotY, 0.f, 0.f, 1.f);

        glTranslatef(2, -2.5, -2);

        glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
            glColor3f(1, 1, 1);
            glVertex3f(-450, -450, 0);
            glVertex3f(450, -450 ,0);
            glVertex3f(450, 450, 0);
            glVertex3f(-450, 450, 0);
        glEnd(); // All OpenGL drawing ends with a glEnd.

        draw_axis(2);

        cube1.draw();
        cube2.draw();
        cube3.draw();
        
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

    // ODE teardown
    dJointGroupDestroy (gODEContactGroup);
    dSpaceDestroy (gODESpace);
    dWorldDestroy (gODEWorld);
    dCloseODE();

    // This is the opposite of glfwInit - do some final cleanup before quitting.
    glfwTerminate();

    // Quit the program.
    exit(EXIT_SUCCESS);
}
