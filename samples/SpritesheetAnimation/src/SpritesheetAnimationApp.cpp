#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"
#include "cinder/Timeline.h"

#include "poSpritesheet.h"
#include "poSpritesheetAnimation.h"

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
	po::SpritesheetAnimationRef mSpritesheetAnimation;
	
	Anim<Vec2f> mPos;
	Vec2f mEndPos;
};

void SpritesheetAnimationApp::setup()
{
	setWindowSize(1024, 768);
	
	mPos = Vec2f(1024, 0);
	mEndPos = Vec2f(0, 768);
	
	gl::TextureRef texture = gl::Texture::create(loadImage(loadAsset("charge.png")));
	JsonTree json = JsonTree(loadAsset("charge.json"));
	
	mSpritesheet = po::Spritesheet::create(texture, json);
	mSpritesheetAnimation = po::SpritesheetAnimation::create(mSpritesheet);
	mSpritesheetAnimation->setIsLoopingEnabled(true);
	mSpritesheetAnimation->play();
	
	timeline().apply(&mPos, mEndPos, 15.0f).loop();
}

void SpritesheetAnimationApp::mouseDown( MouseEvent event )
{
}

void SpritesheetAnimationApp::update()
{
	mSpritesheetAnimation->update();
}

void SpritesheetAnimationApp::draw()
{
	gl::clear(Color::gray(0.2));
	gl::pushModelView();
	Vec2f val = mPos.value();
	gl::translate(val.x - mSpritesheet->getOriginalBounds().getWidth()/2, val.y - mSpritesheet->getOriginalBounds().getHeight()/2);
	mSpritesheetAnimation->draw();
	gl::popModelView();
}

CINDER_APP_NATIVE( SpritesheetAnimationApp, RendererGl )
