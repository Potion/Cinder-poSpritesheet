/*
 Copyright (c) 2015, Potion Design
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
