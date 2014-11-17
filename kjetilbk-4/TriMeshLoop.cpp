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
#include <glm/gtx/compatibility.hpp>

using std::string;
using std::vector;

namespace GfxUtil {

TriMesh* TriMesh::subdivideLoop() {
  vector<glm::vec3> points;
  vector<int> indices;
  /*for each (Triangle* t in m_triangles)
  {
	  points.push_back(glm::lerp(t->getNode(0)->m_pos_, t->getNode(1)->m_pos_, 0.5f));
	  int pointA = points.size() - 1;
	  points.push_back(glm::lerp(t->getNode(1)->m_pos_, t->getNode(2)->m_pos_, 0.5f));
	  int pointB = points.size() - 1;
	  points.push_back(glm::lerp(t->getNode(2)->m_pos_, t->getNode(0)->m_pos_, 0.5f));
	  int pointC = points.size() - 1;

	  if (t->getNode(0)->index == -1){
		  points.push_back(t->getNode(0)->m_pos_);
		  t->getNode(0)->index = points.size() - 1;
	  }
	  if (t->getNode(1)->index == -1){
		  points.push_back(t->getNode(1)->m_pos_);
		  t->getNode(1)->index = points.size() - 1;
	  }
	  if (t->getNode(2)->index == -1){
		  points.push_back(t->getNode(2)->m_pos_);
		  t->getNode(2)->index = points.size() - 1;
	  }
	  int point0 = t->getNode(0)->index;
	  int point1 = t->getNode(1)->index;
	  int point2 = t->getNode(2)->index;

	  indices.push_back(point0); indices.push_back(pointA); indices.push_back(pointC);
	  indices.push_back(pointA); indices.push_back(point1); indices.push_back(pointB);
	  indices.push_back(pointC); indices.push_back(pointB); indices.push_back(point2);
	  indices.push_back(pointA); indices.push_back(pointB); indices.push_back(pointC);
  }
  //skip
  // Generate a new set of points and indices using the Loop subdivision scheme
  // unskip
  */
  return new TriMesh(points, indices);
}

}  // GfxUtil
