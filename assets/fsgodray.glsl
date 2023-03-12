#version 400
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D image;
uniform vec2 LPos;

const float Decay = 0.986f;
const float Exposure = 0.15f;
// const float Density = 0.88f;
const float Density = 0.88f;
const float Weight = 4.0f;
const float STEPS = 400;



 //   Quelle:
 //   https://developer.nvidia.com/gpugems/gpugems3/part-ii-light-and-shadows/chapter-13-volumetric-light-scattering-post-process
 //   https://fabiensanglard.net/lightScattering/

void main()
{
    //Berechnen des Vector zwischen dem Pixel und der Lichtquelle
    vec2 dTexCoord = (TexCoords - LPos.xy);
    vec2 texCoord = TexCoords;

    //durch anzahl steps
    dTexCoord *= 1.0f /STEPS * Density;
    vec3 color = texture(image, TexCoords).rgb;
    float illuminationDecay = 1.0f;


    for(int i = 0; i < STEPS; i++){
        //Richtung licht 1 step gehen
        texCoord -= dTexCoord;

        //Farbe an die momentanen Pos;
        vec3 sampleColor = texture(image, texCoord).rgb;

        sampleColor *= illuminationDecay * Weight;
        color += sampleColor;
        illuminationDecay *= Decay;
    }

    FragColor = vec4(color * Exposure / STEPS, 1.0);
}