#include "../include/RigidBody.hpp"

namespace physics {

    RigidBody::RigidBody(RigidBodyType type, real x, real y, real z, real mass) {
        position = std::make_unique<Vector>(x, y, z);
        velocity = std::make_unique<Vector>(0, 0);
        acceleration = std::make_unique<Vector>(0, 0);
        force_accumulator = std::make_unique<Vector>(0, 0);
        restitution = 0.69;
        set_mass(mass);
        body_type = type;
        damping = 0.995;
    }

    Vector RigidBody::get_velocity() {
        return (*velocity);
    }

    Vector RigidBody::get_acceleration() {
        return (*acceleration);
    }

    Vector &RigidBody::get_position() {
        return *position;
    }

    real RigidBody::get_restitution() const {
        return restitution;
    }

    real RigidBody::get_mass() const {
        return mass;
    }

    real RigidBody::get_inverse_mass() const {
        return inverse_mass;
    }

    RigidBodyType RigidBody::get_body_type() const {
        return body_type;
    }

    bool RigidBody::has_finite_mass() const {
        return !infinite_mass;
    }

    void RigidBody::set_position(const real x, const real y, const real z) {
        position->x = x;
        position->y = y;
        position->z = z;
    }

    void RigidBody::set_position(const physics::Vector &other) {
        position->x = other.x;
        position->y = other.y;
        position->z = other.z;
    }

    void RigidBody::set_mass(const real value) {
        if (value == Constants::REAL_MAX) {
            this->mass = value;
            inverse_mass = 0;
            infinite_mass = true;
            return;
        }
        this->mass = value;
        inverse_mass = 1 / value;
        // just updated mass to be finite
        infinite_mass = false;
    }

    void RigidBody::scale_velocity(const real amount) {
        (*velocity) *= amount;
    }

    void RigidBody::set_restitution(const real e) {
        restitution = e;
    }

    void RigidBody::increment_velocity(const Vector &v) {
        (*velocity) += v;
    }

    void RigidBody::increment_acceleration(const real amount) {
        (*acceleration) += amount;
    }

    void RigidBody::increment_acceleration(const Vector amount) {
        (*acceleration) += amount;
    }

    void RigidBody::reset_force_accumulator() {
        force_accumulator->reset_coordinates();
    }

    // needs to be called before the rigid body is integrated (before changing its position)
    void RigidBody::add_force(const physics::Vector &force) {
        (*force_accumulator) += force; // * mass * Constants::RIGID_BODY_FORCE_SCALE;
    }

    void RigidBody::reset_velocity() {
        velocity->reset_coordinates();
    }

    void RigidBody::reset_acceleration() {
        acceleration->reset_coordinates();
    }

    void RigidBody::operator +=(const Vector &other) {
        if (RigidBody::infinite_mass) return;
        (*position) += other;
    }

    real RigidBody::distance(const physics::RigidBody &other) const {
        return position->distance((*other.position));
    }

    // checking if this is colliding with other, meaning immovable bodies aren't colliding at others, it's others colliding with them
    bool RigidBody::is_colliding_with(physics::RigidBody &other, const real dt) {
        // no collision for immovable bodies
        if (infinite_mass) return false;
        real currentPenetrationDistance = Constants::REAL_MAX;
        // scalar value for the final force direction normal
        real finalPenetrationDistance = Constants::REAL_MAX;
        // axis to implement force direction of the collsion
        Vector finalAxis = Vector::ZERO;
        // axis we're currently checking 
        Vector currentAxis = Vector::ZERO;
        // handle collision by the types of the shapes
        if (body_type == RigidBodyType::CIRCLE) {
            Circle &thisCircle = (Circle&) *this;

            if (other.body_type == RigidBodyType::CIRCLE) {
                Circle &otherCircle = (Circle&) other;
                finalAxis = otherCircle.get_position() - thisCircle.get_position();
                finalPenetrationDistance = thisCircle.get_radius() + otherCircle.get_radius() - finalAxis.norma();
                
            } else if (other.body_type == RigidBodyType::POLYGON) {
                Polygon &otherPolygon = (Polygon&) other;
                
            }
        } else if (body_type == RigidBodyType::POLYGON) {
            Polygon &thisPolygon = (Polygon&) *this;

            if (other.body_type == RigidBodyType::CIRCLE) {

            } else if (other.body_type == RigidBodyType::POLYGON) {
                Polygon &otherPolygon = (Polygon&) other;
                size_t thisVerticesSize = thisPolygon.get_num_vertices();
                size_t otherVerticesSize = otherPolygon.get_num_vertices();
                // checking this edges
                for (int i = 0; i < thisVerticesSize; i++) {
                    Vector thisVertexA = thisPolygon.get_vertices()->at(i);
                    Vector thisVertexB = thisPolygon.get_vertices()->at((i+1) % thisVerticesSize);
                    // edge is A to B
                    Vector thisEdge = thisVertexB - thisVertexA;
                    // getting the normal (i.e the orthogonal vector to the edge), which is the currentAxis we're checking
                    currentAxis = Vector{ -thisEdge.y, thisEdge.x };
                    currentAxis.normalize();
                    // values of projecting to the current axis
                    real thisMin = Constants::REAL_MAX;
                    real thisMax = Constants::REAL_MIN;
                    real otherMin = Constants::REAL_MAX;
                    real otherMax = Constants::REAL_MIN;
                    // traversing all edgs in this polygon and get min/max values
                    for (auto &vertex : *(thisPolygon.get_vertices())) {
                        real currentDotProduct = vertex.dot(currentAxis);
                        thisMin = std::min(thisMin, currentDotProduct);
                        thisMax = std::max(thisMax, currentDotProduct);
                    }
                    // traversing all edges in the other shape and check for overlap
                    for (auto &vertex : *(otherPolygon.get_vertices())) {
                        real currentDotProduct = vertex.dot(currentAxis);
                        otherMin = std::min(otherMin, currentDotProduct);
                        otherMax = std::max(otherMax, currentDotProduct);
                    }

                    // checking overlap
                    if (thisMin < otherMax || otherMin < thisMax) return false;
                    // found a relevant axis
                    currentPenetrationDistance = std::min(thisMax - otherMin, otherMax - thisMin);
                    if (currentPenetrationDistance < finalPenetrationDistance) {
                        finalPenetrationDistance = currentPenetrationDistance;
                        finalAxis = currentAxis;
                    }
                }
            }
        }

        if (finalPenetrationDistance < 0) return false;
        finalAxis = finalAxis.normalized();
        // fixing the direction of the axis if necessary
        // Vector thisToOther = other.getPosition() - this->getPosition();
        // if (thisToOther.dot(finalAxis) < 0) finalAxis *= -1;
        // not found a separating axis, which means it's colliding
        real e = std::min(this->get_restitution(), other.get_restitution());
        real j = -((real) 1 + e) * -finalPenetrationDistance; // for smoother visuals but not accurate
        // real j = -((real) 1 + e) * relativeVelocity.dot(axisNormalized);
        j /= this->get_inverse_mass() + other.get_inverse_mass(); // if axis wasn't normalized, magnitude was needed in the denominator, multiplied by the inverse masses sum
        Vector impulse = finalAxis * j;
        if (this->has_finite_mass()) this->increment_velocity(impulse * -this->get_inverse_mass() * dt);
        if (other.has_finite_mass()) other.increment_velocity(impulse * other.get_inverse_mass() * dt); // incremented position before
        return true;
    }

    void RigidBody::update(const real dt) {
        // updating infinite mass bodies is irrelevant
        if (infinite_mass) return;
        // acceleration = force * mass
        // Vector resultingAcceleration = (*acceleration);
        (*acceleration) += (*force_accumulator) * inverse_mass;
        // if (acceleration->magnitude() > Constants::ACCELERATION_MAGNITUDE_MAX) (*acceleration) *= 0.85;
        (*velocity) += (*acceleration) * dt;
        // drag
        (*velocity) *= pow(damping, dt);
        // applying forces to the position
        (*position) += (*velocity) * dt + (*acceleration) * dt * dt * (real) 0.5;
        // defined friction
        scale_velocity((real) 1/Constants::GRASS_FRICTION_DEGRADATION_CONSTANT);
        // clamping velocity
        if (velocity->magnitude() < Constants::VELOCITY_MAGNITUDE_MIN) velocity->reset_coordinates();
        // if (velocity->magnitude() > Constants::VELOCITY_MAGNITUDE_MAX) scaleVelocity((real) 0.95);
        reset_force_accumulator();
        reset_acceleration();
    }

    Circle::Circle(real x, real y, real z, real r) : RigidBody(RigidBodyType::CIRCLE, x, y, z) {
        radius = r;
    }

    real Circle::get_radius() const {
        return Circle::radius;
    }

    Polygon Polygon::RIGHT_END_SCREEN = Polygon{Constants::FULL_SCREEN_WIDTH + Polygon::MAP_POLYGON_WIDTH/2,
                                    Constants::FULL_SCREEN_HEIGHT/2,
                                    0, // very tall
                                                { // vertices of the polygon (rectangle)
        Vector{Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT}, // bottom left
        Vector{Constants::FULL_SCREEN_WIDTH+Polygon::MAP_POLYGON_WIDTH, Constants::FULL_SCREEN_HEIGHT}, // bottom right
        Vector{Constants::FULL_SCREEN_WIDTH+Polygon::MAP_POLYGON_WIDTH, (real) 0}, // top right
        Vector{Constants::FULL_SCREEN_WIDTH, (real) 0} // top left
                                                },
                                                Constants::REAL_MAX};
    Polygon Polygon::LEFT_END_SCREEN = Polygon{(real) -Polygon::MAP_POLYGON_WIDTH/2,
                                    Constants::FULL_SCREEN_HEIGHT/2,
                                    0, // very tall
                                               { // vertices of the polygon (rectangle)
        Vector{(real) -Polygon::MAP_POLYGON_WIDTH, Constants::FULL_SCREEN_HEIGHT}, // bottom left
        Vector{(real) 0, Constants::FULL_SCREEN_HEIGHT}, // bottom right
        Vector{(real) 0, (real) 0}, // top right
        Vector{(real) -Polygon::MAP_POLYGON_WIDTH, (real) 0} // top left
                                               },
                                               Constants::REAL_MAX};
    Polygon Polygon::TOP_END_SCREEN = Polygon{Constants::FULL_SCREEN_WIDTH/2,
                                              (real) -Polygon::MAP_POLYGON_HEIGHT/2,
                                            0, // very tall
                                              { // vertices of the polygon (rectangle)
        Vector{(real) 0, (real) 0}, // bottom left
        Vector{Constants::FULL_SCREEN_WIDTH, (real) 0}, // bottom right
        Vector{Constants::FULL_SCREEN_WIDTH, (real) -Polygon::MAP_POLYGON_HEIGHT}, // top right
        Vector{(real) 0, (real) -Polygon::MAP_POLYGON_HEIGHT} // top left
                                              },
                                              Constants::REAL_MAX};
    Polygon Polygon::BOTTOM_END_SCREEN = Polygon{Constants::FULL_SCREEN_WIDTH/2,
                                  Constants::FULL_SCREEN_HEIGHT + Polygon::MAP_POLYGON_HEIGHT/2,
                                  0, // very tall
                                                 { // vertices of the polygon (rectangle)
        Vector{(real) 0, Constants::FULL_SCREEN_HEIGHT+Polygon::MAP_POLYGON_HEIGHT}, // bottom left
        Vector{Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT+Polygon::MAP_POLYGON_HEIGHT}, // bottom right
        Vector{Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT}, // top right
        Vector{(real) 0, Constants::FULL_SCREEN_HEIGHT} // top left
                                                 },
                                                 Constants::REAL_MAX};

    Polygon::Polygon(const real x, const real y, const real z, const std::vector<Vector> &newVertices, const real mass)
    : RigidBody(RigidBodyType::POLYGON, x, y, z, mass) {
        Polygon::vertices = std::make_unique<std::vector<Vector>>();
        for (auto &v : newVertices) {
            Polygon::vertices->push_back(v);
        }
    }

    std::vector<Vector>* Polygon::get_vertices() {
        return Polygon::vertices.get();
    }

    size_t Polygon::get_num_vertices() const {
        return Polygon::vertices->size();
    }

    Line Line::RIGHT_END_SCREEN = Line {
        Constants::FULL_SCREEN_WIDTH, 0,
        Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT
    };

    Line Line::LEFT_END_SCREEN = Line {
        0, 0,
        0, Constants::FULL_SCREEN_HEIGHT
    };

    Line Line::TOP_END_SCREEN = Line {
        0, 0,
        Constants::FULL_SCREEN_WIDTH, 0
    };

    Line Line::BOTTOM_END_SCREEN = Line {
        0, Constants::FULL_SCREEN_HEIGHT,
        Constants::FULL_SCREEN_WIDTH, Constants::FULL_SCREEN_HEIGHT
    };

    Line::Line(const real x1, const real y1, const real x2, const real y2, const real mass)
    : RigidBody(RigidBodyType::LINE, (x1-x2)/2, (y1-y2)/2, 0, mass) {
        p1 = Vector{ x1, y1 };
        p2 = Vector{ x2, y2 };
        // no slope, only bias
        if (x1 == x2) slope = 0;
        else slope = (y2-y1)/(x2-x1);
        bias = y1 - slope * x1;
    }

    Vector Line::get_first() const {
        return Line::p1;
    }

    Vector Line::get_second() const {
        return Line::p2;
    }

    real Line::get_slope() const {
        return Line::slope;
    }

    real Line::get_bias() const {
        return Line::bias;
    }
}
