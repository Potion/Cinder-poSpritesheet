/*
 Copyright (c) 2015, Potion Design LLC
 All rights reserved.
 
 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 
 * Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.
 
 * Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following disclaimer in the documentation
 and/or other materials provided with the distribution.
 
 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#pragma once

#include <map>
#include "cinder/gl/Texture.h"
#include "cinder/Json.h"
#include "cinder/Xml.h"
#include "cinder/Rect.h"
#include "cinder/Area.h"

namespace po {
	
	class Spritesheet;
	typedef std::shared_ptr<Spritesheet> SpritesheetRef;
	
	class Spritesheet
	{
	public:
		//	Single texture spritesheet with json or xml
        static SpritesheetRef create(ci::gl::TextureRef texture, ci::JsonTree json);
        static SpritesheetRef create(ci::gl::TextureRef texture, ci::XmlTree xml);
        
		//	Multipacked texture spritesheet with json or xml
        static SpritesheetRef create(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> &data);
        static SpritesheetRef create(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::XmlTree> &data);
        
		~Spritesheet();
		
		//	Source size
		ci::Rectf getOriginalBounds();
		
		//	Sprite source size
		ci::Rectf getFrameBounds();
		
		//	Draw frame bounds
		void drawOriginalBounds(bool isDrawOriginalBounds) { mIsDrawOriginalBounds = isDrawOriginalBounds; }
		void drawFrameBounds(bool isDrawFrameBounds) { mIsDrawFrameBounds = isDrawFrameBounds; }
		
		//	Number of frames in the spritesheet
		int getNumFrames() { return mNumFrames; }
		
		//	Draw frame number
		void drawFrame(int frameNum);
		
		//	Draw frame name
		void drawFrame(std::string frameName);
		
	protected:
		Spritesheet();
		
        //  Json
		void setup(ci::gl::TextureRef texture, ci::JsonTree json);
        void setup(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::JsonTree> data);
        
        //  XML
        void setup(ci::gl::TextureRef texture, ci::XmlTree json);
        void setup(std::vector<ci::gl::TextureRef> &textures, std::vector<ci::XmlTree> data);
		
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

		bool mIsDrawOriginalBounds;
		bool mIsDrawFrameBounds;
        
        void setupSpriteMap(int textureID, ci::JsonTree json);
        FrameData getFrameData(ci::JsonTree json);
        
        void setupSpriteMap(int textureID, ci::XmlTree xml);
        FrameData getFrameData(ci::XmlTree xml);
        
		void drawBounds();
		
	};
	
}