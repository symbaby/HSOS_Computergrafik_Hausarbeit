//
// Created by boery on 21.03.2022.
//

#include "utils/YourClasses/rgbimage.h"
#include "utils/YourClasses/color.h"

#include "assert.h"
#include "string.h"
#include "stdio.h"
#include <math.h>
#include "../../../include/Matrix.h"

RGBImage::RGBImage(unsigned int Width, unsigned int Height) : m_Width(Width), m_Height(Height) {
    // Ein Bild wird beschrieben von hoehe x breite
    m_Image = new Color[Width * Height];
}

RGBImage::~RGBImage() {
    delete (m_Image);
}

void RGBImage::setPixelColor(unsigned int x, unsigned int y, const Color &c) {
    if (x < 0 || x > width() || y < 0 || y > height()) {
        throw std::invalid_argument("falsche Farbwerte erhalten");
    } else {
        m_Image[y * width() + x] = c;
    }
}

int RGBImage::clamp(int a, int min, int max) const {

    if (a < max) {
        a = a;
    } else {
        a = max;
    }

    if (a < min) {
        a = a;
    } else {
        a = min;
    }


    return a;
}

const Color &RGBImage::getPixelColor(unsigned int x, unsigned y) const {
    return m_Image[y * m_Width + x];
}

const Color &RGBImage::getPixelColorClamped(int x, int y) const {
    unsigned int cx = (unsigned int) clamp(x, 0, m_Width);
    unsigned int cy = (unsigned int) clamp(y, 0, m_Height);

    return getPixelColor(cx, cy);
}

unsigned int RGBImage::width() const {

    return m_Width;
}

unsigned int RGBImage::height() const {

    return m_Height;
}

unsigned char RGBImage::convertColorChannel(float v) {
    if (v < 0.0f) {
        v = 0.0f;
    }

    if (v > 1.0f) {
        v = 1.0f;
    }

    return (unsigned char) (v * 255);
}

// Ansatz fuer die Loesung: https://stackoverflow.com/questions/2654480/writing-bmp-image-in-pure-c-c-without-other-libraries
bool RGBImage::saveToDisk(const char *Filename) {
    FILE *f;
    unsigned char *img = nullptr;

    // Wieso 54: Antwort -> 14 byte Bitmap + File Header, 40 byte Bitmap Info Header, 3 Byte RGB
    unsigned int filesize = 54 + 3 * width() * height();  //w is your image width, h is image height, both int


    // Speicher reservieren
    img = (unsigned char *) malloc(3 * width() * height());
    memset(img, 0, 3 * width() * height());

    // Durch Bild Pixel fuer Pixel durchlaufen
    for (int i = 0; i < width(); i++) {
        for (int j = 0; j < height(); j++) {

            // variablen fuer traversierung preppen
            unsigned int x = i;
            unsigned int y = (height() - 1 - j);

            // Wir bringen die RGB float Werte von 0.0f - 1.0f in die 0-255 range
            unsigned char r = convertColorChannel(this->getPixelColor(i, j).R);
            unsigned char g = convertColorChannel(this->getPixelColor(i, j).G);
            unsigned char b = convertColorChannel(this->getPixelColor(i, j).B);


            // Cap fuer Schrottwerte
            if (r > 255) r = 255;
            if (g > 255) g = 255;
            if (b > 255) b = 255;

            img[(x + y * width()) * 3 + 2] = (unsigned char) (r);
            img[(x + y * width()) * 3 + 1] = (unsigned char) (g);
            img[(x + y * width()) * 3 + 0] = (unsigned char) (b);
        }
    }

    // (BITMAPFILEHEADER) Groesse 14 Byte, Jede Zelle = 1 Byte
    unsigned char bmpfileheader[14] = {
            'B',        // bfType
            'M',        // bfType
            0,          // bfSize
            0,          // bfSize
            0,          // bfSize
            0,          // bfSize
            0,          // bfReserved
            0,          // bfReserved
            0,          // bfReserved
            0,          // bfReserved
            54,        // bfOffBits
            0,         // bfOffBits
            0,         // bfOffBits
            0          // bfOffBits
    };

    unsigned char bmpinfoheader[40] = {
            40,         // biSize
            0,          // biSize
            0,          // biSize
            0,          // biSize
            0,          // biWidth
            0,          // biWidth
            0,          // biWidth
            0,          // biWidth
            0,          // biHeight
            0,          // biHeight
            0,         // biHeight
            0,         // biHeight
            1,         // biPlanes
            0,         // biPlanes
            24,
            0
    };

    unsigned char bmppad[3] = {0, 0, 0};

    bmpfileheader[2] = (unsigned char) (filesize);          // bfsize
    bmpfileheader[3] = (unsigned char) (filesize >> 8);     // bfsize
    bmpfileheader[4] = (unsigned char) (filesize >> 16);    // bfsize
    bmpfileheader[5] = (unsigned char) (filesize >> 24);    // bfsize

    bmpinfoheader[4] = (unsigned char) (width());           // biWidth
    bmpinfoheader[5] = (unsigned char) (width() >> 8);      // biWidth
    bmpinfoheader[6] = (unsigned char) (width() >> 16);     // biWidth
    bmpinfoheader[7] = (unsigned char) (width() >> 24);     // biWidth

    /**
     * https://de.wikipedia.org/wiki/Windows_Bitmap @BITMAPINFOHEADER (Gr��e: 40 Byte)
     *
     * Ist der Wert positiv, so ist die Bitmap eine sogenannte "bottom-up"-Bitmap
     * (die Bilddaten beginnen mit der untersten und enden mit der obersten Bildzeile)
     *
     * Ist der Wert negativ, so ist die Bitmap eine �top-down�-Bitmap
     * (die Bilddaten beginnen mit der obersten und enden mit der untersten Bildzeile).
     *
     * */

    bmpinfoheader[8] = (unsigned char) (-1 * height());  // Hier muessen die heights eine negative Zahl sein!
    bmpinfoheader[9] = (unsigned char) (-1 * height() >> 8);
    bmpinfoheader[10] = (unsigned char) (-1 * height() >> 16);
    bmpinfoheader[11] = (unsigned char) (-1 * height() >> 24);


    f = fopen(Filename, "wb");
    fwrite(bmpfileheader, 1, 14, f);
    fwrite(bmpinfoheader, 1, 40, f);
    for (int i = 0; i < height(); i++) {
        fwrite(img + (width() * (height() - i - 1) * 3), 3, width(), f);
        fwrite(bmppad, 1, (4 - (width() * 3) % 4) % 4, f);
    }

    // Speicher freigeben
    free(img);

    // geoffnete Dateien muessen geschlossen werden.
    fclose(f);

    return true;
}


RGBImage &RGBImage::SobelFilter(RGBImage &dst, const RGBImage &src, float factor) {

    // TODO: hier return-Anweisung eingeben
    assert (dst.height() == src.height());
    assert (dst.width() == src.width());


    Matrix K(1, 0, -1, 0,
             2, 0, -2, 0,
             1, 0, -1, 0,
             0, 0, 0, 0);

    Matrix KT = K.transpose();


    for (int y = 3; y < dst.height(); ++y) {
        for (int x = 3; x < dst.width(); ++x) {
            float u = 0, v = 0;
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    u = u + src.getPixelColor(x - i - 1, y - j - 1).R * K.m[i + j * 4] * factor;
                    v = v + src.getPixelColor(x - i - 1, y - j - 1).R * KT.m[j + i * 4] * factor;
                }
            }
            float color = std::sqrt(u * u + v * v);
            dst.setPixelColor(x, y, Color(color, color, color));
        }
    }
    return dst;
}

RGBImage &RGBImage::GaussFilter(RGBImage &dst, const RGBImage &src, float factor) {
    assert(dst.width() == src.width() && dst.height() == src.height());
    RGBImage Tmp(src.width(), src.height());

    float gauss[7] = {0.006f, 0.061f, 0.242f, 0.383f, 0.242f, 0.061f, 0.006f};

    for (int i = 0; i < src.height(); ++i) {
        for (int j = 0; j < src.width(); ++j) {
            Color g(0, 0, 0);
            // horizontal
            for (int k = 0; k < 7; ++k)
                g += src.getPixelColorClamped(j - k - 3, i) * gauss[k];
            Tmp.setPixelColor(j, i, g);
        }
    }
    for (int i = 0; i < src.height(); ++i) {
        for (int j = 0; j < src.width(); ++j) {
            Color g(0, 0, 0);
            // vertical
            for (int k = 0; k < 7; ++k)
                g += Tmp.getPixelColorClamped(j, i - k - 3) * gauss[k];
            dst.setPixelColor(j, i, g);
        }
    }

    return dst;
}
