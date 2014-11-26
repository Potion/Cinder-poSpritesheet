#include "cinder/app/AppNative.h"
#include "cinder/gl/gl.h"

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
	bool mIsReverse;
	void onAnimationFinished(po::SpritesheetAnimationRef animation);
};

void SpritesheetAnimationApp::setup()
{
	setWindowSize(1920, 1080);
	
//	ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("ducktest.png")));
//	ci::JsonTree json = ci::JsonTree(ci::app::loadAsset("ducktest.json"));
//	mSpritesheet = po::Spritesheet::create(texture, json);
//	
//	mSpritesheetAnimation = po::SpritesheetAnimation::create(mSpritesheet);
//	mSpritesheetAnimation->setIsLoopingEnabled(true);
//	mSpritesheetAnimation->play();
	
	mIsReverse = false;
	
	ci::gl::TextureRef texture = ci::gl::Texture::create(ci::loadImage(ci::app::loadAsset("grow_texture_0.png")));
	ci::JsonTree json = ci::JsonTree(ci::app::loadAsset("grow_data_0.json"));
	mSpritesheet = po::Spritesheet::create(texture, json);
	
	mSpritesheetAnimation = po::SpritesheetAnimation::create(mSpritesheet);
	mSpritesheetAnimation->play();
	mSpritesheetAnimation->getSignalPlayingComplete().connect(std::bind(&SpritesheetAnimationApp::onAnimationFinished, this, std::placeholders::_1));
}

void SpritesheetAnimationApp::onAnimationFinished(po::SpritesheetAnimationRef animation)
{
	mIsReverse = !mIsReverse;
	animation->setIsReverse(mIsReverse);
	animation->play();
}

void SpritesheetAnimationApp::mouseDown( MouseEvent event )
{
}

void SpritesheetAnimationApp::update()
{
//	mSpritesheet->update();
	mSpritesheetAnimation->update();
}

void SpritesheetAnimationApp::draw()
{
	// clear out the window with black
	gl::clear( Color( 0, 0, 0 ) );
	
//	mSpritesheet->draw();
	mSpritesheetAnimation->draw();
}

CINDER_APP_NATIVE( SpritesheetAnimationApp, RendererGl )
