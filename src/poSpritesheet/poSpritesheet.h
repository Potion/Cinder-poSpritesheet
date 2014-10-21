 //
//  Spritesheet.h
//  
//
//  Created by bruce on 8/26/14.
//
//

#pragma once

#include "cinder/gl/Texture.h"
#include "cinder/Json.h"
#include "cinder/Rect.h"
#include "cinder/Area.h"

namespace po {
	
	class Spritesheet;
	typedef std::shared_ptr<Spritesheet> SpritesheetRef;
	
	class Spritesheet
	{
	public:
		//	Single texture spritesheet
		static SpritesheetRef create(ci::gl::TextureRef texture, ci::JsonTree json);
		//	Multipacked texture spritesheet
		static SpritesheetRef create(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> &data);
		
		~Spritesheet();

		ci::Rectf getOriginalBounds();
		ci::Rectf getFrameBounds();
		void drawOriginalBounds(bool isDrawOriginalBounds) { mIsDrawOriginalBounds = isDrawOriginalBounds; }
		void drawFrameBounds(bool isDrawFrameBounds) { mIsDrawFrameBounds = isDrawFrameBounds; }
		
		int getNumFrames() { return mNumFrames; }
		void drawFrame(int frameNum);
		
	protected:
		Spritesheet();
		
		void setup(ci::gl::TextureRef texture, ci::JsonTree json);
		void setup(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> data);
		
	private:
		
		// Frame data structure
		struct FrameData {
			FrameData(){};
			
			std::string filename;
			ci::Area frame;
			bool rotated;
			bool trimmed;
			ci::Rectf spriteSourceSize;
			ci::Vec2f sourceSize;
		};
		
		//
		//	Frame data map
		//	key: sprite filename, value: frame data object
		//
		std::map<std::string, FrameData> mFrameData;
		
		//
		//	Texture ids
		//	key: sprite filename, value: texture id
		//
		std::map<std::string, int> mTextureIDs;
		
		//
		//	Texture map
		//	key: texture id, value: texture ref
		//
		std::map<int, ci::gl::TextureRef> mTextures;
		
		//
		//	store the order frames should appear in
		//	this is sorted alphabetically
		//
		std::vector<std::string> mFrameOrder;
		
		int mNumFrames;
		std::string mCurrentFrameKey; // keep track of the current frame key

		bool mIsDrawOriginalBounds, mIsDrawFrameBounds;
		
		FrameData getFrameData(ci::JsonTree json);
		void setupSpriteMap(int textureID, ci::JsonTree json);
		void drawBounds();
		
		
	};
	
}