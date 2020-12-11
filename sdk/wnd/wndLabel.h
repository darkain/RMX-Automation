/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_LABEL_H_
#define _WND_LABEL_H_


#include "wndBase.h"
#include "../TextAlign.h"



class wndLabel : public wndBase {
  public:
    wndLabel(wndBase *parent);
    virtual ~wndLabel();

  public:
    void setTextHAlign(TextHAlign texthalign);
    void setTextVAlign(TextVAlign textvalign);
    void setTextAlign( TextAlign  textalign );
    void setTextAlign( TextHAlign texthalign, TextVAlign textvalign);
    inline TextAlign  getTextAlign()  { return align;   }
    inline TextHAlign getTextHAlign() { return align.h; }
    inline TextVAlign getTextVAlign() { return align.v; }

    void setMarginLeft(  int left  );
    void setMarginTop(   int top   );
    void setMarginRight( int right );
    void setMarginBottom(int bottom);
    void setMargins(RECT margins);
    void setMargins(int left, int top, int right, int bottom);
    inline RECT getMargins()      { return margin;        }
    inline int  getMarginLeft()   { return margin.left;   }
    inline int  getMarginTop()    { return margin.top;    }
    inline int  getMarginRight()  { return margin.right;  }
    inline int  getMarginBottom() { return margin.bottom; }

    void setTextColor(COLORREF color, BOOL redraw=TRUE);
    inline COLORREF getTextColor() { return textcolor; }

    void setTransparent(BOOL trans);
    inline BOOL getTransparent() const { return transparent; }

  protected:
    void drawText(const char *text);

    //inherited items from wndBase:
    virtual void onPaint(HDC hdc, PAINTSTRUCT *ps);
    virtual BOOL customPaint() { return TRUE; }
    virtual BOOL eraseBackground(HDC hdc);

    virtual void onNameChange(const char *newname);

/*
    virtual void onMouseEnter(int x, int y, BOOL track=TRUE);
    virtual void onMouseMove(int x, int y);
    virtual void onMouseLeave(int x, int y);
*/

  protected:
    HDC dc;

  private:
    BOOL transparent;
    TextAlign align;
    RECT margin;
    COLORREF textcolor;
};


#endif //_WND_LABEL_H_
