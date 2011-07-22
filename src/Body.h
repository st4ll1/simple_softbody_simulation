#pragma once
class Body
{
public:
    Body() : isAlive(true){};
    virtual ~Body(void){};
    
    virtual void update( float dt) = 0;
    virtual void draw() = 0;

    bool isAlive;
};

