#include "softBodyPoint.h"

unsigned int SoftBodyPoint::amountOfPoints = 0;
ci::Vec3f SoftBodyPoint::drawSize = ci::Vec3f(0.2f, 0.2f, 0.2f);

SoftBodyPoint::~SoftBodyPoint(void)
{
}

/**
	\todo delete the if, when collision between plane and softbody works
*/
void SoftBodyPoint::update(float dt)
{
  ci::Vec3f acceleration = force / mass;
  force = ci::Vec3f::zero();
  velocity += acceleration * dt;
    
  if(position.y > 0) 
  {
      position += velocity * dt + acceleration * 0.5f * dt * dt;       
  }
  else
  {
      velocity = ci::Vec3f::zero();
  }
    if(velocity.length() < 0.0001f) velocity = ci::Vec3f::zero();
}

void SoftBodyPoint::applyForce(ci::Vec3f const &force) 
{
    this->force += force;
}

void SoftBodyPoint::applyVelocity(ci::Vec3f const &velocity)
{
    this->velocity = velocity;
}

void SoftBodyPoint::draw()
{      
    ci::gl::drawCube(position, drawSize);
}

ci::Vec3f SoftBodyPoint::getPosition() const
{ 
    return position;
}

ci::Vec3f SoftBodyPoint::getVelocity() const
{
    return velocity;
}

float SoftBodyPoint::getMass() const
{
    return mass;
}