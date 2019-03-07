/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _WND_FOREST_ITEM_H_
#define _WND_FOREST_ITEM_H_


#include "wndForestBase.h"
#include "../whk/whkCommand.h"


class wndForestItem : public wndForestBase, public whkCommand {
  public:
    wndForestItem(wndForestBase *parentobject, wndForestBase *insert=NULL);
    virtual ~wndForestItem();

  public:
    inline HBRUSH   getNormalBrush() const { return normalbrush; }
    inline HBRUSH   getHoverBrush()  const { return hoverbrush;  }
    inline COLORREF getNormalColor() const { return normalcolor; }
    inline COLORREF getHoverColor()  const { return hovercolor;  }

    void setNormalBrush(HBRUSH brush);
    void setHoverBrush( HBRUSH brush);
    void setNormalColor(COLORREF clr);
    void setHoverColor( COLORREF clr);

    BOOL isHighlighted() const { return highlighted; }
    void setHighlighted(BOOL highlight);

    void setItemEnabled(BOOL en);
    BOOL isItemEnabled() const { return itemenable; }

    void invalidateForestItem();

  protected:
    void updateForestColors();

    virtual void onSetItemEnabled(BOOL en) {}

    virtual int getOffsetX() { return 20; }

    virtual void onSetHighlighted(BOOL highlighted) {}

    virtual void onInit();
    virtual void onPostInit();
    virtual void onResize(int width, int height);

    virtual void onSetMouseOver(BOOL mouseover);

    virtual void onSetExpanded();
    virtual void onInsertChildObject(wndForestBase *object, wndForestBase *insertAfter);
    virtual void onDelChildObject(wndForestBase *object);

    virtual BOOL eraseBackground(HDC hdc);

    virtual BOOL onCommand(whkInfo *info);

  private:
    wndBase *btn;
    BOOL     mouseOver;
    BOOL     highlighted;
    BOOL     itemenable;
    HBRUSH   normalbrush;
    HBRUSH   hoverbrush;
    COLORREF normalcolor;
    COLORREF hovercolor;
};


#endif //_WND_FOREST_ITEM_H_
