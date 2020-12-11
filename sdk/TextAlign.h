/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef __TEXTALIGN_H_
#define __TEXTALIGN_H_


typedef enum {
  TEXTHALIGN_LEFT,
  TEXTHALIGN_CENTER,               //what ever default center style is... see bellow
  TEXTHALIGN_RIGHT,
  TEXTHALIGN_EVENSPACING,          //add more space between letters/words to make it fit in 100% of the area
  TEXTHALIGN_FITTOWIDTH,           //make the font larger or smaller to fit in 100% of the area
  TEXTHALIGN_LEFT_ELLIPSIS,        //align left, and truncate text to fit if too large
  TEXTHALIGN_CENTER_CENTER,        //if text is too wide, it will still center on the middle, choping off left and right sides
  TEXTHALIGN_CENTER_LEFT,          //will chop off right side if too big
  TEXTHALIGN_CENTER_RIGHT,         //will chop off left side if too big
  TEXTHALIGN_SCROLL,               //if text is too large, it will use default scrolling (see bellow)
  TEXTHALIGN_SCROLL_BACKFORTH,     //text will scroll back and forth if too large
  TEXTHALIGN_SCROLL_TICKER,        //text will scroll in one direction, and loop, using default scroll direction (see bellow)
  TEXTHALIGN_SCROLL_TICKER_LEFT,
  TEXTHALIGN_SCROLL_TICKER_RIGHT,
} TextHAlign;


typedef enum {
  TEXTVALIGN_TOP,
  TEXTVALIGN_MIDDLE,
  TEXTVALIGN_BOTTOM,
} TextVAlign;


typedef struct {
  TextHAlign h;
  TextVAlign v;
} TextAlign;


#endif//__TEXTALIGN_H_
