#version 150

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
 
out vec3 fN;
out vec3 fE;
out vec3 fL;
out vec2 texCoord;
out vec3 R;
out vec3 T;

uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform vec4 LightPosition;
uniform vec4 LightDirection;


void main() 
{

  fE = -(model_matrix * vPosition).xyz;
  fN = (model_matrix * vec4(vNormal, 0.0)).xyz;
  fL = LightPosition.xyz - (model_matrix * vPosition).xyz;
  
  vec3 eyePos = vPosition.xyz;
  vec4 NN = model_matrix * vec4(vNormal, 1.0);
  vec3 N = normalize(NN.xyz);
  R = reflect(eyePos, N);
  T = refract(eyePos, N, 0.75);
  
  texCoord = vTexCoord;
  gl_Position = projection_matrix * model_matrix * vPosition;
} 
