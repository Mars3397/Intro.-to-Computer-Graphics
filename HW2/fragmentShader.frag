#version 330 core
// TO DO:
// Implement fragment shader

in vec2 texCoord;

uniform sampler2D ourTexture;
uniform int effect;
uniform int dark;

out vec4 FragColor;

void main()
{
	FragColor = texture(ourTexture, texCoord);
	if (dark == 1) {
		if (FragColor.r < 0.5 && FragColor.g < 0.5 && FragColor.b < 0.5) {
			FragColor.rgb = vec3(0.2, 0.1, 0.1);
		}
	} 
	if (effect == 1) {
		FragColor.r = 0.75; 
	} 
	else if (effect == 2) {
		FragColor.g = 0.75;
	}
	else if (effect == 3) {
		FragColor.b = 0.75;
	}
}