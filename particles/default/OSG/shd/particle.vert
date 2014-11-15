#version 330
#extension GL_ARB_separate_shader_objects: enable

uniform mat4 osg_ModelViewProjectionMatrix;

uniform vec3 cameraUp;
uniform vec3 cameraRight;

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec4 particlePosition;
layout(location = 2) in vec4 particleColor;


out vec4 color;
out vec2 uvCoord;

void main()
{
	//gl_Position = vec4((vec4(vertexPosition * particlePosition.a, 1.0) + (modelViewProjM * vec4(particlePosition.rgb, 1.0))).rgb, 1.0);

	gl_Position = osg_ModelViewProjectionMatrix 
				* vec4(
				(vertexPosition.x * particlePosition.a * cameraRight)
				+ (vertexPosition.y * particlePosition.a * cameraUp)
				+ particlePosition.rgb, 1.0);

	color = particleColor;

	uvCoord = vertexPosition.rg + vec2(0.5, 0.5);
}