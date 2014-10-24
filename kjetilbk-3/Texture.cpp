#include "Texture.hpp"

#include <stdexcept>
#include <iostream>
#include <map>
#include <GL/glew.h>
#include <gli/gli.hpp>
#include <gli/gtx/gl_texture2d.hpp>
#include <GL/gl.h>

namespace GfxUtil {

Texture::Texture()
    : m_texture_id_(0) {
}

Texture::~Texture() {
  if(m_texture_id_ != 0) {
    // skip
    // Insert texture deletion here
	  glDeleteTextures(1, &m_texture_id_);
    // unskip
  }
}

void Texture::Load(const std::string &filename) {
  // Load only if we havent done so already
  if(m_texture_id_ == 0) {
    // skip
    // Insert texture creation here

	  // Use gli::createTexture2D to load and create texture name and bind it
	  m_texture_id_ = gli::createTexture2D("EarthDiffuse.dds");
	  glBindTexture(GL_TEXTURE_2D, m_texture_id_);
    // Set wrapping to repeat in both S and T directions
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // Set linear interpolation for magnification and linear-mipmap-linear for minification
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    // Construct mipmaps
	  glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE);
    // unskip
  }
}

GLuint Texture::GetTextureId() {
  return m_texture_id_;
}

}  // GfxUtil
