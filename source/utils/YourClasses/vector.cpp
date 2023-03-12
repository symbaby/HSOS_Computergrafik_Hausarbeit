#include <assert.h>
#include <math.h>

#define EPSILON 1e-6

#include "utils/YourClasses/vector.h"

Vector::Vector(float x, float y, float z) {
    this->X = x;
    this->Y = y;
    this->Z = z;
}

Vector::Vector() {}

float Vector::dot(const Vector &v) const {

    return X * v.X + Y * v.Y + Z * v.Z;
}

Vector Vector::cross(const Vector &v) const {
    return Vector(this->Y * v.Z - this->Z * v.Y, this->Z * v.X - this->X * v.Z, this->X * v.Y - this->Y * v.X);
}


Vector Vector::operator+(const Vector &v) const {
    return Vector(this->X + v.X, this->Y + v.Y, this->Z + v.Z);
}

Vector Vector::operator-(const Vector &v) const {
    return Vector(this->X - v.X, this->Y - v.Y, this->Z - v.Z);
}

Vector Vector::operator*(float c) const {
    return Vector(this->X * c, this->Y * c, this->Z * c);
}

Vector Vector::operator-() const {
    return Vector(this->X * -1, this->Y * -1, this->Z * -1);
}

Vector &Vector::operator+=(const Vector &v) {
    this->X += v.X;
    this->Y += v.Y;
    this->Z += v.Z;
    return *this; // dummy (remove)
}

Vector &Vector::normalize() {

    if (lengthSquared() != 0) { //length is unn�tiger sqrt call
        float len = length();
        this->X /= len;
        this->Y /= len;
        this->Z /= len;
    }

    return *this;
}

float Vector::length() const {
    if (lengthSquared() == 0.0f)
        return 0.0f;
    else
        return sqrtf(lengthSquared());
}

float Vector::lengthSquared() const {
    return X * X + Y * Y + Z * Z;
}

Vector Vector::reflection(const Vector &normal) const {
    Vector v = *this + (normal * (this->dot(normal)) * -2);

    return v;
}


bool Vector::triangleIntersection(const Vector &d, const Vector &a, const Vector &b, const Vector &c, float &s) const {
    Vector n = (b - a).cross(c - a).normalize();
    float direction = n.dot(a);

    // s ausrechnen aus VL
    s = (direction - (n.dot(*this))) / (n.dot(d));

    if (s < 0) {
        s = -1 * s;
    }

    // Strahlgleichung aus VL
    Vector p = *this + (d * s);

    // Grosses Dreieck Flaeche
    float gF = (((b - a).cross(c - a)).length()) / 2;

    // Die "Teil-Dreiecke vom grossen Dreieck"
    float test1 = (((b - a).cross(p - a)).length()) / 2;
    float test2 = (((c - b).cross(p - b)).length()) / 2;
    float test3 = (((a - c).cross(p - c).length())) / 2;


    // Wenn die Flaeche + korrekturfaktor >= die Flaeche addiert mit dreiecken, dann Kollsion
    if (gF + EPSILON >= (test1 + test2 + test3)) {
        return true;
    }

    return false;
}

Vector Vector::normal(Vector A, Vector B, Vector C) {
    Vector B_(B - A);
    Vector C_(C - A);

    return B_.cross(C_);
}

std::ostream &operator<<(std::ostream &os, const Vector &vector) {
    os << "X: " << vector.X << " Y: " << vector.Y << " Z: " << vector.Z;
    return os;
}
