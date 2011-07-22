#pragma once

#include "Body.h"
#include "Softbody.h"
#include "Ball.h"
#include "Plane.h"

class Collision
{
public:
    static void checkCollision( Ball *ball, Softbody *softbody )
    {
        checkCollision(softbody, ball);
    }
    static void checkCollision( Softbody *softbody, Ball *ball );

    static void checkCollision (Ball *b1, Ball *b2);

    static void checkCollision( Plane *plane, Ball *ball)
    {
        checkCollision(ball, plane);
    }
    static void checkCollision( Ball *ball, Plane *plane);

    static void checkCollision( Plane *plane, Softbody *softbody)
    {
        checkCollision(softbody, plane);
    }
    static void checkCollision( Softbody *softbody, Plane *plane);

private:
    

};

//template <class BodyType1, class BodyType2>
//class Collision : BaseCollision
//{
//    virtual void checkCollision(Body *body1, Body *body2 ) 
//    {
//        checkCollision(static_cast<BodyType1*>(body1), static_cast<BodyType2*>(body2));
//    }
//
//    virtual void checkCollision(BodyType1 *body1, BodyType2 *body2) = 0;
//};



