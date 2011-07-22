#include "Plane.h"

Plane::~Plane(void)
{
}

void Plane::draw()
{
    ci::gl::color(ci::Color(0.6f,0.6f,0.0f));
    ci::gl::drawCube(position, size);
    ci::gl::color(ci::Color(1.0f,1.0f,1.0f));
}

ci::Vec3f Plane::getSize()
{
    return size;
}


