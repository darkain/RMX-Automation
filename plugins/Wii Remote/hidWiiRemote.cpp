/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "hidWiiRemote.h"


hidWiiRemote::hidWiiRemote(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *devpath) 
 : hidBase(devhandle, devattrib, devpath) {

  attachment = 0x00;
  mode       = 0x00;

  setLights(0x6);
  setInputMode(0x37);
  enableCamera();
  checkAttachment();
}


hidWiiRemote::~hidWiiRemote() {
  setInputMode(0x30);
}



void hidWiiRemote::onReadReport(unsigned char *data, int len) {
  unsigned char report = data[0];

  BOOL print_data = FALSE;

  if (report >= 0x30  &&  report <= 0x3F) {
    processInput(data, len);
  }

  else if (report == 0x20) {
    processAttachmentRequest(data, len);
  }

  else if (report == 0x21) {
    processAttachmentQuery(data, len);
  }

  else {
    print_data = TRUE;
  }

  if (print_data  &&  len > 0x15) {
    char str[96] = "";
    VPRINTF(str, sizeof(str), "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
      data[0x00], data[0x01], data[0x02], data[0x03], data[0x04], data[0x05],
      data[0x06], data[0x07], data[0x08], data[0x09], data[0x0a], data[0x0b],
      data[0x0c], data[0x0d], data[0x0e], data[0x0f], data[0x10], data[0x11],
      data[0x12], data[0x13], data[0x14], data[0x15] );
    DEBUGMSG("Wii Remote", str);
  }
}


BOOL hidWiiRemote::readRequest(unsigned int addr, unsigned short bytes) {
  unsigned char data[7];
  data[0] = 0x17;
  data[1] = (addr  >> 24) & 0xFF;
  data[2] = (addr  >> 16) & 0xFF;
  data[3] = (addr  >>  8) & 0xFF;
  data[4] = (addr  >>  0) & 0xFF;
  data[5] = (bytes >>  8) & 0xFF;
  data[6] = (bytes >>  0) & 0xFF;
  return writeReport(data, sizeof(data));
}


BOOL hidWiiRemote::writeData(unsigned int addr, unsigned char *indata, unsigned char bytes) {
  if (bytes > 17) return FALSE;

  unsigned char data[23];
  ZeroMemory(data, sizeof(data));
  data[0] = 0x16;
  data[1] = (addr >> 24) & 0xFF;
  data[2] = (addr >> 16) & 0xFF;
  data[3] = (addr >>  8) & 0xFF;
  data[4] = (addr >>  0) & 0xFF;
  data[5] = bytes;

  for (int i=0; i<bytes; i++) data[i+6] = indata[i];

  return writeReport(data, sizeof(data));
}


BOOL hidWiiRemote::writeByte(unsigned int addr, unsigned char byte) {
  return writeData(addr, &byte, 1);
}


BOOL hidWiiRemote::setInputMode(unsigned char inputmode) {
  mode = inputmode;

  unsigned char data[3];
  data[0] = 0x12;
  data[1] = 0x00;
  data[2] = mode;
  return writeReport(data, sizeof(data));
}


BOOL hidWiiRemote::setLights(unsigned char lights) {
  return reportByte(0x11, (lights << 4));
}


void hidWiiRemote::checkAttachment() {
  readRequest(0x04A400FE, 0x02);
}


void hidWiiRemote::enableCamera() {
  //enable
  reportByte(0x13, 0x04);
  reportByte(0x1A, 0x04);

  //calibration : TODO: pull data from remote's internal stuff
  writeByte(0x04B00030, 0x08);
  writeByte(0x04B00006, 0x90);
  writeByte(0x04B00008, 0xC0);
  writeByte(0x04B0001A, 0x40);

  writeByte(0x04B00033, 0x01);  //0x01 = standard (10 bytes).  0x03 = extended (12 bytes)
}


//TODO:  tons of range checking
void hidWiiRemote::processInput(unsigned char *data, int len) {
  unsigned char report = data[0];

  if (report == 0x30  &&  mode != 0x30) {
    CONSOLE_MAIN->printWarning("Wii Remote", "Switching to compatibility mode");
    setForceWrite(TRUE);
    setLights(0x6);
    setInputMode(mode);
    enableCamera();
    checkAttachment();
    return;
  }

  switch (data[0]) {
    case 0x31:
      onButtons(data[1] | (data[2] << 8));
      onAccelData(data[3], data[4], data[5]);
      break;

    case 0x32:
      onButtons(data[1] | (data[2] << 8));
      processInputExt(&data[3], 8);
      break;

    case 0x33:
      onButtons(data[1] | (data[2] << 8));
      onAccelData(data[3], data[4], data[5]);
      processInputIr(&data[6], 12);
      break;

    case 0x34:
      onButtons(data[1] | (data[2] << 8));
      processInputExt(&data[3], 19);
      break;

    case 0x35:
      onButtons(data[1] | (data[2] << 8));
      onAccelData(data[3], data[4], data[5]);
      processInputExt(&data[6], 16);
      break;

    case 0x36:
      onButtons(data[1] | (data[2] << 8));
      processInputIr(&data[3], 10);
      processInputExt(&data[13], 9);
      break;

    case 0x37:
      onButtons(data[1] | (data[2] << 8));
      onAccelData(data[3], data[4], data[5]);
      processInputIr(&data[6], 10);
      processInputExt(&data[16], 6);
      break;

    case 0x3d:
      processInputExt(&data[1], 21);
  }
}


void hidWiiRemote::processInputIr(unsigned char *data, int len) {
  double dx = 0;
  double dy = 0;
  int x[4];
  int y[4];
  int count = 0;

  x[0] = data[0] | ((data[2] & 0x30) << 4);
  x[1] = data[3] | ((data[2] & 0x03) << 8);
  x[2] = data[5] | ((data[7] & 0x30) << 4);
  x[3] = data[8] | ((data[7] & 0x03) << 8);

  y[0] = data[1] | ((data[2] & 0xC0) << 2);
  y[1] = data[4] | ((data[2] & 0x0C) << 6);
  y[2] = data[6] | ((data[7] & 0xC0) << 2);
  y[3] = data[9] | ((data[7] & 0x0C) << 6);

  for (int i=0; i<1; i++) {  //TODO:  find some good math to smooth out all 4 values together
    if (x[i] != 1023  &&  y[i] != 1023) {
      count++;
      dx += x[i];
      dy += y[i];
    }
  }

  if (count > 0) {
    dx = 1024 - (dx / count);
    dy =        (dy / count);
    onIrData((unsigned short)dx, (unsigned short)dy);
  }
}


void hidWiiRemote::processInputExt(unsigned char *data, int len) {
  int i;

  for (i=0; i<len; i++) {
    if (data[i] != 0xFF) break;
    if (i == len-1) return;     //no data to process
  }

  for (i=0; i<len; i++) {
    data[i] = (data[i] ^ 0x17) + 0x17;
  }

  switch (attachment) {
    case 0xFE:
      processInputNunchuk(data, len);
      break;

    case 0xFD:
      processInputClassic(data, len);
      break;

    //TODO: add more attechment types here
  }
}


void hidWiiRemote::processInputNunchuk(unsigned char *data, int len) {
  nunchuk_onButtons( (~data[5] & 0x03) );
  nunchuk_onAccelData(data[2], data[3], data[4]);
  nunchuk_onAnalogData(data[0], 255 - data[1]);
}


void hidWiiRemote::processInputClassic(unsigned char *data, int len) {
  unsigned char analog[6];
  analog[0] =   data[0] & 0x3F;
  analog[1] =   data[1] & 0x3F;
  analog[2] = ((data[0] & 0xC0)>>3) | ((data[1] & 0xC0)>>5) | ((data[2] & 0x80)>>7);
  analog[3] =   data[2] & 0x1F;
  analog[4] = ((data[2] & 0x60)>>2) | ((data[3] & 0xE0)>>5);
  analog[5] =   data[3] & 0x1F;

  classic_onButtons( ~(data[5] | (data[4] << 8)) );
  classic_onAnalogDataL(analog[0], analog[1]);
  classic_onAnalogDataR(analog[2], analog[3]);
  classic_onAnalogDataT(analog[4], analog[5]);
}


void hidWiiRemote::processAttachmentRequest(unsigned char *data, int len) {
  if (data[3] & 0x02) {  //attachment added
    DEBUGMSG("Wii Remote", "Attachment Added");
    setInputMode(0x37);
    writeByte(0x04A40040, 0x00);    //enable attachment
    checkAttachment();              //query attachment type

  } else {               //attachment removed
    DEBUGMSG("Wii Remote", "Attachment Removed");
    setInputMode(0x37);
    attachment = 0x00;
    onAttachmentChange(attachment);
  }
}


void hidWiiRemote::processAttachmentQuery(unsigned char *data, int len) {
  if (data[3] == 0x10  && data[5] == 0xFE) {
    attachment = data[6];

    switch (attachment) {
      case 0xFF:
        CONSOLE_MAIN->printError("Wii Remote", "Attachment: ERROR");
        break;

      case 0x00:
        CONSOLE_MAIN->print("Wii Remote", "Attachment: None");
        break;

      case 0xFE:
        CONSOLE_MAIN->print("Wii Remote", "Attachment: Nunchuk");
        break;

      case 0xFD:
        CONSOLE_MAIN->print("Wii Remote", "Attachment: Classic Controller");
        break;

      default: {
        char str[48]="";
        VPRINTF(str, sizeof(str), "Attachment: Unknown (%02X)", attachment);
        CONSOLE_MAIN->printWarning("Wii Remote", str);
      }
    }

    onAttachmentChange(attachment);
  }
}
