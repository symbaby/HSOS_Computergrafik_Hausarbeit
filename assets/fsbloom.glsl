#version 400
out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D image;
uniform float Weights[5];

// Quelle : https://learnopengl.com/Advanced-Lighting/Bloom
void main()
{
    vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
    vec3 result = texture(image, TexCoords).rgb * Weights[0];

    for(int i = 1; i < 5; ++i)
    {
        result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * Weights[i]/2;
        result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * Weights[i]/2;
    }

    for(int i = 1; i < 5; ++i)
    {
        result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * Weights[i]/2;
        result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * Weights[i]/2;
    }

    FragColor = vec4(result, 1.0);
}