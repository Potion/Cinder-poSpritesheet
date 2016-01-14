#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include "poSpritesheet.h"
#include "cinder/app/RendererGl.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class SpritesheetFrameApp : public App {
  public:
	void setup();
	void mouseDown( MouseEvent event );
	void mouseMove( MouseEvent event );
	void update();
	void draw();
	
	po::SpritesheetRef mSpritesheet;
	int mCurrentFrame;
	bool mUseFrameName;
	std::string mFrameName;
};

void SpritesheetFrameApp::setup()
{
	setWindowSize(1024, 768);
	
	mUseFrameName = false;
	mCurrentFrame = 0;
	mFrameName = "0031.png";
	
	gl::TextureRef texture = gl::Texture::create(loadImage(loadAsset("goblin.png")));
	JsonTree json = JsonTree(loadAsset("goblin.json"));
	
	mSpritesheet = po::Spritesheet::create(texture, json);
}

void SpritesheetFrameApp::mouseDown( MouseEvent event )
{
	mUseFrameName = !mUseFrameName;
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
	
	if (mUseFrameName) {
		mSpritesheet->drawFrame(mFrameName);
	} else {
		mSpritesheet->drawFrame(mCurrentFrame);
	}
	
	gl::popModelView();
}

CINDER_APP( SpritesheetFrameApp, RendererGl )
