#pragma once

#include "cinder\Vector.h"
#include "cinder\Color.h"
#include "Body.h"

class RigidBody : public Body
{
public:
    RigidBody( ci::Vec3f position = ci::Vec3f::zero(), float mass = 1.0f, ci::Colorf = ci::Colorf::white()) 
        :   position( position ),
            mass( mass ),
            velocity( ci::Vec3f::zero() ),
            color( color ),
            force ( ci::Vec3f::zero() ) {};

    virtual ~RigidBody(void);

    virtual void update( float dt );
    virtual void draw() = 0;
    /**
    	\param force force applied to the rigidbody
    */
    virtual void applyForce(ci::Vec3f const &force);
    virtual void applyVelocity(ci::Vec3f const &v);

    //setter
    virtual void setPosition( ci::Vec3f const position) ;
    virtual void setVelocity( ci::Vec3f velocity );
    virtual void setMass( float mass);

    //getter
    virtual float getMass() const { return mass; };
    virtual ci::Vec3f getPosition() const { return position; };
    virtual ci::Vec3f getVelocity() const { return velocity; };
        
protected:
    float mass;
    ci::Colorf color;
    ci::Vec3f position;
    ci::Vec3f velocity;
    ci::Vec3f force;
};

