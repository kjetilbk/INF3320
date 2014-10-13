#include "Texture.hpp"

#include <stdexcept>
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
    // unskip
  }
}

void Texture::Load(const std::string &filename) {
  // Load only if we havent done so already
  if(m_texture_id_ == 0) {
    // skip
    // Insert texture creation here
    // Use gli::createTexture2D to load and create texture name and bind it
    // Set wrapping to repear in bith S and T directions
    // Set linear interpolation for magnification and linear-mipmap-linear for minification
    // Construct mipmaps
    // unskip
  }
}

GLuint Texture::GetTextureId() {
  return m_texture_id_;
}

}  // GfxUtil
