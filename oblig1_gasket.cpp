//
//  oblig1_gasket.cpp
//  kjetilbk-1
//
//  Created by Kjetil Kristoffersen on 30/08/14.
//  Copyright (c) 2014 Kjetil Kristoffersen. All rights reserved.
//

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
static GLuint buffs[2];
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
    glBindBuffer(GL_ARRAY_BUFFER, buffs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex)*vertices.size(), vertices.data(), GL_STATIC_DRAW);
    glVertexPointer(2, GL_FLOAT, 0, 0);
    glEnableClientState(GL_VERTEX_ARRAY);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size()*sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
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
    glGenBuffers(2, buffs);
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
			bindDataToBuffers();
            // ... end of your code ...
            break;
        case '-':
            level = level > 0 ? level-1 : 0;
            // Rebuild gasket and bind data to buffers
            // ... insert your code here ...
            rebuildGasket();
			bindDataToBuffers();
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

    glColor3f(0, 1, 0); // Set color to green

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffs[1]);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
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
    glutInitContextVersion(3, 1);
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