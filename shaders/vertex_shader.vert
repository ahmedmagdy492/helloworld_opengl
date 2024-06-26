#version 330 core
layout (location = 0) in vec3 aPos;

uniform float theta;

void main()
{
	mat2 rotationMat = mat2(
		cos(theta), sin(theta),
		sin(theta)*-1, cos(theta)
	);
	vec2 point = aPos.xy;

	vec2 result = rotationMat * point;

	gl_Position = vec4(result, aPos.z, 1.0);
}