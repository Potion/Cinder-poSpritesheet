#pragma once

#include "poSpritesheet.h"
#include "boost/signals2.hpp"

namespace po {
	
	class Spritesheet;
	typedef std::shared_ptr<Spritesheet> SpritesheetRef;
	
	class SpritesheetAnimation;
	typedef std::shared_ptr<SpritesheetAnimation> SpritesheetAnimationRef;
	
	class SpritesheetAnimation
	: public std::enable_shared_from_this<SpritesheetAnimation>
	{
	public:
		//	Spritesheet to animate and frame rate
		static SpritesheetAnimationRef create(SpritesheetRef spritesheet, float fps = 12.f);
		
		~SpritesheetAnimation();
		
		//	Playing complete signal, when not looped
		typedef boost::signals2::signal<void(SpritesheetAnimationRef)> SignalSpritesheetPlayComplete;
		
		virtual void update();
		virtual void draw();
		
        void play();
		void pause() { mIsPlaying = false; }
		void stop();
		void setIsLoopingEnabled(bool isLooping) { mIsLooping = isLooping; }
		void setFrameRate(float frameRate);
		void setIsReverse(bool reverse);
		
		SignalSpritesheetPlayComplete &getSignalPlayingComplete() { return mPlayCompleteSignal; }
		int getCurrentFrame() { return mCurrentFrame; }
		
	protected:
		SpritesheetAnimation();
		
		void setup(SpritesheetRef spritesheet, float fps);
		
	private:
		SpritesheetRef mSpritesheet;
		
		//	Keep track of frames
		int mCurrentFrame;
		int mLastFrame;
		
		//	Keep track of state
		bool mIsPlaying;
		bool mIsLooping;
		bool mIsReverse;
		
		float mFrameRate;
		float mFPS;
		float mCurrentTime;
		float mPreviousTime;
		
		SignalSpritesheetPlayComplete mPlayCompleteSignal;
		
		//	Proceed to next frame
		void nextFrame();
		
	};
	
}
