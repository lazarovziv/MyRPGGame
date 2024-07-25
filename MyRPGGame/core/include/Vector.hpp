#ifndef MYRPGGAME_VECTOR_HPP
#define MYRPGGAME_VECTOR_HPP

#include "Constants.h"
#include <stdexcept>

namespace physics {

    class Vector {
    public:
        real x;
        real y;
        real z;

        static const Vector &ZERO;
        static const Vector &UP_DIRECTION;
        static const Vector &DOWN_DIRECTION;
        static const Vector &RIGHT_DIRECTION;
        static const Vector &LEFT_DIRECTION;

        Vector(real x, real y, real z = 0) : x(x), y(y), z(z) {};
        ~Vector() = default;

        real norma() const;
        real magnitude() const;
        Vector normalized() const;
        void normalize();
        real dot(const Vector &other) const;
        real distance(const Vector &other) const;

        void reset_coordinates();
        void print_coordinates() const;

        void operator +=(const Vector &other);
        void operator *=(const Vector &other);
        void operator -=(const Vector &other);
        void operator /=(const Vector &other);
        Vector operator +(const Vector &other) const;
        Vector operator *(const Vector &other) const;
        Vector operator -(const Vector &other) const;
        Vector operator /(const Vector &other) const;

        void operator +=(const real scalar);
        void operator *=(const real scalar);
        void operator -=(const real scalar);
        void operator /=(const real scalar);
        Vector operator +(const real scalar) const;
        Vector operator *(const real scalar) const;
        Vector operator -(const real scalar) const;
        Vector operator /(const real scalar) const;

        bool operator ==(const Vector &other) const;
        bool operator !=(const Vector &other) const;

        Vector &operator =(const Vector &other);
    };
}

#endif //MYRPGGAME_VECTOR_HPP
