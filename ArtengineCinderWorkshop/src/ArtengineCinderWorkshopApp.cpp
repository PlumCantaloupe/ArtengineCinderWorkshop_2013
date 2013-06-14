#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "cinder/ImageIo.h"         //IO stuff
#include "cinder/gl/Texture.h"      //texture stuff
#include "cinder/TriMesh.h"         //3D stuff
#include "cinder/ObjLoader.h"
#include "cinder/gl/GlslProg.h"     //shader stuff
#include "cinder/MayaCamUI.h"       //Maya Cam

#include "Resources.h"

#include "Emitter.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ArtengineCinderWorkshopApp : public AppNative {
  public:
    
    void prepareSetting(Settings *settings);
	void setup();
	void mouseDown( MouseEvent event );
    void mouseDrag( MouseEvent event );
    void keyDown( KeyEvent event );
    void keyUp( KeyEvent event );
	void update();
	void draw();
    
    gl::Texture         mCowTex;
    gl::Texture			mGlowRedTex;
    TriMesh             mCowModel;
    gl::GlslProg        mBasicShader;
    MayaCamUI           mMayaCam;
    
    Emitter<Particle>   mParticles;
};

void ArtengineCinderWorkshopApp::prepareSetting(Settings *settings)
{
    settings->setFrameRate(30.0f);
    settings->setWindowSize(1024.0f, 768.0f);
}

void ArtengineCinderWorkshopApp::setup()
{
    //load model
    ObjLoader loader( (DataSourceRef)loadResource( RES_OBJ_COW ) );
	loader.load( &mCowModel );
    
    //load textures
    mCowTex         = gl::Texture( loadImage( loadResource( RES_TEX_COW ) ) );
    mGlowRedTex		= gl::Texture( loadImage( loadResource( RES_GLOW_RED_TEX ) ) );
    
    //load shader
    mBasicShader = gl::GlslProg( loadResource( RES_SHADER_VERT_BASIC ), loadResource( RES_SHADER_FRAG_BASIC ) );
    
    //set up camera
    CameraPersp initialCam;
	initialCam.setPerspective( 45.0f, getWindowAspectRatio(), 0.1, 10000 );
	mMayaCam.setCurrentCam( initialCam );
    
    //set up particles
    EmitterFormat format1;
	format1.emitterPosition = Vec3f( -1.2f, 1.4f, 0.0f );
    format1.particlePerlinType = EmitterFormat::PERLIN_TYPE_INDIVIDUAL;
    format1.particlesPerSecond = 0;
	format1.particleSize = 0.5f;
    format1.particleGravity = Vec3f(-0.0006f, 0.00f, 0.0f);
    format1.particleSpeed = 0.0004f;
    format1.particleLifespanSeconds = 1.0f;
    format1.particleTurbulance = 0.001f;
    mParticles.setup(format1);
    
    glEnable(GL_TEXTURE_2D);
//    gl::enableWireframe();
}

void ArtengineCinderWorkshopApp::mouseDown( MouseEvent event )
{
    if( event.isAltDown() ) {
		mMayaCam.mouseDown( event.getPos() );
    }
}

void ArtengineCinderWorkshopApp::mouseDrag( MouseEvent event )
{
    if( event.isAltDown() ) {
		mMayaCam.mouseDrag( event.getPos(), event.isLeftDown(), event.isMiddleDown(), event.isRightDown() );
    }
}

void ArtengineCinderWorkshopApp::keyDown( KeyEvent event )
{
    switch ( event.getCode() )
	{
        case KeyEvent::KEY_SPACE:
            mParticles.mFormat.particlesPerSecond = 100;
            break;
    };
}

void ArtengineCinderWorkshopApp::keyUp( KeyEvent event )
{
    switch ( event.getCode() )
	{
        case KeyEvent::KEY_SPACE:
            mParticles.mFormat.particlesPerSecond = 0;
            break;
    };
}

void ArtengineCinderWorkshopApp::update()
{
    mParticles.update();
}

void ArtengineCinderWorkshopApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0.0f, 0.1f, 0.2f ) );
    gl::setMatrices(mMayaCam.getCamera());
    
    gl::disableAlphaBlending();
    gl::enableDepthRead();
    
    mCowTex.bind(0);
    mBasicShader.bind();
    mBasicShader.uniform("tex0", 0);
    gl::draw(mCowModel);
//    gl::drawCube(Vec3f(0.0f,0.0f,0.0f), Vec3f(10.0f, 10.0f, 10.0f));
    mBasicShader.unbind();
    mCowTex.unbind(0);
    
    //particles
    gl::enableAdditiveBlending();
    gl::disableDepthRead();
    mGlowRedTex.bind();
    mParticles.render(mMayaCam.getCamera());
    mGlowRedTex.unbind();
}

CINDER_APP_NATIVE( ArtengineCinderWorkshopApp, RendererGl )
