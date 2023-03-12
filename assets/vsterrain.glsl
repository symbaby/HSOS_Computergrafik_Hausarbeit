#version 400

layout(location=0) in vec4 VertexPos;
layout(location=1) in vec4 VertexNormal;
layout(location=2) in vec2 VertexTexcoord;

out vec3 Position;
out vec3 Normal;
out vec2 Texcoord;


uniform mat4 ModelMat;
uniform mat4 ModelViewProjMat;
uniform vec3 Scaling;



void main()
{
    // Exercise 1
    // TODO: apply scaling on terrain model..

    /* Position = (ModelMat * VertexPos).xyz;
     Normal = (ModelMat * vec4(VertexNormal.xyz,0)).xyz ;
     Texcoord = VertexTexcoord;
     gl_Position = ModelViewProjMat * vec4(VertexPos.x*Scaling.x, VertexPos.y * Scaling.y, VertexPos.z * Scaling.z, 1);*/

    // Variante 1
    vec4 neueVertexPos = VertexPos;
    neueVertexPos.x *= Scaling.x;
    neueVertexPos.y *= Scaling.y;
    neueVertexPos.z *= Scaling.z;

    vec4 neueVertexNormal = VertexNormal;

    neueVertexNormal.x /= Scaling.x;
    neueVertexNormal.y /= Scaling.y;
    neueVertexNormal.z /= Scaling.z;
    neueVertexNormal = normalize(neueVertexNormal);


    // Die magische Formel aus der VL
    mat4 normalMat =  inverse(transpose(ModelMat));
    Position = (normalMat * neueVertexPos).xyz;
    Normal = (normalMat * vec4(neueVertexNormal.xyz, 0)).xyz;

    // Die Textcords raussaugen
    Texcoord = VertexTexcoord;

    gl_Position = ModelViewProjMat * neueVertexPos;

    // Variante 2
/*
    mat4 scaleMatrix;
    for(int i = 0; i < 4; i++){
        for(int j = 0; j < 4; j++){
            scaleMatrix[i][j] = 0;
        }
    }
    //auf die hauptdiagonalen die skalierungsfaktoren
    scaleMatrix[0][0] = Scaling.x;
    scaleMatrix[1][1] = Scaling.y;
    scaleMatrix[2][2] = Scaling.z;
    scaleMatrix[3][3] = 1;

    mat4 scaledModelMat = scaleMatrix * ModelMat;
    Position = (scaledModelMat * VertexPos).xyz;
    Texcoord = VertexTexcoord;
    mat4 normalMat =  inverse(transpose(scaledModelMat));
    Normal = (normalMat * vec4(VertexNormal.xyz, 0)).xyz;
    //von links ranmultiplizieren geht hier wohl nicht
    //die vertexpos zu skalieren ist das gleiche
    mat4 scaledModelViewProjMat =  ModelViewProjMat * scaleMatrix;
    //vec4 scaledVertexPos = scaleMatrix * VertexPos;
    //gl_Position = ModelViewProjMat * scaledVertexPos;
    gl_Position = scaledModelViewProjMat * VertexPos;
*/

}

