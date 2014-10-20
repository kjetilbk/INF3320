#version 120

varying vec3 VSPosition;
varying vec3 VSNormal;
varying vec2 TexCoord;

varying vec3 Normal;

void main() {
  // Insert the vertex shader here
  // Transform the vertex position to view space for fragment sahder
  // Transform the normal to view space for fragment sahder (remeber the matrix!)
  // Pass on the texture coordinates
  Normal = gl_NormalMatrix*gl_Normal;
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  VSPosition = vec3(gl_Position);
}
