#include "../include/Vector.hpp"

namespace physics {

    real Vector::norma() const {
        return (real) sqrt(magnitude());
    }

    real Vector::magnitude() const {
        return x*x + y*y + z*z;
    }

    Vector Vector::normalize() {
        return (*this) / norma();
    }

    real Vector::dot(const physics::Vector &other) {
        return x * other.x + y * other.y + z * other.z;
    }

    void Vector::operator +=(const Vector &other) {
        x += other.x;
        y += other.y;
        z += other.z;
    }

    void Vector::operator *=(const Vector &other) {
        x *= other.x;
        y *= other.y;
        z *= other.z;
    }

    void Vector::operator -=(const Vector &other) {
        x -= other.x;
        y -= other.y;
        z -= other.z;
    }

    void Vector::operator /=(const Vector &other) {
        if (other.x == 0 || other.y == 0 || other.z == 0) {
            throw std::runtime_error("Division by zero!");
        }
        x /= other.x;
        y /= other.y;
        z /= other.z;
    }

    Vector Vector::operator +(const Vector &other) const {
        Vector result(x, y, z);
        result += other;
        return result;
    }

    Vector Vector::operator *(const Vector &other) const {
        Vector result(x, y, z);
        result *= other;
        return result;
    }

    Vector Vector::operator -(const Vector &other) const {
        Vector result(x, y, z);
        result -= other;
        return result;
    }

    Vector Vector::operator /(const Vector &other) const {
        Vector result(x, y, z);
        result /= other;
        return result;
    }

    void Vector::operator +=(const real scalar) {
        x += scalar;
        y += scalar;
        z += scalar;
    }

    void Vector::operator *=(const real scalar) {
        x *= scalar;
        y *= scalar;
        z *= scalar;
    }

    void Vector::operator -=(const real scalar) {
        x -= scalar;
        y -= scalar;
        z -= scalar;
    }

    void Vector::operator /=(const real scalar) {
        if (scalar == 0) {
            throw std::runtime_error("Division by zero!");
        }
        x /= scalar;
        y /= scalar;
        z /= scalar;
    }

    Vector Vector::operator +(const real scalar) const {
        Vector result(x, y, z);
        result += scalar;
        return result;
    }

    Vector Vector::operator *(const real scalar) const {
        Vector result(x, y, z);
        result *= scalar;
        return result;
    }

    Vector Vector::operator -(const real scalar) const {
        Vector result(x, y, z);
        result -= scalar;
        return result;
    }

    Vector Vector::operator /(const real scalar) const {
        Vector result(x, y, z);
        result /= scalar;
        return result;
    }

    bool Vector::operator ==(const Vector &other) const {
        return x == other.x && y == other.y && z == other.z;
    }

    bool Vector::operator !=(const Vector &other) const {
        return !(*this == other);
    }

}
