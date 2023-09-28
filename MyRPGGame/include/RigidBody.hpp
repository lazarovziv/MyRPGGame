#ifndef MYRPGGAME_RIGIDBODY_HPP
#define MYRPGGAME_RIGIDBODY_HPP

#include "Vector.hpp"

namespace physics {
    enum class RigidBodyType {
        CIRCLE, BOX
    };

    class RigidBody {
    protected:
        Vector position = Vector::ZERO;
        Vector velocity = Vector::ZERO;
        Vector acceleration = Vector::ZERO;
        // used for D'Alembert rule, accumulating all forces that act on the rigid body
        Vector forceAccumulator = Vector::ZERO;
        real restitution;
        // 1/mass
        real inverseMass;
        real damping;

        RigidBodyType bodyType;

    public:
        RigidBody(RigidBodyType type, real x, real y, real z = 0);
        ~RigidBody() = default;

        Vector getPosition() const;
        Vector getVelocity() const;
        Vector getAcceleration() const;
        real getRestitution() const;
        real getInverseMass() const;
        real getMass() const;
        RigidBodyType getBodyType() const;

        bool hasFiniteMass() const;

        void setPosition(const real x, const real y, const real z = 0);
        void setPosition(const Vector &other);
        void setRestitution(const real e);
        void setMass(const real mass); // set inverseMass accordingly
        void incrementVelocity(const Vector &v);
        void incrementAcceleration(const real amount);
        void resetForceAccumulator();
        void addForce(const Vector &force);

        void operator +=(const Vector &other); // refers to the position vector!!

        real distance(const RigidBody &other) const;

        void update(const real dt);
    };

    class Circle : public RigidBody {
    private:
        real radius;

    public:
        Circle(real x, real y, real z, real r = 1);

        real getRadius() const;
    };

    // the position field for this class is not the bottom left corner of the box, but it's center!
    class Box : public RigidBody {
    private:
        real width;
        real height;
        real depth;

    public:
        Box(real x, real y, real z, real width, real height, real depth = 1);

        real getWidth() const;
        real getHeight() const;
        real getDepth() const;
        Vector getTopRightCornerPosition();
        Vector getBottomLeftCornerPosition();
    };
}

#endif //MYRPGGAME_RIGIDBODY_HPP
