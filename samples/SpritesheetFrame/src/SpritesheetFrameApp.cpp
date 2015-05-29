#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "poSpritesheet.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SpritesheetFrameApp : public AppNative {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void mouseMove( MouseEvent event );
	void update();
	void draw();
	
	po::SpritesheetRef mSpritesheet;
	int mCurrentFrame;
};

void SpritesheetFrameApp::setup()
{
	setWindowSize(1024, 768);
	
	mCurrentFrame = 0;
	
	gl::TextureRef texture = gl::Texture::create(loadImage(loadAsset("goblin.png")));
	JsonTree json = JsonTree(loadAsset("goblin.json"));
	
	mSpritesheet = po::Spritesheet::create(texture, json);
}

void SpritesheetFrameApp::mouseDown( MouseEvent event )
{
}

void SpritesheetFrameApp::mouseMove(cinder::app::MouseEvent event)
{
	float posMap = lmap<float>(event.getPos().x, 0, getWindowWidth(), 0, mSpritesheet->getNumFrames() - 1);
	int frame = floorf(posMap);
	if (frame < 0) frame = 0;
	if (frame >= mSpritesheet->getNumFrames() - 1) frame = mSpritesheet->getNumFrames() - 1;
	mCurrentFrame = frame;
}

void SpritesheetFrameApp::update()
{
}

void SpritesheetFrameApp::draw()
{
	gl::clear(Color::gray(0.2));
	gl::pushModelView();
	gl::translate(getWindowWidth()/2 - mSpritesheet->getOriginalBounds().getWidth()/2, getWindowHeight()/2 - mSpritesheet->getOriginalBounds().getHeight()/2);
	mSpritesheet->drawFrame(mCurrentFrame);
	gl::popModelView();
}

CINDER_APP_NATIVE( SpritesheetFrameApp, RendererGl )
