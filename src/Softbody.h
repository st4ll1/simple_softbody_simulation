#pragma once

#include <vector>

//cinder includes
#include "cinder/Vector.h"
#include "cinder/Color.h"

//
#include "softBodyEdge.h"
#include "softBodyPoint.h"
#include "Body.h"

class Softbody : public Body
{
public:
    Softbody(void) : drawEdges(true), drawVertices(true), edgeStrength(30.0f){};
    ~Softbody(void);
    /** 
        \param dt delta time between to simulationsteps
    */
    virtual void update( float dt);
    virtual void draw();
    /**
        Function to add a vertex to the softbody
        \param p vertex to add
    */
    virtual bool addVertex( SoftBodyPoint* p );
    /**
        Function to add an edge to the softbody
        \param p1 startvertex of the edge
        \param p2 endvertex of the edgev
        \return whether the edges was added
    */
    virtual bool addEdge(SoftBodyPoint* p1, SoftBodyPoint* p2);
    /** 
        \see addEdge(SoftBodyPoint* p1, SoftBodyPoint* p2)
        \param p1 index of startvertex of the edge
        \param p2 index of endvertex of the edge
        \return whether the edges was added
    */
    virtual bool addEdge(int p1, int p2);

    virtual SoftBodyPoint* getVertex(int index) { return vertices[index];};

    
    bool drawVertices;
    bool drawEdges;

    float edgeStrength;
    float edgeDumpingFactor;

    int getAmountOfVertices() const;
    int getAmountOfEdges() const;

    void applyForceAtVertex( ci::Vec3f force, int index );

private:
    std::vector<SoftBodyPoint*> vertices;
    std::vector<SoftBodyEdge*> edges;   
       
};

