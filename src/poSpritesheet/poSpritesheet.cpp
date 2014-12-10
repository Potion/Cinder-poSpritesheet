//
//  Spritesheet.cpp
//  
//
//  Created by bruce on 8/26/14.
//
//

#include "poSpritesheet.h"

namespace po {
	
	//------------------------------------------
	//	Single texture spritesheet setup
	//------------------------------------------
	SpritesheetRef Spritesheet::create(ci::gl::TextureRef texture, ci::JsonTree json)
	{
		SpritesheetRef ref(new Spritesheet());
		ref->setup(texture, json);
		return ref;
	}
	
	void Spritesheet::setup(ci::gl::TextureRef texture, ci::JsonTree json)
	{
		mTextures[0] = texture;
		setupSpriteMap(0, json);
	}
	
	//------------------------------------------
	//	Multipacked spritesheet setup
	//------------------------------------------
	void Spritesheet::setup(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> data)
	{
		int counter = 0;
		for (auto json : data) {
			int textureID = counter;
			mTextures[textureID] = textures[counter];
			setupSpriteMap(textureID, json);
			counter++;
		}
	}
	
	SpritesheetRef Spritesheet::create(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> &data)
	{
		SpritesheetRef ref(new Spritesheet());
		ref->setup(textures, data);
		return ref;
	}
	
	//------------------------------------------
	//	Spritesheet
	//------------------------------------------
	
	Spritesheet::Spritesheet()
	: mIsDrawOriginalBounds(false)
	, mIsDrawFrameBounds(false)
	, mCurrentFrameKey("")
	{}
	
	Spritesheet::~Spritesheet()
	{}
	
	//
	//	Setup frame data and texture maps
	//
	void Spritesheet::setupSpriteMap(int textureID, ci::JsonTree json)
	{
		// get all the frames in the json
		for (auto frame : json.getChild("frames")) {
			FrameData frameData = getFrameData(frame);
			std::string frameKey = frameData.filename;
			mFrameData[frameKey] = frameData;
			mTextureIDs[frameKey] = textureID;
			mFrameOrder.push_back(frameKey);
		}
		
		// sort the frame order alphabetically
		std::sort(mFrameOrder.begin(), mFrameOrder.end());
		mNumFrames = mFrameOrder.size();
		mCurrentFrameKey = mFrameOrder[0];
	}
	
	//
	//	Create framedata object from json
	//
	Spritesheet::FrameData Spritesheet::getFrameData(ci::JsonTree json)
	{
		FrameData frameData = FrameData();
		frameData.filename = json.getChild("filename").getValue<std::string>();
		frameData.frame = ci::Area(
			json.getChild("frame").getChild("x").getValue<float>(),
			json.getChild("frame").getChild("y").getValue<float>(),
			json.getChild("frame").getChild("x").getValue<float>() +
			json.getChild("frame").getChild("w").getValue<float>(),
			json.getChild("frame").getChild("y").getValue<float>() +
			json.getChild("frame").getChild("h").getValue<float>()
		);
		frameData.rotated = json.getChild("rotated").getValue<bool>();
		frameData.trimmed = json.getChild("trimmed").getValue<bool>();
		frameData.spriteSourceSize = ci::Rectf(
			json.getChild("spriteSourceSize").getChild("x").getValue<float>(),
			json.getChild("spriteSourceSize").getChild("y").getValue<float>(),
			json.getChild("spriteSourceSize").getChild("x").getValue<float>() +
			json.getChild("spriteSourceSize").getChild("w").getValue<float>(),
			json.getChild("spriteSourceSize").getChild("y").getValue<float>() +
			json.getChild("spriteSourceSize").getChild("h").getValue<float>()
		);
		frameData.sourceSize = ci::Vec2f(
			json.getChild("sourceSize").getChild("w").getValue<float>(),
			json.getChild("sourceSize").getChild("h").getValue<float>()
		);
		
		return frameData;
	}
	
	//
	//	Draw bounds for the original size or the current frame size
	//
	void Spritesheet::drawBounds()
	{
		ci::gl::color(ci::Color(1,0,0));
		if (mIsDrawOriginalBounds) ci::gl::drawStrokedRect(getOriginalBounds());
		if (mIsDrawFrameBounds) ci::gl::drawStrokedRect(getFrameBounds());
	}
	
	//
	//	Draw the texture for the current frame
	//
	void Spritesheet::drawFrame(int frameNum)
	{
		ci::gl::pushMatrices();
		
		ci::gl::enableAlphaBlending();
		
		mCurrentFrameKey = mFrameOrder[frameNum];
		ci::gl::draw(mTextures[mTextureIDs[mCurrentFrameKey]], mFrameData[mCurrentFrameKey].frame, mFrameData[mCurrentFrameKey].spriteSourceSize);
		
		ci::gl::disableAlphaBlending();
		ci::gl::popMatrices();
	}
	
	//
	//	Get the original bounds for the frame
	//
	ci::Rectf Spritesheet::getOriginalBounds()
	{
		ci::Rectf bounds(0, 0, mFrameData[mCurrentFrameKey].sourceSize.x, mFrameData[mCurrentFrameKey].sourceSize.y);
		return bounds;
	}
	
	//
	//	Get the frame bounds for the current frame
	//
	ci::Rectf Spritesheet::getFrameBounds()
	{
		return mFrameData[mCurrentFrameKey].spriteSourceSize;
	}
	
}