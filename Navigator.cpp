


//#include <windows.h>    // For using the sleep function

#include <math.h>       //include file for math
#include  "glut.h"      //include file for glut

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

/*
Chelsea Alysson Ongjoco
ID: 2552139
Navigator.cpp
In this program there are 2 rooms connected by a door and the camera that navigates through the room can be manipulated using the arrow and keyboard keys.
*/


float DoorHingeAngle = 90.0f;   // For the Door Hinge
bool doorIsOpen = true;         // For when the door is 90 degrees open


float angle = 0.0f;             // angle for camera

const float rotationSpeed = 0.05f;



// Set the dimensions of the window
int windowWidth = 1024;                 // width
int windowHeight = 768;                 // height


// Aspect ratio
float aspect = float(windowWidth) / float(windowHeight);

// Initialize the variables for the camera (spin and rotating)
float spin = 0.0;           // spin for camera
bool rotating = false;      // rotating for camera


// These are for the properties of the camera
double eye[] = { 0,0,1 };       // position
double center[] = { 0,0,0 };    // where it is looking/pointed at
double up[] = { 0,1,0 };        // up vector (important for tilt)


// Computes the cross product of a and b and stores it in c
void crossProduct(double a[], double b[], double c[])
{

    c[0] = a[1] * b[2] - a[2] * b[1]; // x component
    c[1] = a[2] * b[0] - a[0] * b[2]; // y component
    c[2] = a[0] * b[1] - a[1] * b[0]; // z component

}


// normalizes the given vector to make the length 1 (unit vector)
void normalize(double a[])
{
    double norm;                                        // magnitude
    norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];     // calculates magnitude pt 1
    norm = sqrt(norm);                                  // calculates magnitude pt 2

    // After calculating the magnitude, divide the vector by it
    a[0] /= norm;   // normalized a, x component
    a[1] /= norm;   // normalized a, y component
    a[2] /= norm;   // normalized a, z component

}


// Rotates the point p around the axis a by an angle theta
void rotatePoint(double a[], double theta, double p[])
{
    // Store point p temporarily in temp
    double temp[3];
    temp[0] = p[0];     // x component
    temp[1] = p[1];     // y component
    temp[2] = p[2];     // z component

    // Computes the perpendicular component of the rotation
    temp[0] = -a[2] * p[1] + a[1] * p[2];       // x component
    temp[1] = a[2] * p[0] - a[0] * p[2];        // y component
    temp[2] = -a[1] * p[0] + a[0] * p[1];       // z component

    // Scale by sin of the rotation angle (theta that was passed to the function)
    temp[0] *= sin(theta);     // x component
    temp[1] *= sin(theta);     // y component
    temp[2] *= sin(theta);     // z component

    // Compute the parallel component of the rotation
    temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);     // x component
    temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);     // y component
    temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);     // z component
    
// Add the original points (but scaled by cosine of the angle)
    temp[0] += cos(theta) * p[0];     // x component
    temp[1] += cos(theta) * p[1];     // y component
    temp[2] += cos(theta) * p[2];     // z component

    // Reassign the point p with the updated temp points
    p[0] = temp[0];     // x component
    p[1] = temp[1];     // y component
    p[2] = temp[2];     // z component

}



// Function to rotate the 'center' vector around the 'up' vector by a given angle
void rotateCamera(double angle) {
    // Calculate the direction vector from the camera position (eye) to the center point
    double direction[3] = { center[0] - eye[0], center[1] - eye[1], center[2] - eye[2] };

    // Calculate the cosine and sine of the angle
    double cosAngle = cos(angle);
    double sinAngle = sin(angle);

    // Compute the new direction vector after rotation
    double newDirX = direction[0] * cosAngle + direction[2] * sinAngle;
    double newDirZ = direction[2] * cosAngle - direction[0] * sinAngle;

    // Update the center point based on the rotated direction
    center[0] = eye[0] + newDirX;
    center[2] = eye[2] + newDirZ;
}

// Rotate left (positive angle)
void Left() {
    double speed = M_PI / 32;  // Set the camera rotation speed
    rotateCamera(speed);
}

// Rotate right (negative angle)
void Right() {
    double speed = M_PI / 32;  // Set the camera rotation speed
    rotateCamera(-speed);
}




// Rotates the camera upwards (by tilting)
void Up()
{
    center[1] += 0.05;
}


// Tilts the view so that the camera rotates downward
void Down()
{
    center[1] -= 0.05;
}

void Forward() {
    double speed = 3.14 / 32;

    // Calculate the direction vector from eye to center
    double direction[3] = { 0,0,0 };
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    // Normalize the direction vector
    double magnitude = sqrt(direction[0] * direction[0] +
        direction[1] * direction[1] +
        direction[2] * direction[2]);

    if (magnitude != 0) {  // Avoid division by zero
        direction[0] /= magnitude;
        direction[1] /= magnitude;
        direction[2] /= magnitude;
    }

    // Move both the eye and center points in the forward direction
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;
}


void Backward() {

    double speed = -3.14 / 32;

    // Calculate the direction vector from eye to center
    double direction[3] = { 0,0,0 };
    direction[0] = center[0] - eye[0];
    direction[1] = center[1] - eye[1];
    direction[2] = center[2] - eye[2];

    // Move both the eye and center points in the backward direction
    eye[0] += direction[0] * speed;
    eye[1] += direction[1] * speed;
    eye[2] += direction[2] * speed;

    center[0] += direction[0] * speed;
    center[1] += direction[1] * speed;
    center[2] += direction[2] * speed;

}



// Handles the inputs of the special keys (the keyboard keys) to move the camera
void specialKeys(int key, int x, int y)
{
    const float angleStep = 5.0f;
    // for choices of keys
    switch (key)
    {
    case GLUT_KEY_LEFT: Left(); break;          // So that when the left key is pressed, the left() function is called
    case GLUT_KEY_RIGHT: Right(); break;        // So that when the right key is pressed, the right() function is called
    case GLUT_KEY_UP:    Up(); break;           // So that when the up key is pressed, the up() function is called
    case GLUT_KEY_DOWN:  Down(); break;         // So that when the down key is pressed, the down() function is called
    }

    glutPostRedisplay();                        // Update display

}

void drawRoom1(void) {

    glBegin(GL_QUADS);

    // Top face (y = 1.0f)
    glColor3f(0.3f, 0.7f, 0.3f);    // Green
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.7f, 0.3f);    // Orange
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    // Front face  (z = 1.0f)

    
    glColor3f(0.9f, 0.3f, 0.3f);    // Red
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);

    // Back face (z = -1.0f)
    glColor3f(1.0f, 0.9f, 0.4f);    // Yellow
    glVertex3f(1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, -1.0f);
    

    // Left face (x = -1.0f)
    glColor3f(0.4f, 0.6f, 0.9f);    // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);


    /*
    // Right face (x = 1.0f)
    glColor3f(0.8f, 0.4f, 0.7f);    // Magenta
    glVertex3f(1.0f, 1.0f, -1.0f);
    glVertex3f(1.0f, 1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, 1.0f);
    glVertex3f(1.0f, -1.0f, -1.0f);

    */

    glEnd();
}

void drawDoor(void) {
       
    glBegin(GL_QUADS);

    // Door opening on the right face (x = 1.0f)
    glColor3f(0.5f, 0.0f, 0.5f); // Dark magenta 

    glVertex3f(1.0f, 0.5f, -0.2f);  // Top-left of the door
    glVertex3f(1.0f, 0.5f, 0.2f);   // Top-right of the door
    glVertex3f(1.0f, -1.0f, 0.2f);  // Bottom-right of the door
    glVertex3f(1.0f, -1.0f, -0.2f); // Bottom-left of the door

    glEnd();
}

void drawRoom2(void) {

    glBegin(GL_QUADS);

    // Top face (y = 1.0f)
    glColor3f(0.3f, 0.7f, 0.3f);    // Green
    glVertex3f(2.0f, 1.0f, -1.0f);  // 1.0f to 2.0f in x
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(2.0f, 1.0f, 1.0f);   // 1.0f >> 2.0f in x

    // Bottom face (y = -1.0f)
    glColor3f(1.0f, 0.7f, 0.3f);    // Orange
    glVertex3f(2.0f, -1.0f, 1.0f);  // 1.0f >> 2.0f in x
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(2.0f, -1.0f, -1.0f); // 1.0f >> 2.0f in x

    // Front face  (z = 1.0f)
    glColor3f(0.9f, 0.3f, 0.3f);    // Red
    glVertex3f(2.0f, 1.0f, 1.0f);   // 1.0f >> 2.0f in x
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    glVertex3f(2.0f, -1.0f, 1.0f);  // 1.0f >> 2.0f in x

    // Back face (z = -1.0f)
    glColor3f(1.0f, 0.9f, 0.4f);    // Yellow
    glVertex3f(2.0f, -1.0f, -1.0f); // 1.0f >> 2.0f in x
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(2.0f, 1.0f, -1.0f);  // 1.0f >> 2.0f in x

    /*
    // Left face (x = -1.0f)
    glColor3f(0.4f, 0.6f, 0.9f);    // Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);
    glVertex3f(-1.0f, 1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, -1.0f);
    glVertex3f(-1.0f, -1.0f, 1.0f);
    */

    // Right face (x = 1.0f) --> Changed to x = 2.0f (basically everything changed)
    glColor3f(0.8f, 0.4f, 0.7f);    // Magenta
    glVertex3f(2.0f, 1.0f, -1.0f);  
    glVertex3f(2.0f, 1.0f, 1.0f);   
    glVertex3f(2.0f, -1.0f, 1.0f);  
    glVertex3f(2.0f, -1.0f, -1.0f); 



    glEnd();
}

void drawWall(void) {

    glBegin(GL_QUADS);

    // Right face (x = 1.0f)
    glColor3f(0.8f, 0.4f, 0.7f);    // Magenta

    // Top part of the wall (above the door)
    glVertex3f(1.0f, 1.0f, -1.0f);  // Top-right front
    glVertex3f(1.0f, 1.0f, 1.0f);   // Top-right back
    glVertex3f(1.0f, 0.5f, 1.0f);   // Top edge of door (back)
    glVertex3f(1.0f, 0.5f, -1.0f);  // Top edge of door (front)

    // Left side of the wall (left of the door)
    glVertex3f(1.0f, 0.5f, -1.0f);  // Top-left corner of door
    glVertex3f(1.0f, -1.0f, -1.0f); // Bottom-left corner of wall
    glVertex3f(1.0f, -1.0f, -0.2f); // Bottom edge of door [left]
    glVertex3f(1.0f, 0.5f, -0.2f);  // Top edge of door [left]

    // Right side of the wall (right of the door)
    glVertex3f(1.0f, 0.5f, 0.2f);   // Top edge of door [right]
    glVertex3f(1.0f, -1.0f, 0.2f);  // Bottom edge of door [right]
    glVertex3f(1.0f, -1.0f, 1.0f);  // Bottom-right corner of wall
    glVertex3f(1.0f, 0.5f, 1.0f);   // Top-right corner of door



    glEnd();
}


const float doorSpeed = 5.0f;  // Speed of door opening/closing (degrees per frame)
const int updateInterval = 10; // Milliseconds between each update

void doorAnimation(int value) {

    //changes the angle depending on if the door is open
    if (doorIsOpen) {
        if (DoorHingeAngle < 90.0f) {
            DoorHingeAngle += doorSpeed;
            glutPostRedisplay();  // Redraw the window
            glutTimerFunc(updateInterval, doorAnimation, 0);  // Call again after 'updateInterval'
        }
    }
    else {
        if (DoorHingeAngle > 0.0f) {
            DoorHingeAngle -= doorSpeed;
            glutPostRedisplay();
            glutTimerFunc(updateInterval, doorAnimation, 0);
        }
    }
}


void keyboard(unsigned char key, int x, int y) {


    switch (key) {
    case 'f':
        Forward();  // Foward
        break;
    case 'b':
        Backward(); // Backward
        break;
    case ' ': //spacebar
        
        
        doorIsOpen = !doorIsOpen;            // Toggle the door state (open to close vise versa)
        glutTimerFunc(0, doorAnimation, 0);  // Start animation
        

        break;

    default:
        break;
    }

    //updateCamera();
    glutPostRedisplay();  // Redraw the window
}


// The official display function that displays the figure
void display(void)
{

    glClearColor(0.0, 0.0, 0.0, 0.0);           // Set background to black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);               // clears color buffer AND the Depth buffer

    glMatrixMode(GL_MODELVIEW);                 // Set the matrix mode to model view
    glLoadIdentity();                           // Reset the model view matrix

    
    
    gluLookAt(eye[0], eye[1], eye[2],           // Set the camera position (eye)
        center[0], center[1], center[2],        // Set the 'center' (the point the camera is looking at)
        up[0], up[1], up[2]);                   // Set the 'up' vector (for camera)

    


    glPushMatrix();                         // Save the current matrix state
        drawRoom1();
    glPopMatrix();                          // Restore the previous matrix state


    glPushMatrix(); 
        glTranslatef(2.0, 0.0, 0.0);        //Move the 2nd (bigger) room
        drawRoom2();
    glPopMatrix();

    glPushMatrix();
        drawWall();                         //Draw the wall
    glPopMatrix();

    glPushMatrix();

        glTranslatef(1.0, 0.0, 0.0);
        glTranslatef(0.0, 0.0, 0.2);

        glRotatef(DoorHingeAngle, 0.0f, 1.0f, 0.0f);    // make sure the 1.0 is at the y axis!!

        glTranslatef(0.0, 0.0, -0.2);
        glTranslatef(-1.0, 0.0, 0.0);                   // move to the origin first before rotation

        drawDoor();

    glPopMatrix();


    glFlush();                              // Flush the rendering pipeline
    glutSwapBuffers();                      // Swap the buffers to display the updated/new image



}

// The main function. It has the code of initializing the environment
void main(int argc, char** argv)
{
    glutInitWindowSize(windowWidth, windowHeight);  // Initialize the window size
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);    // Set display mode... double buffering and RGB color... ALSO DEPTH TESTING
    glutInitWindowPosition(50, 50);                 // Set the initial window position


    glutCreateWindow("Navigation through rooms");  // Create a window with title

    glMatrixMode(GL_PROJECTION);                    // Set the matrix mode to gl_projection


    //gluPerspective(60, aspect, 0, 10);              // Set perspective projection [old]
    gluPerspective(60, aspect, 0.1, 10);                // Made sure near plane was set to a positive number

    glutDisplayFunc(display);                       // Set the display function to be called
    glutSpecialFunc(specialKeys);                   // Used to that we can use special key inputs (arrow keys) to rotate the camera

    glutKeyboardFunc(keyboard);                     // So the keys do something

    glEnable(GL_DEPTH_TEST); // Don't forget to enable depth test

    glutMainLoop();                                 // Starts the main loop and it keeps running
}