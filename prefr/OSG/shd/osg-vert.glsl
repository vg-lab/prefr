#version 150 compatibility

uniform mat4 osg_ModelViewProjectionMatrix;

uniform vec3 cameraUp;
uniform vec3 cameraRight;

in vec3 vertexPosition;
in vec4 particlePosition;
in vec4 particleColor;


out vec4 color;
out vec2 uvCoord;

void main()
{
	gl_Position = osg_ModelViewProjectionMatrix *
	        vec4(
				(vertexPosition.x * particlePosition.a * cameraRight)
				+ (vertexPosition.y * particlePosition.a * cameraUp)
				+ particlePosition.rgb, 1.0);
	
	//vec4(vertexPosition, 1.0f);// + particlePosition.rgb, 1.0);

//  gl_Position = osg_ModelViewProjectionMatrix *
//          vec4(
//              vertexPosition + particlePosition.rgb, 1);
//        (vertexPosition.x * particlePosition.a)// * cameraRight)
//        + (vertexPosition.y * particlePosition.a)// * cameraUp)
//        + particlePosition.rgb, 1.0);
//  

	color = particleColor;

	uvCoord = vertexPosition.rg + vec2(0.5, 0.5);
}