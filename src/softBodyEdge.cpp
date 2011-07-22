#include "softBodyEdge.h"

SoftBodyEdge::~SoftBodyEdge(void)
{

}

ci::Vec3f SoftBodyEdge::getRadius() const
{
  return endPoint->getPosition() - startPoint->getPosition();
}

void SoftBodyEdge::update(float dt)
{
  ci::Vec3f radius = getRadius();    
  float length = radius.length();
  float displacement = length - nominalLength;
  //if(displacement < 0.0f) return;
  float forceMagnitude = -strength * displacement;
  //if(length > 0.1)
  {
    radius = radius / length;
    radius = radius * forceMagnitude;
    startPoint->applyForce(-radius);
    endPoint->applyForce(radius);
  }  
}

bool SoftBodyEdge::isBroken() const
{
  return (maximalLength < getRadius().length());
}

void SoftBodyEdge::draw()
{
  float length = getRadius().length();
  if(length < nominalLength * 0.9f)
    ci::gl::color( ci::Color(1.0f,0.0f,0.0f) );
  else if( length > nominalLength * 1.1f )
    ci::gl::color(ci::Color(0.0f,1.0f,0.0f));
  //else if(length == nominalLength)
  else
    ci::gl::color(ci::Color(0.0f,0.0f,1.0f));
  //else
  //  ci::gl::color(ci::Color(0.0f,1.0f,1.0f));
  ci::gl::drawLine(startPoint->getPosition(), endPoint->getPosition());
  ci::gl::color(ci::Color(1.0f,1.0f,1.0f));
}

