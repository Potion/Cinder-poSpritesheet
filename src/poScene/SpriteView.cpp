//
//  Sprite.cpp
//  ForestFriends
//
//  Created by bruce on 9/11/14.
//
//

#include "SpriteView.h"

namespace po
{
	namespace scene
	{
		//-----------------------------------
		//	Single texture
		//-----------------------------------
		SpriteViewRef SpriteView::create( ci::gl::TextureRef texture, ci::JsonTree json )
		{
			SpriteViewRef ref( new SpriteView() );
			ref->setup( texture, json );
			return ref;
		}

		SpriteViewRef SpriteView::create( ci::gl::TextureRef texture, ci::XmlTree xml )
		{
			SpriteViewRef ref( new SpriteView() );
			ref->setup( texture, xml );
			return ref;
		}

		void SpriteView::setup( ci::gl::TextureRef texture, ci::JsonTree json )
		{
			mSpritesheet = po::Spritesheet::create( texture, json );
			setupAnimation();
		}

		void SpriteView::setup( ci::gl::TextureRef texture, ci::XmlTree xml )
		{
			mSpritesheet = po::Spritesheet::create( texture, xml );
			setupAnimation();
		}

		//-----------------------------------
		// Multipacked textures
		//-----------------------------------
		SpriteViewRef SpriteView::create( std::vector<ci::gl::TextureRef>& textures, std::vector<ci::JsonTree>& data )
		{
			SpriteViewRef ref( new SpriteView() );
			ref->setup( textures, data );
			return ref;
		}

		SpriteViewRef SpriteView::create( std::vector<ci::gl::TextureRef>& textures, std::vector<ci::XmlTree>& data )
		{
			SpriteViewRef ref( new SpriteView() );
			ref->setup( textures, data );
			return ref;
		}

		void SpriteView::setup( std::vector<ci::gl::TextureRef>& textures, std::vector<ci::JsonTree>& data )
		{
			mSpritesheet = po::Spritesheet::create( textures, data );
			setupAnimation();
		}

		void SpriteView::setup( std::vector<ci::gl::TextureRef>& textures, std::vector<ci::XmlTree>& data )
		{
			mSpritesheet = po::Spritesheet::create( textures, data );
			setupAnimation();
		}

		//-----------------------------------
		//	Spritesheet
		//-----------------------------------
		SpriteViewRef SpriteView::create( po::SpritesheetRef spriteSheet )
		{
			SpriteViewRef ref( new SpriteView() );
			ref->setup( spriteSheet );
			return ref;
		}

		void SpriteView::setup( po::SpritesheetRef spriteSheet )
		{
			mSpritesheet = spriteSheet;
			setupAnimation();
		}

		//-----------------------------------
		//	Sprite Data
		//-----------------------------------
		SpriteViewRef SpriteView::create( SpriteDataJson& spriteData )
		{
			return create( spriteData.textures, spriteData.jsonFiles );
		}

		SpriteViewRef SpriteView::create( SpriteDataXml& spriteData )
		{
			return create( spriteData.textures, spriteData.xmlFiles );
		}

		//-----------------------------------
		//	Sprite
		//-----------------------------------

		SpriteView::SpriteView()
			: mIsKeyShaderEnabled( false )
		{
		}

		//
		//	Create the spritesheet animation
		//
		void SpriteView::setupAnimation()
		{
			mSpritesheetAnimation = po::SpritesheetAnimation::create( mSpritesheet );
		}

		//
		//	Update the spritesheet animation
		//
		void SpriteView::update()
		{
			View::update();
			setAlignment( getAlignment() ); // Update alignment b/c the sprite size is constantly changing
			mSpritesheetAnimation->update();
		}

		//
		//	Draw the spritesheet animation
		//
		void SpriteView::draw()
		{
			if( mIsKeyShaderEnabled ) {
				ci::gl::ScopedGlslProg keyShader( mShader );
				mShader->bind();
				//			mShader->uniform("keyColor", Colors::KEY_COLOR);
				mShader->uniform( "replacementColor", mReplacementColor );
				mShader->uniform( "alpha", getAppliedAlpha() );
				mSpritesheetAnimation->draw();

			}
			else {
				ci::gl::color( ci::ColorA( getFillColor(), getAppliedAlpha() ) );
				mSpritesheetAnimation->draw();
			}
		}

		//
		//	Set the replacement color for the shader
		//
		void SpriteView::setReplacementColor( ci::Color color )
		{
			mIsKeyShaderEnabled = true;
			mReplacementColor = color;
		}

		bool SpriteView::pointInside( const ci::vec2& point, bool localize )
		{
			ci::vec2 pos = localize ? windowToLocal( point ) : point;
			return getSpritesheet()->getFrameBounds().contains( pos );
		}
	}
}