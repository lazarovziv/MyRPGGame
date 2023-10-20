#include "../include/Collision.hpp"

namespace physics {

    bool physics::resolveCollisions(physics::RigidBody *first, physics::RigidBody *second, real dt) {
        if (!first->hasFiniteMass() && !second->hasFiniteMass()) return false;
        // direction vector to project first body
        Vector axisToProject = second->getPosition() - first->getPosition();
        Vector axisNormalized = axisToProject.normalized();
        // checking moving in the same direction
        Vector relativeVelocity = second->getVelocity() - first->getVelocity();
        if (relativeVelocity.dot(axisNormalized) > 0) return false;
        // scaler to the direction vector
        real penetrationDistance = Constants::REAL_MAX;
        // dividing into all types of bodies
        // TODO: add check for first contained in the second
        if (first->getBodyType() == RigidBodyType::CIRCLE) {
            if (second->getBodyType() == RigidBodyType::CIRCLE) {
                // distance of the overlapping area
                penetrationDistance = ((Circle*)first)->getRadius() + ((Circle*)second)->getRadius() - axisToProject.norma();
                if (penetrationDistance < 0) return false;
            } else if (second->getBodyType() == RigidBodyType::POLYGON) {
                Circle &circle = ((Circle&) *first);
                Polygon &polygon = ((Polygon&) *second);
                if (!physics::areColliding(circle, polygon, axisNormalized, &penetrationDistance)) return false;

            } else if (second->getBodyType() == RigidBodyType::LINE) {
                Circle &firstAsCircle = ((Circle&) *first);
                Line &secondAsLine = ((Line&) *second);
                if (!physics::areColliding(firstAsCircle, secondAsLine, axisNormalized, &penetrationDistance)) return false;
            }
        } else if (first->getBodyType() == RigidBodyType::POLYGON) {
            if (second->getBodyType() == RigidBodyType::CIRCLE) {
                Polygon &polygon = ((Polygon&) *first);
                Circle &circle = ((Circle&) *second);
                if (!physics::areColliding(circle, polygon, axisNormalized, &penetrationDistance)) return false;
            } else if (second->getBodyType() == RigidBodyType::POLYGON) {
                Polygon &firstAsPolygon = ((Polygon&) *first);
                Polygon &secondAsPolygon = ((Polygon&) *second);
                if (!physics::areColliding(firstAsPolygon, secondAsPolygon, axisNormalized, &penetrationDistance)) return false;
            }
        } else if (first->getBodyType() == RigidBodyType::LINE) {
            if (second->getBodyType() == RigidBodyType::CIRCLE) {
                Line &firstAsLine = ((Line&) *first);
                Circle &secondAsCircle = ((Circle&) *second);
                if (!physics::areColliding(secondAsCircle, firstAsLine, axisNormalized, &penetrationDistance)) return false;
            }
        }

        // calculated relevant penetration distance
        // if (penetrationDistance < 0) return false;
        // using restitution and getting collision response values
        relativeVelocity = first->getPosition() - second->getPosition();
        
        real e = std::min(first->getRestitution(), second->getRestitution());
        real j = -((real) 1 + e) * relativeVelocity.dot(axisNormalized);
        j /= first->getInverseMass() + second->getInverseMass(); // if axis wasn't normalized, magnitude was needed in the denominator, multiplied by the inverse masses sum
        Vector impulse = axisNormalized * (2 * j);
        if (first->hasFiniteMass()) (*first).incrementVelocity(impulse * -first->getInverseMass() * dt);
        if (second->hasFiniteMass()) (*second).incrementVelocity(impulse * second->getInverseMass() * dt); // incremented position before

        // resolving interpenetration

        /*
        // setting the separation distance for each body
        real firstDistance = 0, secondDistance = 0;
        firstDistance = second->getMass()/(first->getMass() + second->getMass());
        secondDistance = first->getMass()/(first->getMass() + second->getMass());
        if (first->hasFiniteMass()) (*first) += axisNormalized * dt * -penetrationDistance * firstDistance;
        if (second->hasFiniteMass()) (*second) += axisNormalized * dt * penetrationDistance * secondDistance;
        */
        return true;
    }

    Vector& physics::closestVertexTo(physics::Vector origin, std::vector<Vector> &vertices) {
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

    bool physics::areColliding(physics::Polygon &first, physics::Polygon &second, Vector &projectionNormal, real *penetrationDistance) {
        size_t firstNumVertices = first.getNumVertices();
        size_t secondNumVertices = second.getNumVertices();
        real minFirst, maxFirst;
        real minSecond, maxSecond;
        real currentPenetration = 0;
        // checking first polygon's edges
        for (int i = 0; i < firstNumVertices; i++) {
            const Vector &vertexA = first.getVertices()->at(i);
            const Vector &vertexB = first.getVertices()->at((i+1) % firstNumVertices);
            Vector edge = vertexB - vertexA; // vector from A to B
            // getting orthogonal vector to the edge, i.e: the axis
            Vector axis = Vector{-edge.y, edge.x};
            axis.normalize();
            // clamping
            physics::clampVertices(first, axis, &minFirst, &maxFirst);
            physics::clampVertices(second, axis, &minSecond, &maxSecond);
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
            const Vector &vertexA = second.getVertices()->at(i);
            const Vector &vertexB = second.getVertices()->at((i+1) % secondNumVertices);
            Vector edge = vertexB - vertexA; // vector from A to B
            // getting orthogonal vector to the edge, i.e: the axis
            Vector axis = Vector{-edge.y, edge.x};
            axis.normalize();
            // clamping
            physics::clampVertices(first, axis, &minFirst, &minFirst);
            physics::clampVertices(second, axis, &minSecond, &maxSecond);
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
        Vector firstToSecondDirectionVector = second.getPosition() - first.getPosition();
        // if not, change its direction
        if (firstToSecondDirectionVector.dot(projectionNormal) < 0) projectionNormal *= -1;
        return true;
    }

    bool physics::areColliding(physics::Circle &circle, physics::Polygon &polygon, physics::Vector &projectionNormal, real *penetrationDistance) {
        size_t firstNumVertices = polygon.getNumVertices();
        real circleMin, circleMax;
        real polygonMin, polygonMax;
        real currentPenetration = 0;
        // checking polygon's edges
        for (int i = 0; i < firstNumVertices; i++) {
            const Vector &vertexA = polygon.getVertices()->at(i);
            const Vector &vertexB = polygon.getVertices()->at((i+1) % firstNumVertices);
            Vector edge = vertexB - vertexA; // vector from A to B
            // getting orthogonal vector to the edge, i.e: the axis
            Vector axis = Vector{-edge.y, edge.x};
            axis.normalize();
            // clamping
            physics::clampVertices(polygon, axis, &polygonMin, &polygonMax);
            physics::clampCircle(circle, axis, &circleMin, &circleMax);
            // if there's no overlap in one axis, they're not colliding
            if (polygonMin >= circleMax || circleMin >= polygonMax) return false;
            // getting the penetration value and updating the projection vector accordingly
            currentPenetration = std::min(circleMax - polygonMin, polygonMax - circleMin);
            if (currentPenetration < *penetrationDistance) {
                *penetrationDistance = currentPenetration;
                projectionNormal = axis;
                if (circle.getPosition().dot(projectionNormal) < 0) projectionNormal *= -1;
            }
        }
        
        // checking validity of projection's direction
        Vector polygonPosition = Vector{0, 0, 0};
        physics::polygonCenterPosition(polygon, polygonPosition);
        Vector circleToPolygonVector = polygonPosition - circle.getPosition();

        // if not, change its direction
        if (circleToPolygonVector.dot(projectionNormal) < 0) projectionNormal *= -1;

        return true;
    }

    bool physics::areColliding(Circle &circle, Line &line, Vector &projectionNormal, real *penetrationDistance) {
        const Vector &first = line.getFirst();
        const Vector &second = line.getSecond();
        Vector lineVector = second - first;
        real lineLength = lineVector.norma();
        Vector circleToFirst = first - circle.getPosition();
        
        real lengthFromFirstToCollision = 0;
        if (std::min(lineVector.dot(circleToFirst), lineLength) > 0) lengthFromFirstToCollision = std::min(lineVector.dot(circleToFirst), lineLength);
        
        // normalizing the length to get values from 0 to 1
        lengthFromFirstToCollision /= lineLength;
        Vector collisionVector = first + (lineVector * lengthFromFirstToCollision);
        // getting distance to the circle
        real distanceToLine = collisionVector.distance(circle.getPosition());
        std::cout << distanceToLine << std::endl;

        if (distanceToLine <= circle.getRadius()) {
            *penetrationDistance = circle.getRadius() - distanceToLine;
            projectionNormal.x = -collisionVector.y;
            projectionNormal.y = collisionVector.x;
            projectionNormal = projectionNormal.normalized();

            if (projectionNormal.dot(circle.getPosition()) < 0) projectionNormal *= -1;
            return true;
        } 
        
        return false;
    }

    void physics::clampVertices(physics::Polygon &polygon, physics::Vector &axis, real *min, real *max) {
        *min = Constants::REAL_MAX;
        *max = Constants::REAL_MIN;

        for (auto &vertex : *(polygon.getVertices())) {
            real dotProduct = vertex.dot(axis);
            *min = std::min(*min, dotProduct);
            *max = std::max(*max, dotProduct);
        }
    }

    void physics::clampVertex(Vector &vertex, Vector &axis, real *min, real *max) {
        *min = Constants::REAL_MAX;
        *max = Constants::REAL_MIN;

        real dotProduct = vertex.dot(axis);
        *min = std::min(*min, dotProduct);
        *max = std::max(*max, dotProduct);
    }

    void physics::clampCircle(physics::Circle &circle, physics::Vector &axis, real *min, real *max) {
        real radius = circle.getRadius();
        // declaring the axis for the points on the circle and scaling it by the radius
        Vector scaledAxis = axis * radius;
        // declaring the 2 points on the circle to clamp with the axis
        Vector circleVertexA = circle.getPosition() + scaledAxis;
        Vector circleVertexB = circle.getPosition() - scaledAxis;
        // clamping
        real dotA = circleVertexA.dot(axis);
        real dotB = circleVertexB.dot(axis);

        *min = std::min(dotA, dotB);
        *max = std::max(dotA, dotB);
    }

    void physics::polygonCenterPosition(physics::Polygon &polygon, physics::Vector &position) {
        real xSum = 0;
        real ySum = 0;
        real zSum = 0;
        size_t numVertices = polygon.getVertices()->size();

        for (auto &vertex : *(polygon.getVertices())) {
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
