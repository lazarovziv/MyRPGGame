#ifndef MYRPGGAME_COLLISION_HPP
#define MYRPGGAME_COLLISION_HPP

#include "RigidBody.hpp"

namespace physics {

    bool resolve_collisions(RigidBody *first, RigidBody *second, real dt);
    physics::Vector &closest_vertex_to(Vector origin, std::vector<Vector> &vertices);

    bool are_colliding(Polygon &first, Polygon &second, Vector &project_normal, real *penetration_distance);
    bool are_colliding(Circle &circle, Polygon &polygon, Vector &projection_normal, real *penetration_distance);
    bool are_colliding(Circle &circle, Line &line, Vector &projection_normal, real *penetration_distance);

    void clamp_vertices(Polygon &polygon, Vector &axis, real *min, real *max);
    void clamp_vertex(Vector &vertex, Vector &axis, real *min, real *max);
    void clamp_circle(Circle &circle, Vector &axis, real *min, real *max);

    void polygon_center_position(Polygon &polygon, Vector &position);
}

#endif //MYRPGGAME_COLLISION_HPP
