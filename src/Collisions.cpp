#include "Collisions.h"

/**
	collision between a softbody and a plane
    \param softbody the softbody
    \param plane the plane
    \todo improve collisioncheck
*/
void Collision::checkCollision( Softbody *softbody, Plane *plane )
{
    SoftBodyPoint* currentVertex;
   
    ci::Vec3f corner1 = plane->getPosition() - plane->getSize()/2;
    ci::Vec3f corner2 = corner1 + plane->getSize();
    ci::Vec3f vertexPosition;
    int i;
    int size = softbody->getAmountOfVertices();
    for (i = 0; i < size; ++i)
    {
        currentVertex = softbody->getVertex(i);
        vertexPosition = currentVertex->getPosition();
        
        if(vertexPosition.x > corner2.x || vertexPosition.x < corner1.x 
            || vertexPosition.z > corner2.z ||vertexPosition.z < corner1.z )
        {
            continue;
        }

        ci::Vec3f vertexVelocity = currentVertex->getVelocity();
        if( vertexPosition.y < corner1.y && vertexPosition.y + vertexVelocity.y < corner1.y
            || vertexPosition.y > corner2.y && vertexPosition.y + vertexVelocity.y > corner2.y)
        {
            continue;
        }

        //collision
        float inverseMassPlane = plane->getMass();
        if(inverseMassPlane != 0.0f) inverseMassPlane = 1/inverseMassPlane;
        float inverseMassVertex = currentVertex->getMass();
        if(inverseMassVertex != 0.0f) inverseMassVertex = 1/inverseMassVertex;

        ci::Vec3f n0 = ci::Vec3f(0.0f, corner1.y - vertexPosition.y, 0.0f).normalized();
        float v1n = plane->getVelocity().dot( n0 );
        float v2n = currentVertex->getVelocity().dot( n0 );

        ci::Vec3f deltaP = n0 * (v1n - v2n) / (inverseMassPlane + inverseMassVertex);
        float coefficientOfRestitution = 1.0f;

        plane->applyVelocity( -(1+coefficientOfRestitution) * deltaP * inverseMassPlane);
        currentVertex->applyVelocity( (1+coefficientOfRestitution) * deltaP * inverseMassVertex);
    }
}

/**
	collision between two balls
    \param b1 first ball
    \param b2 second ball
    \bug speed up after collision
*/
void Collision::checkCollision( Ball *b1, Ball *b2 )
{
    ci::Vec3f x1 = b1->getPosition();
    ci::Vec3f x2 = b2->getPosition();    
    ci::Vec3f radius = x2 - x1;
    float distance = x2.distance(x1);    

    if( distance > b1->getRadius() + b2->getRadius() )
    {
        return;
    }

    ci::Vec3f n0 = (x2 - x1).normalized();
    float v1n = b1->getVelocity().dot( n0 );
    float v2n = b2->getVelocity().dot( n0 );

    if(v2n - v1n <= 0)
    {
        return;
    }
    
    float inverseMass1 = b1->getMass();
    if(inverseMass1 != 0.0f) inverseMass1 = 1/inverseMass1;
    float inverseMass2 = b2->getMass();
    if(inverseMass2 != 0.0f) inverseMass2 = 1/inverseMass2;

    ci::Vec3f deltaP = n0 * (v1n - v2n) / (inverseMass1 + inverseMass2);
    float coefficientOfRestitution = 0.5f;

    b1->applyVelocity( (1+coefficientOfRestitution) * deltaP * inverseMass1);
    b2->applyVelocity( -(1+coefficientOfRestitution) * deltaP * inverseMass2);
}


/**
	collision between a softbody and a ball
    \param softbody softbody to calculate
    \param ball ball to calculate
    \todo programm this function
    \bug speedup after collision
*/
void Collision::checkCollision( Softbody *softbody, Ball *ball )
{
    SoftBodyPoint* currentVertex;
    
    ci::Vec3f x2 = ball->getPosition();
    float inverseMass2 = ball->getMass();
    if(inverseMass2 != 0.0f) inverseMass2 = 1/inverseMass2;

    int i;
    int size = softbody->getAmountOfVertices();
    for (i = 0; i < size; ++i)
    {
        currentVertex = softbody->getVertex(i);
        ci::Vec3f x1 = currentVertex->getPosition();            
        ci::Vec3f radius = x2 - x1;
        float distance = x2.distance(x1);    

        if( distance > ball->getRadius() )
        {
            continue;
        }

        ci::Vec3f n0 = (x2 - x1).normalized();
        float v1n = currentVertex->getVelocity().dot( n0 );
        float v2n = ball->getVelocity().dot( n0 );

        if(v2n - v1n <= 0)
        {
            continue;
        }

        float inverseMass1 = currentVertex->getMass();
        if(inverseMass1 != 0.0f) inverseMass1 = 1/inverseMass1;        

        ci::Vec3f deltaP = n0 * (v1n - v2n) / (inverseMass1 + inverseMass2);
        float coefficientOfRestitution = 0.5f;

        currentVertex->applyVelocity( (1+coefficientOfRestitution) * deltaP * inverseMass1);
        ball->applyVelocity( -(1+coefficientOfRestitution) * deltaP * inverseMass2);
    }
}


/**
    \todo implement this collisioncheck
*/
void Collision::checkCollision( Ball *ball, Plane *plane )
{

}


