#include "../include/Vector.hpp"

namespace physics {

    const Vector &Vector::ZERO = Vector(0, 0, 0);
    const Vector &Vector::UP_DIRECTION = Vector(0, -1, 0);
    const Vector &Vector::DOWN_DIRECTION = Vector(0, 1, 0);
    const Vector &Vector::RIGHT_DIRECTION = Vector(1, 0, 0);
    const Vector &Vector::LEFT_DIRECTION = Vector(-1, 0, 0);

    real Vector::norma() const {
        if ((*this) == Vector::ZERO) return 0;
        return (real) std::sqrt(magnitude());
    }

    real Vector::magnitude() const {
        return x*x + y*y + z*z;
    }

    Vector Vector::normalized() const {
        if ((*this) == Vector::ZERO) return Vector::ZERO;
        return (*this) / norma();
    }

    void Vector::normalize() {
        real norm = this->norma();
        if (norm == 0) return; // TODO: raise error?
        *this /= norm;
    }

    real Vector::dot(const physics::Vector &other) const {
        return x * other.x + y * other.y + z * other.z;
    }

    real Vector::distance(const physics::Vector &other) const {
        return (real) std::sqrt((x - other.y)*(x - other.y) + (y - other.y)*(y - other.y) + (z - other.z)*(z - other.z));
    }

    void Vector::resetCoordinates() {
        x = 0;
        y = 0;
        z = 0;
    }

    void Vector::printCoordinates() const {
        std::cout << "(" << x << ", " << y << "," << z << ")" << std::endl;
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

    Vector& Vector::operator =(const Vector &other) {
        x = other.x;
        y = other.y;
        z = other.z;
        return *this;
    }

}
