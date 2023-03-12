#version 400
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D MainFrame;
uniform sampler2D BloomFrame;
uniform sampler2D FogFrame;
uniform sampler2D GodrayFrame;



void main(){
    vec3 mainColor = texture(MainFrame, TexCoords).rgb;
    vec3 bloomColor = texture(BloomFrame, TexCoords).rgb;
    vec3 fogColor = texture(FogFrame, TexCoords).rgb;
    vec3 godrayColor = texture(GodrayFrame, TexCoords).rgb;

    mainColor += bloomColor + godrayColor + fogColor;


    FragColor = vec4(mainColor , 1.0);
}