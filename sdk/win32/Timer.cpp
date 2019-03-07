/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007-2012 Vincent E. Milum Jr., All rights reserved.   *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "Timer.h"
#include "Win32.h"


vTimerList *vTimer::list = NULL;


vTimer::vTimer(UINT timeout, BOOL autoStart) {
  Started = FALSE;

  if (!list) new vTimerList();

  list->appendItem(this);
  setTimeout(timeout);
  if (autoStart) startTimer();
}


vTimer::~vTimer() {
  stopTimer();
  if (!list) return;
  list->removeItem(this);
  if (!list->hasItems()) delete list;
}


void vTimer::setTimeout(UINT ms) {
  Timeout = ms;
  if (Started) if (list) list->setTimer(this);
}


void vTimer::startTimer() volatile {
  if (Started) return;
  Started = TRUE;
  if (list) list->setTimer((vTimer*)this);
}


void vTimer::stopTimer() volatile {
  if (list) list->killTimer((vTimer*)this);
  if (!Started) return;
  Started = FALSE;
}


//-----------------------------------------------------------------------------------------------


vTimedCallback::vTimedCallback() : vTimer(1, FALSE) {
}

vTimedCallback::~vTimedCallback() {
}

void vTimedCallback::onTimer(DWORD systemTime) {
  void *event = events.getFirstItem();
  removeEvent(event);
  onEvent(event);
}

void vTimedCallback::addEvent(void *event) {
  events.appendItem(event);
  if (!isStarted()) startTimer();
}

void vTimedCallback::removeEvent(void *event) {
  events.removeItem(event);
  if (events.getItemCount() == 0) stopTimer();
}


//-----------------------------------------------------------------------------------------------


vTimerList::vTimerList() : vMessageWnd("vTimer") {
  init();
  vTimer::list = this;
}

vTimerList::~vTimerList() {
  vTimer *timer = getFirstItem();
  while (timer) {
    killTimer(timer);
    removeItem(timer);
    timer = getFirstItem();
  }
  vTimer::list = NULL;
}

void vTimerList::setTimer(vTimer *t) {
  if (!t) return;
  SetTimer(getWnd(), (UINT_PTR)t, t->getTimeout(), NULL);
}


void vTimerList::killTimer(vTimer *t) {
  if (!t) return;
  KillTimer(getWnd(), (UINT_PTR)t);
}


VUINT vTimerList::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg == WM_TIMER) {
    vTimer *t = (vTimer*)param1;
    if (hasItem(t)) t->onTimer(RMXOS::getTick());
    return 0;
  }
  return vMessageWnd::wndProc(msg, param1, param2);
}
