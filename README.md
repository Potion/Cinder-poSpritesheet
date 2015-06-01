#Cinder-poSpritesheet

[Potion's](http://www.potiondesign.com) spritesheet block for [Cinder](http://libcinder.org).

The block consists of two classes:

- **Spritesheet:** parses and keeps track of the frame data
- **SpritesheetAnimation:** provides control for frame playback

Tested on OS X Yosemite and Windows 8.

## Features 
- Supports spritesheets created with [TexturePacker](https://www.codeandweb.com/texturepacker)
- JSON (Array), XML (generic) data formats from within [TexturePacker](https://www.codeandweb.com/texturepacker)
- Multipacked textures
- Play, pause, stop, loop, reverse animation
- Set animation frame rate
- Get signal when animation has finished playing

*Currently doesn't support rotated sprites.*

## Samples
- **SpritesheetFrame:** draws a spritesheet frame based on mouse position
- **SpritesheetAnimation:** animates spritesheet frames across the app window

> [goblin](http://opengameart.org/content/goblin) by [Clint Bellanger](http://opengameart.org/users/clint-bellanger) used under [Creative Commons Attribution (CC-BY) 3.0 License](http://creativecommons.org/licenses/by/3.0/) / Scaled up from original.

## Getting started

To draw a specific frame from the spritesheet:

```
gl::TextureRef texture = gl::Texture::create(loadImage(loadAsset("goblin.png")));
JsonTree json = JsonTree(loadAsset("goblin.json"));
	
mSpritesheet = po::Spritesheet::create(texture, json);
mSpritesheet->drawFrame(6);
```

To animate the spritesheet:

```
gl::TextureRef texture = gl::Texture::create(loadImage(loadAsset("charge.png")));
JsonTree json = JsonTree(loadAsset("charge.json"));
	
mSpritesheet = po::Spritesheet::create(texture, json);
mSpritesheetAnimation = po::SpritesheetAnimation::create(mSpritesheet);
mSpritesheetAnimation->play();
```
