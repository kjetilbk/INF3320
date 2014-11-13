/* -*- mode: C++; tab-width:4; c-basic-offset: 4; indent-tabs-mode:nil -*-
 *
 * Time-stamp: <2005-07-13 12:01:05 dyken>
 *
 * (C) 2005 Christopher Dyken, <dyken@cma.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "TriMesh.hpp"
#include "OpenGLError.hpp"

#include <stdexcept>
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

using std::string;
using std::vector;

namespace GfxUtil {

void TriMesh::render() {  
  // Bind VBO's
  glBindBuffer(GL_ARRAY_BUFFER, m_vertices_vbo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_vbo_id);
  // Setup the strides, offsets etc.
  glVertexPointer(3, GL_FLOAT, sizeof(Node), BUFFER_OFFSET(0));
  glNormalPointer(GL_FLOAT, sizeof(Node), BUFFER_OFFSET(12));
  // Enable the above arrays
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  // Call the drawing function
  glDrawElements(GL_TRIANGLES, 3*m_triangles.size(), GL_UNSIGNED_INT, BUFFER_OFFSET(0));
  // Unbind VBOs
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  // Disable arrays
  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
}

void TriMesh::prepareBufferObjects(const vector<int>& indices) { 
  glGenBuffers(1, &m_vertices_vbo_id);
  glGenBuffers(1, &m_indices_vbo_id);

  // Bind VBOs and provide data to them  
  glBindBuffer(GL_ARRAY_BUFFER, m_vertices_vbo_id);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indices_vbo_id);
  
  glBufferData(GL_ARRAY_BUFFER, m_nodes.size() * sizeof(Node), &m_nodes[0], GL_STATIC_DRAW);    
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

glm::vec3 calculateNormal(glm::vec3 i, glm::vec3 j, glm::vec3 k){
  return glm::cross((j-i), (k-i)) / glm::length(glm::cross(j-i, k-i));
}

void TriMesh::computeNormals() {
  // skip
  // Calculate triangle normals
  for(int i = 0; i < m_triangles.size(); i++){
    glm::vec3 v1; glm::vec3 v2; glm::vec3 v3;
    HalfEdge* tempLeader = m_triangles[i]->getLeadingHalfEdge();
    v1 = tempLeader->getSourceNode()->m_pos_;
    tempLeader = tempLeader->getNext();
    v2 = tempLeader->getSourceNode()->m_pos_;
    tempLeader = tempLeader->getNext();
    v3 = tempLeader->getSourceNode()->m_pos_;
    m_triangles[i]->m_N_ = calculateNormal(v1,v2,v3);

  }
  // Calculate vertex normals by averaging triange normals
  for(int i = 0; i < m_nodes.size(); i++){
    Node n = m_nodes[i];
    HalfEdge* startEdge = n.getLeadingHalfEdge();
    Triangle* startTriangle = startEdge->getTriangle();
    glm::vec3 accNormal = startTriangle->m_N_;
    int triangleCount = 1;
    for(HalfEdge* tempEdge = startEdge->getVtxRingNext(); tempEdge != startEdge; tempEdge = tempEdge->getVtxRingNext()){
		if (tempEdge == NULL)
			break;
		accNormal += tempEdge->getTriangle()->m_N_;
		triangleCount++;
    }
    glm::vec3 count = glm::vec3(triangleCount);
    m_nodes[i].m_N_ = accNormal/count;

  }

  // unskip
}

}  // GfxUtil
