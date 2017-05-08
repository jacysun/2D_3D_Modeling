#version 150

in vec2 texCoord;
out vec4 fColor;

uniform sampler2D texture;
uniform int textureMode;
uniform float uD, uR;

const float PI = 3.14;

void main() 
{ 
    ivec2 ires = textureSize(texture, 0);
	float ResS = float(ires.s);
	float ResT = float(ires.t);
    vec3 irgb = texture2D(texture, texCoord).rgb;
	const vec3 LUMCOEFFS = vec3(0.2125, 0.7154, 0.0721);

	vec2 stp0 = vec2(1.0/ResS, 0);
	vec2 st0p = vec2(0, 1.0/ResT);
	vec2 stpp = vec2(1.0/ResS, 1.0/ResT);
	vec2 stpm = vec2(1.0/ResS, -1.0/ResT);

	float i00 = dot(texture2D(texture, texCoord).rgb, LUMCOEFFS);
	float im1m1 = dot(texture2D(texture, texCoord-stpp).rgb, LUMCOEFFS);
	float ip1p1 = dot(texture2D(texture, texCoord+stpp).rgb, LUMCOEFFS);
	float im1p1 = dot(texture2D(texture, texCoord-stpm).rgb, LUMCOEFFS);
	float ip1m1 = dot(texture2D(texture, texCoord+stpm).rgb, LUMCOEFFS);
	float im10 = dot(texture2D(texture, texCoord-stp0).rgb, LUMCOEFFS);
	float ip10 = dot(texture2D(texture, texCoord+stp0).rgb, LUMCOEFFS);
	float i0m1 = dot(texture2D(texture, texCoord-st0p).rgb, LUMCOEFFS);
	float i0p1 = dot(texture2D(texture, texCoord+st0p).rgb, LUMCOEFFS);

	float h = -1.0*im1p1 - 2.0*i0p1 -1.0*ip1p1 + 1.0*im1m1 + 2.0*i0m1 + 1.0*ip1m1;
	float v = -1.0*im1m1 - 2.0*im10 -1.0*im1p1 + 1.0*ip1m1 + 2.0*ip10 + 1.0*ip1p1;

	switch(textureMode) {
	    case 0:
		    fColor = texture2D(texture, texCoord);
		    break;
		case 1:
		    const vec3 W = vec3(0.2125, 0.7154, 0.0721);
			float luminance = dot(irgb, W);
			fColor = vec4(luminance, luminance, luminance, 1.0);
			break;
		case 2:
		    fColor = 1.0 - texture2D(texture, texCoord);
			break;
		case 3: 
		    float mag = sqrt(h*h + v*v);
			vec3 target = vec3(mag, mag, mag);
			fColor = vec4(mix(irgb, target, 1.0), 1.0);
			break;
		case 4:
		    vec3 c00 = texture2D(texture, texCoord).rgb;
			vec3 cp1p1 = texture2D(texture, texCoord+stpp).rgb;
			
			vec3 diffs = c00 - cp1p1;
			float max = diffs.r;
			if(abs(diffs.g)>abs(max))
			    max = diffs.g;
			if(abs(diffs.b)>abs(max))
			    max = diffs.b;

			float gray = clamp(max+0.5, 0.0, 1.0);
			vec4 grayVersion = vec4(gray, gray, gray, 1.0);
			vec4 colorVersion = vec4(gray*c00, 1.0);
			fColor = mix(grayVersion, colorVersion, 0.1);
			break;
		case 5:  // Toon
		    mag = length(vec2(h, v));
			if(mag > 0.8) {
			    fColor = vec4(0.0, 0.0, 0.0, 1.0);
			} else {
			    float uQuantize = 5.0;
			    irgb *= uQuantize;
				irgb += vec3(0.5, 0.5, 0.5);
				ivec3 intrgb = ivec3(irgb);
				irgb = vec3(intrgb)/uQuantize;
				fColor = vec4(irgb, 1.0);
			}
			break;
		case 6:  // Twirl
		    vec2 st = texCoord;
			float Radius = ResS * uR;
			vec2 xy = ResS * st;

			vec2 dxy = xy - ResS/2.0;
			float r = length(dxy);
			float beta = atan(dxy.y, dxy.x) + radians(uD) * (Radius - r)/Radius;
		    vec2 xy1 = xy;
			if(r <= Radius) {
			    xy1 = ResS/2.0 + r * vec2(cos(beta), sin(beta));
			}
			
			st = xy1/ResS;
			irgb = texture2D(texture, st).rgb;
		    fColor = vec4(irgb, 1.0);
			break;
		case 7:  // Ripple
		    st = texCoord;
			xy = ResS * st;
			float temp1 = 2*PI*xy.y/120.0;
			float temp2 = 2*PI*xy.x/250.0;
			xy1 = xy + vec2(10.0*sin(temp1), 15.0*sin(temp2));
			
			st = xy1/ResS;
			irgb = texture2D(texture, st).rgb;
		    fColor = vec4(irgb, 1.0);
			break;
		case 8: // Spherical
		    float rmax = ResS/2.0;
			float p = 1.8;
			st = texCoord;
			xy = ResS * st;

			float dx = xy.x - ResS/2.0;
			float dy = xy.y - ResS/2.0;
			r = sqrt(dx*dx + dy*dy);
			float z = sqrt(rmax*rmax - r*r);
			temp1 = (1 - 1/p)*(asin(dx/sqrt(dx*dx + z*z)));
			temp2 = (1 - 1/p)*(asin(dy/sqrt(dy*dy + z*z)));

			if (r > rmax) {
			    xy1 = xy;
			} else {
			    xy1 = xy - vec2(z*tan(temp1), z*tan(temp2));
			}

			st = xy1/ResS;
			irgb = texture2D(texture, st).rgb;
		    fColor = vec4(irgb, 1.0);
			break;
	}       
}