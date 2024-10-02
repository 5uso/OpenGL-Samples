//
// CGV Semana 3 por Jesús Mosteiro García (Grupo 3.2)
// Esquema de control comentado antes de la función keyboard
//

//
// Control scheme:
//     +  -
//  X  Q  A
//  Y  W  S
//  Z  E  D
//
// Holding shift or blocking caps switches control to the second joint with the same keys.
// 1, 2, 3, 4 switches selected extremity.
//
// Right click opens a context menu with options.
// 
// CLicking and dragging moves the legs. If holding CTRL, they are moved in a different axis.
//

#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Default window size
GLint ancho = 1280;
GLint alto = 720;

// Amount to rotate per interrupt
const float moveAmt = 1.0f;

// Struct to hold a vec3 more comfortably
typedef struct vec3
{
    float x, y, z;
} vec3;

// Static vec3s to store the rotations, changed during keyboard interrupts
static int selected_extremity = 0;
static vec3 rotations[8]; // Use an array to store the 8 rotations

// Variables set from right click menu
static int do_movement = 0, move_speed = 1, do_wire = 0;

// Typical reshape stuff
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(70.0f, (GLfloat)width / (GLfloat)height, 1.0f, 20.0f);
    glMatrixMode(GL_MODELVIEW);

    ancho = width;
    alto = height;
}

// Functions for menus / submenus
void main_menu(int n)
{
    // Nothing here
}

void submenu1(int n)
{
    do_wire = n;
}

void submenu2(int n)
{
    do_movement = n;
}

void submenu3(int n)
{
    move_speed = 1 + n;
}

// Adds right click menu
void addMenu()
{
    int menu1 = glutCreateMenu(submenu1);
    glutAddMenuEntry("Solido", 0);
    glutAddMenuEntry("Alambre", 1);

    int menu2 = glutCreateMenu(submenu2);
    glutAddMenuEntry("Off", 0);
    glutAddMenuEntry("On", 1);

    int menu3 = glutCreateMenu(submenu3);
    glutAddMenuEntry("Lento", 0);
    glutAddMenuEntry("Medio", 1);
    glutAddMenuEntry("Rapido", 2);

    glutCreateMenu(main_menu);
    glutAddSubMenu("Solido/Alambre", menu1);
    glutAddSubMenu("Movimiento automatico", menu2);
    glutAddSubMenu("Cambio velocidad", menu3);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Draws a joint
void drawJoint(GLdouble radius, vec3 rotation)
{
    // Sphere is the joint base and uses the current transformation matrix
    glColor3f(1.0f, 0.0f, 0.0f);
    if (do_wire)
        glutWireSphere(radius / 2.0, 20, 20);
    else
        glutSolidSphere(radius / 2.0, 20, 20);

    // We rotate based on the vec3 passed as parameter
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);

    // Move the cube in the direction we rotated
    glTranslatef(0.0f, 1.25f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);

    glPushMatrix();

    // Scale only this cube
    glScalef(1.0f, 1.5f, 1.0f);
    if (do_wire)
        glutWireCube(radius);
    else
        glutSolidCube(radius);

    glPopMatrix();

    // We don't reset the transformation here so later calls of this function chain joints easily
    glTranslatef(0.0f, 1.25f, 0.0f);
}

void drawExtremity(float posx, float posy, float posz, float scale, int id)
{
    // Create a copy of the transformation matrix to work on
    glPushMatrix();

    glTranslatef(posx, posy, posz);
    glScalef(scale, scale, scale);

    drawJoint(1.0, rotations[id * 2]);
    // We want the joint to be displaced in the direction the previous one was facing
    drawJoint(1.0, rotations[id * 2 + 1]);

    // Restore transformation matrix so the function can be used in other contexts
    glPopMatrix();
}

void drawBody(float scalex, float scaley, float scalez)
{
    // Create a copy of the transformation matrix to work on
    glPushMatrix();

    // Scale only this cube
    glScalef(scalex, scaley, scalez);

    if (do_wire)
        glutWireCube(1.0f);
    else
        glutSolidCube(1.0f);

    // Restore transformation matrix so the function can be used in other contexts
    glPopMatrix();
}

void display()
{
    // Set buffer stuff
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place things in front of the camera, please
    glLoadIdentity();
    glTranslatef(0.0f, 0.0f, -5.0f);

    // Draw body
    drawBody(1.0f, 2.0f, 1.0f);

    // Draw arms
    drawExtremity(-0.66f, 0.7f, 0.0f, 0.3f, 0);
    drawExtremity(0.66f, 0.7f, 0.0f, 0.3f, 1);

    // Draw legs
    drawExtremity(-0.33f, -1.16f, 0.0f, 0.3f, 2);
    drawExtremity(0.33f, -1.16f, 0.0f, 0.3f, 3);

    glFlush();
    glutSwapBuffers();
}

// Used for automatic leg movement and called from a timer
void rotate_legs(int n)
{
    if (do_movement)
    {
        for (int i = 4; i < 8; i += 2)
            rotations[i].z += move_speed;
    }

    // Timer calls itself
    glutTimerFunc(16, rotate_legs, 0);
}

void init()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST); // We want depth to work, yes

    addMenu();

    // Make the legs start facing down
    for (int i = 4; i < 8; i += 2)
        rotations[i].x = 180.0f;
}

void idle()
{
    display();
}

// Keyboard input, controls explained at the file head
void keyboard(unsigned char key, int x, int y)
{
    // Changes selected extremity
    if (key > '0' && key < '5')
    {
        selected_extremity = key - '1';
        return;
    }

    int i = (selected_extremity * 2) + (key >= 'a' ? 0 : 1);       // What rotation to access in the array
    int effectiveKey = key - (key >= 'a' ? 'a' : 'A') + 'A'; // To reduce the amount of switch cases

    switch (effectiveKey)
    {
    case 'Q': // X+
        rotations[i].x += moveAmt;
        break;
    case 'A': // X-
        rotations[i].x -= moveAmt;
        break;
    case 'W': // Y+
        rotations[i].y += moveAmt;
        break;
    case 'S': // Y-
        rotations[i].y -= moveAmt;
        break;
    case 'E': // Z+
        rotations[i].z += moveAmt;
        break;
    case 'D': // Z-
        rotations[i].z -= moveAmt;
        break;
    }
}

// We store the mouse modifiers at the start of a click to use them in the movement function
static int mouse_modifiers = 0;
void getMouseModiers(int button, int state, int x, int y)
{
    mouse_modifiers = glutGetModifiers();
}

// Rotates leg based on mouse movement
void mouse(int x, int y)
{
    for (int i = 5; i < 8; i += 2)
    {
        if (mouse_modifiers & GLUT_ACTIVE_CTRL)
        {
            rotations[i].x = x;
        }
        else
        {
            rotations[i].y = x;
        }
    }
}

int main(int argc, char **argv)
{
    // Inititalize basic stuff
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ancho, alto);
    glutCreateWindow("Leg");

    // Another init function to keep things organized
    init();

    // Set up callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, rotate_legs, 0);
    glutMouseFunc(getMouseModiers);
    glutMotionFunc(mouse);

    // Let's go
    glutMainLoop();
    return 0;
}