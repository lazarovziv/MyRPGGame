#ifndef MYRPGGAME_COLLISION_HPP
#define MYRPGGAME_COLLISION_HPP

#include "RigidBody.hpp"

namespace physics {

    bool resolveCollisions(RigidBody *first, RigidBody *second, real dt);
    physics::Vector &closestVertexTo(Vector origin, std::vector<Vector> &vertices);

    bool areColliding(Polygon &first, Polygon &second, Vector &projectionNormal, real *penetrationDistance);
    bool areColliding(Circle &circle, Polygon &polygon, Vector &projectionNormal, real *penetrationDistance);

    void clampVertices(Polygon &polygon, Vector &axis, real *min, real *max);
    void clampCircle(Circle &circle, Vector &axis, real *min, real *max);

    void polygonCenterPosition(Polygon &polygon, Vector &position);
}

#endif //MYRPGGAME_COLLISION_HPP
