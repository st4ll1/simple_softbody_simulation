#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/MayaCamUI.h"
#include "cinder/params/Params.h"
#include "cinder/gl/GlslProg.h"

#include "SimpleGUI.h"
#include <vector>

#include "Body.h"
#include "Softbody.h"
#include "softBodyPoint.h"
#include "softBodyEdge.h"
#include "RigidBody.h"
#include "Plane.h"
#include "Ball.h"

#include "Collisions.h"

#define COLUMN_WIDTH                            150

#define SOFTBODY_WIDTH                          6
#define SOFTBODY_HEIGHT                         15
#define SOFTBODY_DEPTH                          6

#define MAX_SHOOTSPEED                          50.0f
#define TIME_IN_SECONDS_TO_GET_FULL_SHOOTSPEED  3


using namespace ci; 
using namespace ci::app;
using namespace std;

class SoftBodyApp : public AppBasic {

private:       
    Plane* groundPlane;
    vector<Ball*> bodies;
    
    //softbody
    Softbody* softbody;    
    int width, height, depth;

    //GUI    
    bool rotatingCamera;
    std::string framerate;
    std::string deltatime;
    int amountOfVertices;
    int amountOfEdges;
    float forceFactor;
    ci::Vec3f force;
    int vertexIndex;
    ColorA guiBackgroundColor;   

    //SimpleGUI
    mowa::sgui::SimpleGUI* gui;
    mowa::sgui::ButtonControl* forceApplyButton;
    mowa::sgui::LabelControl *framerateLabel, *deltatimeLabel;
    mowa::sgui::PanelControl *forceApplyPanel, *mainPanel, *viewPanel, *infoPanel;
    mowa::sgui::IntVarControl *vertexIndexControl;
    mowa::sgui::ColumnControl *infoColumn;    

    //shoot
    float shootSpeed;
    ci::Vec3f shootDirection;
    bool isAiming;

    CameraPersp cam;
    MayaCamUI	mayaCam;
    Vec2i		  mousePos;

    float time;    
    ci::Vec2i windowSize;

public:
    void prepareSettings( Settings *settings );
    void setup();	  
    void update();

    void draw();
    void drawGrid(float size=100.0f, float step=10.0f);

    void mouseMove( MouseEvent event );
    void mouseDown( MouseEvent event );
    void mouseUp( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void mouseWheel( MouseEvent event );
    void resize( ResizeEvent event );
    void createScene();
    void destroyScene();
    void resetScene();

    void GuiSetup();

    void createSoftbody(int width, int depth, int height);

    bool applyForce( MouseEvent event );

    void spawnSphere(ci::Vec3f position, ci::Vec3f velocity);
    void keyDown( KeyEvent event );
};

void SoftBodyApp::prepareSettings( Settings *settings )
{
    settings->setWindowSize( 800, 600 );
    settings->setFrameRate( 60.0f );
    settings->setResizable( false );
    settings->setFullScreen( false );
}

void SoftBodyApp::createScene()
{
    rotatingCamera = false;    
    groundPlane = new Plane(ci::Vec3f::zero(), ci::Vec3f(200.0f, 0.0f, 200.0f));  
    groundPlane->setMass(0.0f);
    //

    createSoftbody(SOFTBODY_WIDTH, SOFTBODY_DEPTH, SOFTBODY_HEIGHT);    
}

void SoftBodyApp::createSoftbody(int width, int depth, int height)
{
    softbody = new Softbody();
    SoftBodyPoint** points;

    points = new SoftBodyPoint*[width*height*depth];
    for(int k = 0; k < height; k++)
    {
        for(int j = 0; j < depth; j++)
        {
            for(int i = 0; i < width; i++)
            {
                points[k*width*depth+j*width+i] = new SoftBodyPoint( (2 * Vec3f(i-width/2,k,j-depth/2)) + Vec3f( 0.0f, 10.0f, 0.0f ) );               
                softbody->addVertex( points[k*width*depth+j*width+i] );
            }
        }
    }

    for(int j = 0; j < SoftBodyPoint::amountOfPoints-1; j++){
        for(int i = j+1; i< SoftBodyPoint::amountOfPoints; i++){
            if((points[i]->getPosition() - points[j]->getPosition()).length() <= 3.9f)
                softbody->addEdge( points[j], points[i] );
        }
    }

    amountOfEdges = softbody->getAmountOfEdges();
    amountOfVertices = softbody->getAmountOfVertices();
    
    //bodies.push_back(softbody);    
}

void SoftBodyApp::destroyScene()
{
    delete groundPlane;    
    bodies.clear();
}

void SoftBodyApp::resetScene()
{
    rotatingCamera = false;
    destroyScene();
    createScene();    
}

void SoftBodyApp::GuiSetup()
{
    //simpleGUI setup
    gui = new mowa::sgui::SimpleGUI(this);   

    //apply Force at Menu    
    gui->addColumn( windowSize.x - COLUMN_WIDTH - 10 , 10);
    forceApplyPanel =  gui->addPanel();
    gui->addLabel("Press 'Space' to apply Force");
    gui->addSeparator();
    gui->addParam("Force x (-1.0 to 1.0)", &force.x, -1.0f, 1.0f, 0.0f);
    gui->addParam("Force y (-1.0 to 1.0)", &force.y, -1.0f, 1.0f, 0.0f);
    gui->addParam("Force z (-1.0 to 1.0)", &force.z, -1.0f, 1.0f, 0.0f);
    gui->addSeparator();
    gui->addParam("Forcestrengh", &forceFactor, 100.0f, 1000.0f, 200.0f);
    vertexIndexControl = gui->addParam("Vertex Index: ", &vertexIndex, 0, amountOfVertices-1, amountOfVertices/2);

    //controls
    gui->addColumn(10, 400);
    infoPanel = gui->addPanel();
    gui->addLabel("Press 'ESC' to quit");
    gui->addLabel("Click 'Right' to shoot");

    //infos
    gui->addLabel("Amount of Vertices: " + ci::toString(amountOfVertices));
    gui->addLabel("Amount of Edges: " + ci::toString(amountOfEdges));
    framerateLabel = gui->addLabel("Framerate: ");
    deltatimeLabel = gui->addLabel("Delta Time: ");

    //viewMenu    
    gui->addColumn(10,10);
    gui->addParam("Draw Edges", &(softbody->drawEdges), true);
    gui->addParam("Draw Vertices", &(softbody->drawVertices), true);
    gui->addParam("RotatingCamera", &rotatingCamera, false);
    gui->addParam("Force Menu", &(forceApplyPanel->enabled), true);
    gui->addParam("Info & Stats", &(infoPanel->enabled), true);
    //gui->addParam("Gui Backgroundcolor", &guiBackgroundColor, ci::ColorA(0.5f, 0.3f, 0.3f, 0.5f));

    //shootbar
    gui->addColumn(200,10);
    gui->addParam("Shoot Speed", &shootSpeed, 0.0f, MAX_SHOOTSPEED, 0.0f);

    //gui settings
    gui->bgColor = guiBackgroundColor;   
    gui->sliderSize = ci::Vec2f(COLUMN_WIDTH, 10);
    gui->labelSize = ci::Vec2f(COLUMN_WIDTH, 10);
    gui->separatorSize = ci::Vec2f(COLUMN_WIDTH, 5);

}

void SoftBodyApp::setup()
{ 
    windowSize = ci::Vec2f(getWindowWidth(), getWindowHeight());
    createScene();

    forceFactor = 1.0f;
    force = Vec3f(1.0f, 0.0f, 0.0f);
    vertexIndex = 0;
    isAiming = false;

    guiBackgroundColor = ci::ColorA(0.0f, 0.0f, 0.0f, 1.0f);

    cam.setEyePoint( Vec3f(5.0f, 10.0f, 10.0f) );
    cam.setCenterOfInterestPoint( Vec3f(0.0f, 2.5f, 0.0f) );
    cam.setPerspective( 60.0f, getWindowAspectRatio(), 1.0f, 1000.0f );
    //mayaCam.setCurrentCam( cam );
   
    GuiSetup();                        
}


void SoftBodyApp::update()
{    
    if(vertexIndex < 0) vertexIndex = 0;
    if(vertexIndex >= amountOfVertices) vertexIndex = amountOfVertices - 1;
    float t = getElapsedSeconds();
    float dt = (t - time);
    time = t;
    if(dt > 0.05f) 
        dt = 0.05f;

    if(isAiming && shootSpeed < MAX_SHOOTSPEED)
        shootSpeed += dt * (MAX_SHOOTSPEED / TIME_IN_SECONDS_TO_GET_FULL_SHOOTSPEED);

    cam.lookAt(
        ( rotatingCamera 
            ? Vec3f( sin(time / 2 ) * 50.0f , 20.0f , cos( time / 2) * 50.0f ) 
            : Vec3f( 50.0f, 20.0f, 50.0f )
        ),
        Vec3f( 0.0f, 0.0f, 0.0f ),
        Vec3f( 0.0f, 1.0f, 0.0f ) );  
    //*/   

    //update
    softbody->update( dt );
    //*/
    
    for(int i(bodies.size()-1); i >= 0; --i)
    {
        bodies[i]->update( dt );
        if(!bodies[i]->isAlive) 
        {
            delete bodies[i];            
            bodies.erase(bodies.begin()+i);
        }
    }


    for(int i(bodies.size()-1); i>=1; --i)
    {
        Collision::checkCollision(softbody, bodies[i]);
        for(int j(i-1); j>=0; --j)
        {
            Collision::checkCollision(bodies[i], bodies[j]);
        }
    }
    if(bodies.size()>0) Collision::checkCollision(softbody, bodies[0]);    
    //Collision::checkCollision(groundPlane, softbody);

    //update the gui
    framerateLabel->setText("Framerate: " + ci::toString(App::getAverageFps()));
    //deltatimeLabel->setText("Delta Time: " + ci::toString( dt ) );
    deltatimeLabel->setText("Delta Time: " + ci::toString( dt ) );
    gui->bgColor = guiBackgroundColor;   

    vertexIndexControl->name = "Vertex Index: " + ci::toString( vertexIndex );    
}

void SoftBodyApp::spawnSphere(ci::Vec3f position, ci::Vec3f velocity)
{
    Ball* ball = new Ball( 4.0f, position, 10.0f, 3.0f );
    ball->applyForce( velocity );
    bodies.push_back(ball);
}

void SoftBodyApp::keyDown( KeyEvent event )
{
    if( event.getChar() == 'n' ) resetScene();
    switch ( event.getChar())
    {
    case 'n': 
        resetScene();
        break;
    case KeyEvent::KEY_SPACE:
        softbody->applyForceAtVertex( forceFactor * force, vertexIndex );
        break;
    case KeyEvent::KEY_ESCAPE:
        quit();
        break;
    }    
}

bool SoftBodyApp::applyForce( MouseEvent event )
{
    softbody->applyForceAtVertex( forceFactor * force, vertexIndex );
    return true;
}

void SoftBodyApp::draw()
{
    // clear out the window with black
    gl::clear( Color( 0, 0, 0 ) );

    //Camera
    gl::pushMatrices();
    //gl::setMatrices( mayaCam.getCamera() );
    gl::setMatrices( cam );  
    // draw the grid on the floor
    drawGrid();
    groundPlane->draw();
   
    //Softbody
    softbody->draw();

    int size = bodies.size();
    for(int i(0); i < size; ++i)
    {
        bodies[i]->draw();
    }

    gl::popMatrices();

    //draw gui    
    gui->draw();
}

void SoftBodyApp::drawGrid(float size, float step)
{
    gl::color( Colorf(0.2f, 0.2f, 0.2f) );
    for(float i=-size;i<=size;i+=step) {
        gl::drawLine( Vec3f(i, 0.0f, -size), Vec3f(i, 0.0f, size) );
        gl::drawLine( Vec3f(-size, 0.0f, i), Vec3f(size, 0.0f, i) );
    }
    gl::color( Colorf(1.0f, 1.0f, 1.0f) );
}


void SoftBodyApp::mouseMove( MouseEvent event )
{
    // keep track of the mouse
    mousePos = event.getPos();
}

void SoftBodyApp::mouseWheel( MouseEvent event )
{

}

void SoftBodyApp::mouseDown( MouseEvent event )
{	
    if(event.isLeft())
    {
        isAiming = true;
        shootSpeed = 1.0f;
    }    
}

void SoftBodyApp::mouseUp( MouseEvent event)
{
    if(event.isLeft())
    {    
        isAiming = false;
        float u = mousePos.x / (float) getWindowWidth();
        float v = mousePos.y / (float) getWindowHeight();
        shootDirection = cam.generateRay(u , 1.0f - v, cam.getAspectRatio() ).getDirection();
            //cam.getViewDirection();// + cam.getWorldUp() * up + cam.getViewDirection().cross(cam.getWorldUp()) * right;
        spawnSphere( cam.getEyePoint() + shootDirection * 10, shootDirection * shootSpeed * 100 ); 
        shootSpeed = 0.0f;
    }
}

void SoftBodyApp::mouseDrag( MouseEvent event )
{
    // keep track of the mouse
    mousePos = event.getPos();
    // let the camera handle the interaction  
    mayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
}

void SoftBodyApp::resize( ResizeEvent event )
{
    // adjust aspect ratio
    CameraPersp cam = mayaCam.getCamera();
    cam.setAspectRatio( getWindowAspectRatio() );
    mayaCam.setCurrentCam( cam );
    windowSize = ci::Vec2f(getWindowWidth(), getWindowHeight());
}

CINDER_APP_BASIC( SoftBodyApp, RendererGl )
