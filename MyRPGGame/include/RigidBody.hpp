#ifndef MYRPGGAME_RIGIDBODY_HPP
#define MYRPGGAME_RIGIDBODY_HPP

#include "Vector.hpp"
#include <memory>

namespace physics {
    enum class RigidBodyType {
        CIRCLE, POLYGON
    };

    class RigidBody {
    protected:
        std::unique_ptr<Vector> position;
        std::unique_ptr<Vector> velocity;
        std::unique_ptr<Vector> acceleration;
        // used for D'Alembert rule, accumulating all forces that act on the rigid body
        std::unique_ptr<Vector> forceAccumulator;
        real restitution;
        bool infiniteMass = true;
        // 1/mass
        real inverseMass;
        real damping;

        RigidBodyType bodyType;

    public:
        RigidBody(RigidBodyType type, real x, real y, real z = 0);
        ~RigidBody() = default;

        Vector getPosition();
        Vector getVelocity();
        Vector getAcceleration();
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
        explicit Circle(real x, real y, real z, real r = 1);

        real getRadius() const;
    };

    // the position field for this class is it's center
    class Polygon : public RigidBody {
    private:
        std::unique_ptr<std::vector<Vector>> vertices;

    public:
        explicit Polygon(const real x, const real y, const real z, const std::vector<Vector> &newVertices);

        std::vector<Vector>* getVertices();

        // bodies for collision when entities trying to exit map's bounds
        static Polygon RIGHT_END_SCREEN;
        static Polygon LEFT_END_SCREEN;
        static Polygon TOP_END_SCREEN;
        static Polygon BOTTOM_END_SCREEN;
    };
}

#endif //MYRPGGAME_RIGIDBODY_HPP
