//
//  oblig1_gasket.cpp
//  kjetilbk-1
//
//  Created by Kjetil Kristoffersen on 30/08/14.
//  Copyright (c) 2014 Kjetil Kristoffersen. All rights reserved.
//

#include "oblig1_gasket.h"

/* $Id: oblig1_gasket.cpp, v1.0 2011/09/07$
 *
 * Author: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include <cstdlib>
#include <cmath>
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <OpenGL/gl3.h>
#include <GL/freeglut.h>


#include <glm/glm.hpp>

#include "OpenGLError.hpp"

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

// Vertex structure
struct Vertex {
    Vertex(float x, float y)
    : position(x, y) {
    }
    
    Vertex(const glm::vec2 &p)
    : position(p) {
    }
    
    glm::vec2 position;
};

// Global variables
static std::vector<Vertex> vertices;
static std::vector<unsigned int> indices;
static int level = 0;

// Vertex buffer object IDs
// ... insert your code here ...
// ... end of your code ...

void pushTriangle(unsigned int a, unsigned int b, unsigned int c){
    indices.push_back(a);
    indices.push_back(b);
    indices.push_back(c);
}

void sierpinskiGasket(unsigned int a, unsigned int b, unsigned int c, unsigned int level) {
    if(level == 0) {
        pushTriangle(a,b,c);
        // Generate new vertices and call this function recursively
        // ... insert your code here ...
        // ... end of your code ...

    }
    else{
        Vertex newTrianglePointA = Vertex((vertices[a].position.x+vertices[b].position.x)/2,
                                          (vertices[a].position.y+vertices[b].position.y)/2);
        Vertex newTrianglePointB = Vertex((vertices[b].position.x+vertices[c].position.x)/2,
                                          (vertices[b].position.y+vertices[c].position.y)/2);
        Vertex newTrianglePointC = Vertex((vertices[c].position.x+vertices[a].position.x)/2,
                                          (vertices[c].position.y+vertices[a].position.y)/2);
        int indexA = vertices.size();
        int indexB = indexA+1;
        int indexC = indexB+1;
        
        vertices.push_back(newTrianglePointA);
        vertices.push_back(newTrianglePointB);
        vertices.push_back(newTrianglePointC);
        level = level-1;
        
        sierpinskiGasket(a, indexA, indexC, level);
        sierpinskiGasket(indexA, b, indexB, level);
        sierpinskiGasket(indexC, indexB, c, level);
        
        
        
    }
}

void rebuildGasket() {
    // Clear the data arrays
    vertices.clear();
    indices.clear();
    
    // Insert basic vertices
    vertices.push_back(Vertex(-1.0f, -1.0f));
    vertices.push_back(Vertex( 1.0f, -1.0f));
    vertices.push_back(Vertex( 0.0f,  1.0f));
    
    // Start the recursion
    sierpinskiGasket(0, 1, 2, level);
}

void bindDataToBuffers() {
    // Bind VBOs and provide data to them
    // ... insert your code here ...
    // ... end of your code ...
    
    // Unbind the VBO's after we are done
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    
    CHECK_OPENGL;
}

void myInit()
{
    // Initialize the gasket
    rebuildGasket();
    
    // Generate VBO ids
    // ... insert your code here ...

    // ... end of your code ...
    
    // Bind data to buffers
    bindDataToBuffers();
    
    // Initialize OpenGL specific part
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    
    CHECK_OPENGL;
}

void myKeyboard(unsigned char key, int /* x */, int /* y */)
{
    switch(key) {
        case '+':
            level++;
            // Rebuild gasket and bind data to buffers
            // ... insert your code here ...
            rebuildGasket();
            // ... end of your code ...
            break;
        case '-':
            level = level > 0 ? level-1 : 0;
            // Rebuild gasket and bind data to buffers
            // ... insert your code here ...
            rebuildGasket();
            // ... end of your code ...
            break;
    }
    
    glutPostRedisplay();
}

void myDisplay()
{
    // Clear the backgound
    glClear(GL_COLOR_BUFFER_BIT);
    
    // Bind VBO's and call a drawing function
    // ... insert your code here ...
    glMatrixMode( GL_MODELVIEW); // Model view transformation
    glLoadIdentity(); // (model -> eye space)
    glTranslatef(0, 0, -1); // A simple translation
    
    glMatrixMode( GL_PROJECTION); // Projection transformation
    glLoadIdentity(); // (eye -> clip space)
    glFrustum(-1,1,-1,1,1,1000); // Perspective camera model
    
    glBegin( GL_TRIANGLES); // Begin issuing a triangle
    glColor3f(0, 1, 0); // Set color to green

    
    for(int i = 0; i<indices.size(); i++){
        glVertex2f(vertices[indices[i]].position.x, vertices[indices[i]].position.y);
    }

    glEnd(); // Finish issuing the polygon
    // ... end of your code ...
    
    // Switch the buffer
    glFlush();
    glutSwapBuffers();
    
    CHECK_OPENGL;
}

void myShutdown() {
    // Delete VBOs
    // ... insert your code here ...
    // ... end of your code ...
}

int main(int argc, char **argv)
{
    // Initialization of GLUT
    glutInit(&argc, argv);
#ifndef __APPLE__
    glutInitContextVersion(3, 1);
#else
    glutInitContextVersion(2,1);
#endif
    glutInitContextFlags(GLUT_DEBUG);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(512, 512);
    glutCreateWindow( __FILE__ );
    
    // Init GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err) {
        std::cerr << "Error: " << glewGetErrorString(err) << std::endl;
        return 1;
    }
    
#ifndef __APPLE__
    if(!GLEW_VERSION_3_1) {
        std::cerr << "Driver does not support OpenGL 3.1" << std::endl;
        return 1;
    }
#endif
    
    // Attach handlers
    glutDisplayFunc(myDisplay);
    glutKeyboardFunc(myKeyboard);
    
    // A nasty trick to get a shutdown handler
    atexit(myShutdown);
    
    // Application specific initialization
    myInit();
    
    // Run the GLUT main loop
    glutMainLoop();
    return 0;
}