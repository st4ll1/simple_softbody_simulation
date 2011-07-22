#pragma once

#include "cinder/gl/gl.h"
#include "RigidBody.h"

class Plane : public RigidBody
{
public:
    Plane(ci::Vec3f position, ci::Vec3f size)
        :   position(position),
            size(size)
    {
        
    }        
    
    virtual ~Plane(void);
    virtual void draw();
    virtual ci::Vec3f getSize();
private:
    ci::Vec3f position, size;
};

