//
//  Spritesheet.cpp
//  
//
//  Created by bruce on 8/26/14.
//
//

#include "Spritesheet.h"

SpritesheetRef Spritesheet::create(ci::gl::TextureRef texture, ci::JsonTree json)
{
	SpritesheetRef ref(new Spritesheet());
	ref->setup(texture, json);
	return ref;
}

Spritesheet::Spritesheet()
: mCurrentFrame(0)
, mIsPlaying(false)
, mIsLooping(false)
, mFPS(24.f)
, mPreviousTime(0.f)
, mCurrentTime(0.f)
, mIsDrawOriginalBounds(false)
, mIsDrawFrameBounds(false)
{}

Spritesheet::~Spritesheet()
{}

void Spritesheet::setup(ci::gl::TextureRef texture, ci::JsonTree json)
{
	setFrameRate(mFPS);
	
	mTexture = texture;
	parseJSON(json);
}

void Spritesheet::update()
{
	mCurrentTime = ci::app::getElapsedSeconds() * 1000;
	float timeDiff = mCurrentTime - mPreviousTime;
	if (timeDiff > mFrameRate) {
		mPreviousTime = mCurrentTime;
		nextFrame();
	}
}

void Spritesheet::draw()
{
	ci::gl::pushMatrices();
	drawBounds();
	drawFrame();
	ci::gl::popMatrices();
}

//
//	Parse the frame data json and store in data structures
//

void Spritesheet::parseJSON(ci::JsonTree json)
{
//	ci::app::console() << json << std::endl;
	
	ci::JsonTree metadata = json.getChild("meta");
	ci::JsonTree framesJSON = json.getChild("frames");
	
	mNumFrames = framesJSON.getChildren().size();
	
	//	parse metadata
//	ci::app::console() << metadata << std::endl;
	
	mMetadata.image = metadata.getChild("image").getValue<std::string>();
	mMetadata.size = ci::Vec2f(metadata.getChild("size").getChild("w").getValue<int>(), metadata.getChild("size").getChild("h").getValue<int>());
	mMetadata.scale = metadata.getChild("scale").getValue<float>();
	
//	ci::app::console() << mMetadata.image << ", " << mMetadata.size << ", " << mMetadata.scale << std::endl;
	
	//	parse frame data
	for (ci::JsonTree::Iter frameJSON = framesJSON.begin(); frameJSON != framesJSON.end(); ++frameJSON) {
		ci::JsonTree thisFrameJSON = *frameJSON;
//		ci::app::console() << thisFrameJSON << std::endl;
		
		FrameData frameData = FrameData();
		frameData.filename = thisFrameJSON.getChild("filename").getValue<std::string>();
		frameData.frame = ci::Area(
			thisFrameJSON.getChild("frame").getChild("x").getValue<float>(),
			thisFrameJSON.getChild("frame").getChild("y").getValue<float>(),
			thisFrameJSON.getChild("frame").getChild("x").getValue<float>() + thisFrameJSON.getChild("frame").getChild("w").getValue<float>(),
			thisFrameJSON.getChild("frame").getChild("y").getValue<float>() + thisFrameJSON.getChild("frame").getChild("h").getValue<float>()
		);
		frameData.rotated = thisFrameJSON.getChild("rotated").getValue<bool>();
		frameData.trimmed = thisFrameJSON.getChild("trimmed").getValue<bool>();
		frameData.spriteSourceSize = ci::Rectf(
			thisFrameJSON.getChild("spriteSourceSize").getChild("x").getValue<float>(),
			thisFrameJSON.getChild("spriteSourceSize").getChild("y").getValue<float>(),
			thisFrameJSON.getChild("spriteSourceSize").getChild("x").getValue<float>() +
			thisFrameJSON.getChild("spriteSourceSize").getChild("w").getValue<float>(),
			thisFrameJSON.getChild("spriteSourceSize").getChild("y").getValue<float>() +
			thisFrameJSON.getChild("spriteSourceSize").getChild("h").getValue<float>()
		);
		frameData.sourceSize = ci::Vec2f(
			thisFrameJSON.getChild("sourceSize").getChild("w").getValue<float>(),
			thisFrameJSON.getChild("sourceSize").getChild("h").getValue<float>()
		);
		mFrames.push_back(frameData);
		
		ci::app::console() << frameData.filename << ", " << frameData.frame <<  ", " << frameData.spriteSourceSize << ", " << frameData.sourceSize << std::endl;
	}
	
}

//
//	Proceed to the next frame
//	If it's not looping send a complete signal
//

void Spritesheet::nextFrame()
{
	if (mIsPlaying) {
		mCurrentFrame = (mCurrentFrame += 1) % mNumFrames;
		if (!mIsLooping) {
			if (mCurrentFrame == mNumFrames - 1) {
				mIsPlaying = false;
				mPlayCompleteSignal(shared_from_this());
			}
		}
	}
}

//
//	Draw bounds for the original size or the current frame size
//

void Spritesheet::drawBounds()
{
	ci::gl::color(ci::Color(1,0,0));
	
	if (mIsDrawOriginalBounds) {
		ci::gl::drawStrokedRect(getOriginalBounds());
	}
	
	if (mIsDrawFrameBounds) {
		ci::gl::drawStrokedRect(getFrameBounds());
	}
	
	ci::gl::color(ci::Color(1,1,1));
}

//
//	Draw the texture for the current frame
//

void Spritesheet::drawFrame()
{
	FrameData frame = mFrames[mCurrentFrame];
	ci::gl::enableAlphaBlending();
	ci::gl::draw(mTexture, frame.frame, frame.spriteSourceSize);
	ci::gl::disableAlphaBlending();
}

//
//	Get the original bounds for the frame
//

ci::Rectf Spritesheet::getOriginalBounds()
{
	ci::Rectf bounds(0, 0, mFrames[mCurrentFrame].sourceSize.x, mFrames[mCurrentFrame].sourceSize.y);
	return bounds;
}

//
//	Get the frame bounds for the current frame
//

ci::Rectf Spritesheet::getFrameBounds()
{
	return mFrames[mCurrentFrame].spriteSourceSize;
}

//
//	Set frames/second
void Spritesheet::setFrameRate(float frameRate)
{
	mFPS = frameRate;
	mFrameRate = 1000.f / mFPS;
}