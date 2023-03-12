//
// Created by boery on 21.03.2022.
//

#include "utils/YourClasses/color.h"
#include <assert.h>

// Standardkonstruktor
// 0 = schwarz, 1 = maximum
Color::Color() {
    this->R = 0.0f;
    this->G = 0.0f;
    this->B = 0.0f;
}

Color::Color(float r, float g, float b) : R(r), G(g), B(b) {

}

Color Color::operator*(const Color &c) const {
    Color erg;
    erg.R = this->R * c.R;
    erg.G = this->G * c.G;
    erg.B = this->B * c.B;
    return erg;
}

Color Color::operator*(const float Factor) const {
    Color erg;
    erg.R = this->R * Factor;
    erg.G = this->G * Factor;
    erg.B = this->B * Factor;
    return erg;

}

Color Color::operator+(const Color &c) const {
    Color erg;
    erg.R = this->R + c.R;
    erg.G = this->G + c.G;
    erg.B = this->B + c.B;
    return erg;
}

Color &Color::operator+=(const Color &c) {

    this->R += c.R;
    this->G += c.G;
    this->B += c.B;


    return *this;
}


// RGB in die Konsole printen
std::ostream &operator<<(std::ostream &s, const Color &out) {
    s << "R: " << out.R << std::endl
      << "G: " << out.G << std::endl
      << "B: " << out.B << std::endl;

    return s;
}