/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _DX_SPRITE_H_
#define _DX_SPRITE_H_


#include "dxRender.h"
#include "dxTexture.h"


class dxSprite : public dxRender {
  public:
    dxSprite(const char *name, dxTexture *texture, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL);
    virtual ~dxSprite();


  protected:
    virtual int  onRender();
    virtual void onMove() {}

  public:
    inline BOOL isVisible() const { return visible; }


    void  move(float x, float y) { setX(x);  setY(y); }

    inline void  moveLeft( float left)  { setX(getX() - left);  }
    inline void  moveRight(float right) { setX(getX() + right); }
    inline void  moveUp(   float up)    { setY(getY() - up);    }
    inline void  moveDown( float down)  { setY(getY() + down);  }

    inline void setX(float x) { setParamNumber("x", x); }
    inline void setY(float y) { setParamNumber("y", y); }

    inline float getX() const { return dest.x; }
    inline float getY() const { return dest.y; }

    virtual void onSetParam(const prmBase *param);


/*
    void resize(int w, int h);
    void setWidth( int w);
    void setHeight(int h);
    int  getWidth()  const;
    int  getHeight() const;
    virtual int validateW(int w) { return w; }
    virtual int validateH(int h) { return h; }
    virtual void onResize() {}
*/
    inline RECT        getSrcRect()  const { return src;  }
    inline D3DXVECTOR3 getDestRect() const { return dest; }

    inline dxTexture *getTexture() { return tex; }

  private:
    RECT        src;
    D3DXVECTOR3 dest;
    BOOL        visible;
    dxTexture *tex;
};


class dxSpriteSingle : public dxTexture, public dxSprite {
  public:
    dxSpriteSingle(const char *name, const char *file, cfgBase *parent, cfgBase *insert=NULL, HPARAMLIST paramlist=NULL)
      : dxTexture(file), dxSprite(name, getTex(), parent, insert, paramlist) {}
};


#endif //_DX_SPRITE_H_
