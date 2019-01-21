#version 330

in vec4 color;
in vec2 uvCoord;
in float id;

out vec4 outputColor;

float module(float x, float y) {
	return x - y * floor(x / y);
}

vec3 unpackColor(float f) {
	/*vec3 color;
	color.b = floor(f / (256 * 256));
	color.g = floor((f - color.b * 256 * 256) / 256);
	color.r = floor(module(f, 256.0));
	return color / 255.0;*/

	vec3 color = fract(vec3(1.0/255.0, 1.0/(255.0*255.0), 1.0/(255.0*255.0*255.0)) * f);
	color -= color.xxy * vec3(0.0, 1.0/255.0, 1.0/255.0);

	return color;
}

void main()
{	
	vec3 cc = unpackColor(id);
	outputColor = vec4(cc, 1.0);
}