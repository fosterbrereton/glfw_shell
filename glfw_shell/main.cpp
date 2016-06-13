// Seth's Simulator!

#include <GLFW/glfw3.h>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <OpenGL/glu.h>
#include <cmath>
#include <iomanip>
#include <unistd.h>

// soil
#include "SOIL.h"

// ODE
#include "ode/ode.h"

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
bool MoveDown{false};
bool Sprint{false};
bool Zoom{false};
bool CarSprint{false};
bool placeCube{false};
bool fall{false};

bool MouseOut{false};

float camRotateX{-260};
float camRotateY{0};
time_t  timev;
float DecreaseClimbRate{0.1};
float IncreaseFallRate{0.05};

// ODE global variables
static dWorldID      gODEWorld;
static dSpaceID      gODESpace;
static dJointGroupID gODEContactGroup;

static void ODEContactCallback (void *data, dGeomID o1, dGeomID o2)
{
    dBodyID  b1 = dGeomGetBody(o1);
    dBodyID  b2 = dGeomGetBody(o2);
    dContact contact;

    contact.surface.mode = dContactBounce | dContactSoftCFM;
    // friction parameter
    contact.surface.mu = dInfinity;
    // bounce is the amount of "bouncyness".
    contact.surface.bounce = 0.1;
    // bounce_vel is the minimum incoming velocity to cause a bounce
    contact.surface.bounce_vel = 0.2;
    // constraint force mixing parameter
    contact.surface.soft_cfm = 0.00001;

    if (int numc = dCollide (o1,o2,1,&contact.geom,sizeof(dContact))) {
        dJointID c = dJointCreateContact (gODEWorld,gODEContactGroup,&contact);
        dJointAttach (c,b1,b2);
    }
}

struct texture_t {
    texture_t()=default;
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
texture_t gTextureCerealbox{"cerealbox"};
texture_t gTextureCerealboxBlank{"yellowPaper"};
texture_t gTextureCerealboxNutFacts{"cerealboxNutFacts"};

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
    if(Zoom){
        deltaX=deltaX/10;
        deltaY=deltaY/10;
    }
    camRotateY+=deltaX;
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
        MoveUp=action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    else if (key == GLFW_KEY_LEFT_SHIFT)
    {
        MoveDown=action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    else if (key == GLFW_MOUSE_BUTTON_1)
    {
        placeCube=true;
    }
    /*else if (key == GLFW_KEY_LEFT_SHIFT)
    {
        Sprint=action == GLFW_PRESS || action == GLFW_REPEAT;
    }*/
    else if (key == GLFW_KEY_E)
    {
        CarSprint=action == GLFW_PRESS || action == GLFW_REPEAT;
    }
    else if (key == GLFW_KEY_Q)
    {
        Zoom=action == GLFW_PRESS || action == GLFW_REPEAT;
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









/*

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
}*/

struct point {
    explicit point(float x = 0, float y = 0, float z = 0) :
        x_m(x),
        y_m(y),
        z_m(z)
    { }

    double x_m;
    double y_m;
    double z_m;
};

point operator+(const point& a, const point& b) {
    point result;

    result.x_m = a.x_m + b.x_m;
    result.y_m = a.y_m + b.y_m;
    result.z_m = a.z_m + b.z_m;

    return result;
}

point& operator+=(point& a, const point& b) {
    a.x_m += b.x_m;
    a.y_m += b.y_m;
    a.z_m += b.z_m;

    return a;
}

point operator-(const point& a, const point& b) {
    point result;

    result.x_m = a.x_m - b.x_m;
    result.y_m = a.y_m - b.y_m;
    result.z_m = a.z_m - b.z_m;

    return result;
}

point& operator-=(point& a, const point& b) {
    a.x_m -= b.x_m;
    a.y_m -= b.y_m;
    a.z_m -= b.z_m;

    return a;
}

struct cubeD_D {
    
    cubeD_D(){
        
        SetboxBandG();
        dGeomSetBody (boxGeom_m, boxBody_m);
        
    }

    
    
    dBodyID boxBody_m;
    dGeomID boxGeom_m;
    
    double h_m{1};
    double w_m{1};
    double d_m{1};
    
    float density{1};
    
    double r_m{255};
    double g_m{255};
    double b_m{255};
    texture_t tex1;
    texture_t tex2;
    texture_t tex3;
    texture_t tex4;
    texture_t tex5;
    texture_t tex6;

    void draw();
    void setTexture(texture_t);
    void SetboxBandG();
    void SetLocation(float x,float y,float z);
    void SetSize(float x, float y, float z);
};

void cubeD_D::SetLocation(float x,float y,float z) {
    dBodySetPosition (boxBody_m, x,y,z);
   
}

void cubeD_D::SetSize(float x, float y, float z) {
    h_m = x;
    w_m = y;
    d_m = z;

    dMass mass;

    dMassSetBox(&mass, density, h_m, w_m, d_m);
    dBodySetMass(boxBody_m, &mass);
    dGeomBoxSetLengths(boxGeom_m, h_m, w_m, d_m);
}

void cubeD_D::SetboxBandG() {
    
    boxBody_m = dBodyCreate (gODEWorld);
    
    boxGeom_m = dCreateBox (gODESpace, 1,1,1);
    
}

void cubeD_D::setTexture(texture_t tex){
    tex1 = tex;
    tex2 = tex;
    tex3 = tex;
    tex4 = tex;
    tex5 = tex;
    tex6 = tex;
}

void orient_body_in_opengl(dBodyID body) {
    const dReal* pos = dBodyGetPosition(body);
    const dReal* rot = dBodyGetRotation(body);

    GLfloat matrix[16] = {
        rot[0], rot[4], rot[8],  0,
        rot[1], rot[5], rot[9],  0,
        rot[2], rot[6], rot[10], 0,
        pos[0], pos[1], pos[2],  1
    };

    glMultMatrixf(matrix);
}

struct BGcubeD_D {
    
    BGcubeD_D(){
        
        BGSetboxBandG();
        
        
    }
    
    
    
    
    dGeomID boxGeom_m;
    
    double h_m{1};
    double w_m{1};
    double d_m{1};
    
    float density{1};
    
    double r_m{255};
    double g_m{255};
    double b_m{255};
    texture_t tex1;
    texture_t tex2;
    texture_t tex3;
    texture_t tex4;
    texture_t tex5;
    texture_t tex6;
    
    void BGdraw();
    void BGsetTexture(texture_t);
    void BGSetboxBandG();
    void BGSetLocation(float x,float y,float z);
    void BGSetSize(float x, float y, float z);
};

void BGcubeD_D::BGSetLocation(float x,float y,float z) {
    dGeomSetPosition (boxGeom_m, x,y,z);
    
}

void BGcubeD_D::BGSetSize(float x, float y, float z) {
    h_m = x;
    w_m = y;
    d_m = z;
    
    dGeomBoxSetLengths(boxGeom_m, h_m, w_m, d_m);
}

void BGcubeD_D::BGSetboxBandG() {
    
    
    
    boxGeom_m = dCreateBox (gODESpace, 1,1,1);
    
}

void BGcubeD_D::BGsetTexture(texture_t tex){
    tex1 = tex;
    tex2 = tex;
    tex3 = tex;
    tex4 = tex;
    tex5 = tex;
    tex6 = tex;
}




void cubeD_D::draw() {
    glPushMatrix();

    orient_body_in_opengl(boxBody_m);

    glColor3f(r_m/255, g_m/255, b_m/255);
    tex1.activate();
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p5(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1); p8(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0); p7(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0); p6(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    tex2.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p8(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1);p4(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0);p3(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0);p7(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    tex3.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p5(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1);p6(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0);p2(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0);p1(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    // Top face
    tex4.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p6(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1);p7(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0);p3(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0);p2(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    // Bottom face
    tex5.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p4(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1); p8(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0); p5(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0); p1(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.

    // Back face
    tex6.activate();
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p1(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1); p2(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0); p3(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0); p4(0,0,0,h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    glPopMatrix();
}

void BGcubeD_D::BGdraw() {
    glPushMatrix();
    
    const dReal* pos = dGeomGetPosition(boxGeom_m);
    glColor3f(r_m/255, g_m/255, b_m/255);
    
    tex1.activate();
   
    
    // THIS IS WHERE THE DRAWING HAPPENS!
    // The front face :)
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p5(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1); p8(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0); p7(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0); p6(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Right face
    tex2.activate();
    
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p8(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1);p4(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0);p3(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0);p7(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Left face
    tex3.activate();
    
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p5(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1);p6(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0);p2(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0);p1(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Top face
    tex4.activate();
    
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1);p6(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1);p7(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0);p3(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0);p2(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Bottom face
    tex5.activate();
    
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p4(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1); p8(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0); p5(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0); p1(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glEnd(); // All OpenGL drawing ends with a glEnd.
    
    // Back face
    tex6.activate();
    
    glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
    glTexCoord2f(0, 1); p1(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 1); p2(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(1, 0); p3(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
    glTexCoord2f(0, 0); p4(pos[0],pos[1],pos[2],h_m,w_m,d_m,r_m,g_m,b_m);
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
    dWorldSetGravity(gODEWorld, 0, 0, -0.98);
    dWorldSetCFM(gODEWorld, 1e-5);
    dCreatePlane(gODESpace, 0, 0, 1, 0); // create the base plane
    gODEContactGroup = dJointGroupCreate (0);
    
    static dBodyID playBody = dBodyCreate (gODEWorld);
    static dGeomID playGeom = dCreateSphere (gODESpace, 4);
    dGeomSetBody (playGeom, playBody);
    
    

    dMass* mass2;
    mass2 = new dMass;
    dMassSetBox(mass2, 1, 1, 1, 1);
    dBodySetMass(playBody, mass2);

    static dBodyID sphereBody = dBodyCreate (gODEWorld);
    static dGeomID sphereGeom  = dCreateSphere(gODESpace, 1);
    
    dGeomSetBody (sphereGeom, sphereBody);

    dMass mass3;
    dMassSetSphere(&mass3, 2, 1);
    dBodySetMass(sphereBody, &mass3);

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
    
    // Set some OpenGL gODEWorld options.
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glCullFace(GL_BACK);

    //comment this out to go to normal colors
    glEnable(GL_TEXTURE_2D);

    gTextureSteel.load();
    gTexture.load();
    gTextureWood.load();
    gTextureLeaves.load();
    gTextureRoad.load();
    gTextureRoadY.load();
    gTextureWhite.load();
    gTextureBall.load();
    gTextureClear.load();
    gTextureCerealbox.load();
    gTextureCerealboxNutFacts.load();
    gTextureCerealboxBlank.load();
    
    cubeD_D myCube;
    myCube.setTexture(gTextureBall);
    myCube.SetLocation(-10, 0, 20);
    myCube.density=0.5;
    myCube.SetSize(0.5, 0.5, 0.5);
    
    cubeD_D myCube1;
    myCube1.setTexture(gTextureWhite);
    myCube1.SetLocation(5, 1, 49);
    myCube1.density=6;
    myCube1.SetSize(2, 2, 2);    
    
    BGcubeD_D BGCube1;
    BGCube1.BGsetTexture(gTextureRoad);
    BGCube1.BGSetLocation(4, 5, 7);
    BGCube1.BGSetSize(20, 1, 14);
    
    BGcubeD_D BGCube2;
    BGCube2.BGsetTexture(gTextureRoad);
    BGCube2.BGSetLocation(14.5, 14.5, 7);
    BGCube2.BGSetSize(1, 20, 14);
    
    BGcubeD_D BGCube3;
    BGCube3.BGsetTexture(gTextureRoad);
    BGCube3.BGSetLocation(-6.5, 14.5, 7);
    BGCube3.BGSetSize(1, 20, 14);
    
    BGcubeD_D BGCube4;
    BGCube4.BGsetTexture(gTextureRoad);
    BGCube4.BGSetLocation(4, 24, 7);
    BGCube4.BGSetSize(20, 1, 14);
    
    BGcubeD_D Road1;
    Road1.BGsetTexture(gTextureRoad);
    Road1.BGSetLocation(0, 0, 0.1);
    Road1.BGSetSize(1000, 8, 0.5);
    
    cubeD_D myCube3;
    myCube3.setTexture(gTextureRoadY);
    myCube3.SetLocation(4, 0, 89);
    myCube3.density=2;
    myCube1.SetSize(4, 4, 4);
    
    

    cubeD_D BouncyBlock;
    BouncyBlock.setTexture(gTextureSteel);
    BouncyBlock.SetLocation(0, 3, 40);
    BouncyBlock.r_m=255;
    BouncyBlock.g_m=0;
    BouncyBlock.b_m=0;
    dBodyAddForce(BouncyBlock.boxBody_m, 5, 5, 0);
    myCube1.SetSize(7, 7, 7);
    
    cubeD_D cerealbox;
    cerealbox.tex4=gTextureCerealbox;
    cerealbox.tex3=gTextureCerealboxNutFacts;
    cerealbox.tex2=gTextureCerealboxNutFacts;
    cerealbox.tex1=gTextureCerealboxBlank;
    cerealbox.tex5=gTextureCerealboxBlank;
    cerealbox.tex6=gTextureCerealboxBlank;
    cerealbox.SetLocation(4, -20, 1);
    cerealbox.density=1;
    cerealbox.SetSize(2, 1, 2.5);
    
    cubeD_D freezerBack;
    freezerBack.setTexture(gTextureWhite);
    freezerBack.SetLocation(20, 30, 2.5);
    freezerBack.SetSize(10,1,5);
    

    dBodySetPosition(sphereBody,0,0,50);

    static const float simulation_start_k = glfwGetTime();
    static const float real_min_per_game_day_k = 24; // CHANGE ONLY HERE TO AFFECT DAY/NIGHT SPEED
    static const float real_sec_per_game_day_k = real_min_per_game_day_k * 60;
    static const float real_sec_per_game_hrs_k = real_sec_per_game_day_k / 24;
    static const float real_sec_per_game_min_k = real_sec_per_game_hrs_k / 60;
    static const float game_min_per_real_sec_k = 1 / real_sec_per_game_min_k;
    static const float min_per_day_k = 24 * 60;

    //look position
    camRotateX=-90;

    // This is the main processing loop that draws the spinning rectangle.
    while (!glfwWindowShouldClose(window)) // this will loop until the window should close.
    {
        
        float elapsed_real_sec = glfwGetTime() - simulation_start_k;
        float elapsed_game_min = game_min_per_real_sec_k * elapsed_real_sec;
        float elapsed_game_hrs = elapsed_game_min / 60;
        float percent_of_day = (static_cast<int>(elapsed_game_min) % static_cast<int>(min_per_day_k)) / min_per_day_k;
        float sky_cycle = std::sin(percent_of_day * M_PI);
        float sky = 0 * (1-sky_cycle) + 0.9803921569 * sky_cycle;
        // int game_hrs_mil = static_cast<int>(elapsed_game_hrs) % 24; // military hours

// Set to #if 1 to enable displaying the time
#if 0
        std::cout.width(2);
        std::cout.fill('0');
        std::cout << game_hrs_mil << ":";
        std::cout.width(2);
        std::cout << (static_cast<int>(elapsed_game_min)%60) << '\n';
#endif

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
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        glRotatef(camRotateX, 1.f, 0.f, 0.f);
        glRotatef(camRotateY, 0.f, 0.f, 1.f);
        const dReal* pos = dBodyGetPosition(playBody);

        if(MoveForward){
            
            //dBodySetPosition (playBody, camX-2,camY+2.5,camZ);
            //camX -= std::sin(DegreesToRads(camRotateY))*0.1;
            //camY -= std::cos(DegreesToRads(camRotateY))*0.1;
            dBodySetForce(playBody, std::sin(DegreesToRads(camRotateY))*1, std::cos(DegreesToRads(camRotateY))*1, 0);
            
            
            
        }
        if(MoveRight){
            //camY += std::cos(DegreesToRads(camRotateY-90))*0.1;
            //camX += std::sin(DegreesToRads(camRotateY-90))*0.1;
            dBodySetForce(playBody, -std::sin(DegreesToRads(camRotateY-90))*1, -std::cos(DegreesToRads(camRotateY-90))*1, 0);
        }
        if(MoveLeft){
            //camY += std::cos(DegreesToRads(camRotateY+90))*0.1;
            //camX += std::sin(DegreesToRads(camRotateY+90))*0.1;
            dBodySetForce(playBody, -std::sin(DegreesToRads(camRotateY+90))*1, -std::cos(DegreesToRads(camRotateY+90))*1, 0);
        }
        if(MoveBackward){
            //camY += std::cos(DegreesToRads(camRotateY))*0.1;
            //camX += std::sin(DegreesToRads(camRotateY))*0.1;
            dBodySetForce(playBody, -std::sin(DegreesToRads(camRotateY))*1, -std::cos(DegreesToRads(camRotateY))*1, 0);
        }
        
        
        if(MoveUp){
            //camZ += DecreaseClimbRate;
            //DecreaseClimbRate-=0.0077;
            camZ=pos[2]-1;
            dBodySetForce(playBody, 0,0,2);
        }
        if(MoveDown){
            //camZ += DecreaseClimbRate;
            //DecreaseClimbRate-=0.0077;
            camZ=pos[2]-1;
            dBodySetForce(playBody, 0,0,-2);
        }
        if(Sprint && CarSprint){
            camY += std::cos(DegreesToRads(camRotateY))*-0.55;
            camX += std::sin(DegreesToRads(camRotateY))*-0.55;
        }
        if(CarSprint){
            camY -= std::cos(DegreesToRads(camRotateY))*1;
            camX -= std::sin(DegreesToRads(camRotateY))*1;
        }
        if(Zoom){
            camY -= std::cos(DegreesToRads(camRotateY))*50.5;
            camX -= std::sin(DegreesToRads(camRotateY))*50.5;
        }
        
        if(MoveUp==true){
            camZ=pos[2]-1;
            
            
        }
        
        if(MoveUp==false && MoveDown==false){
            camZ=pos[2]-1;
            
            
            
            
        }
        const dReal* speedSAVE = dBodyGetLinearVel(playBody);
        //std::cout << "X=" << speedSAVE[1] << '\n';
        
        
        
        
        
        if(MoveForward==false && MoveBackward==false && MoveLeft==false && MoveRight==false){
            //dBodyGetLinearVel(cubeD_D().boxBody_m);
            
            
           
            
            //speedy = -std::cos(DegreesToRads(camRotateY))*1
            if(speedSAVE[0]>0.2){
                dBodySetForce(playBody, -2.5, 0, 0);
            }
            if(speedSAVE[0]<-0.2){
                dBodySetForce(playBody, 2.5 , 0, 0);
            }
            
            if(speedSAVE[1]>0.2){
                dBodySetForce(playBody, 0, -2.5, 0);
            }
            if(speedSAVE[1]<-0.2){
                dBodySetForce(playBody, 0, 2.5, 0);
            }
           
            
            
            //dBodySetPosition (playBody, camX,camY,camZ);
            
        }
        
        /*if(MoveLeft==false){
            //dBodyGetLinearVel(cubeD_D().boxBody_m);
            
            
            
            
            //speedy = -std::cos(DegreesToRads(camRotateY))*4
            if(speedSAVE[1]<0.2){
                dBodySetForce(playBody, 0, -std::cos(DegreesToRads(camRotateY))*4, 0);
            }
            if(speedSAVE[1]>-0.2){
                dBodySetForce(playBody, 0, -std::cos(DegreesToRads(camRotateY))*4, 0);
            }
            
            
            
           
            
        }*/
        //dBodySetPosition (playBody, -camX-2,-camY+2.5,camZ+1);
        
        if(camZ<=0){
            camZ += 0.1;
            
            DecreaseClimbRate=0.2;
            MoveUp=false;
        }
        if(camX>=1){
            fall=true;
        }
        else{fall=false;}
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

        glTranslatef(camX+2, camY-2.5, -camZ-2);

        gTexture.activate();
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glBegin(GL_QUADS); // All OpenGL drawing begins with a glBegin.
            glColor3f(1, 1, 1);
            glTexCoord2f(0, 0); glVertex3f(-450, -450, 0);
            glTexCoord2f(450, 0); glVertex3f(450, -450 ,0);
            glTexCoord2f(450, 450); glVertex3f(450, 450, 0);
            glTexCoord2f(0, 450); glVertex3f(-450, 450, 0);
        glEnd(); // All OpenGL drawing ends with a glEnd.

        //If you would like to make a custom make change this to true v
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        camX=-pos[0]-2;
        camY=-pos[1]+2.5;
        
        /*glPushMatrix();
        orient_body_in_opengl(playBody);
        gTextureBall.activate();
        GLUquadricObj*quad2=gluNewQuadric();
        gluQuadricTexture( quad2, GL_TRUE);
        gluSphere(quad2, 0.5, 15, 15);
        gluDeleteQuadric(quad2);
        glPopMatrix();*/

        // Position and draw the sphere
        glPushMatrix();
            orient_body_in_opengl(sphereBody);
            gTextureBall.activate();
            GLUquadricObj*quad=gluNewQuadric();
            gluQuadricTexture( quad, GL_TRUE);
            gluSphere(quad, 0.5, 15, 15);
            gluDeleteQuadric(quad);
        glPopMatrix();

        myCube.draw();
        myCube1.draw();
        
        BGCube1.BGdraw();
        BGCube2.BGdraw();
        BGCube3.BGdraw();
        BGCube4.BGdraw();
        
        //dBodyDisable(myCube2.boxBody_m);

        myCube3.draw();
        BouncyBlock.draw();
        
        Road1.BGdraw();
        cerealbox.draw();
        
        freezerBack.draw();
        
        
        
        
        
        /*const dReal* pos = dBodyGetPosition(BouncyBlock.boxBody_m);
        camX=-pos[0]-2;
        camY=-pos[1]+2.5;
        camZ=pos[2];*/
        //dBodySetPosition (playBody, camX-2,camY+2.5,camZ);
        
        
        camX=-pos[0]-2;
        camY=-pos[1]+2.5;
        
        
        dBodySetPosition (playBody, pos[0],pos[1],camZ+1);
        
        //std::cout << "X=" << camX << '\n';
        //std::cout << "Y=" << camY << '\n';
        //std::cout << "Z=" << camZ << '\n';
        
        
        
        
        
        //dMatrix3* R;
        
        

        //dBodySetRotation(playBody, *R);
        //dBodySetForce(playBody, 0, 0, 0);
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
