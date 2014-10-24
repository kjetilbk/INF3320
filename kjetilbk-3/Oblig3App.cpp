/* $Id: Oblig3App.cpp, v1.1 2011/09/20$
 *
 * Author: Atgeirr F Rasmussen <atgeirr@sintef.no>
 * Reviewed by: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "Oblig3App.hpp"

#include <iostream>
#include <cstdlib>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "OpenGLError.hpp"
#include "ReadTextfile.hpp"

void Oblig3App::initGL() {
  glClearColor(0.0f, 0.0f, 0.5f, 0.0f);
  glEnable(GL_CULL_FACE);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_LIGHTING);
  
  m_sphere_.init(64, 64, 1.5f);
  
  m_texture_.Load("EarthDiffuse.dds");
  
  setupShaders();
  setupLightParameters();
}

void Oblig3App::display() {
  // Clear buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Set up projection matrix
  glMatrixMode(GL_PROJECTION);
  glm::mat4x4 projection_matrix = m_viewer_.getProjectionMatrix();
  glLoadMatrixf(glm::value_ptr(projection_matrix));

  // Set the matrix mode to Modelview
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  //Light stationary with regard to camera (because MV matrix is empty)
  GLfloat light0_position[] = { 2.0f,  2.0f,  2.0f,  1.0f };
  glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
  
  // skip
  // Activate the sahader
  m_shader_.Activate();
  // Bind the texture and bind it to the uniform in the shader
  glBindTexture(GL_TEXTURE_2D, m_texture_.GetTextureId());
  GLint location = m_shader_.GetUniform("DiffuseTex");
  glUniform1i(location, m_texture_.GetTextureId()-1);
  // Set the value of the SpecularColor uniform in the FragmentShader
  location = m_shader_.GetUniform("SpecularColor");
  glUniform4f(location, 1.0f, 1.0f, 1.0f, 256.0f);
  //unskip
  
  // Render the sphere
  glm::mat4x4 model_view_matrix = m_viewer_.getModelViewMatrix();
  glLoadMatrixf(glm::value_ptr(model_view_matrix));
  m_sphere_.render();

  // Deactivate the shader
  m_shader_.Deactivate();

  // DEBUG
  // m_viewer_.renderDebugGraphics();

  glutSwapBuffers();
  CHECK_OPENGL;
}

void Oblig3App::reshape(int w, int h) {
  // skip
  glViewport(0, 0, w, h);
  m_viewer_.setWindowSize(w,h);
  // unskip
}

void Oblig3App::keyboard(unsigned char key, int /*x*/, int /*y*/) {
  if (key == 'q') {
    std::exit(0);
  }
}

void Oblig3App::mouse(int b, int s, int x, int y) {
  // skip
  if (s == GLUT_DOWN) {
    switch (b) {
      case GLUT_LEFT_BUTTON:   m_viewer_.rotationBegin(x, y); break;
      case GLUT_MIDDLE_BUTTON: m_viewer_.panBegin(x, y); break;
      case GLUT_RIGHT_BUTTON:  m_viewer_.zoomBegin(x, y); break;
    }
  } else {
    m_viewer_.resetState(x,y);
  }
  // unskip
  glutPostRedisplay();
}

void Oblig3App::motion(int x, int y) {
  // skip
  m_viewer_.motion(x, y);
  // unskip
  glutPostRedisplay();
}


int Oblig3App::displayMode() {
  return GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH;
}

std::string Oblig3App::windowName() {
  return "Oblig 3";
}

void Oblig3App::setupShaders() {
  // skip
  // Load and compile the shader files (see Shader class)
  std::string vs = GfxUtil::ReadTextfile("MyShader.vert");
  std::string fs = GfxUtil::ReadTextfile("MyShader.frag");

  m_shader_.CompileShader(GL_VERTEX_SHADER, vs);
  m_shader_.CompileShader(GL_FRAGMENT_SHADER, fs);
  // Link the shader program

  m_shader_.LinkProgram();

  std::cout << m_shader_.GetCompileLog(GL_VERTEX_SHADER) << std::endl;
  std::cout << m_shader_.GetCompileLog(GL_FRAGMENT_SHADER) << std::endl;
  std::cout << m_shader_.GetLinkLog() << std::endl;


  // unskip
}

void Oblig3App::setupLightParameters() {
  //Light atributes
  GLfloat black_light[] = {  0.0f,  0.0f,  0.0f,  1.0f };
  GLfloat white_light[] = {  1.0f,  1.0f,  1.0f,  1.0f };
  
  //Setup light 0
  glLightfv(GL_LIGHT0, GL_DIFFUSE,  white_light);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
  glLightfv(GL_LIGHT0, GL_AMBIENT,  black_light);
  
  // Enable the light
  glEnable(GL_LIGHT0); 
}
