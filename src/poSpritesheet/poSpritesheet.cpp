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

#include "poSpritesheet.h"

namespace po
{

	//------------------------------------------
	//	Single texture spritesheet setup
	//------------------------------------------

	SpritesheetRef Spritesheet::create( ci::gl::TextureRef texture, ci::JsonTree json )
	{
		SpritesheetRef ref( new Spritesheet() );
		ref->setup( texture, json );
		return ref;
	}

	SpritesheetRef Spritesheet::create( ci::gl::TextureRef texture, ci::XmlTree xml )
	{
		SpritesheetRef ref( new Spritesheet() );
		ref->setup( texture, xml );
		return ref;
	}

	void Spritesheet::setup( ci::gl::TextureRef texture, ci::JsonTree json )
	{
		mTextures[0] = texture;
		setupSpriteMap( 0, json );
	}

	void Spritesheet::setup( ci::gl::TextureRef texture, ci::XmlTree xml )
	{
		mTextures[0] = texture;
		setupSpriteMap( 0, xml );
	}



	//------------------------------------------
	//	Multipacked spritesheet setup
	//------------------------------------------

	void Spritesheet::setup( std::vector<ci::gl::TextureRef>& textures, std::vector<ci::JsonTree> data )
	{
		int counter = 0;

		for( auto json : data ) {
			int textureID = counter;
			mTextures[textureID] = textures[counter];
			setupSpriteMap( textureID, json );
			counter++;
		}
	}

	void Spritesheet::setup( std::vector<ci::gl::TextureRef>& textures, std::vector<ci::XmlTree> data )
	{
		int counter = 0;

		for( auto xml : data ) {
			int textureID = counter;
			mTextures[textureID] = textures[counter];
			setupSpriteMap( textureID, xml );
			counter++;
		}
	}

	SpritesheetRef Spritesheet::create( std::vector<ci::gl::TextureRef>& textures, std::vector<ci::JsonTree>& data )
	{
		SpritesheetRef ref( new Spritesheet() );
		ref->setup( textures, data );
		return ref;
	}

	SpritesheetRef Spritesheet::create( std::vector<ci::gl::TextureRef>& textures, std::vector<ci::XmlTree>& data )
	{
		SpritesheetRef ref( new Spritesheet() );
		ref->setup( textures, data );
		return ref;
	}


	//------------------------------------------
	//	Spritesheet
	//------------------------------------------

	Spritesheet::Spritesheet()
		: mIsDrawOriginalBounds( false )
		, mIsDrawFrameBounds( false )
		, mCurrentFrameKey( "" )
	{}

	Spritesheet::~Spritesheet()
	{}


	//
	//	Setup frame data and texture maps
	//
	void Spritesheet::setupSpriteMap( int textureID, ci::JsonTree json )
	{
		// get all the frames in the json
		for( auto frame : json.getChild( "frames" ) ) {
			FrameData frameData = getFrameData( frame );
			std::string frameKey = frameData.filename;
			mFrameData[frameKey] = frameData;
			mTextureIDs[frameKey] = textureID;
			mFrameOrder.push_back( frameKey );
		}

		// sort the frame order alphabetically
		std::sort( mFrameOrder.begin(), mFrameOrder.end() );
		mNumFrames = mFrameOrder.size();
		mCurrentFrameKey = mFrameOrder[0];
	}

	//
	//	Create framedata object from json
	//
	Spritesheet::FrameData Spritesheet::getFrameData( ci::JsonTree json )
	{
		FrameData frameData = FrameData();
		frameData.filename = json.getChild( "filename" ).getValue<std::string>();
		frameData.frame = ci::Area(
		                      json.getChild( "frame" ).getChild( "x" ).getValue<float>(),
		                      json.getChild( "frame" ).getChild( "y" ).getValue<float>(),
		                      json.getChild( "frame" ).getChild( "x" ).getValue<float>() +
		                      json.getChild( "frame" ).getChild( "w" ).getValue<float>(),
		                      json.getChild( "frame" ).getChild( "y" ).getValue<float>() +
		                      json.getChild( "frame" ).getChild( "h" ).getValue<float>()
		                  );
		frameData.rotated = json.getChild( "rotated" ).getValue<bool>();
		frameData.trimmed = json.getChild( "trimmed" ).getValue<bool>();
		frameData.spriteSourceSize = ci::Rectf(
		                                 json.getChild( "spriteSourceSize" ).getChild( "x" ).getValue<float>(),
		                                 json.getChild( "spriteSourceSize" ).getChild( "y" ).getValue<float>(),
		                                 json.getChild( "spriteSourceSize" ).getChild( "x" ).getValue<float>() +
		                                 json.getChild( "spriteSourceSize" ).getChild( "w" ).getValue<float>(),
		                                 json.getChild( "spriteSourceSize" ).getChild( "y" ).getValue<float>() +
		                                 json.getChild( "spriteSourceSize" ).getChild( "h" ).getValue<float>()
		                             );
		frameData.sourceSize = ci::vec2(
		                           json.getChild( "sourceSize" ).getChild( "w" ).getValue<float>(),
		                           json.getChild( "sourceSize" ).getChild( "h" ).getValue<float>()
		                       );

		return frameData;
	}


	//
	//	Setup frame data and texture maps
	//
	void Spritesheet::setupSpriteMap( int textureID, ci::XmlTree xml )
	{

		// get all the frames in the json
		for( auto frame : xml.getChild( "TextureAtlas" ) ) {
			FrameData frameData = getFrameData( frame );
			std::string frameKey = frameData.filename;
			mFrameData[frameKey] = frameData;
			mTextureIDs[frameKey] = textureID;
			mFrameOrder.push_back( frameKey );
		}

		// sort the frame order alphabetically
		std::sort( mFrameOrder.begin(), mFrameOrder.end() );
		mNumFrames = mFrameOrder.size();
		mCurrentFrameKey = mFrameOrder[0];
	}


	//
	//	Create framedata object from json
	//
	Spritesheet::FrameData Spritesheet::getFrameData( ci::XmlTree xml )
	{
		FrameData frameData = FrameData();
		frameData.filename = xml.getAttribute( "n" ).getValue<std::string>();

		//  Get Sprite size in spritesheet
		frameData.frame = ci::Area(
		                      xml.getAttribute( "x" ).getValue<float>(),
		                      xml.getAttribute( "y" ).getValue<float>(),
		                      xml.getAttribute( "x" ).getValue<float>() +
		                      xml.getAttribute( "w" ).getValue<float>(),
		                      xml.getAttribute( "y" ).getValue<float>() +
		                      xml.getAttribute( "h" ).getValue<float>()
		                  );
		frameData.sourceSize = ci::vec2(
		                           xml.getAttribute( "w" ).getValue<float>(),
		                           xml.getAttribute( "h" ).getValue<float>()
		                       );

		//  Get rotation
		if( xml.hasAttribute( "r" ) ) {
			frameData.rotated = xml.getChild( "r" ).getValue<bool>();
		}
		else {
			frameData.rotated = false;
		}

		//  Get Trim/Actual size
		if( xml.hasAttribute( "oX" ) ) {
			frameData.trimmed = true;
			frameData.spriteSourceSize = ci::Area(
			                                 xml.getAttribute( "oX" ).getValue<float>(),
			                                 xml.getAttribute( "oY" ).getValue<float>(),
			                                 xml.getAttribute( "oX" ).getValue<float>() +
			                                 xml.getAttribute( "oW" ).getValue<float>(),
			                                 xml.getAttribute( "oY" ).getValue<float>() +
			                                 xml.getAttribute( "oH" ).getValue<float>()
			                             );
		}
		else {
			frameData.trimmed = false;
			frameData.spriteSourceSize = ci::Area( 0, 0, frameData.sourceSize.x, frameData.sourceSize.y );
		}


		return frameData;
	}


	//
	//	Draw bounds for the original size or the current frame size
	//
	void Spritesheet::drawBounds()
	{
		if( mIsDrawOriginalBounds ) {
			ci::gl::ScopedColor color( ci::Color( 1, 0, 0 ) );
			ci::gl::drawStrokedRect( getOriginalBounds() );
		}

		if( mIsDrawFrameBounds ) {
			ci::gl::ScopedColor color( ci::Color( 0, 1, 0 ) );
			ci::gl::drawStrokedRect( getFrameBounds() );
		}
	}

	//
	//	Draw the texture for the current frame
	//
	void Spritesheet::drawFrame( int frameNum )
	{
		ci::gl::ScopedMatrices matrices;
		ci::gl::ScopedBlendAlpha alpha;

		mCurrentFrameKey = mFrameOrder[frameNum];
		ci::gl::draw( mTextures[mTextureIDs[mCurrentFrameKey]], mFrameData[mCurrentFrameKey].frame, mFrameData[mCurrentFrameKey].spriteSourceSize );

		drawBounds();
	}

	void Spritesheet::drawFrame( std::string frameName )
	{
		int framenum = std::find( mFrameOrder.begin(), mFrameOrder.end(), frameName ) - mFrameOrder.begin();

		if( framenum < mFrameOrder.size() ) {
			drawFrame( framenum );
		}
		else {
			ci::app::console() << "No frame named: " << frameName << std::endl;
		}
	}

	//
	//	Get the original bounds for the frame
	//
	ci::Rectf Spritesheet::getOriginalBounds()
	{
		ci::Rectf bounds( 0, 0, mFrameData[mCurrentFrameKey].sourceSize.x, mFrameData[mCurrentFrameKey].sourceSize.y );
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