/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "../../sdk/Win32/Win32.h"
#include "uiKeyboard.h"
#include "uiKeyboardFunction.h"


uiKeyboardFunction::uiKeyboardFunction() : funcPress("Keyboard Emulation", "sendkeys") {
  setTooltip("Emulates key strokes");
  new funcParam("Key String", "f_keys", "string", this);
}


uiKeyboardFunction::~uiKeyboardFunction() {
}


void uiKeyboardFunction::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  emulateKeyStrokes( source->list_getParam(input, "f_keys"), (state ? 0 : KEYEVENTF_KEYUP) );
}



void uiKeyboardFunction::emulateKeyStrokes(const char *string, DWORD flags) {
  if (!string) return;
  int len = VSTRLEN(string);
  if (len < 1) return;

  BOOL lock = keyboard->isLocked();
  if (!lock) keyboard->lock();

  while (*string) {

    if (*string == '[') {
      string++;

      const char *end = string;
      while (*end != NULL  &&  *end != ']') end++;

      if (*end) {
        int len = end - string;
        char *temp = (char*)malloc(len+1);
        memcpy(temp, string, len);
        temp[len] = NULL;

        if (!VSTRCMPI(temp, "SHIFT"))  sendMod(0x01);
        if (!VSTRCMPI(temp, "CTRL"))   sendMod(0x02);
        if (!VSTRCMPI(temp, "ALT"))    sendMod(0x04);
        if (!VSTRCMPI(temp, "WIN"))    sendMod(0x08);
        if (!VSTRCMPI(temp, "KANA"))   sendMod(0x10);
        if (!VSTRCMPI(temp, "/SHIFT")) sendMod(0x01, KEYEVENTF_KEYUP);
        if (!VSTRCMPI(temp, "/CTRL"))  sendMod(0x02, KEYEVENTF_KEYUP);
        if (!VSTRCMPI(temp, "/ALT"))   sendMod(0x04, KEYEVENTF_KEYUP);
        if (!VSTRCMPI(temp, "/WIN"))   sendMod(0x08, KEYEVENTF_KEYUP);
        if (!VSTRCMPI(temp, "/KANA"))  sendMod(0x10, KEYEVENTF_KEYUP);

        if (!VSTRCMPI(temp, "F1"))  sendKey(VK_F1,  flags);
        if (!VSTRCMPI(temp, "F2"))  sendKey(VK_F2,  flags);
        if (!VSTRCMPI(temp, "F3"))  sendKey(VK_F3,  flags);
        if (!VSTRCMPI(temp, "F4"))  sendKey(VK_F4,  flags);
        if (!VSTRCMPI(temp, "F5"))  sendKey(VK_F5,  flags);
        if (!VSTRCMPI(temp, "F6"))  sendKey(VK_F6,  flags);
        if (!VSTRCMPI(temp, "F7"))  sendKey(VK_F7,  flags);
        if (!VSTRCMPI(temp, "F8"))  sendKey(VK_F8,  flags);
        if (!VSTRCMPI(temp, "F9"))  sendKey(VK_F9,  flags);
        if (!VSTRCMPI(temp, "F10")) sendKey(VK_F10, flags);
        if (!VSTRCMPI(temp, "F11")) sendKey(VK_F11, flags);
        if (!VSTRCMPI(temp, "F12")) sendKey(VK_F12, flags);
        if (!VSTRCMPI(temp, "F13")) sendKey(VK_F13, flags);
        if (!VSTRCMPI(temp, "F14")) sendKey(VK_F14, flags);
        if (!VSTRCMPI(temp, "F15")) sendKey(VK_F15, flags);
        if (!VSTRCMPI(temp, "F16")) sendKey(VK_F16, flags);
        if (!VSTRCMPI(temp, "F17")) sendKey(VK_F17, flags);
        if (!VSTRCMPI(temp, "F18")) sendKey(VK_F18, flags);
        if (!VSTRCMPI(temp, "F19")) sendKey(VK_F19, flags);
        if (!VSTRCMPI(temp, "F20")) sendKey(VK_F20, flags);
        if (!VSTRCMPI(temp, "F21")) sendKey(VK_F21, flags);
        if (!VSTRCMPI(temp, "F22")) sendKey(VK_F22, flags);
        if (!VSTRCMPI(temp, "F23")) sendKey(VK_F23, flags);
        if (!VSTRCMPI(temp, "F24")) sendKey(VK_F24, flags);
        
        if (!VSTRCMPI(temp, "NUMPAD0")) sendKey(VK_NUMPAD0, flags);
        if (!VSTRCMPI(temp, "NUMPAD1")) sendKey(VK_NUMPAD1, flags);
        if (!VSTRCMPI(temp, "NUMPAD2")) sendKey(VK_NUMPAD2, flags);
        if (!VSTRCMPI(temp, "NUMPAD3")) sendKey(VK_NUMPAD3, flags);
        if (!VSTRCMPI(temp, "NUMPAD4")) sendKey(VK_NUMPAD4, flags);
        if (!VSTRCMPI(temp, "NUMPAD5")) sendKey(VK_NUMPAD5, flags);
        if (!VSTRCMPI(temp, "NUMPAD6")) sendKey(VK_NUMPAD6, flags);
        if (!VSTRCMPI(temp, "NUMPAD7")) sendKey(VK_NUMPAD7, flags);
        if (!VSTRCMPI(temp, "NUMPAD8")) sendKey(VK_NUMPAD8, flags);
        if (!VSTRCMPI(temp, "NUMPAD9")) sendKey(VK_NUMPAD9, flags);

        if (!VSTRCMPI(temp, "BACK"))        sendKey(VK_BACK,       flags);
        if (!VSTRCMPI(temp, "TAB"))         sendKey(VK_TAB,        flags);
        if (!VSTRCMPI(temp, "CLEAR"))       sendKey(VK_CLEAR,      flags);
        if (!VSTRCMPI(temp, "ENTER"))       sendKey(VK_RETURN,     flags);
        if (!VSTRCMPI(temp, "RETURN"))      sendKey(VK_RETURN,     flags);
        if (!VSTRCMPI(temp, "PAUSE"))       sendKey(VK_PAUSE,      flags);
        if (!VSTRCMPI(temp, "CAPSLOCK"))    sendKey(VK_CAPITAL,    flags);
        if (!VSTRCMPI(temp, "CAPS LOCK"))   sendKey(VK_CAPITAL,    flags);
        if (!VSTRCMPI(temp, "CAPITAL"))     sendKey(VK_CAPITAL,    flags);
        if (!VSTRCMPI(temp, "HIRAKANA"))    sendKey(VK_KANA,       flags);
        if (!VSTRCMPI(temp, "KATAKANA"))    sendKey(VK_KANA,       flags);
        if (!VSTRCMPI(temp, "JUNJA"))       sendKey(VK_JUNJA,      flags);
        if (!VSTRCMPI(temp, "FINAL"))       sendKey(VK_FINAL,      flags);
        if (!VSTRCMPI(temp, "KANJI"))       sendKey(VK_KANJI,      flags);
        if (!VSTRCMPI(temp, "ESC"))         sendKey(VK_ESCAPE,     flags);
        if (!VSTRCMPI(temp, "ESCAPE"))      sendKey(VK_ESCAPE,     flags);
        if (!VSTRCMPI(temp, "CONVERT"))     sendKey(VK_CONVERT,    flags);
        if (!VSTRCMPI(temp, "NON-CONVERT")) sendKey(VK_NONCONVERT, flags);
        if (!VSTRCMPI(temp, "ACCEPT"))      sendKey(VK_ACCEPT,     flags);
        if (!VSTRCMPI(temp, "MODE CHANGE")) sendKey(VK_MODECHANGE, flags);
        if (!VSTRCMPI(temp, "SPACE"))       sendKey(VK_SPACE,      flags);
        if (!VSTRCMPI(temp, "PRIOR"))       sendKey(VK_PRIOR,      flags);
        if (!VSTRCMPI(temp, "NEXT"))        sendKey(VK_NEXT,       flags);
        if (!VSTRCMPI(temp, "END"))         sendKey(VK_END,        flags);
        if (!VSTRCMPI(temp, "HOME"))        sendKey(VK_HOME,       flags);
        if (!VSTRCMPI(temp, "LEFT"))        sendKey(VK_LEFT,       flags);
        if (!VSTRCMPI(temp, "UP"))          sendKey(VK_UP,         flags);
        if (!VSTRCMPI(temp, "RIGHT"))       sendKey(VK_RIGHT,      flags);
        if (!VSTRCMPI(temp, "DOWN"))        sendKey(VK_DOWN,       flags);
        if (!VSTRCMPI(temp, "SELECT"))      sendKey(VK_SELECT,     flags);
        if (!VSTRCMPI(temp, "PRINT"))       sendKey(VK_PRINT,      flags);
        if (!VSTRCMPI(temp, "EXECUTE"))     sendKey(VK_EXECUTE,    flags);
        if (!VSTRCMPI(temp, "SNAPSHOT"))    sendKey(VK_SNAPSHOT,   flags);
        if (!VSTRCMPI(temp, "INSERT"))      sendKey(VK_INSERT,     flags);
        if (!VSTRCMPI(temp, "DELETE"))      sendKey(VK_DELETE,     flags);
        if (!VSTRCMPI(temp, "HELP"))        sendKey(VK_HELP,       flags);
        if (!VSTRCMPI(temp, "APPS"))        sendKey(VK_APPS,       flags);
        if (!VSTRCMPI(temp, "MULTIPLY"))    sendKey(VK_MULTIPLY,   flags);
        if (!VSTRCMPI(temp, "ADD"))         sendKey(VK_ADD,        flags);
        if (!VSTRCMPI(temp, "SEP"))         sendKey(VK_SEPARATOR,  flags);
        if (!VSTRCMPI(temp, "SEPERATOR"))   sendKey(VK_SEPARATOR,  flags);
        if (!VSTRCMPI(temp, "SUB"))         sendKey(VK_SUBTRACT,   flags);
        if (!VSTRCMPI(temp, "SUBTRACT"))    sendKey(VK_SUBTRACT,   flags);
        if (!VSTRCMPI(temp, "DOT"))         sendKey(VK_DECIMAL,    flags);
        if (!VSTRCMPI(temp, "DEC"))         sendKey(VK_DECIMAL,    flags);
        if (!VSTRCMPI(temp, "DECIMAL"))     sendKey(VK_DECIMAL,    flags);
        if (!VSTRCMPI(temp, "DEVIDE"))      sendKey(VK_DIVIDE,     flags);
        if (!VSTRCMPI(temp, "NUMLOCK"))     sendKey(VK_NUMLOCK,    flags);
        if (!VSTRCMPI(temp, "NUMBER LOCK")) sendKey(VK_NUMLOCK,    flags);
        if (!VSTRCMPI(temp, "SCROLL LOCK")) sendKey(VK_SCROLL,     flags);
        if (!VSTRCMPI(temp, "PROCESS"))     sendKey(VK_PROCESSKEY, flags);
        if (!VSTRCMPI(temp, "ATTN"))        sendKey(VK_ATTN,       flags);
        if (!VSTRCMPI(temp, "ATTENTION"))   sendKey(VK_ATTN,       flags);
        if (!VSTRCMPI(temp, "CRSEL"))       sendKey(VK_CRSEL,      flags);
        if (!VSTRCMPI(temp, "EXSEL"))       sendKey(VK_EXSEL,      flags);
        if (!VSTRCMPI(temp, "EREOF"))       sendKey(VK_EREOF,      flags);
        if (!VSTRCMPI(temp, "PLAY"))        sendKey(VK_PLAY,       flags);
        if (!VSTRCMPI(temp, "ZOOM"))        sendKey(VK_ZOOM,       flags);
        if (!VSTRCMPI(temp, "ANY KEY"))     sendKey(VK_NONAME,     flags);
        if (!VSTRCMPI(temp, "ANYKEY"))      sendKey(VK_NONAME,     flags);
        if (!VSTRCMPI(temp, "NONAME"))      sendKey(VK_NONAME,     flags);
        if (!VSTRCMPI(temp, "SLEEP"))       sendKey(VK_SLEEP,      flags);

        if (!VSTRCMPI(temp, "BROWSER BACK"))         sendKey(VK_BROWSER_BACK,        flags);
        if (!VSTRCMPI(temp, "BROWSER FORWARD"))      sendKey(VK_BROWSER_FORWARD,     flags);
        if (!VSTRCMPI(temp, "BROWSER REFRESH"))      sendKey(VK_BROWSER_REFRESH,     flags);
        if (!VSTRCMPI(temp, "BROWSER STOP"))         sendKey(VK_BROWSER_STOP,        flags);
        if (!VSTRCMPI(temp, "BROWSER SEARCH"))       sendKey(VK_BROWSER_SEARCH,      flags);
        if (!VSTRCMPI(temp, "BROWSER FAVORITES"))    sendKey(VK_BROWSER_FAVORITES,   flags);
        if (!VSTRCMPI(temp, "BROWSER HOME"))         sendKey(VK_BROWSER_HOME,        flags);
        if (!VSTRCMPI(temp, "VOLUME MUTE"))          sendKey(VK_VOLUME_MUTE,         flags);
        if (!VSTRCMPI(temp, "VOLUME UP"))            sendKey(VK_VOLUME_DOWN,         flags);
        if (!VSTRCMPI(temp, "VOLUME DOWN"))          sendKey(VK_VOLUME_UP,           flags);
        if (!VSTRCMPI(temp, "MEDIA NEXT TRACK"))     sendKey(VK_MEDIA_NEXT_TRACK,    flags);
        if (!VSTRCMPI(temp, "MEDIA PREVIOUS TRACK")) sendKey(VK_MEDIA_PREV_TRACK,    flags);
        if (!VSTRCMPI(temp, "MEDIA STOP"))           sendKey(VK_MEDIA_STOP,          flags);
        if (!VSTRCMPI(temp, "MEDIA PLAY/PAUSE"))     sendKey(VK_MEDIA_PLAY_PAUSE,    flags);
        if (!VSTRCMPI(temp, "LAUNCH MAIL"))          sendKey(VK_LAUNCH_MAIL,         flags);
        if (!VSTRCMPI(temp, "LAUNCH MEDIA"))         sendKey(VK_LAUNCH_MEDIA_SELECT, flags);
        if (!VSTRCMPI(temp, "LAUNCH APP 1"))         sendKey(VK_LAUNCH_APP1,         flags);
        if (!VSTRCMPI(temp, "LAUNCH APP 2"))         sendKey(VK_LAUNCH_APP2,         flags);

        if (!VSTRCMPI(temp, "OEM 1"))             sendKey(VK_OEM_1,          flags);
        if (!VSTRCMPI(temp, "OEM PLUS"))          sendKey(VK_OEM_PLUS,       flags);
        if (!VSTRCMPI(temp, "OEM COMMA"))         sendKey(VK_OEM_COMMA,      flags);
        if (!VSTRCMPI(temp, "OEM MINUS"))         sendKey(VK_OEM_MINUS,      flags);
        if (!VSTRCMPI(temp, "OEM PERIOD"))        sendKey(VK_OEM_PERIOD,     flags);
        if (!VSTRCMPI(temp, "OEM 2"))             sendKey(VK_OEM_2,          flags);
        if (!VSTRCMPI(temp, "OEM 3"))             sendKey(VK_OEM_3,          flags);
        if (!VSTRCMPI(temp, "OEM 4"))             sendKey(VK_OEM_4,          flags);
        if (!VSTRCMPI(temp, "OEM 5"))             sendKey(VK_OEM_5,          flags);
        if (!VSTRCMPI(temp, "OEM 6"))             sendKey(VK_OEM_6,          flags);
        if (!VSTRCMPI(temp, "OEM 7"))             sendKey(VK_OEM_7,          flags);
        if (!VSTRCMPI(temp, "OEM CLEAR"))         sendKey(VK_OEM_CLEAR,      flags);
        if (!VSTRCMPI(temp, "OEM JISHO"))         sendKey(VK_OEM_FJ_JISHO,   flags);
        if (!VSTRCMPI(temp, "OEM MASSHOU"))       sendKey(VK_OEM_FJ_MASSHOU, flags);
        if (!VSTRCMPI(temp, "OEM TOUROKU"))       sendKey(VK_OEM_FJ_TOUROKU, flags);
        if (!VSTRCMPI(temp, "OEM LEFT OYAYUBI"))  sendKey(VK_OEM_FJ_LOYA,    flags);
        if (!VSTRCMPI(temp, "OEM RIGHT OYAYUBI")) sendKey(VK_OEM_FJ_ROYA,    flags);
        if (!VSTRCMPI(temp, "OEM AX"))            sendKey(VK_OEM_AX,         flags);
        if (!VSTRCMPI(temp, "ICO HELP"))          sendKey(VK_ICO_HELP,       flags);
        if (!VSTRCMPI(temp, "ICO 00"))            sendKey(VK_ICO_00,         flags);
        if (!VSTRCMPI(temp, "ICO CLEAR"))         sendKey(VK_ICO_CLEAR,      flags);
        if (!VSTRCMPI(temp, "OEM RESET"))         sendKey(VK_OEM_RESET,      flags);
        if (!VSTRCMPI(temp, "ORM JUMP"))          sendKey(VK_OEM_JUMP,       flags);
        if (!VSTRCMPI(temp, "OEM PA1"))           sendKey(VK_OEM_PA1,        flags);
        if (!VSTRCMPI(temp, "OEM PA2"))           sendKey(VK_OEM_PA2,        flags);
        if (!VSTRCMPI(temp, "OEM PA3"))           sendKey(VK_OEM_PA3,        flags);
        if (!VSTRCMPI(temp, "OEM WSCTRL"))        sendKey(VK_OEM_WSCTRL,     flags);
        if (!VSTRCMPI(temp, "OEM CURSEL"))        sendKey(VK_OEM_CUSEL,      flags);
        if (!VSTRCMPI(temp, "OEM ATTN"))          sendKey(VK_OEM_ATTN,       flags);
        if (!VSTRCMPI(temp, "OEM FINISH"))        sendKey(VK_OEM_FINISH,     flags);
        if (!VSTRCMPI(temp, "OEM COPY"))          sendKey(VK_OEM_COPY,       flags);
        if (!VSTRCMPI(temp, "OEM AUTO"))          sendKey(VK_OEM_AUTO,       flags);
        if (!VSTRCMPI(temp, "OEM ENVW"))          sendKey(VK_OEM_ENLW,       flags);
        if (!VSTRCMPI(temp, "OEM BACKTAB"))       sendKey(VK_OEM_BACKTAB,    flags);
        if (!VSTRCMPI(temp, "PA1"))               sendKey(VK_PA1,            flags);
        if (!VSTRCMPI(temp, "PACKET"))            sendKey(VK_PACKET,         flags);

        if (!VSTRCMPI(temp, "LBRACKET"))      sendKey(LOBYTE(VkKeyScan('[')), flags);
        if (!VSTRCMPI(temp, "RBRACKET"))      sendKey(LOBYTE(VkKeyScan('[')), flags);
        if (!VSTRCMPI(temp, "LEFT BRACKET"))  sendKey(LOBYTE(VkKeyScan(']')), flags);
        if (!VSTRCMPI(temp, "RIGHT BRACKET")) sendKey(LOBYTE(VkKeyScan(']')), flags);

        free(temp);
        string = end;

      } else {
        CONSOLE_ACTIVE->printError(keyboard->getName(), "String parse error: Missing ']'");
        string = end - 1;
      }
    }

    else {

      SHORT value  = VkKeyScan(*string);
      char  vkcode = LOBYTE(value);
      char  mods   = HIBYTE(value) & 0x07;
      
      sendKey(vkcode, mods);
    }

    string++;
  }

  if (!lock) keyboard->unlock();
}


void uiKeyboardFunction::sendKey(short vkcode, DWORD flags, DWORD modifiers) {
  if (!(flags & KEYEVENTF_KEYUP)) {
    sendMod(modifiers);
  }

  INPUT inputdata;
  ZeroMemory(&inputdata, sizeof(inputdata));

  inputdata.type           = INPUT_KEYBOARD;
  inputdata.ki.wVk         = vkcode;
  inputdata.ki.dwFlags     = flags;
  inputdata.ki.dwExtraInfo = GetMessageExtraInfo();

  SendInput(1, &inputdata, sizeof(INPUT));

  if (flags & KEYEVENTF_KEYUP) {
    sendMod(modifiers);
  }
}


void uiKeyboardFunction::sendMod(DWORD modifiers, DWORD flags) {
  int count = 0;
  if (modifiers & 0x01) count++;
  if (modifiers & 0x02) count++;
  if (modifiers & 0x04) count++;
  if (modifiers & 0x08) count++;
  if (modifiers & 0x10) count++;
  if (count == 0) return;

  INPUT *inputs = (INPUT*) malloc(sizeof(INPUT) * count);
  ZeroMemory(inputs, sizeof(INPUT) * count);

  int i = 0;

  if (modifiers & 0x01) {
    inputs[i].type           = INPUT_KEYBOARD;
    inputs[i].ki.wVk         = VK_SHIFT;
    inputs[i].ki.dwFlags     = flags;
    inputs[i].ki.dwExtraInfo = GetMessageExtraInfo();
    i++;
  }

  if (modifiers & 0x02) {
    inputs[i].type           = INPUT_KEYBOARD;
    inputs[i].ki.wVk         = VK_CONTROL;
    inputs[i].ki.dwFlags     = flags;
    inputs[i].ki.dwExtraInfo = GetMessageExtraInfo();
    i++;
  }

  if (modifiers & 0x04) {
    inputs[i].type           = INPUT_KEYBOARD;
    inputs[i].ki.wVk         = VK_MENU;
    inputs[i].ki.dwFlags     = flags;
    inputs[i].ki.dwExtraInfo = GetMessageExtraInfo();
    i++;
  }

  if (modifiers & 0x08) {
    inputs[i].type           = INPUT_KEYBOARD;
    inputs[i].ki.wVk         = VK_LWIN;
    inputs[i].ki.dwFlags     = flags;
    inputs[i].ki.dwExtraInfo = GetMessageExtraInfo();
    i++;
  }

  if (modifiers & 0x10) {
    inputs[i].type           = INPUT_KEYBOARD;
    inputs[i].ki.wVk         = VK_KANA;
    inputs[i].ki.dwFlags     = flags;
    inputs[i].ki.dwExtraInfo = GetMessageExtraInfo();
    i++;
  }

  SendInput(count, inputs, sizeof(INPUT));

  free(inputs);
}
