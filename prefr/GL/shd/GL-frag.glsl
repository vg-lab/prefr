#version 330

in vec4 color;
in vec2 uvCoord;

out vec4 outputColor;

void main()
{	
	vec2 p = -1.0 + 2.0 * uvCoord;
	float l = sqrt(dot(p,p));// + color.a;
	
	l = 1.0 - clamp(l, 0.0, 1.0);

	l *= color.a;

	//float margin = 0.8;

	//float alpha = float(l <= margin) + (float(l > margin) * (1.0 -((l - margin) / (1.0 - margin)))); 

	//l = 1.0 - l;
	
	outputColor = vec4(color.rgb, l);
}