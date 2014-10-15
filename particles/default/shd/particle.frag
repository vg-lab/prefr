#version 330

in vec4 color;
in vec2 uvCoord;

out vec4 outputColor;

void main()
{	
	//vec2 diff = uvCoord - vec2(0.5, 0.5);
	//float d = abs(sqrt(pow(diff.x,2) + pow(diff.y,2)));

	//d = clamp(d/0.5, 0, 1);

	vec2 p = -1.0 + 2.0 * uvCoord;
	float l = sqrt(dot(p,p)) + color.a;
	
	l = 1.0 - clamp(l, 0.0, 1.0);

	outputColor = vec4(color.rgb, l);
}