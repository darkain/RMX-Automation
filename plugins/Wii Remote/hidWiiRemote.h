/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#ifndef _HID_WII_H_
#define _HID_WII_H_


#include "hidEnum.h"
#include "hidBase.h"


class hidWiiRemote : public hidBase {
  public:
    hidWiiRemote(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *devpath);
    virtual ~hidWiiRemote();

  protected:
    virtual void onReadReport(unsigned char *data, int len);

    virtual void onAttachmentChange(unsigned char attachment) {}

    virtual void onButtons(unsigned short buttons) {}
    virtual void onAccelData(unsigned char accl1, unsigned char accl2, unsigned char accl3) {}
    virtual void onIrData(unsigned short x, unsigned short y) {}

    virtual void nunchuk_onButtons(unsigned char buttons) {}
    virtual void nunchuk_onAccelData(unsigned char accl1, unsigned char accl2, unsigned char accl3) {}
    virtual void nunchuk_onAnalogData(unsigned char x, unsigned char y) {}

    virtual void classic_onButtons(unsigned short buttons) {}
    virtual void classic_onAnalogDataL(unsigned char x, unsigned char y) {}  //left analog stick
    virtual void classic_onAnalogDataR(unsigned char x, unsigned char y) {}  //right analog stick
    virtual void classic_onAnalogDataT(unsigned char l, unsigned char r) {}  //left and right analog triggers

  public:
    BOOL readRequest(unsigned int addr, unsigned short bytes);
    BOOL writeData(unsigned int addr, unsigned char *indata, unsigned char bytes);
    BOOL writeByte(unsigned int addr, unsigned char byte);

    BOOL setInputMode(unsigned char inputmode);
    BOOL setLights(unsigned char lights);
    void checkAttachment();
    void enableCamera();


  private:
    void processInput(unsigned char *data, int len);
    void processInputIr(unsigned char *data, int len);

    void processInputExt(unsigned char *data, int len);
    void processInputNunchuk(unsigned char *data, int len);
    void processInputClassic(unsigned char *data, int len);

    void processAttachmentRequest(unsigned char *data, int len);
    void processAttachmentQuery(unsigned char *data, int len);

  private:
    unsigned char attachment;
    unsigned char mode;
};


#endif //_HID_WII_H_
