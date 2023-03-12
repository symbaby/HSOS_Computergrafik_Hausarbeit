#version 400
layout (location = 0) in vec3 pos;
layout (location = 1) in vec2 texCoords;

out vec2 TexCoords;
out vec3 Position;

// Weitergeben an Fragment Shader
void main(){
    TexCoords = texCoords;
    Position = pos;
    gl_Position = vec4(pos, 1.0);
}