//
// Created by boery on 22.08.2022.
//

#include "Terrain.h"
#include "TerrainShader.h"


Terrain::Terrain(const char *HeightMap, const char *DetailMap1, const char *DetailMap2) : Size(10, 1, 10) {
    if (HeightMap && DetailMap1 && DetailMap2) {
        bool loaded = load(HeightMap, DetailMap1, DetailMap2);
        if (!loaded)
            throw std::exception();
    }
}

Terrain::~Terrain() {

}

bool Terrain::load(const char *HeightMap, const char *DetailMap1, const char *DetailMap2) {
    if (!HeightTex.load(HeightMap))
        return false;
    if (!DetailTex[0].load(DetailMap1))
        return false;
    if (!DetailTex[1].load(DetailMap2))
        return false;
    if (!MixTex.load(ASSET_DIRECTORY"mixmap.bmp")) {
        return false;
    }

    // TODO: add code for creating terrain model
    const unsigned int w = HeightTex.getRGBImage()->width();
    const unsigned int h = HeightTex.getRGBImage()->height();

    // Vertices und Normal Vector
    std::vector<Vector> Vertices;
    std::vector<Vector> Normals;

    // Platz reservieren
    Vertices.reserve(w * h);
    Normals.reserve(w * h);

    float startX = -0.5f;
    float startZ = -0.5f;
    const RGBImage *HeightImg = HeightTex.getRGBImage();

    for (unsigned int i = 0; i < h; ++i)
        for (unsigned int j = 0; j < w; ++j) {

            // Wir hatten vorher einen Vektor auf den Heap aber auf den Stack ist es schneller und sicherer!
            Vector p;
            float ni = (float) i / (float) (h - 1);
            float nj = (float) j / (float) (w - 1);
            p = Vector(startX + nj, HeightImg->getPixelColor(j, i).R, startZ + ni);
            Vertices.push_back(p);
            Normals.emplace_back(0, 0, 0);
        }

    IB.begin();
    for (int i = 0; i < h - 1; i++)
        for (int j = 0; j < w - 1; j++) {
            // first triangle
            unsigned int pos = i * w + j;
            IB.addIndex(pos + w);
            IB.addIndex(pos + 1);
            IB.addIndex(pos);
            // second triangle
            IB.addIndex((pos + 1) + w);
            IB.addIndex(pos + 1);
            IB.addIndex(pos + w);
        }
    IB.end();
    glFinish();
    GLenum Error = glGetError();
    if (Error != 0) {
        std::cout << "error" << std::endl;
    }

    for (int i = 0; i < IB.indexCount(); i += 3) {
        unsigned int i0 = IB.indices()[i];
        unsigned int i1 = IB.indices()[i + 1];
        unsigned int i2 = IB.indices()[i + 2];

        // Giga brain!
        Vector a, b, c, n;

        a = Vertices[i0];
        b = Vertices[i1];
        c = Vertices[i2];
        n = (b - a).cross(c - a);
        Normals[i0] += n;
        Normals[i1] += n;
        Normals[i2] += n;
    }

    VB.begin();
    for (unsigned int i = 0; i < h; ++i)
        for (unsigned int j = 0; j < w; ++j) {
            float ni = (float) (i) / (float) (h - 1);
            float nj = (float) (j) / (float) (w - 1);
            VB.addTexcoord0(nj, ni, 0);
            VB.addNormal(Normals[i * w + j].normalize());
            VB.addVertex(Vertices[i * w + j]);
        }
    VB.end();

    glFinish();
    Error = glGetError();
    if (Error != 0){
        std::cout << "error" << std::endl;
    }

    return false;
}


void Terrain::shader(BaseShader *shader, bool deleteOnDestruction) {
    BaseModel::shader(shader, deleteOnDestruction);
}

void Terrain::draw(const BaseCamera &Cam) {
    applyShaderParameter();
    BaseModel::draw(Cam);
    // TODO: add code for drawing index and vertex buffers

    VB.activate();
    IB.activate();
    glDrawElements(GL_TRIANGLES, IB.indexCount(), IB.indexFormat(), 0);
    IB.deactivate();
    VB.deactivate();

}

void Terrain::applyShaderParameter() {
    auto *Shader = dynamic_cast<TerrainShader *>(BaseModel::shader());
    if (!Shader)
        return;

    Shader->mixTex(&MixTex);
    for (int i = 0; i < 2; i++)
        Shader->detailTex(i, &DetailTex[i]);
    Shader->scaling(Size);

    // TODO: add additional parameters if needed..

}
