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
        std::unique_ptr<Vector> force_accumulator;
        real restitution;
        bool infinite_mass = true;
        real mass = 1;
        // 1/mass
        real inverse_mass;
        real damping;

        RigidBodyType body_type;

    public:
        RigidBody(RigidBodyType type, real x, real y, real z = 0, real mass = 1);
        ~RigidBody() = default;

        Vector &get_position();
        Vector get_velocity();
        Vector get_acceleration();
        // TODO: add [[nodiscard]] for all return const functions to avoid calling the function w/o using its' return value
        real get_restitution() const;
        real get_mass() const;
        real get_inverse_mass() const;
        RigidBodyType get_body_type() const;

        bool has_finite_mass() const;

        void set_position(real x, real y, real z = 0);
        void set_position(const Vector &other);
        void set_restitution(real e);
        void set_mass(real mass); // set inverseMass accordingly
        void scale_velocity(real amount);
        void increment_velocity(const Vector &v);
        void increment_acceleration(real amount);
        void increment_acceleration(Vector amount);
        void reset_force_accumulator();
        void add_force(const Vector &force);

        void reset_velocity();
        void reset_acceleration();

        void operator +=(const Vector &other); // refers to the position vector!!

        real distance(const RigidBody &other) const;
        bool is_colliding_with(RigidBody &other, real dt);

        void update(real dt);
    };

    class Circle : public RigidBody {
    private:
        real radius;

    public:
        explicit Circle(real x, real y, real z, real r = 1);

        real get_radius() const;
    };

    // the position field for this class is it's center
    class Polygon : public RigidBody {
    private:
        std::unique_ptr<std::vector<Vector>> vertices;

    public:
        explicit Polygon(real x, real y, real z,
        const std::vector<Vector> &new_vertices, real mass);

        std::vector<Vector>* get_vertices();
        size_t get_num_vertices() const;

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

        Vector get_first() const;
        Vector get_second() const;
        real get_slope() const;
        real get_bias() const;

        static Line RIGHT_END_SCREEN;
        static Line LEFT_END_SCREEN;
        static Line TOP_END_SCREEN;
        static Line BOTTOM_END_SCREEN;
    };
}

#endif //MYRPGGAME_RIGIDBODY_HPP
