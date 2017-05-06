#version 150
#extension GL_NV_shadow_samplers_cube : enable

out vec4 fcolor;
in vec2 texCoord;

in vec3 fN;
in vec3 fL;
in vec3 fE;
in vec3 R;
in vec3 T;

uniform sampler2D texture;
uniform samplerCube texMap;

uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct;
uniform mat4 projection_matrix;
uniform mat4 model_matrix;
uniform vec4 LightPosition;
uniform float Shininess;
uniform float shadow;
uniform float drawTex;
uniform float drawEnv;
uniform float mode;


void main() 
{ 
	    vec3 N = normalize(fN);
		vec3 E = normalize(fE);
		vec3 L = normalize(fL);
		vec3 H = normalize(L + E);
		
		vec4 ambient = AmbientProduct;
		float cos_theta = max(dot(L, N), 0.0);
		vec4 diffuse = cos_theta * DiffuseProduct;
        float cos_phi = pow(max(dot(N, H), 0.0), Shininess);
        vec4 specular = cos_phi * SpecularProduct;
        if(dot(L, N) < 0.0) specular = vec4(0.0, 0.0, 0.0, 1.0);
  
        vec4 color = ambient + diffuse + specular;
        color.a = 1.0;
        
		if(mode == 0.0) {
		   fcolor = ambient;
		   fcolor.a = 1.0;
		} else if(mode == 1.0) {
		      if(drawTex == 1.0) {
			     fcolor = texture2D(texture, texCoord);
			  } else {
			     fcolor = vec4(0.8, 0.8, 0.8, 1.0);
			  }
		   } else if (mode == 2.0) {
		      if(drawEnv == 1.0) {
		         vec4 reflectColor = textureCube(texMap, R);
		         fcolor = reflectColor;
		      } else if (drawEnv == 2.0) {
		         vec4 refractColor = textureCube(texMap, T);
			     fcolor = mix(refractColor, vec4(1.0, 1.0, 1.0, 1.0), 0.3);
		      } else {
			     fcolor = color;
			  }
		   }
		
} 

