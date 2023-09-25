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

        static Vector ZERO;

        Vector(real x, real y, real z = 0) : x(x), y(y), z(z) {};
        ~Vector() = default;

        real norma() const;
        real magnitude() const;
        Vector normalize();
        real dot(const Vector &other);

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
    };
}

#endif //MYRPGGAME_VECTOR_HPP
