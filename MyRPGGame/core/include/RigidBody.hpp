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

        Vector &getPosition();
        Vector getVelocity();
        Vector getAcceleration();
        // TODO: add [[nodiscard]] for all return const functions to avoid calling the function w/o using its' return value
        real getRestitution() const;
        real getMass() const;
        real getInverseMass() const;
        RigidBodyType getBodyType() const;

        bool hasFiniteMass() const;

        void setPosition(real x, real y, real z = 0);
        void setPosition(const Vector &other);
        void setRestitution(real e);
        void setMass(real mass); // set inverseMass accordingly
        void scaleVelocity(real amount);
        void incrementVelocity(const Vector &v);
        void incrementAcceleration(real amount);
        void incrementAcceleration(Vector amount);
        void resetForceAccumulator();
        void addForce(const Vector &force);

        void resetVelocity();
        void resetAcceleration();

        void operator +=(const Vector &other); // refers to the position vector!!

        real distance(const RigidBody &other) const;
        bool isCollidingWith(RigidBody &other, real dt);

        void update(real dt);
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
        explicit Polygon(real x, real y, real z,
        const std::vector<Vector> &newVertices, real mass);

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
        explicit Line(real x1, real y1, real x2, real y2, real mass = Constants::REAL_MAX);
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
