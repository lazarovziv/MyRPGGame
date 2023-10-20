#ifndef MYRPGGAME_RIGIDBODY_HPP
#define MYRPGGAME_RIGIDBODY_HPP

#include "Vector.hpp"
#include <memory>
#include <vector>
#include <algorithm>

namespace physics {
    enum class RigidBodyType {
        CIRCLE, POLYGON, LINE
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
        real mass = 1;
        // 1/mass
        real inverseMass;
        real damping;

        RigidBodyType bodyType;

    public:
        RigidBody(RigidBodyType type, real x, real y, real z = 0, real mass = 1);
        ~RigidBody() = default;

        Vector getPosition();
        Vector getVelocity();
        Vector getAcceleration();
        real getRestitution() const;
        real getMass() const;
        real getInverseMass() const;
        RigidBodyType getBodyType() const;

        bool hasFiniteMass() const;

        void setPosition(const real x, const real y, const real z = 0);
        void setPosition(const Vector &other);
        void setRestitution(const real e);
        void setMass(const real mass); // set inverseMass accordingly
        void scaleVelocity(const real amount);
        void incrementVelocity(const Vector &v);
        void incrementAcceleration(const real amount);
        void incrementAcceleration(const Vector amount);
        void resetForceAccumulator();
        void addForce(const Vector &force);

        void resetVelocity();
        void resetAcceleration();

        void operator +=(const Vector &other); // refers to the position vector!!

        real distance(const RigidBody &other) const;
        bool isCollidingWith(RigidBody &other, const real dt);
        bool collides(RigidBody &other, real dt);

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
        explicit Polygon(const real x, const real y, const real z,
        const std::vector<Vector> &newVertices, const real mass);

        std::vector<Vector>* getVertices();
        size_t getNumVertices() const;

        // bodies for collision when entities trying to exit map's bounds
        static Polygon RIGHT_END_SCREEN;
        static Polygon LEFT_END_SCREEN;
        static Polygon TOP_END_SCREEN;
        static Polygon BOTTOM_END_SCREEN;
        static constexpr real MAP_POLYGON_WIDTH = Constants::TILE_SIZE;
        static constexpr real MAP_POLYGON_HEIGHT = Constants::TILE_SIZE;
    };

    class Line : public RigidBody {
    private:
        // points defining the linear function
        Vector p1 = Vector::ZERO;
        Vector p2 = Vector::ZERO;
        real slope;
        real bias;
    public:
        explicit Line(const real x1, const real y1, const real x2, const real y2,
        const real mass = Constants::REAL_MAX);
        // TODO: add static lines for edges of screen if things will work well

        Vector getFirst() const;
        Vector getSecond() const;
        real getSlope() const;
        real getBias() const;

        static Line RIGHT_END_SCREEN;
        static Line LEFT_END_SCREEN;
        static Line TOP_END_SCREEN;
        static Line BOTTOM_END_SCREEN;
    };
}

#endif //MYRPGGAME_RIGIDBODY_HPP
