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

 * Neither the name of copyright holder nor the names of its
 contributors may be used to endorse or promote products derived from
 this software without specific prior written permission.

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

#include "poSpritesheetAnimation.h"
#include "cinder/app/App.h"

namespace po
{

	SpritesheetAnimationRef SpritesheetAnimation::create( SpritesheetRef spritesheet, float fps )
	{
		SpritesheetAnimationRef ref( new SpritesheetAnimation() );
		ref->setup( spritesheet, fps );
		return ref;
	}

	SpritesheetAnimation::SpritesheetAnimation()
		: mCurrentFrame( 0 )
		, mIsPlaying( false )
		, mIsLooping( false )
		, mFPS( 12.f )
		, mPreviousTime( 0.f )
		, mCurrentTime( 0.f )
		, mIsReverse( false )
	{}

	SpritesheetAnimation::~SpritesheetAnimation()
	{}

	void SpritesheetAnimation::setup( SpritesheetRef spritesheet, float fps )
	{
		mSpritesheet = spritesheet;
		setFrameRate( fps );
		mLastFrame = mSpritesheet->getNumFrames() - 1;
	}

	//
	//	Goto the next frame based on fps
	//
	void SpritesheetAnimation::update()
	{
		if( mIsPlaying ) {
			mCurrentTime = ci::app::getElapsedSeconds() * 1000;
			float timeDiff = mCurrentTime - mPreviousTime;

			if( timeDiff > mFrameRate ) {
				float over = timeDiff - mFrameRate;
				mPreviousTime = mCurrentTime - over;
				nextFrame();
			}
		}
	}

	//
	//	Draw the current frame in the texture
	//
	void SpritesheetAnimation::draw()
	{
		mSpritesheet->drawFrame( mCurrentFrame );
	}

	//
	//	Set the frame rate
	//
	void SpritesheetAnimation::setFrameRate( float frameRate )
	{
		mFPS = frameRate;
		mFrameRate = 1000.f / mFPS;
	}

	//
	//	Go to the next frame
	//
	void SpritesheetAnimation::nextFrame()
	{
		if( mIsPlaying ) {

			if( mIsReverse ) {
				mCurrentFrame = ( mCurrentFrame -= 1 ) % mSpritesheet->getNumFrames();

				if( mCurrentFrame < 0 ) { mCurrentFrame = 0; }
			}
			else {
				mCurrentFrame = ( mCurrentFrame += 1 ) % mSpritesheet->getNumFrames();

				if( mCurrentFrame > mLastFrame ) { mCurrentFrame = mLastFrame; }
			}

			if( !mIsLooping ) {

				int targetFrame = !mIsReverse ? mLastFrame : 0;

				if( mCurrentFrame == targetFrame ) {
					mIsPlaying = false;
					mPlayCompleteSignal.emit( shared_from_this() );
				}
			}
		}
	}

	//
	//  Start Playing
	//
	void SpritesheetAnimation::play()
	{
		mPreviousTime = ci::app::getElapsedSeconds() * 1000.0f;
		mIsPlaying = true;
	}

	//
	//	Stop playing
	//
	void SpritesheetAnimation::stop()
	{
		mIsPlaying = false;

		if( mIsReverse ) {
			mCurrentFrame = mSpritesheet->getNumFrames() - 1;
		}
		else {
			mCurrentFrame = 0;
		}
	}

	//
	//	Play in reverse
	//
	void SpritesheetAnimation::setIsReverse( bool reverse, bool andSkipToStartFrame )
	{
		mIsReverse = reverse;

		if( andSkipToStartFrame ) {
			if( mIsReverse ) {
				if( mCurrentFrame == 0 ) {
					mCurrentFrame = mSpritesheet->getNumFrames() - 1;
				}

				mLastFrame = 0;
			}
			else {
				mLastFrame = mSpritesheet->getNumFrames() - 1;
			}
		}
	}

}