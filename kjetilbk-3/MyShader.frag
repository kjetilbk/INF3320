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
uniform vec4 SpecularColor;

varying vec3 VSPosition;
varying vec3 VSNormal;
varying vec2 TexCoord;

void main() {
  // Get the light vector
  // Insert phong shading model here (point light with attenuation)

  // Take the light parameters from gl_LightSource[0] structure.
  vec3 l = gl_LightSource[0].position.xyz - VSPosition;
  vec3 lightVec = normalize(l);
  //Attenuation calculation
  float constAtten = gl_LightSource[0].constantAttenuation;
  float linearAtt = gl_LightSource[0].linearAttenuation;
  float quadAtt = gl_LightSource[0].quadraticAttenuation;
  float dist = length(l);
  float attenuation = 1 / (constAtten + (linearAtt * dist) + (quadAtt * (dist*dist)));
  // Take the diffuse color from DiffuseTex texture.
  vec4 DiffuseColor = texture2D(DiffuseTex, TexCoord);
  // Vector calculation
  // Use half-vector instead of reflection vector for specular reflection.
  vec3 normalVec = normalize(VSNormal);
  vec3 eyeVec = normalize(vec3(0.0, 0.0, 3.0) - VSPosition.xyz);
  vec3 halfVec = normalize(lightVec + eyeVec);
  // Use the same color for ambiet lighting that you used for diffuse lighting.
  vec4 ambient = gl_LightSource[0].ambient * DiffuseColor;
  vec4 diffuse = gl_LightSource[0].diffuse * vec4(max(dot(lightVec, normalVec), 0.0)) * DiffuseColor;
  // Take the specular color from RGB components of SpecularColor.
  vec4 specCol = vec4(SpecularColor.rgb, 1.0);
  // Take the specular exponent from A component of SpecularColor (scale it by muliplying by 256)
  float specExp = SpecularColor.a * 256.0;

  //Calculate the specular
  vec4 s = vec4(max(dot(normalVec, halfVec), 0.0));
  vec4 specular = gl_LightSource[0].specular * pow(s, vec4(SpecularColor.a*256.0)) * vec4(SpecularColor.rgb, 1.0) * attenuation;

  
  gl_FragColor = ambient + diffuse + specular;
}
