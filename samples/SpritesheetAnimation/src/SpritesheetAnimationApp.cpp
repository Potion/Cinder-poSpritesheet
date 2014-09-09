#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

#include "poSpritesheet.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SpritesheetAnimationApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	
	po::SpritesheetRef mSpritesheet;
};

void SpritesheetAnimationApp::setup()
{
	setWindowSize(1920, 1080);
	
	ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("ducktest.png")));
	ci::JsonTree json = ci::JsonTree(ci::app::loadAsset("ducktest.json"));
	mSpritesheet = po::Spritesheet::create(texture, json);
	mSpritesheet->setIsLoopingEnabled(true);
	mSpritesheet->play();
}

void SpritesheetAnimationApp::mouseDown( MouseEvent event )
{
}

void SpritesheetAnimationApp::update()
{
	mSpritesheet->update();
}

void SpritesheetAnimationApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
	mSpritesheet->draw();
}

CINDER_APP_NATIVE( SpritesheetAnimationApp, RendererGl )
