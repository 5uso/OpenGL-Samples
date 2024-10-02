//
// CGV Examen 1por Jesús Mosteiro García (Grupo 3.2)
// Esquema de control estándar
//

#include <stdio.h>
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

// Default window size
GLint ancho = 1280;
GLint alto = 720;

// Global variables
static float base_angle = 0.0f, tea_angle = 0.0f, pitch_angle = 0.0f, tea_tilt = 0.0f;
static int do_perspective = 1, base_direction = 0, pitch_direction = 0, do_movement = 1, cube_color = 0, move_speed = 0;

// Typical reshape stuff
void reshape(int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    // We can change between perspective and orthographic
    if(do_perspective) gluPerspective(60.0f, (GLfloat)width/(GLfloat)height, 1.0f, 20.0f);
    else glOrtho(-4, 4, -4, 4, 1, 10);

    glMatrixMode(GL_MODELVIEW);

    ancho = width;
    alto = height;
}

// Functions for menus / submenus
void main_menu(int n)
{
    if(n == 2) exit(0);
}

void submenu1(int n)
{
    cube_color = n;
}

void submenu2(int n)
{
    move_speed = n;
}

// Adds right click menu
void addMenu()
{
    int menu1 = glutCreateMenu(submenu1);
    glutAddMenuEntry("Rojo", 0);
    glutAddMenuEntry("Verde", 1);
    glutAddMenuEntry("Azul", 2);
    glutAddMenuEntry("Magenta", 3);

    int menu2 = glutCreateMenu(submenu2);
    glutAddMenuEntry("Lenta", 0);
    glutAddMenuEntry("Rapida", 2);

    glutCreateMenu(main_menu);
    glutAddSubMenu("Color cubos", menu1);
    glutAddSubMenu("Velocidad movimiento", menu2);
    glutAddMenuEntry("Salir", 2);

    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

// Draws the base
void drawBase() {
    glPushMatrix();
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.7f, 0.1f, 0.1f);
    GLUquadricObj *quadratic = gluNewQuadric();
    gluCylinder(quadratic, 1.0, 1.0, 0.5, 20, 20);
    glPopMatrix();

    // This rotation is kept after the function ends so the top is also rotated
    glRotatef(base_angle, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glTranslatef(0.0f, 0.5f, 0.0f);
    glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.0f, 0.2f, 0.7f);
    glutSolidCone(1.0, 0.5, 20, 20);
    glPopMatrix();
}

// Draws the small platform
void drawTea() {
    glRotatef(tea_tilt, 1.0f, 0.0f, 0.0f);
    glRotatef(tea_angle, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
    glColor3f(0.1f, 0.7f, 0.1f);
    glutSolidCone(0.5, 0.5, 20, 20);
    glPopMatrix();

    glPushMatrix();
    glTranslatef(0.0f, 0.1f, 0.0f);
    glTranslatef(-0.25f, 0.0f, -0.25f);

    switch(cube_color) {
        case 0: glColor3f(1.0f, 0.0f, 0.0f); break;
        case 1: glColor3f(0.0f, 1.0f, 0.0f); break;
        case 2: glColor3f(0.0f, 0.0f, 1.0f); break;
        case 3: glColor3f(1.0f, 0.0f, 1.0f); break;
    }

    glutSolidCube(0.2);
    glTranslatef(0.5f, 0.0f, 0.0f);
    glutSolidCube(0.2);
    glTranslatef(0.0f, 0.0f, 0.5f);
    glutSolidCube(0.2);
    glTranslatef(-0.5f, 0.0f, 0.0f);
    glutSolidCube(0.2);
    glPopMatrix();
}

// Draws the top
void drawTop() {
    glPushMatrix();

    glRotatef(pitch_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(1.0f, 0.0f, 0.0f);

    glPushMatrix();
    glScalef(2.0, 0.25, 0.25);
    glColor3f(0.6f, 0.6f, 0.6f);
    glutSolidCube(1.0);
    glPopMatrix();

    glTranslatef(1.0f, 0.0f, 0.0f);
    glRotatef(-pitch_angle, 0.0f, 0.0f, 1.0f);
    glTranslatef(0.0f, 0.5f, 0.0f);

    drawTea();

    glPopMatrix();
}

// Draws teapots, leaving matrices as they were
void drawTeapots() {
    glPushMatrix();
    drawBase();
    glTranslatef(0.0f, 1.0f, 0.0f);
    drawTop();
    glPopMatrix();
}

void display()
{
    // Set buffer stuff
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place things in front of the camera, please
    glLoadIdentity();
    glTranslatef(0.0f, -1.5f, -5.0f);

    //Draw teapots
    drawTeapots();

    glFlush();
    glutSwapBuffers();
}

// Used for automatic movement and called from a timer
void rotate(int n)
{
    if(do_movement) {
        base_angle += base_direction ? -1.0f : 1.0f;

        tea_angle -= 2.0f;

        pitch_angle += pitch_direction ? -1.0f : 1.0f;
        if(pitch_angle > 45.0f) pitch_direction = 1;
        else if(pitch_angle < 0.0f) pitch_direction = 0;
    }

    // Timer calls itself
    glutTimerFunc(move_speed ? 4 : 16, rotate, 0);
}

void init()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST); // We want depth to work, yes

    addMenu();
}

void idle()
{
    display();
}

// Keyboard input
void keyboard(unsigned char key, int x, int y)
{
    int effectiveKey = key - (key >= 'a' ? 'a' : 'A') + 'A';

    switch (effectiveKey)
    {
    case 'O':
        do_perspective = 0;
        reshape(ancho, alto);
        break;
    case 'P':
        do_perspective = 1;
        reshape(ancho, alto);
        break;
    case 'R':
        base_direction = base_direction ? 0 : 1;
        break;
    case 'S':
        do_movement = do_movement ? 0 : 1;
        break;
    }
}

// We store the mouse modifiers at the start of a click to use them in the movement function
static int mouse_modifiers = 0;
void getMouseModiers(int button, int state, int x, int y)
{
    mouse_modifiers = glutGetModifiers();
}

// Rotates based on mouse movement (holding alt)
void mouse(int x, int y)
{
    if (mouse_modifiers & GLUT_ACTIVE_ALT)
    {
        base_angle = ((float)x / (float)ancho) * 360.0f;       
    }
}

// Tilts small platform
void mousePassive(int x, int y)
{
    tea_tilt = (((float)x / (float)ancho) - 0.5f) * 45.0f;
}

int main(int argc, char **argv)
{
    // Inititalize basic stuff
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(ancho, alto);
    glutCreateWindow("Teapots");

    // Another init function to keep things organized
    init();

    // Set up callbacks
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutIdleFunc(idle);
    glutKeyboardFunc(keyboard);
    glutTimerFunc(16, rotate, 0);
    glutMouseFunc(getMouseModiers);
    glutMotionFunc(mouse);
    glutPassiveMotionFunc(mousePassive);

    // Let's go
    glutMainLoop();
    return 0;
}