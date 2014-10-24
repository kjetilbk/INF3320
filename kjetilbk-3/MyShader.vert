#version 120

varying vec3 VSPosition;
varying vec3 VSNormal;
varying vec2 TexCoord;


void main() {
  // Insert the vertex shader here
  // Transform the vertex position to view space for fragment shader
  VSPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
  // Transform the normal to view space for fragment sahder (remember the matrix!)
  VSNormal = vec3(gl_NormalMatrix*gl_Normal);
  // Pass on the texture coordinates
  TexCoord = gl_MultiTexCoord0.st;

  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
