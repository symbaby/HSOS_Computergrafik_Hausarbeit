#version 400
uniform vec3 EyePos;
uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform sampler2D MixTex;// for exercise 3
uniform sampler2D DetailTex[2];// for exercise 3
uniform int k = 100;
uniform vec3 Scaling;

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
out vec4 FragColor;

vec3 auge;

float sat(in float a)
{
    return clamp(a, 0.0, 1.0);
}


void main(){
    vec3 N      = normalize(Normal);
    vec3 L      = normalize(LightPos);// light is treated as directional source
    vec3 D      = EyePos-Position;
    float Dist  = length(D);
    vec3 E      = D/Dist;
    vec3 R      = reflect(-L, N);

    vec3 DiffuseComponent = LightColor * DiffuseColor * sat(dot(N, L));
    vec3 SpecularComponent = LightColor * SpecularColor * pow(sat(dot(R, E)), SpecularExp);

    // Exercise 3
    // TODO: Add texture blending code here..

    // Vektor erstellen und in Detail Tex Array zuweisen
    vec4 grass = texture(DetailTex[0], Texcoord*k);
    vec4 stone = texture(DetailTex[1], Texcoord*k);
    vec4 mixed = texture(MixTex, Texcoord );
    // float mixedScaled = mixed.x * sat(Scaling.y);
    vec4 reflect = vec4(((DiffuseComponent + AmbientColor)+ SpecularComponent), 1); // gegeben
    vec4 tex_final = mix(grass, stone, mixed); // interpolieren

    // Nebel Aufgabe
    float dmin = 0;
    float dmax = 30;
    float a = 3;

    // Translation in der Applikation Offseten
    auge = EyePos;
    auge.y = EyePos.y + 1;
    auge.z = EyePos.z + 40;
    float d = length(vec3(Position.x, Position.y, Position.z) - auge);
    float n = pow(((d - dmin) / (dmax - dmin)), a);
    //float s = 0; // Kein Nebel
    float s = sat(n);  // mit Nebel
    vec3 colorFog = vec3(0.15, 0.15, 0.35);

    // Mit fog
   //  FragColor = (1-s)*(tex_final * reflect) + vec4(s*colorFog, 1);
    FragColor = (tex_final * reflect);

    float luminance = (FragColor.r + FragColor.g + FragColor.b) / 3;
    FragColor.r = FragColor.r;
    FragColor.g = luminance;
    FragColor.b = luminance;




}