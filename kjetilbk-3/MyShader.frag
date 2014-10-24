#version 120

// A LITTLE CHEAT SHEET OF LIGHT PROPERTIES!
//Light properties
// struct gl_LightSourceParameters
// {
//   vec4 position;
//   vec4 halfVector;
//
//   vec4 ambient;
//   vec4 diffuse;
//   vec4 specular;
//
//   vec3 spotDirection;
//   float spotExponent;
//   float spotCutoff;
//   float spotCosCutoff;
//
//   float constantAttenuation;
//   float linearAttenuation;
//   float quadraticAttenuation;
// };
//
// uniform gl_LightSourceParameters gl_LightSource[gl_MaxLights];

uniform sampler2D DiffuseTex;
varying float testy;
uniform vec4 SpecularColor;

varying vec3 VSPosition;
varying vec3 VSNormal;
varying vec2 TexCoord;

//Non-boilerplate here
varying vec3 N;
varying vec3 v;
varying vec3 Normal;
//end self-made poop

void main() {
  // Get the light vector
  // Insert phong shading model here (point light with attenuation)
  // Use half-vector instead of reflection vector for specular reflection.
  // Take the light parameters from gl_LightSource[0] structure.
  // Take the diffuse color from DiffuseTex texture.
  // Use the same color for ambiet lighting that you used for diffuse lighting.
  // Take the specular color from RGB components of SpecularColor.
  // Take the specular exponent from A component of SpecularColor (scale it by muliplying by 256)
  // Remeber to normalize vectors.

  vec4 difMat = gl_FrontMaterial.diffuse;
  vec4 specMat = gl_FrontMaterial.specular;

  vec3 l = gl_LightSource[0].position.xyz - VSPosition;
  vec3 lightVec = normalize(l);
  
  vec3 normalVec = normalize(Normal);
  vec3 eyeVec = normalize(vec3(0.0, 0.0, 3.0) - VSPosition.xyz);
  vec3 halfVec = normalize(lightVec + eyeVec);
  
  vec4 ambient = gl_LightSource[0].ambient;
  
  vec4 diffuse = gl_LightSource[0].diffuse * vec4(max(dot(lightVec, normalVec), 0.0)) * difMat;

  
  vec4 s = vec4(max(dot(normalVec, halfVec), 0.0));
  vec4 specular = gl_LightSource[0].specular * pow(s, vec4(testy));

  
  gl_FragColor = ambient + diffuse + specular;
}