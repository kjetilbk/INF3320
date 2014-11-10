/* -*- mode: C++; tab-width:4; c-basic-offset: 4; indent-tabs-mode:nil -*-
 *
 * Time-stamp: <2005-07-13 12:01:05 dyken>
 *
 * (C) 2005 Christopher Dyken, <dyken@cma.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "TriMesh.hpp"

#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>

using std::string;
using std::vector;

namespace GfxUtil {

TriMesh* TriMesh::subdivideSqrt3() {
  vector<glm::vec3> points;
  vector<int> indices;
  vector<glm::vec3> new_triangle_points;

  for(int i = 0; i<m_triangles.size(); i++){
  	glm::vec3 new_point;
  	glm::vec3 v1; glm::vec3 v2; glm::vec3 v3;
    HalfEdge* tempLeader = m_triangles[i]->getLeadingHalfEdge();
    v1 = tempLeader->getSourceNode()->m_pos_;
    tempLeader = tempLeader->getNext();
    v2 = tempLeader->getSourceNode()->m_pos_;
    tempLeader = tempLeader->getNext();
    v3 = tempLeader->getSourceNode()->m_pos_;

    new_point = (v1+v2+v3)/glm::vec3(3);
    new_triangle_points.push_back(new_point);
  }

  for()

  // skip
  // Generate a new set of points and indices using the sqrt(3) subdivision scheme
  // unskip
  
  return new TriMesh(points, indices);
}

}  // GfxUtil
