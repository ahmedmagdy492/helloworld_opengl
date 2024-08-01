#version 330 core

in vec3 outColor;
in vec2 texCoord;
in vec2 texCoord2;

out vec4 FragColor;

uniform sampler2D ourTexture;
uniform sampler2D secTexture;

void main() {
	//FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
	//FragColor = vec4(outColor, 1.0);
	//FragColor = texture(ourTexture, texCoord) * vec4(outColor, 1.0);

	FragColor = mix(texture(ourTexture, texCoord), texture(secTexture, texCoord2), 0.2);
}