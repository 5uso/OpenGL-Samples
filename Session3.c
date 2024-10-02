//
// CGV Semana 3 por Jesús Mosteiro García (Grupo 3.2)
// Esquema de control comentado antes de la función keyboard
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
static vec3 firstCubeRotation = {0.0f, 0.0f, 0.0f}, secondCubeRotation = {0.0f, 0.0f, 0.0f};

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

// Draws a joint
void drawJoint(GLdouble radius, vec3 rotation)
{
    // Sphere is the joint base and uses the current transformation matrix
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(radius / 2.0, 20, 20);

    // We rotate based on the vec3 passed as parameter
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);

    // Move the cube in the direction we rotated
    glTranslatef(0.0f, 1.0f, 0.0f);

    glColor3f(0.0f, 0.0f, 1.0f);
    glutSolidCube(radius);

    // We don't reset the transformation here so later calls of this function chain joints easily
}

void drawLeg()
{
    // Create a copy of the transformation matrix to work on
    glPushMatrix();

    drawJoint(1.0, firstCubeRotation);
    // We want the joint to be displaced in the direction the previous one was facing
    glTranslatef(0.0f, 1.0f, 0.0f);
    drawJoint(1.0, secondCubeRotation);

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

    // Draw a leg
    drawLeg();

    glFlush();
    glutSwapBuffers();
}

void init()
{
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST); // We want depth to work, yes
}

void idle()
{
    display();
}

//
// Control scheme:
//     +  -
//  X  Q  A
//  Y  W  S
//  Z  E  D
//
// Holding shift or blocking caps switches control to the second joint with the same keys.
//
void keyboard(unsigned char key, int x, int y)
{
    vec3 *affectedRotation = key >= 'a' ? &firstCubeRotation : &secondCubeRotation; // Address the rotation vectors via pointer
    int effectiveKey = key - (key >= 'a' ? 'a' : 'A') + 'A';                        // To reduce the amount of switch cases

    switch (effectiveKey)
    {
    case 'Q': // X+
        affectedRotation->x += moveAmt;
        break;
    case 'A': // X-
        affectedRotation->x -= moveAmt;
        break;
    case 'W': // Y+
        affectedRotation->y += moveAmt;
        break;
    case 'S': // Y-
        affectedRotation->y -= moveAmt;
        break;
    case 'E': // Z+
        affectedRotation->z += moveAmt;
        break;
    case 'D': // Z-
        affectedRotation->z -= moveAmt;
        break;
    case 27: // Escape
        exit(0);
        break;
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

    // Let's go
    glutMainLoop();
    return 0;
}