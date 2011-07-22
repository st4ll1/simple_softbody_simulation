#include "RigidBody.h"

RigidBody::~RigidBody(void)
{
}

void RigidBody::applyForce( ci::Vec3f const &force)
{
    this->force += force;
}

void RigidBody::applyVelocity( ci::Vec3f const &velocity)
{
    this->velocity += velocity;
}

void RigidBody::update( float dt)
{
    ci::Vec3f acceleration = force / mass;
    
    force = ci::Vec3f::zero();

    velocity += acceleration * dt;
    position += velocity * dt + acceleration * 0.5f * dt * dt;
    if(velocity.length() < 0.0001f) velocity = ci::Vec3f::zero();
}

void RigidBody::setPosition( ci::Vec3f const position )
{
    this->position = position;
}

void RigidBody::setVelocity( ci::Vec3f const position ) 
{
    this->velocity = velocity;
}

void RigidBody::setMass( float mass )
{
    this->mass = mass;
}
