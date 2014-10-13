/* $Id: SimpleViewer.cpp, v1.1 2011/09/20$
*
* Author: Christopher Dyken, <dyken@cma.uio.no>
* Reviewed by: Bartlomiej Siwek, <bartloms@ifi.uio.no>
*
* Distributed under the GNU GPL.
*/
#include "SimpleViewer.hpp"
#include <iostream>
#include <cmath>
#include <stdexcept>
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
namespace GfxUtil {
	SimpleViewer::SimpleViewer() {
		setViewVolume(glm::vec3(0.0f, 0.0f, 0.0f), 1.74);
	}
	void SimpleViewer::setWindowSize(int w, int h) {
		m_window_width = w;
		m_window_height = h;
		m_window_aspect = (float)w / (float)h;
	}
	void SimpleViewer::setViewVolume(glm::vec3 origin, float radius) {
// skip
// set viewvolume related variables...
		m_viewvolume_origin = origin;
		m_viewvolume_radius = radius;
		m_viewvolume_epsilon = 0.000001f;
// unskip
	}
	void SimpleViewer::rotationBegin(int x, int y) {
// skip
		this->m_state = ROTATING;
		m_rotation_orientation_i = glm::fquat(m_camera_orientation);
		m_rotation_mousepos_i = getPointOnUnitSphere(glm::vec2(x,y));
// handle beginning of the rotation...
// unskip
	}
	void SimpleViewer::panBegin(int x, int y) {
// skip
// handle beginning of panning... (OPTIONAL)
// unskip
	}
	void SimpleViewer::zoomBegin(int x, int y) {
// skip
// handle beginning of zoom... (OPTIONAL)
// unskip
	}
void SimpleViewer::resetState(int /*x*/, int /*y*/) {
	m_state = NONE;
}
void SimpleViewer::motion(int x, int y) {
// skip
// handle mouse motion while one of the mouse buttons is down
	m_rotation_mousepos_c = getPointOnUnitSphere(glm::vec2(x, y));
	glm::fquat t = getGreatCircleRotation(m_rotation_mousepos_c, m_rotation_mousepos_i);
	m_camera_orientation = m_rotation_orientation_i*t;
// unskip
}
glm::mat4x4 SimpleViewer::getProjectionMatrix() {
	glm::mat4x4 result;
// skip
	result = glm::perspective(90.0f, 1.0f, 0.01f, 100.0f);
// compute projection matrix...
// unskip
	return result;
}
glm::vec3 multiplyWithQuat(glm::vec3 vec, glm::fquat quat){
	return quat*vec;
}
glm::mat4x4 SimpleViewer::getModelViewMatrix() {
	glm::mat4x4 result;
// skip
// compute modelview matrix (a rotation should be preceded by a translation of the camera)...
	glm::vec3 cameraPos = multiplyWithQuat(glm::vec3(0.0f, 0.0f, 3.0f), m_camera_orientation);
	glm::vec3 lookingAt = multiplyWithQuat(glm::vec3(0.0f, 0.0f, 0.0f), m_camera_orientation);
	glm::vec3 upVec = multiplyWithQuat(glm::vec3(0.0f, 1.0f, 0.0f), m_camera_orientation);
	result = glm::lookAt(cameraPos, lookingAt, upVec);
// unskip
	return result;
}
void SimpleViewer::renderDebugGraphics() {
// This is a debug method that uses the deprecated API
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(-m_window_aspect, m_window_aspect, -1.0f, 1.0f, -1.0f, 1.0f);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	switch (m_state) {
		case ROTATING:
		glLineWidth(3.0);
		glColor3f(0.4, 0.4, 0.4);
		glBegin(GL_LINE_LOOP);
		for (size_t i = 0; i<50; i++)
			glVertex2f(glm::cos(2.0*M_PI*i / (float)50), glm::sin(2.0*M_PI*i / (float)50));
		glEnd();
		glBegin(GL_LINES);
		glColor3f(1.0, 0.5, 0.5);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(&m_rotation_axis[0]);
		glColor3f(0.5, 1.0, 0.5);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(&m_rotation_mousepos_i[0]);
		glColor3f(0.5, 0.5, 1.0);
		glVertex3f(0.0f, 0.0f, 0.0f);
		glVertex3fv(&m_rotation_mousepos_c[0]);
		glEnd();
		break;
		default:
// Do nothing.
		break;
	}
}
glm::vec2 SimpleViewer::getNormalizedCoords(int x, int y) {
// skip
	float _x = ( (float)x / (float)m_window_width) - 0.5f;
	float _y = 0.5f - (float)y / (float)m_window_height;
// unskip
	return glm::vec2(_x, _y);
}
glm::vec3 SimpleViewer::getPointOnUnitSphere(glm::vec2 p) {
// skip
// project mouse position in a convinient coordinate system to a unit sphere...
	glm::vec2 _p = getNormalizedCoords(p.x, p.y);
	float r = glm::sqrt((_p.x*_p.x) + (_p.y*_p.y));
	float x;
	float y;
	float z;
	if (r < 0.5f){
		x = 2 * _p.x;
		y = 2 * _p.y;
		z = glm::sqrt(1 - (4 * r*r));
	}
	else{
		x = _p.x / r;
		y = _p.y / r;
		z = 0;
	}
	std::cout << r << std::endl;
	return glm::vec3(x, y, z);
// unskip
}
// This function computes the quaternion corresponding to the great circle rotation between two vectors
glm::fquat SimpleViewer::getGreatCircleRotation(glm::vec3 a, glm::vec3 b) {
	glm::vec3 axis = glm::cross(a, b);
	float length = glm::length(axis);
	if (length > std::numeric_limits<float>::epsilon()) {
		axis = axis / length;
		float angle = glm::acos(glm::dot(a, b));
		float c = glm::cos(0.5*angle);
		float s = glm::sin(0.5*angle);
		return glm::fquat(c, s*axis[0], s*axis[1], s*axis[2]);
	}
	else {
		return glm::fquat(1.0f, 0.0f, 0.0f, 0.0f);
	}
}
// This function computes the quaternion corresponding to the rotation by a give angle around given axis
glm::fquat SimpleViewer::getAxisAngleRotation(glm::vec3& axis, float angle) {
	float c = glm::cos(0.5f * angle);
	float s = glm::sin(0.5f * angle);
	return glm::fquat(c, s*axis[0], s*axis[1], s*axis[2]);
}
} // namespace GfxUtil