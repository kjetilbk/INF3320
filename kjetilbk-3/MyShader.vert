#version 120

uniform float test;
varying vec3 VSPosition;
varying vec3 VSNormal;
varying vec2 TexCoord;
varying float testy;

varying vec3 Normal;


void main() {
  // Insert the vertex shader here
  // Transform the vertex position to view space for fragment sahder
  // Transform the normal to view space for fragment sahder (remeber the matrix!)
  // Pass on the texture coordinates
  Normal = vec3(gl_NormalMatrix*gl_Normal);
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  VSPosition = vec3(gl_ModelViewMatrix * gl_Vertex);
  testy = test;
}
