/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/cfg/cfgHelper.h"
#include "uiKeyboardEvent.h"
#include "uiKeyboard.h"


uiKeyboardEvent::uiKeyboardEvent(const char *name, cfgBase *parent, HPARAMLIST paramlist)
 : evntPress(name, parent, NULL, paramlist) {

//  par = parent;
//  if (par) par->insertEvent(this);

  cfgtext = new uiKeyboardEventText("Key Combination", this);
  cfgtext->setEnabled(FALSE);

  cfgraw = new uiKeyboardEventText("Raw Data", this);
  cfgraw->setEnabled(FALSE);
  
//  new vRemoveParent(this);
  new cfgRemoveParent("Remove Keyboard Shortcut", this);
  line = new cfgLine(getParent());
}


uiKeyboardEvent::~uiKeyboardEvent() {
//  if (par) par->removeEvent(this);
  vdelnull(line);
}


void uiKeyboardEvent::setText(const char *text, const char *raw) {
  cfgtext->setText(text);
  cfgraw->setText(raw);
}


const char *uiKeyboardEvent::getRawText() const {
  if (cfgraw) return cfgraw->getText();
  return NULL;
}


//--------------------------------------------------------------------


uiKeyboardEventText::uiKeyboardEventText(const char *name, uiKeyboardEvent *par) : cfgText(name, par) {
  parent = par;
}


uiKeyboardEventText::~uiKeyboardEventText() {
}


void uiKeyboardEventText::onGainFocus(HPLUGIN source) {
  cfgText::onGainFocus(source);
  if (source->isMaster()) keyboard->setActive(parent);
}


void uiKeyboardEventText::onLostFocus(HPLUGIN source) {
  cfgText::onLostFocus(source);
  if (source->isMaster()) keyboard->setActive(NULL);
}
