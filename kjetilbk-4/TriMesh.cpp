/* $Id: TriMesh.cpp, v1.1 2011/10/17$
 *
 * Author: Christopher Dyken, <dyken@cma.uio.no>
 * Reviewed by: Bartlomiej Siwek, <bartloms@ifi.uio.no>
 *
 * Distributed under the GNU GPL.
 */

#include "TriMesh.hpp"
#include "OpenGLError.hpp"

#include <stdexcept>
#include <fstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <GL/glew.h>
#include <glm/glm.hpp>

 using std::runtime_error;
 using std::fstream;
 using std::string;
 using std::vector;

 namespace GfxUtil {

  TriMesh::TriMesh() {
  }

  TriMesh::TriMesh(const string& filename) {
    readMesh(filename);
    CHECK_OPENGL;
  }

  TriMesh::TriMesh(const vector<glm::vec3>& points, const vector<int>& indices) {
    buildTriangulation(points, indices);
    CHECK_OPENGL;
  }

  void TriMesh::buildTriangulation(const vector<glm::vec3>& points, const vector<int>& indices) {
  // create nodes
    for(size_t i = 0; i < points.size(); i++) {
      m_nodes.push_back(Node(points[i]));
    }

  // create triangles and half-edges
    for(size_t j = 0; j < indices.size()/3; j++) {
      HalfEdge* he[3];
      for(size_t i = 0; i < 3; i++) {
        he[i] = new HalfEdge(&m_nodes[indices[3*j+i]], NULL, NULL, NULL);
        m_halfedges.push_back(he[i]);
      }
      Triangle* tri = new Triangle(he[0]);
      m_triangles.push_back(tri);

      for(size_t i=0; i<3; i++) {
        he[i]->m_next_ =  he[(i+1)%3];
        he[i]->m_face_ = tri;
        if(he[i]->m_from_->m_he_ == NULL)
          he[i]->m_from_->m_he_ = he[i];
      }
    }

  // stitch together neighbouring half-edges
    buildConnectivity();

    calcBBox();
    computeNormals();

    prepareBufferObjects(indices);
  }

  TriMesh::~TriMesh() {
    if(m_vertices_vbo_id != 0) {
      glDeleteBuffers(1, &m_vertices_vbo_id);
    }
    if(m_indices_vbo_id != 0) {
      glDeleteBuffers(1, &m_indices_vbo_id);
    }

    for(size_t i = 0; i < m_triangles.size(); i++) {
      delete m_triangles[i];
    }

    for(size_t i = 0; i < m_halfedges.size(); i++) {
      delete m_halfedges[i];
    }
  }

  void TriMesh::readMesh(const std::string& filename) {
    fstream in(filename.c_str(), std::ios::in);
    if(!in.good()) {
      throw runtime_error("Error reading from " + filename);
    }

    size_t Nv, Nt;
    in >> Nv >> Nt;

    vector<glm::vec3> points;
    for(size_t i=0; i < Nv; i++) {
      float x, y, z;
      in >> x >> y >> z;
      points.push_back(glm::vec3(x,y,z));
    }

    vector<int> indices;
    for(size_t i=0; i < 3*Nt; i++) {
      int ix;
      in >> ix;
      indices.push_back(ix);
    }
    in.close();

    buildTriangulation(points, indices);
  }

  struct TriMesh::SortElement {
    SortElement(HalfEdge *he) {
      m_he = he;
      m_a = m_he->m_from_;
      m_b = m_he->m_next_->m_from_;
      if(m_b < m_a) {
        Node *t = m_a; m_a = m_b; m_b=t;
      }
    }

    friend bool operator<(const SortElement &a, const SortElement &b)
    {
      if(a.m_a == b.m_a) {
        return a.m_b < b.m_b;
      }
      return a.m_a < b.m_a;
    }

    Node *m_a;
    Node *m_b;
    HalfEdge *m_he;  
  };

  void TriMesh::calcBBox() {
    if(m_nodes.size() == 0) {
      return;
    }
    m_bbox_min = m_bbox_max = m_nodes[0].m_pos_;
    for(size_t j=1; j<m_nodes.size(); j++) {
      const glm::vec3& p = m_nodes[j].m_pos_;

      for(size_t i=0; i<3; i++) {
        m_bbox_min[i] = p[i] < m_bbox_min[i] ? p[i] : m_bbox_min[i];
        m_bbox_max[i] = p[i] > m_bbox_max[i] ? p[i] : m_bbox_max[i];
      }
    }
  }

  void TriMesh::buildConnectivity() {
  // skip
  // Use the algorithm from the lecture to categorize the edges into inner, boundary or non-manifold
  // Set the twin edge appropriately to the category of the current edge
  // For each node take its leading halfedge and rewind (using getVtxRingPrev) until:
  // - We get the one that we started with (no need to do anything)
  // - We get NULL - then the previous choice should be used as the nodes leading halfedge
  // unskip

    std::vector<SortElement> helper = std::vector<SortElement>();

    for(int i = 0; i < m_halfedges.size(); i++){
      helper.push_back(m_halfedges[i]);
    }

    std::sort(helper.begin(), helper.end());
    int i,j;
  //Loop through all edges
    for(j=0; j<helper.size(); j++){
      //Find i so that i and i+1 are different edges
      for(i=j; i<helper.size()-1; i++){
        if(helper[i].m_a != helper[i+1].m_a || helper[i].m_b != helper[i+1].m_b){
          break;
        }
      }

      if(i==j){
        helper[j].m_he->m_twin_ = NULL;
        // helper[j] points to a boundary edge
      }

      else if (i==j+1){
        helper[i].m_he->m_twin_ = helper[j].m_he;
        helper[j].m_he->m_twin_ = helper[i].m_he;
        // connect the triangles of helper[i] and helper[j]
      }
      else{
        std::cout << "Non-manifold mesh!" << std::endl;
      }
      j = i;
    }

	for (int i = 0; i < m_nodes.size(); i++){
		HalfEdge* beginLead = m_nodes[i].getLeadingHalfEdge();
		HalfEdge* tempLead = m_nodes[i].getLeadingHalfEdge();
		while(true){
			HalfEdge* rewindHe = tempLead->getVtxRingPrev();
			if (rewindHe == NULL){
				m_nodes[i].m_he_ = tempLead;
				break;
			}
			else if (rewindHe == beginLead)
				break;
			tempLead = rewindHe;
		}
	}

  }

  TriMesh* TriMesh::subdivide() {
  // Choose one call:
   //return subdivideLoop();
   return subdivideSqrt3();
    return 0;
  }

}  // GfxUtil
