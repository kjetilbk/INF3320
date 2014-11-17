/* -*- mode: C++; tab-width:4; c-basic-offset: 4; indent-tabs-mode:nil -*-
 *
 * Time-stamp: <2005-07-13 12:01:05 dyken>
 *
 * (C) 2005 Christopher Dyken, <dyken@cma.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "TriMesh.hpp"

#include <cmath>
#include <stdexcept>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/compatibility.hpp>

using std::string;
using std::vector;

namespace GfxUtil {

TriMesh* TriMesh::subdivideSqrt3() {
  vector<glm::vec3> points;
  vector<int> indices;
  for (int i = 0; i < m_nodes.size(); i++){
	  HalfEdge* start_edge = m_nodes[i].getLeadingHalfEdge();
	  int surroundingPoints = 0;
	  glm::vec3 new_point = glm::vec3(0);
	  points.push_back(m_nodes[i].m_pos_);	//Pushing node vertex to points.
  	  m_nodes[i].index = points.size() - 1; //Setting node index.
	  HalfEdge* he = start_edge;
	  do{
		  int center_of_triangle;
		  //Checking if triangle center already exists.
		  if (he->getTriangle()->getCenter() == -1){
			  points.push_back(he->getTriangle()->calculateBarycenter());
			  he->getTriangle()->setCenter(points.size() - 1);
		  }
		  center_of_triangle = he->getTriangle()->getCenter();

		  /*Boundary case*/
		  if (he->isBoundary()){
			  points.push_back(glm::lerp(m_nodes[i].m_pos_, he->getDestinationNode()->m_pos_, 0.5f));	//Pushing mid-edge point.
			  int middle_of_edge = points.size() - 1;
			  int end_of_edge;
			  //Checking if end point on edge already exists.
			  if (he->getDestinationNode()->index == -1){
				  points.push_back(he->getDestinationNode()->m_pos_);		//Pushing if not.
				  he->getDestinationNode()->index = points.size() - 1;
			  }
			  end_of_edge = he->getDestinationNode()->index;
			  
			  //Pushing boundary triangle 1.
			  indices.push_back(middle_of_edge);
			  indices.push_back(center_of_triangle);
			  indices.push_back(m_nodes[i].index);
			  
			  //Pushing boundary triangle 2.
			  indices.push_back(middle_of_edge);
			  indices.push_back(end_of_edge);
			  indices.push_back(center_of_triangle);
			  
			  
		  }
		  /*Normal non-boundary edge*/
		  else{
			  int center_of_twin_triangle;
			  Triangle* twin_triangle = he->getTwin()->getTriangle();
			  //Checking if triangle center already exists.
			  if (twin_triangle->getCenter() == -1){
				  points.push_back(twin_triangle->calculateBarycenter());
				  twin_triangle->setCenter(points.size() - 1);
			  }
			  center_of_twin_triangle = twin_triangle->getCenter();

			  //Pushing new triangle
			  indices.push_back(m_nodes[i].index);
			  indices.push_back(center_of_twin_triangle);
			  indices.push_back(center_of_triangle);  
		  }
		  surroundingPoints++;
		  new_point += he->getDestinationNode()->m_pos_;
		  he = he->getVtxRingNext();
	  } while (he != start_edge);
	  glm::vec3 beta = glm::vec3((4 - 2*cos(2*M_PI/surroundingPoints))/(9*surroundingPoints));
	  new_point = (1 - surroundingPoints * beta.x)*m_nodes[i].m_pos_ + beta*new_point;
	  points[m_nodes[i].index] = new_point;
  }


  // skip
  // Generate a new set of points and indices using the sqrt(3) subdivision scheme
  // unskip
  
  return new TriMesh(points, indices);
}

}  // GfxUtil
