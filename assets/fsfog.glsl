#version 400

in vec3 Position; // kp ob da "pos" reinkommt, ausprobierne spaeter
in vec2 TexCoords;
out vec4 FragColor;

uniform vec3 EyePos;
uniform sampler2D image;

// Vorwaertsdeklaration um Fehler zu Vermeiden
float sat(in float a);
float calcFog(float dmin, float dmax, float a);
vec3 colorFog = vec3(0.25, 0.25, 0.6);

void main() {


    vec3 result = texture(image, TexCoords).rgb;
    float s = calcFog(0, 30, 1);

    // FragColor = (1 - s) * vec4(result,1) + vec4(s * colorFog, 1);

   FragColor = vec4(result,1);

}


float sat(in float a) {
    return clamp(a, 0.0, 1.0);
}

float calcFog(float dmin, float dmax, float a){
    float d = length(vec3(Position.x, Position.y, Position.z) - EyePos); // Hier EyePos reinbekommen. Irgendwie mit Shader kommunizieren
    float n = pow(((d - dmin) / (dmax - dmin)), a);
    float s = sat(n);

    return s;
}
