#pragma once

#include "RigidBody.h"
#include "cinder\gl\gl.h"

class Ball : public RigidBody
{
public:
    Ball(float radius, ci::Vec3f position = ci::Vec3f::zero(), float maxLifetime = 5.0f, float mass = 1.0f, ci::Colorf color = ci::Colorf::white() )
        : RigidBody( position, mass, color), radius(radius), lifeTime(0.0f), maxLifeTime(maxLifetime) {};
    virtual ~Ball(void);

    virtual void draw();
    virtual void update( float dt );
    

    virtual float getRadius() const { return radius;};
private:
    float radius;   
    float lifeTime;
    float maxLifeTime;
};



