#ifndef SDL_FUNCS_H
#define SDL_FUNCS_H


#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>

enum buttonSprites {
    MOUSE_OUT = 0,
    MOUSE_OVER = 1,
    MOUSE_DOWN = 2,
    MOUSE_UP = 3,
    TOTAL_SPRITES = 4
};

class TextureWrapper {
private:

  // hardware texture
  SDL_Texture* mTexture;

  //  image dimensions
  int mWidth;
  int mHeight;

public:


  TextureWrapper() : mTexture{NULL}, mWidth{0}, mHeight{0} {}

  // deallocate memory
  ~TextureWrapper() {
    free();
  }

  bool loadFromFile(SDL_Renderer* pRenderer, std::string path) {

    // free previously allocated members
    free();

    // finall texture that will be assigned to pTexture
    SDL_Texture* newTexture = NULL;

    SDL_Surface* tempSurface = NULL;
    tempSurface = IMG_Load(path.c_str());

    if (tempSurface == NULL) {
      printf("Failed @loadFromFile. IMG Error: %s", IMG_GetError());
      return false;
    }


    // Setting color key (transparent pixel when rendering over other textures)
    if (SDL_SetColorKey( tempSurface, SDL_TRUE, SDL_MapRGB( tempSurface -> format, 0x00, 0xFF, 0xFF) ) ) {
      printf("Failed @loadFromFile. SDL Error: %s", SDL_GetError());
      return false;
    }

    newTexture = SDL_CreateTextureFromSurface(pRenderer, tempSurface);
    if (newTexture == NULL) {
      printf("Failed @loadFromFile. SDL Error: %s", SDL_GetError());
      return false;
    }

    // if everything went right, set texture width and height to the image's dimensions
    mWidth = tempSurface->w; mHeight = tempSurface->h;

    // get rid of temporary surface memory allocation space
    SDL_FreeSurface(tempSurface);

    // set the SDL_Texture* member to newTexture
    mTexture = newTexture;

    return true;

  }

  // creates image from font string
  bool loadFromRenderedText(SDL_Renderer* pRenderer, std::string text, TTF_Font* fontStyle, SDL_Color textColor) {

    // free preexisting texture
    free();

    // Render text surface
    SDL_Surface* newSurface = TTF_RenderText_Blended_Wrapped( fontStyle, text.c_str(), textColor, 400);

    if ( newSurface == NULL) {
      printf( "Unable to render text surface! SDL_ttf Error: %s\n", TTF_GetError());
      return false;
    } else {

      // Create texture from text surface
      mTexture = SDL_CreateTextureFromSurface(pRenderer, newSurface);
      if (mTexture == NULL) {

        printf("Unable to load texture from surface! SDL Error: %s\n", SDL_GetError());
        return false;

      } else {

        // get texture dimensions
        mWidth = newSurface->w;
        mHeight = newSurface->h;

      }

    }

  // freeing allcated surface memory.
  SDL_FreeSurface(newSurface);
  return true;
  }

  // deallocating texture
  void free() {

    if (mTexture != NULL) {

      // deallocate previous texture pointer and set it to NULL
      SDL_DestroyTexture(mTexture);
      mTexture = NULL;

      // set width and height to default values (zeros)
      mWidth = 0;
      mHeight = 0;
    }

  }

  // color modulation
  void setColor(Uint8 R, Uint8 G, Uint8 B) {
    SDL_SetTextureColorMod(mTexture, R, G, B);
  }

  // alpha modulation
  void setAlpha(Uint8 A) {
    SDL_SetTextureAlphaMod(mTexture, A);
  }

  // blend mode - necessary for alpha modulation
  void setBlendMode(SDL_BlendMode blending) {
    SDL_SetTextureBlendMode(mTexture, blending);
  }

  // Renders the attached texture
  void render(  SDL_Renderer* pRenderer, int x = 0, int y = 0, SDL_Rect* selectFrom = NULL, const double angle = 0.0,
                SDL_Point* center = NULL, SDL_RendererFlip flip = SDL_FLIP_NONE) {

    // set rendering space and position
    SDL_Rect newTextureRect = {x, y, mWidth, mHeight};

    if (selectFrom != NULL) {

      newTextureRect.w = selectFrom->w;
      newTextureRect.h = selectFrom->h;

    }

    SDL_RenderCopyEx(pRenderer, mTexture, selectFrom, &newTextureRect, angle, center, flip);

  }

  int getWidth() const {
    return mWidth;
  }

  int getHeight() const {
    return mHeight;
  }

};

class SimpleButton {
  public:

    // initializing members
    SimpleButton(TextureWrapper* pTexture, SDL_Rect pRects[4]) : xPosition{0}, yPosition{0}, mCurrentSprite{MOUSE_OUT} {

      buttonSpritesTexture = pTexture;
      buttonSpritesRects = pRects;

      // initializing button width and height assuming all sprites are of equal size
      mWidth = buttonSpritesRects[0].w;
      mHeight = buttonSpritesRects[0].h;
    }

    ~SimpleButton(void) {
      // Deallocate here
    }

    void setPosition(int x=0, int y=0) {
      xPosition = x;
      yPosition = y;
    }

    void handleEvent(Uint32 eventType) {

      // checks whether a mouse event has occured
      if ( (eventType == SDL_MOUSEMOTION) || (eventType == SDL_MOUSEBUTTONDOWN) || (eventType == SDL_MOUSEBUTTONUP) ) {

        int x, y;
        SDL_GetMouseState(&x, &y);

        // whether the mouse is inside the button or not
        bool inside = true;

        if (x < xPosition) {
          inside = false;
        } else if (y < yPosition) {
          inside = false;
        } else if ( x > (xPosition + mWidth)) {
          inside = false;
        } else if ( y > (yPosition + mHeight)) {
          inside = false;
        }

        if (!inside) {

          mCurrentSprite = MOUSE_OUT;

        } else {

          switch (eventType) {

            case SDL_MOUSEMOTION :      mCurrentSprite = MOUSE_OVER;
                                        break;

            case SDL_MOUSEBUTTONUP :    mCurrentSprite = MOUSE_UP;
                                        break;

            case SDL_MOUSEBUTTONDOWN :  mCurrentSprite = MOUSE_DOWN;
                                        break;

            default :                   break;

          }
        }
      } // END IF


    }

    void render(SDL_Renderer* pRenderer) {
      buttonSpritesTexture->render(pRenderer, xPosition, yPosition, &buttonSpritesRects[mCurrentSprite]);
    }

  private:

    // top left button position
    int xPosition;
    int yPosition;

    // button width and height
    int mWidth;
    int mHeight;

    buttonSprites mCurrentSprite;

    // a pointer to a texture containing all the button's sprites
    TextureWrapper* buttonSpritesTexture;

    // an array of SDL_Rect defining different sprites
    SDL_Rect* buttonSpritesRects;

};

#endif
