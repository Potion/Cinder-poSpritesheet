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

namespace po {
	
	class Spritesheet;
	typedef std::shared_ptr<Spritesheet> SpritesheetRef;
	
	class Spritesheet
	: public std::enable_shared_from_this<Spritesheet>
	{
	public:
		static SpritesheetRef create(ci::gl::TextureRef texture, ci::JsonTree json);
		~Spritesheet();
		
		typedef boost::signals2::signal<void(SpritesheetRef)> SignalSpritesheetPlayComplete;
		
		virtual void update();
		virtual void draw();
		void play() { mIsPlaying = true; }
		void pause() { mIsPlaying = false; }
		void setIsLoopingEnabled(bool isLooping) { mIsLooping = isLooping; }
		void setFrameRate(float frameRate);
		ci::Rectf getOriginalBounds();
		ci::Rectf getFrameBounds();
		SignalSpritesheetPlayComplete &getSignalPlayingComplete() { return mPlayCompleteSignal; }
		int getCurrentFrame() { return mCurrentFrame; }
		void drawOriginalBounds(bool isDrawOriginalBounds) { mIsDrawOriginalBounds = isDrawOriginalBounds; }
		void drawFrameBounds(bool isDrawFrameBounds) { mIsDrawFrameBounds = isDrawFrameBounds; }
		
	protected:
		Spritesheet();
		
		void setup(ci::gl::TextureRef texture, ci::JsonTree json);
		
	private:
		
		// Sprite metadata
		struct MetaData {
			MetaData(){};
			
			std::string image;
			ci::Vec2f size;
			float scale;
		};
		
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
		
		ci::gl::TextureRef mTexture;
		MetaData mMetadata;
		std::vector<FrameData> mFrames;
		
		int mCurrentFrame;
		int mNumFrames;
		bool mIsPlaying;
		bool mIsLooping;
		float mFrameRate;
		float mFPS;
		float mCurrentTime;
		float mPreviousTime;
		bool mIsDrawOriginalBounds;
		bool mIsDrawFrameBounds;
		
		SignalSpritesheetPlayComplete mPlayCompleteSignal;
		
		void parseJSON(ci::JsonTree json);
		void nextFrame();
		void drawBounds();
		void drawFrame();
		
	};
	
}