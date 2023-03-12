#ifndef __SimpleRayTracer__rgbimage__
#define __SimpleRayTracer__rgbimage__

#include <iostream>

class Color;

class RGBImage {
public:
    RGBImage(unsigned int Width, unsigned Height);

    ~RGBImage();

    void setPixelColor(unsigned int x, unsigned int y, const Color &c);

    const Color &getPixelColor(unsigned int x, unsigned int y) const;

    const Color &getPixelColorClamped(int x, int y) const;

    bool saveToDisk(const char *Filename);

    unsigned int width() const;

    unsigned int height() const;

    static unsigned char convertColorChannel(float f);

    int clamp(int a, int min, int max) const;

    static RGBImage &GaussFilter(RGBImage &dst, const RGBImage &src, float factor = 1.0f);

    static RGBImage &SobelFilter(RGBImage &dst, const RGBImage &src, float factor = 1.0f);


protected:
    Color *m_Image;
    unsigned int m_Height;
    unsigned int m_Width;


};

#endif /* defined(__SimpleRayTracer__rgbimage__) */
