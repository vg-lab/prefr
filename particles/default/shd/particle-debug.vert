#version 330
#extension GL_ARB_separate_shader_objects: enable

uniform mat4 modelViewProjM;

in vec3 inVertex;

out vec4 color;
out vec2 uvCoord;

void main()
{
	gl_Position = modelViewProjM * vec4(inVertex, 1.0);

	color = vec4(0.0, 0.0, 0.0, 1.0);

	uvCoord = vec2(0.5, 0.5);
}