#include "Ball.h"

Ball::~Ball(void)
{
}

void Ball::update( float dt )
{
    lifeTime += dt;
    RigidBody::update( dt );    
    if(lifeTime > maxLifeTime && maxLifeTime != 0.0f) isAlive = false;
}

void Ball::draw()
{    
    //ci::gl::color( color );
    ci::gl::drawSphere( position, radius );
    ci::gl::color( ci::Color( 1.0f, 1.0f, 1.0f) );  
}    

