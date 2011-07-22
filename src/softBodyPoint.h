#pragma once

#include "cinder/Utilities.h"
#include "cinder/gl/gl.h"
#include "Plane.h"

class SoftBodyPoint
{
private:
    unsigned int id;
    ci::Vec3f position, velocity, force;
    float mass;

    static ci::Vec3f drawSize;

public:
    /**
    	\param position startposition of vertex
        \param mass mass of the vertex        
    */
    SoftBodyPoint(ci::Vec3f position, float mass = 1.0f): position(position), velocity(ci::Vec3f::zero()), force(ci::Vec3f::zero()), mass(mass)
    {
        id = amountOfPoints++;
        drawSize = ci::Vec3f(0.2f, 0.2f, 0.2f);
    }
    ~SoftBodyPoint(void);
    void update(float dt);
    void draw();
    void applyForce(ci::Vec3f const &force);
    void applyVelocity(ci::Vec3f const &velocity);
    /**
    	\return returns the vertexposition
    */
    ci::Vec3f getPosition() const;
    ci::Vec3f getVelocity() const;
    float getMass() const;

    static void checkCollisions();
    static void checkCollision(Plane plane);

    static unsigned int amountOfPoints;
};

