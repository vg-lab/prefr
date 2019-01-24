#version 330

in vec4 color;
in vec2 uvCoord;
in float id;

out vec4 outputColor;

vec3 unpackColor( float f )
{
	vec3 color = fract(vec3(1.0/255.0, 1.0/(255.0*255.0), 
		1.0/(255.0*255.0*255.0)) * f);
	color -= color.xxy * vec3(0.0, 1.0/255.0, 1.0/255.0);

	return color;
}

void main( )
{	
	vec3 cc = unpackColor(id);
	outputColor = vec4(cc, 1.0);
}