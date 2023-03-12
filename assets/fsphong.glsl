#version 400

const int MAX_LIGHTS = 14;
struct Light {
    int Type;
    vec3 Color;
    vec3 Position;
    vec3 Direction;
    vec3 Attenuation;
    vec3 SpotRadius;
    int ShadowIndex;
};

uniform Lights {
    int LightCount;
    Light lights[MAX_LIGHTS];
};

in vec3 Position;
in vec3 Normal;
in vec2 Texcoord;
in vec3 Tangent;
in vec3 Bitangent;
in vec4 depth;

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;
layout (location = 2) out vec4 FogColor;
layout (location = 3) out vec4 DepthColor;


uniform vec3 LightPos;
uniform vec3 LightColor;
uniform vec3 EyePos;
uniform vec3 DiffuseColor;
uniform vec3 SpecularColor;
uniform vec3 AmbientColor;
uniform float SpecularExp;

uniform sampler2D DiffuseTexture;
uniform sampler2D NormalTexture;
uniform sampler2D ShadowMapTexture[MAX_LIGHTS];
uniform mat4 ShadowMapMat[MAX_LIGHTS];

uniform sampler2D Tex0;
uniform vec3 Weights;



// Vorwaertsdeklaration um Fehler zu Vermeiden
vec3 N, H;
float sat(in float a);
vec3 PointLight(in Light light);
vec3 SpotLight(in Light light);
float specularBlinn(in vec3 L);
float calcFog(float dmin, float dmax, float a);
float LinearizeDepth(float depth);
vec3 colorFog = vec3(0.95, 0.95, 1);


float near = 0.1;
float far  = 10000.0;


void main() {
    // Aufgabe 3.3
    mat3 NormalMat = mat3(Tangent, -Bitangent, Normal);
    vec4 NormTex = texture(NormalTexture, Texcoord);
    N.x = NormTex.r * 2 - 1;
    N.y = NormTex.g * 2 - 1;
    N.z = NormTex.b * 2 - 1;
    N = NormalMat * N;// Weltraum
    N = normalize(N);

    // gegeben
    vec4 DiffTex = texture(DiffuseTexture, Texcoord);

    if (DiffTex.a < 0.3f){
        discard;
    }
    vec3 N = normalize(Normal);
    vec3 L = normalize(LightPos - Position);
    vec3 E = normalize(EyePos - Position);
    vec3 R = reflect(-L, N);
    vec3 DiffuseComponent;
    vec3 SpecularComponent;
    vec3 LightColor;


    for (int i = 0; i < LightCount; i++) {
        Light l = lights[i];

        // Bonus Aufgabe Schatten
        vec4 PosSM = ShadowMapMat[i] * vec4(Position.xyz, 1);
        PosSM.xyz /= PosSM.w;// Perspektivische Teilung
        PosSM.xy =PosSM.xy * 0.5 + 0.5;// Kooridnaten von norm. Bildraum [-1,1] in TextCord [0,1]
        vec4 DepthSM = texture(ShadowMapTexture[i], PosSM.xy);

        if (PosSM.z > DepthSM.x) { // Vergleiche ob DepthSM < PosSM.z ist, wenn ja, Fragment im Schatten
            continue;
        }

        switch (l.Type) {
            case 0: { // Point Light
                L = normalize(l.Position - Position);
                LightColor = l.Color;
                LightColor = PointLight(l);
                DiffuseComponent += LightColor * DiffuseColor * sat(dot(N, L));
                SpecularComponent += LightColor * SpecularColor * specularBlinn(L);
                break;
            }

            case 1: { // Directional Light
                L = -normalize(l.Direction);// - aus Formel
                LightColor = l.Color;
                DiffuseComponent += LightColor * DiffuseColor * sat(dot(N, L));
                SpecularComponent += LightColor * SpecularColor * specularBlinn(L);
                break;
            }

            case 2: { // Spot Light
                L = normalize(l.Position + Position);
                LightColor = SpotLight(l);
                DiffuseComponent += LightColor * DiffuseColor * sat(dot(N, L));
                SpecularComponent += LightColor * SpecularColor * specularBlinn(L);
                break;
            }
        }
    }

    bool hasFog = true;
    float s = 0; // s = Fog Koeffizient


    // ORIGINAL IMAGE
    FragColor = vec4((DiffuseComponent + AmbientColor) * DiffTex.rgb +SpecularComponent, DiffTex.a);

    // Schwarz Weiss
    float luminance = (FragColor.r + FragColor.g + FragColor.b) / 3;
    // FragColor.r = luminance;
    FragColor.r = FragColor.r;
    FragColor.g = luminance;
    FragColor.b = luminance;


    if (hasFog){
        s = calcFog(0, 30, 1);
    }

    // FOG //
    FogColor =  (1 - s) * FragColor + vec4(s * colorFog, 1);
    // FogColor = 1 - FragColor;
    // FragColor = vec4(N, 1);

    // Parameter von : https://learnopengl.com/Advanced-Lighting/Bloom
    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
    if(brightness > 1.0){
        BrightColor = vec4(FragColor.rgb, 1.0);
    } else {
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
    }


    // GODRAY //
    vec4 depthTmp = depth;
    depthTmp.z =  depth.z * 2;
    depthTmp = depthTmp * 10 / (depthTmp.w * depthTmp.w);
    float depth2 = LinearizeDepth(gl_FragCoord.z) / far;
    if (depth2 > 1)
    depth2 = 1;
    DepthColor = vec4(vec3(depth2), 1.0);
}

vec3 PointLight(Light light) {
    float r = length(light.Position - Position);
    return light.Color / (light.Attenuation.x + light.Attenuation.y * r +light.Attenuation.z * r * r);
}

vec3 SpotLight(Light light) {
    vec3 L = -normalize(light.Position - Position);// Spotlight umdrehen
    float ohm = acos(dot(L, normalize(light.Direction)));
    float innerPhi = light.SpotRadius.x;
    float outerPhi = light.SpotRadius.y;

    return (PointLight(light)) *(1 - sat((ohm - innerPhi) / (outerPhi - innerPhi)));// Skalarprodukt aus Formel.
}

float specularBlinn(vec3 L) {
    vec3 E = normalize(EyePos - Position);
    vec3 H = normalize(E + L);
    float angle = max(0.0, dot(N, H));
    return pow(angle, SpecularExp);
}

float sat(in float a) {
    return clamp(a, 0.0, 1.0);
}

float calcFog(float dmin, float dmax, float a){
    float d = length(vec3(Position.x, Position.y, Position.z) - EyePos);
    float n = pow(((d - dmin) / (dmax - dmin)), a);
    // float s = 0.0;
    float s = sat(n);// mit Nebel

    return s;
}

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // back to NDC
    return (2.0 * near * far) / (far + near - z * (far - near));
}
