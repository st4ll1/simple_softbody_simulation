#include "Softbody.h"

Softbody::~Softbody(void)
{
    int i = 0;
    int size = edges.size();
    //erase all edges and vertices 
    for(i = 0; i < size; ++i)
    {        
        delete edges[i];        
    }
    edges.clear();
    size = vertices.size();
    for(i = 0; i < size; ++i)
    {
        delete vertices[i];        
    }    
    edges.clear();
}

void Softbody::update(float dt)
{
    int i;
    int size = getAmountOfEdges();
    //edges
    for(i = size-1 ; i >= 0 ; --i)
    {
        edges[i]->update( dt );   
        if(edges[i]->isBroken())
        {
            delete edges[i];
            edges.erase(edges.begin()+i);
        }
    }
    //vertices  
    for(i = 0; i < SoftBodyPoint::amountOfPoints; ++i)
    {
        vertices[i]->applyForce( ci::Vec3f( 0.0f, -1.0f, 0.0f ) );
        vertices[i]->update( dt );    
    }
}

void Softbody::draw()
{
    int i;
    int size;
    //vertices
    if( drawVertices )
    {               
        size = getAmountOfVertices();
        for(i = 0; i < size; ++i)
        {
            vertices[i]->draw();            
        }
    }    
    //edges
    if( drawEdges )
    {
        size = getAmountOfEdges();
        for(i = 0; i < size; ++i)
        {
            edges[i]->draw();
        }
    }
}

bool Softbody::addVertex(SoftBodyPoint* p)
{
    if(p != nullptr)
    {
        vertices.push_back( p );
        return true;
    }
    return false;
}

bool Softbody::addEdge(SoftBodyPoint* p1, SoftBodyPoint* p2)
{
    if( p1 != nullptr && p2 != nullptr)
    {       
        edges.push_back( new SoftBodyEdge( p1, p2, 2.0f, edgeStrength ) );
        return true;
    }

    return false;
}

bool Softbody::addEdge(int p1, int p2)
{
    int size = vertices.size();
    if( p1 < size && p2 < size)
    {
        edges.push_back( new SoftBodyEdge( vertices[p1], vertices[p2], 2.0f, edgeStrength ) );
        return true;
    }
    return false;
}

int Softbody::getAmountOfEdges() const
{
    return edges.size();
}

int Softbody::getAmountOfVertices() const
{
    return vertices.size();
}

void Softbody::applyForceAtVertex( ci::Vec3f force, int index )
{
    vertices[index]->applyForce( force );
}