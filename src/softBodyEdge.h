#pragma once

#include "softBodyPoint.h"
#include "cinder/gl/gl.h"

class SoftBodyEdge
{
private:
  SoftBodyPoint* startPoint;
  SoftBodyPoint* endPoint;
  float nominalLength;
  float strength;
  float dampingFactor;
  float maximalLength;
  ci::Vec3f getRadius() const;
public:
  SoftBodyEdge(SoftBodyPoint* startPoint, SoftBodyPoint* endPoint, float maximalLengthFactor = 2.0f , float strength = 60.0f, float dampingFactor = 2.0f) 
    :   startPoint(startPoint), 
        endPoint(endPoint),
        nominalLength((endPoint->getPosition() - startPoint->getPosition()).length()),
        strength(strength),
        dampingFactor(dampingFactor),
        maximalLength((endPoint->getPosition() - startPoint->getPosition()).length() * maximalLengthFactor)
  {}

  ~SoftBodyEdge(void);
  void update(float dt);
  void draw();
  /**
  	\return returns true when length is greater then maximallength
  */
  bool isBroken() const;
};
