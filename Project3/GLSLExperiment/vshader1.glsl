#version 150

in vec4 vPosition;
in vec3 vNormal;
flat out vec4 flatColor;  
out vec3 fN;
out vec3 fE;
out vec3 fL;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform vec4 LightPosition;
uniform float Shininess;
uniform vec4 LightDirection;
uniform float cutoff_theta;

void main() 
{

  fE = -(model_matrix * vPosition).xyz;
  fN = (model_matrix * vec4(vNormal, 0.0)).xyz;
  fL = LightPosition.xyz - (model_matrix * vPosition).xyz;
  
  vec3 L = normalize(fL);
  vec3 E = normalize(fE);
  vec3 N = normalize(fN);
  vec3 H = normalize(L + E);
  
  float phi = acos(dot(L, normalize(LightDirection.xyz)));
  float intensity = 0.0;
  float dropoff_e = 5.0f * cutoff_theta;
  intensity = pow(cos(phi), dropoff_e);
  
  vec4 ambient = AmbientProduct * intensity;
  float cos_theta = max(dot(L, N), 0.0);
  vec4 diffuse = cos_theta * DiffuseProduct * intensity;
  float cos_phi = pow(max(dot(N, H), 0.0), Shininess);
  vec4 specular = cos_phi * SpecularProduct * intensity;
  if(dot(L, N) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
  
  flatColor = ambient + diffuse + specular;
  flatColor.a = 1.0;
  
  gl_Position = projection_matrix * model_matrix * vPosition;
} 
