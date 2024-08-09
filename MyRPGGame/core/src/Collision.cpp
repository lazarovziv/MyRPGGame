#include "Collision.hpp"

namespace physics {

    bool resolve_collisions(physics::RigidBody *first, physics::RigidBody *second, const real dt) {
        if (!first->has_finite_mass() && !second->has_finite_mass()) return false;
        // direction vector to project first body
        Vector axisToProject = second->get_position() - first->get_position();
        Vector axisNormalized = axisToProject.normalized();
        // checking moving in the same direction
        Vector relativeVelocity = second->get_velocity() - first->get_velocity();
        // scaler to the direction vector
        real penetrationDistance = Constants::REAL_MAX;
        // dividing into all types of bodies
        // TODO: add check for first contained in the second
        if (first->get_body_type() == RigidBodyType::CIRCLE) {
            if (second->get_body_type() == RigidBodyType::CIRCLE) {
                // distance of the overlapping area
                penetrationDistance = ((Circle*)first)->get_radius() + ((Circle*)second)->get_radius() - axisToProject.norma();
                if (penetrationDistance < 0) return false;
            } else if (second->get_body_type() == RigidBodyType::POLYGON) {
                Circle &circle = ((Circle&) *first);
                Polygon &polygon = ((Polygon&) *second);
                if (!physics::are_colliding(circle, polygon, axisNormalized, &penetrationDistance)) return false;

            } else if (second->get_body_type() == RigidBodyType::LINE) {
                Circle &firstAsCircle = ((Circle&) *first);
                Line &secondAsLine = ((Line&) *second);
                if (!physics::are_colliding(firstAsCircle, secondAsLine, axisNormalized, &penetrationDistance)) return false;
            }
        } else if (first->get_body_type() == RigidBodyType::POLYGON) {
            if (second->get_body_type() == RigidBodyType::CIRCLE) {
                Polygon &polygon = ((Polygon&) *first);
                Circle &circle = ((Circle&) *second);
                if (!physics::are_colliding(circle, polygon, axisNormalized, &penetrationDistance)) return false;
            } else if (second->get_body_type() == RigidBodyType::POLYGON) {
                Polygon &firstAsPolygon = ((Polygon&) *first);
                Polygon &secondAsPolygon = ((Polygon&) *second);
                if (!physics::are_colliding(firstAsPolygon, secondAsPolygon, axisNormalized, &penetrationDistance)) return false;
            }
        } else if (first->get_body_type() == RigidBodyType::LINE) {
            if (second->get_body_type() == RigidBodyType::CIRCLE) {
                Line &firstAsLine = ((Line&) *first);
                Circle &secondAsCircle = ((Circle&) *second);
                if (!physics::are_colliding(secondAsCircle, firstAsLine, axisNormalized, &penetrationDistance)) return false;
            }
        }

        // calculated relevant penetration distance
        // if (penetrationDistance < 0) return false;
        // using restitution and getting collision response values
        relativeVelocity = first->get_position() - second->get_position();
        
        real e = std::min(first->get_restitution(), second->get_restitution());
        real j = -((real) 1 + e) * relativeVelocity.dot(axisNormalized);
        j /= first->get_inverse_mass() + second->get_inverse_mass(); // if axis wasn't normalized, magnitude was needed in the denominator, multiplied by the inverse masses sum
        Vector impulse = axisNormalized * j;
        if (first->has_finite_mass()) (*first).increment_velocity(impulse * -first->get_inverse_mass() * dt);
        if (second->has_finite_mass()) (*second).increment_velocity(impulse * second->get_inverse_mass() * dt); // incremented position before
        return true;
    }

    Vector& closest_vertex_to(physics::Vector origin, std::vector<Vector> &vertices) {
        Vector &closest = vertices[0];
        real minDistanceSquared = Constants::REAL_MAX;

        for (auto &vertex : vertices) {
            real xDiff = origin.x - vertex.x;
            real yDiff = origin.y - vertex.y;
            real distanceSquared = xDiff*xDiff + yDiff*yDiff;
            if (distanceSquared < minDistanceSquared) {
                minDistanceSquared = distanceSquared;
                closest = vertex;
            }
        }
        return closest;
    }

    bool are_colliding(physics::Polygon &first, physics::Polygon &second,
     Vector &projectionNormal, real *penetrationDistance) {
        size_t firstNumVertices = first.get_num_vertices();
        size_t secondNumVertices = second.get_num_vertices();
        real minFirst, maxFirst;
        real minSecond, maxSecond;
        real currentPenetration = 0;
        // checking first polygon's edges
        for (int i = 0; i < firstNumVertices; i++) {
            const Vector &vertexA = first.get_vertices()->at(i);
            const Vector &vertexB = first.get_vertices()->at((i+1) % firstNumVertices);
            Vector edge = vertexB - vertexA; // vector from A to B
            // getting orthogonal vector to the edge, i.e: the axis
            Vector axis = Vector{-edge.y, edge.x};
            axis.normalize();
            // clamping
            physics::clamp_vertices(first, axis, &minFirst, &maxFirst);
            physics::clamp_vertices(second, axis, &minSecond, &maxSecond);
            // if there's no overlap, they're not colliding (by AABB theorem)
            if (minFirst >= maxSecond || minSecond >= maxFirst) return false;
            // getting the penetration value and updating the projection vector accordingly
            currentPenetration = std::min(maxFirst - minSecond, maxSecond - minFirst);
            if (currentPenetration < *penetrationDistance) {
                *penetrationDistance = currentPenetration;
                projectionNormal = axis;
            }
        }
        // checking second polygon
        for (int i = 0; i < secondNumVertices; i++) {
            const Vector &vertexA = second.get_vertices()->at(i);
            const Vector &vertexB = second.get_vertices()->at((i+1) % secondNumVertices);
            Vector edge = vertexB - vertexA; // vector from A to B
            // getting orthogonal vector to the edge, i.e: the axis
            Vector axis = Vector{-edge.y, edge.x};
            axis.normalize();
            // clamping
            physics::clamp_vertices(first, axis, &minFirst, &minFirst);
            physics::clamp_vertices(second, axis, &minSecond, &maxSecond);
            // if there's no overlap, they're not colliding (by AABB theorem)
            if (minFirst >= maxSecond || minSecond >= minFirst) return false;
            // getting the penetration value and updating the projection vector accordingly
            currentPenetration = std::min(maxFirst - minSecond, maxSecond - minFirst);
            if (currentPenetration < *penetrationDistance) {
                *penetrationDistance = currentPenetration;
                projectionNormal = axis;
            }
        }
        // axis and penetration distance are normalized correctly
        // checking validity of projection's direction
        Vector firstToSecondDirectionVector = second.get_position() - first.get_position();
        // if not, change its direction
        if (firstToSecondDirectionVector.dot(projectionNormal) < 0) projectionNormal *= -1;
        return true;
    }

    bool are_colliding(physics::Circle &circle, physics::Polygon &polygon,
     physics::Vector &projectionNormal, real *penetrationDistance) {
        size_t firstNumVertices = polygon.get_num_vertices();
        real circleMin, circleMax;
        real polygonMin, polygonMax;
        real currentPenetration = 0;
        // checking polygon's edges
        for (int i = 0; i < firstNumVertices; i++) {
            const Vector &vertexA = polygon.get_vertices()->at(i);
            const Vector &vertexB = polygon.get_vertices()->at((i+1) % firstNumVertices);
            Vector edge = vertexB - vertexA; // vector from A to B
            // getting orthogonal vector to the edge, i.e: the axis
            Vector axis = Vector{-edge.y, edge.x};
            axis.normalize();
            // clamping
            physics::clamp_vertices(polygon, axis, &polygonMin, &polygonMax);
            physics::clamp_circle(circle, axis, &circleMin, &circleMax);
            // if there's no overlap in one axis, they're not colliding
            if (polygonMin >= circleMax || circleMin >= polygonMax) return false;
            // getting the penetration value and updating the projection vector accordingly
            currentPenetration = std::min(circleMax - polygonMin, polygonMax - circleMin);
            if (currentPenetration < *penetrationDistance) {
                *penetrationDistance = currentPenetration;
                projectionNormal = axis;
                if (circle.get_position().dot(projectionNormal) < 0) projectionNormal *= -1;
            }
        }
        
        // checking validity of projection's direction
        Vector polygonPosition = Vector{0, 0, 0};
        physics::polygon_center_position(polygon, polygonPosition);
        Vector circleToPolygonVector = polygonPosition - circle.get_position();

        // if not, change its direction
        if (circleToPolygonVector.dot(projectionNormal) < 0) projectionNormal *= -1;

        return true;
    }

    bool are_colliding(Circle &circle, Line &line, Vector &projectionNormal, real *penetrationDistance) {
        const Vector &first = line.get_first();
        const Vector &second = line.get_second();
        Vector lineVector = second - first;
        Vector axis = Vector{ -lineVector.y, lineVector.x };
        axis.normalize();
        // checking if circle is between the two vertices
        if (circle.get_position().dot(lineVector) < 0 ||
        circle.get_position().dot(lineVector * -1) < 0) return false;
        // check collision distance
        Vector firstToCircle = circle.get_position() - first;
        Vector secondToCircle = circle.get_position() - second;
        // calculating distance from the line as axis is normalized
        real distanceFromLine = std::min(firstToCircle.dot(axis), secondToCircle.dot(axis));
        if (distanceFromLine > circle.get_radius()) return false;
        projectionNormal = axis;
        *penetrationDistance = distanceFromLine;
        return true;
    }

    void clamp_vertices(physics::Polygon &polygon, physics::Vector &axis, real *min, real *max) {
        *min = Constants::REAL_MAX;
        *max = Constants::REAL_MIN;

        for (auto &vertex : *(polygon.get_vertices())) {
            real dotProduct = vertex.dot(axis);
            *min = std::min(*min, dotProduct);
            *max = std::max(*max, dotProduct);
        }
    }

    void clamp_vertex(Vector &vertex, Vector &axis, real *min, real *max) {
        *min = Constants::REAL_MAX;
        *max = Constants::REAL_MIN;

        real dotProduct = vertex.dot(axis);
        *min = std::min(*min, dotProduct);
        *max = std::max(*max, dotProduct);
    }

    void clamp_circle(physics::Circle &circle, physics::Vector &axis, real *min, real *max) {
        real radius = circle.get_radius();
        // declaring the axis for the points on the circle and scaling it by the radius
        Vector scaledAxis = axis * radius;
        // declaring the 2 points on the circle to clamp with the axis
        Vector circleVertexA = circle.get_position() + scaledAxis;
        Vector circleVertexB = circle.get_position() - scaledAxis;
        // clamping
        real dotA = circleVertexA.dot(axis);
        real dotB = circleVertexB.dot(axis);

        *min = std::min(dotA, dotB);
        *max = std::max(dotA, dotB);
    }

    void polygon_center_position(physics::Polygon &polygon, physics::Vector &position) {
        real xSum = 0;
        real ySum = 0;
        real zSum = 0;
        size_t numVertices = polygon.get_vertices()->size();

        for (auto &vertex : *(polygon.get_vertices())) {
            xSum += vertex.x;
            ySum += vertex.y;
            zSum += vertex.z;
        }
        // average of each coordinate for a rough estimation of its' center
        xSum /= (real) numVertices;
        ySum /= (real) numVertices;
        zSum /= (real) numVertices;

        position.x = xSum;
        position.y = ySum;
        position.z = zSum;
    }
}
