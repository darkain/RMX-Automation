/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "wiiremote.h"


static plugWii *WII = new plugWii();


plugWii::plugWii() : plugBase("Wii Remote")/*, vTimer(5000, FALSE)*/ {
}


void plugWii::onInit() {
  enumerate();
//  startTimer();
}


void plugWii::onQuit() {
//  stopTimer();
}


hidBase *plugWii::enumDevice(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *path) {
  if (devattrib->VendorID == WII_REMOTE_VENDOR) {
    if (devattrib->ProductID == WII_REMOTE_PRODUCT) {
      return new wiiRemote(devhandle, devattrib, path);
    }
  }
  return NULL;
}


//---------------------------------------------------------------------


wiiRemote::wiiRemote(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *devpath)
 : hidWiiRemote(devhandle, devattrib, devpath), cfgTree("Wii Remote") {

  DEBUGMSG("Wii Remote", devpath);


  (insertParam("unique", devpath))->setReadOnly(TRUE);


  cfgTree *raw = new cfgTree("Raw Data", this);
  raw->setSavable(FALSE);

  new cfgNull(devpath, raw);

  rawdata = new cfgNull("Raw: NO DATA", raw);


  //Wii Remote - IR/Camera
  cursor[0] = new evntAbsolute("Camera X", this);
  cursor[1] = new evntAbsolute("Camera Y", this);
  cursor[0]->setMinMax(0, 1024);
  cursor[1]->setMinMax(0,  768);
  cursor[0]->setHint("cursorx");
  cursor[1]->setHint("cursory");
  new cfgLine(this);

  //Wii Remote - Accelerometers
  remaccl[0] = new evntRelative("Accelerometer: 1", this);
  remaccl[1] = new evntRelative("Accelerometer: 2", this);
  remaccl[2] = new evntRelative("Accelerometer: 3", this);
  remaccl[0]->setMinMax(0, 255);
  remaccl[1]->setMinMax(0, 255);
  remaccl[2]->setMinMax(0, 255);
  new cfgLine(this);

  //Wii Remote - Buttons
  for (int i=0; i<16; i++) buttons[i] = NULL;
  (buttons[ 3] = new evntPress("D-Pad Up",    this))->setHint("up"     );
  (buttons[ 2] = new evntPress("D-Pad Down",  this))->setHint("down"   );
  (buttons[ 0] = new evntPress("D-Pad Left",  this))->setHint("left"   );
  (buttons[ 1] = new evntPress("D-Pad Right", this))->setHint("right"  );
  (buttons[11] = new evntPress("A",           this))->setHint("action0");
  (buttons[10] = new evntPress("B",           this))->setHint("action1");
  (buttons[ 9] = new evntPress("1",           this))->setHint("action2");
  (buttons[ 8] = new evntPress("2",           this))->setHint("action3");
  (buttons[ 4] = new evntPress("+",           this))->setHint("accept" );
  (buttons[12] = new evntPress("-",           this))->setHint("cancel" );
  (buttons[15] = new evntPress("Home",        this))->setHint("back"   );

  
  //Nunchuk
  nunchuk = new cfgTree("Nunchuk", this);
  nunchuk->setVisible(FALSE);

  //Nunchuk - Analog Stick
  nunaxis[0] = new evntRelative("X (Analog)", nunchuk);
  nunaxis[1] = new evntRelative("Y (Analog)", nunchuk);
  nunaxis[0]->setMinMax(0, 255);
  nunaxis[1]->setMinMax(0, 255);
  new cfgLine(nunchuk);

  //Nunchuk - Accelerometer
  nunaccl[0] = new evntRelative("Accelerometer: 1", nunchuk);
  nunaccl[1] = new evntRelative("Accelerometer: 2", nunchuk);
  nunaccl[2] = new evntRelative("Accelerometer: 3", nunchuk);
  nunaccl[0]->setMinMax(0, 255);
  nunaccl[1]->setMinMax(0, 255);
  nunaccl[2]->setMinMax(0, 255);
  new cfgLine(nunchuk);

  //Nunchuk - Buttons
  nunbtns[0] = new evntPress("C", nunchuk);
  nunbtns[1] = new evntPress("Z", nunchuk);


  //Classic
  classic = new cfgTree("Classic Controller", this);
  classic->setVisible(FALSE);

  //Classic - Analog Sticks
  cscaxis[0] = new evntRelative("X (Left Analog)",  classic);
  cscaxis[1] = new evntRelative("Y (Left Analog)",  classic);
  cscaxis[2] = new evntRelative("X (Right Analog)", classic);
  cscaxis[3] = new evntRelative("Y (Right Analog)", classic);
  cscaxis[0]->setMinMax(0, 63);
  cscaxis[1]->setMinMax(0, 63);
  cscaxis[2]->setMinMax(0, 31);
  cscaxis[3]->setMinMax(0, 31);
  new cfgLine(classic);

  //Classic - Buttons
  for (int j=0; j<16; j++) cscbtns[j] = NULL;
  (cscbtns[ 0] = new evntPress("D-Pad Up",            classic))->setHint("up"   );
  (cscbtns[14] = new evntPress("D-Pad Down",          classic))->setHint("down" );
  (cscbtns[ 1] = new evntPress("D-Pad Left",          classic))->setHint("left" );
  (cscbtns[15] = new evntPress("D-Pad Right",         classic))->setHint("right");
  (cscbtns[ 4] = new evntPress("A",                   classic))->setHint("action0");
  (cscbtns[ 6] = new evntPress("B",                   classic))->setHint("action1");
  (cscbtns[ 3] = new evntPress("X",                   classic))->setHint("action2");
  (cscbtns[ 5] = new evntPress("Y",                   classic))->setHint("action3");
  (cscbtns[10] = new evntPress("+",                   classic))->setHint("accept");
  (cscbtns[12] = new evntPress("-",                   classic))->setHint("cancel");
  (cscbtns[11] = new evntPress("Home",                classic))->setHint("back"  );
  cscbtns[  7] = new evntPress("Z (Left)",            classic);
  cscbtns[  2] = new evntPress("Z (Right)",           classic);
  cscbtns[ 13] = new evntPress("L (Digital Trigger)", classic);
  cscbtns[  9] = new evntPress("R (Digital Trigger)", classic);

  //Classic - Analog Triggers
  csctrig[0] = new evntAbsolute("L (Analog Trigger)", classic);
  csctrig[1] = new evntAbsolute("R (Analog Trigger)", classic);
  csctrig[0]->setMinMax(0, 31);
  csctrig[1]->setMinMax(0, 31);
}


wiiRemote::~wiiRemote() {
}


void wiiRemote::onReadReport(unsigned char *data, int len) {
  unsigned char report = data[0];
  if (report >= 0x30  && report <= 0x3F) {
    char str[96] = "";
    VPRINTF(str, sizeof(str), "Raw: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
      data[0x00], data[0x01], data[0x02], data[0x03], data[0x04], data[0x05],
      data[0x06], data[0x07], data[0x08], data[0x09], data[0x0a], data[0x0b],
      data[0x0c], data[0x0d], data[0x0e], data[0x0f], data[0x10], data[0x11],
      data[0x12], data[0x13], data[0x14], data[0x15] );

    rawdata->setName(str);
  }

  hidWiiRemote::onReadReport(data, len);
}


void wiiRemote::onAttachmentChange(unsigned char attachment) {
  nunchuk->setVisible( attachment == 0xFE );
  classic->setVisible( attachment == 0xFD );
}


void wiiRemote::onButtons(unsigned short buttondata) {
  for (int i=0; i<16; i++) {
    if (buttons[i]) buttons[i]->setPress( buttondata & (1<<i) );
  }
}


void wiiRemote::onAccelData(unsigned char accl1, unsigned char accl2, unsigned char accl3) {
  remaccl[0]->setRelativeValue(accl1);
  remaccl[1]->setRelativeValue(accl2);
  remaccl[2]->setRelativeValue(accl3);
}


void wiiRemote::onIrData(unsigned short x, unsigned short y) {
  cursor[0]->setAbsoluteValue(x);
  cursor[1]->setAbsoluteValue(y);
}


void wiiRemote::nunchuk_onButtons(unsigned char buttondata) {
  nunbtns[0]->setPress( buttondata & 0x02 );
  nunbtns[1]->setPress( buttondata & 0x01 );
}


void wiiRemote::nunchuk_onAccelData(unsigned char accl1, unsigned char accl2, unsigned char accl3) {
  nunaccl[0]->setRelativeValue(accl1);
  nunaccl[1]->setRelativeValue(accl2);
  nunaccl[2]->setRelativeValue(accl3);
}


void wiiRemote::nunchuk_onAnalogData(unsigned char x, unsigned char y) {
  nunaxis[0]->setRelativeValue(x);
  nunaxis[1]->setRelativeValue(y);
}


void wiiRemote::classic_onButtons(unsigned short buttondata) {
  for (int i=0; i<16; i++) {
    if (cscbtns[i]) cscbtns[i]->setPress( buttondata & (1<<i) );
  }
}


void wiiRemote::classic_onAnalogDataL(unsigned char x, unsigned char y) {
  cscaxis[0]->setRelativeValue(x);
  cscaxis[1]->setRelativeValue(y);
}


void wiiRemote::classic_onAnalogDataR(unsigned char x, unsigned char y) {
  cscaxis[2]->setRelativeValue(x);
  cscaxis[3]->setRelativeValue(y);
}


void wiiRemote::classic_onAnalogDataT(unsigned char l, unsigned char r) {
  csctrig[0]->setAbsoluteValue(l);
  csctrig[1]->setAbsoluteValue(r);
}


//---------------------------------------------------------------------
/*

wiiRemote::wiiRemote(const char *name, cfgBase *parent)
: cfgTree(name, parent), wndBase(name), vTimer(2000, FALSE) {

  extmode    = 0;
//  forcewrite = FALSE;

  PROFILE();

//  hid = new CWiimoteHID();

  cfgTree *raw = new cfgTree("Raw Data", this);


  button_text = new cfgNull("No Data", raw);
  button_text->setSavable(FALSE);


  lights[0] = new wiiLight("Light 1", this);
  lights[1] = new wiiLight("Light 2", this);
  lights[2] = new wiiLight("Light 3", this);
  lights[3] = new wiiLight("Light 4", this);


  new cfgLine(this);


  camera[0] = new cfgIntSlider("Camera X", raw);
  camera[1] = new cfgIntSlider("Camera Y", raw);
  camera[0]->setMinMax(0, 1024);
  camera[1]->setMinMax(0,  768);


  cursor[0] = new evntAbsolute("Camera X", this);
  cursor[1] = new evntAbsolute("Camera Y", this);
  cursor[0]->setMinMax(0, 1024);
  cursor[1]->setMinMax(0,  768);
  cursor[0]->setHint("cursorx");
  cursor[1]->setHint("cursory");


  new cfgLine(this);


  remaccl[0] = new evntRelative("Accelerometer: 1", this);
  remaccl[1] = new evntRelative("Accelerometer: 2", this);
  remaccl[2] = new evntRelative("Accelerometer: 3", this);
  remaccl[0]->setMinMax(0, 255);
  remaccl[1]->setMinMax(0, 255);
  remaccl[2]->setMinMax(0, 255);


  new cfgLine(this);


  nunchuk = new cfgTree("Nunchuk", this);
  nunchuk->setVisible(FALSE);

  nunbtns[0] = new evntPress("C", nunchuk);
  nunbtns[1] = new evntPress("Z", nunchuk);

  new cfgLine(nunchuk);

  nunaxis[0] = new evntRelative("X (Analog)", nunchuk);
  nunaxis[1] = new evntRelative("Y (Analog)", nunchuk);
  nunaxis[0]->setMinMax(0, 255);
  nunaxis[1]->setMinMax(0, 255);

  new cfgLine(nunchuk);

  nunaccl[0] = new evntRelative("Accelerometer: 1", nunchuk);
  nunaccl[1] = new evntRelative("Accelerometer: 2", nunchuk);
  nunaccl[2] = new evntRelative("Accelerometer: 3", nunchuk);
  nunaccl[0]->setMinMax(0, 255);
  nunaccl[1]->setMinMax(0, 255);
  nunaccl[2]->setMinMax(0, 255);



  classic = new cfgTree("Classic Controller", this);
  classic->setVisible(FALSE);

  cscaxis[0] = new evntRelative("X (Left Analog)",  classic);
  cscaxis[1] = new evntRelative("Y (Left Analog)",  classic);
  cscaxis[2] = new evntRelative("X (Right Analog)", classic);
  cscaxis[3] = new evntRelative("Y (Right Analog)", classic);

  cscaxis[0]->setMinMax(0, 63);
  cscaxis[1]->setMinMax(0, 63);
  cscaxis[2]->setMinMax(0, 31);
  cscaxis[3]->setMinMax(0, 31);


  new cfgLine(classic);

  
  for (int j=0; j<16; j++) cscbtns[j] = NULL;
  
  (cscbtns[ 0] = new evntPress("D-Pad Up",    classic))->setHint("up"   );
  (cscbtns[14] = new evntPress("D-Pad Down",  classic))->setHint("down" );
  (cscbtns[ 1] = new evntPress("D-Pad Left",  classic))->setHint("left" );
  (cscbtns[15] = new evntPress("D-Pad Right", classic))->setHint("right");

  (cscbtns[ 4] = new evntPress("A", classic))->setHint("action0");
  (cscbtns[ 6] = new evntPress("B", classic))->setHint("action1");
  (cscbtns[ 3] = new evntPress("X", classic))->setHint("action2");
  (cscbtns[ 5] = new evntPress("Y", classic))->setHint("action3");

  (cscbtns[10] = new evntPress("+",    classic))->setHint("accept");
  (cscbtns[12] = new evntPress("-",    classic))->setHint("cancel");
  (cscbtns[11] = new evntPress("Home", classic))->setHint("back"  );

  cscbtns[  7] = new evntPress("Z (Left)",    classic);
  cscbtns[  2] = new evntPress("Z (Right)",   classic);

  cscbtns[ 13] = new evntPress("L (Digital Trigger)", classic);
  cscbtns[  9] = new evntPress("R (Digital Trigger)", classic);

  csctrig[0] = new evntAbsolute("L (Analog Trigger)", classic);
  csctrig[1] = new evntAbsolute("R (Analog Trigger)", classic);

  csctrig[0]->setMinMax(0, 31);
  csctrig[1]->setMinMax(0, 31);


  for (int x=0; x<3; x++) {
    char str[32] = "";
    VPRINTF(str, sizeof(str), "Remote Accelerometer: %d", x+1);
    sliders[x] = new cfgIntSlider(str, raw);
    sliders[x]->setMinMax(0, 255);
    sliders[x]->setSavable(FALSE);
  }

  for (int x=0; x<3; x++) {
    char str[32] = "";
    VPRINTF(str, sizeof(str), "Nunchuk Accelerometer: %d", x+1);
    sliders[x+3] = new cfgIntSlider(str, raw);
    sliders[x+3]->setMinMax(0, 255);
    sliders[x+3]->setSavable(FALSE);
  }


  for (int i=0; i<16; i++) buttons[i] = NULL;

  (buttons[ 3] = new evntPress("D-Pad Up",    this))->setHint("up"   );
  (buttons[ 2] = new evntPress("D-Pad Down",  this))->setHint("down" );
  (buttons[ 0] = new evntPress("D-Pad Left",  this))->setHint("left" );
  (buttons[ 1] = new evntPress("D-Pad Right", this))->setHint("right");

  (buttons[11] = new evntPress("A", this))->setHint("action0");
  (buttons[10] = new evntPress("B", this))->setHint("action1");

  (buttons[ 9] = new evntPress("1", this))->setHint("action2");
  (buttons[ 8] = new evntPress("2", this))->setHint("action3");

  (buttons[ 4] = new evntPress("+",    this))->setHint("accept");
  (buttons[12] = new evntPress("-",    this))->setHint("cancel");
  (buttons[15] = new evntPress("Home", this))->setHint("back"  );

  if (hid.connect(WII_REMOTE_VENDOR, WII_REMOTE_PRODUCT)) {
    CONSOLE_MAIN->print(cfgBase::getName(), "Successfully Connected");
    updateLights();

    wndBase::init();
    startTimer();
    Begin(THREAD_PRIORITY_ABOVE_NORMAL);

  } else {
    CONSOLE_MAIN->printError(cfgBase::getName(), "ERROR CONNECTING");
  }
}




wiiRemote::~wiiRemote() {
  setInputMode(0x00);  //Disable reports

  PROFILE_STR("Ending Thread");
  End();
  PROFILE_STR("Closing HID");
//  vdelnull(hid);
  hid.disconnect();
  PROFILE_STR("Finished");
}


void wiiRemote::onTimer(DWORD systemTime) {
  stopTimer();

  CONSOLE_MAIN->printWarning(cfgBase::getName(), "Buggy BlueTooth stack detected");

  hid.setForceWrite(TRUE);

  updateLights();
  setInputMode(0x37);
  enableCamera();
  checkExt();
}


DWORD wiiRemote::ThreadProc() {
  unsigned char readdata[256];

  setInputMode(0x37);
  enableCamera();
  checkExt();
  
  while (isRunning()) {
    Sleep(1);

    readdata[0] = 0x00;
    if (hid.readReport(readdata, sizeof(readdata))) {
      beginThreadSafe();

        unsigned char *msg = (unsigned char*) malloc(sizeof(readdata));
        memcpy(msg, readdata, sizeof(readdata));
        buffers.appendItem(msg);

      endThreadSafe();

      postMessage(WM_USER+1);
    }
  }

  PROFILE_STR("ThreadProc");

  return FALSE;
}


VUINT wiiRemote::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg = WM_USER+1) {
    if (isStarted()) stopTimer();

    beginThreadSafe();

      unsigned char *buf;
      while (buf = buffers.getFirstItem()) {

        BOOL print_data = FALSE;

        if (buf[0] >= 0x30  &&  buf[0] <= 0x3F) {
          processInput(buf);
        }

        else if (buf[0] == 0x20) {
          if (buf[3] & 0x02) {              //attachment attached
            DEBUGMSG(cfgBase::getName(), "PLUG IN");
            setInputMode(0x37);
            writeByte(0x04A40040, 0x00);    //enable attachment
            checkExt();                     //query attachment type

          } else {                          //attachment removed
            setInputMode(0x37);
            DEBUGMSG(cfgBase::getName(), "PLUG OUT");
            nunchuk->setVisible(FALSE);
            classic->setVisible(FALSE);
          }
        }


        else if (buf[0] == 0x21) {        //attachment query return
          if (buf[3] == 0x10  && buf[5] == 0xFE) {
            if (buf[6] == 0xFF  ||  buf[7] == 0xFF) {
              CONSOLE_MAIN->printError(cfgBase::getName(), "Extension Error");
              extmode = 0;
              nunchuk->setVisible(FALSE);
              classic->setVisible(FALSE);

            } else if (buf[6] == 0x00  ||  buf[7] == 0x00) {
              CONSOLE_MAIN->printWarning(cfgBase::getName(), "No Extension");
              extmode = 0;
              nunchuk->setVisible(FALSE);
              classic->setVisible(FALSE);

            } else if (buf[6] == 0xFE  ||  buf[7] == 0xFE) {
              CONSOLE_MAIN->print(cfgBase::getName(), "Nunchuk Detected");
              extmode = 0xFE;
              nunchuk->setVisible(TRUE);
              classic->setVisible(FALSE);

            } else if (buf[6] == 0xFD  ||  buf[7] == 0xFD) {
              CONSOLE_MAIN->print(cfgBase::getName(), "Classic Controller Detected");
              extmode = 0xFD;
              nunchuk->setVisible(FALSE);
              classic->setVisible(TRUE);

            } else {
              nunchuk->setVisible(FALSE);
              classic->setVisible(FALSE);

              char str[32] = "";
              VPRINTF(str, sizeof(str), "Unknown Extension : %02X %02X", buf[6], buf[7]);
              CONSOLE_MAIN->printWarning(cfgBase::getName(), str);
              extmode = 0;
              print_data = TRUE;
            }

          } else {
            print_data = TRUE;
          }
        }

        else {
          print_data = TRUE;
        }

        if (print_data) {
          char str[96] = "";
          VPRINTF(str, sizeof(str), "%02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
            buf[0x00], buf[0x01], buf[0x02], buf[0x03], buf[0x04], buf[0x05],
            buf[0x06], buf[0x07], buf[0x08], buf[0x09], buf[0x0a], buf[0x0b],
            buf[0x0c], buf[0x0d], buf[0x0e], buf[0x0f], buf[0x10], buf[0x11],
            buf[0x12], buf[0x13], buf[0x14], buf[0x15] );
          DEBUGMSG(cfgBase::getName(), str);
        }


        free(buf);
        buffers.removeItem(buf);
      }

    endThreadSafe();
  }
  return wndBase::wndProc(msg, param1, param2);
}



void wiiRemote::updateLights() {
  int leds = 0;

  leds |= ( lights[0]->getValue() << 4 );
  leds |= ( lights[1]->getValue() << 5 );
  leds |= ( lights[2]->getValue() << 6 );
  leds |= ( lights[3]->getValue() << 7 );

  reportByte(0x11, leds);
}


void wiiRemote::checkExt() {
  readRequest(0x04A400FE, 0x02);
}


BOOL wiiRemote::readRequest(unsigned int addr, unsigned short bytes) {
  unsigned char data[7];
  data[0] = 0x17;
  data[1] = (addr  >> 24) & 0xFF;
  data[2] = (addr  >> 16) & 0xFF;
  data[3] = (addr  >>  8) & 0xFF;
  data[4] = (addr  >>  0) & 0xFF;
  data[5] = (bytes >>  8) & 0xFF;
  data[6] = (bytes >>  0) & 0xFF;
  return hid.writeReport(data, sizeof(data));
}


BOOL wiiRemote::writeData(unsigned int addr, unsigned char *indata, unsigned char bytes) {
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

  return hid.writeReport(data, sizeof(data));
}


BOOL wiiRemote::writeByte(unsigned int addr, unsigned char byte) {
  return writeData(addr, &byte, 1);
}


BOOL wiiRemote::reportByte(unsigned char report, unsigned char byte) {
  unsigned char data[2];
  data[0] = report;
  data[1] = byte;
  return hid.writeReport(data, sizeof(data));
}


BOOL wiiRemote::setInputMode(unsigned char mode) {
  unsigned char data[3];
  data[0] = 0x12;
  data[1] = 0x00;
  data[2] = mode;
  return hid.writeReport(data, sizeof(data));
}


void wiiRemote::enableCamera() {
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


void wiiRemote::processInput(unsigned char *data) {
  char str[96] = "";
  VPRINTF(str, sizeof(str), "RAW: %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X %02X",
    data[0x00], data[0x01], data[0x02], data[0x03], data[0x04], data[0x05],
    data[0x06], data[0x07], data[0x08], data[0x09], data[0x0a], data[0x0b],
    data[0x0c], data[0x0d], data[0x0e], data[0x0f], data[0x10], data[0x11],
    data[0x12], data[0x13], data[0x14], data[0x15] );

  button_text->setName(str);
  unsigned short btn = data[1] | (data[2] << 8);


  unsigned char *acc = NULL;
  unsigned char *ir  = NULL;
  unsigned char *ext = NULL;


  if (data[0] == 0x32  ||  data[0] == 0x34) {
    ext = data +  3;
  } else if (data[0] == 0x31) {
    acc = data +  3;
  } else if (data[0] == 0x33) {
    acc = data +  3;
    ir  = data +  6;
  } else if (data[0] == 0x35) {
    acc = data +  3;
    ext = data +  6;
  } else if (data[0] == 0x36) {
    ir  = data +  3;
    ext = data + 13;
  } else if (data[0] == 0x37) {
    acc = data +  3;
    ir  = data +  6;
    ext = data + 16;
  } else if (data[0] == 0x3D) {
    ext = data +  1;
  }


  //process button data
  for (int i=0; i<16; i++) {
    if (buttons[i]) buttons[i]->setPress( btn & (1<<i) );
  }


  //accelerometer data
  if (acc) {
    sliders[0]->setValue(acc[0]);
    sliders[1]->setValue(acc[1]);
    sliders[2]->setValue(acc[2]);

    remaccl[0]->setRelativeValue(acc[0]);
    remaccl[1]->setRelativeValue(acc[1]);
    remaccl[2]->setRelativeValue(acc[2]);
  }


  //camera data
  if (ir) {
    double dx = 0;
    double dy = 0;
    int x[4];
    int y[4];
    int count = 0;

    x[0] = ir[0] | ((ir[2] & 0x30) << 4);
    x[1] = ir[3] | ((ir[2] & 0x03) << 8);
    x[2] = ir[5] | ((ir[7] & 0x30) << 4);
    x[3] = ir[8] | ((ir[7] & 0x03) << 8);

    y[0] = ir[1] | ((ir[2] & 0xC0) << 2);
    y[1] = ir[4] | ((ir[2] & 0x0C) << 6);
    y[2] = ir[6] | ((ir[7] & 0xC0) << 2);
    y[3] = ir[9] | ((ir[7] & 0x0C) << 6);

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

      camera[0]->setUserDataNumber(dx);
      camera[1]->setUserDataNumber(dy);

      cursor[0]->setAbsoluteValue(dx);
      cursor[1]->setAbsoluteValue(dy);
    }
  }


  //Check to verify that EXT data is valid
  if (ext) {
    for (int cnt=0; cnt<6; cnt++) {
      if (ext[cnt] != 0xFF) break;
      if (cnt == 5) ext = NULL;
    }
  }

  //process EXT data
  if (ext) {

    //Nunchuk extension
    if (extmode == 0xFE) {

      //Analog Stick
      for (int i=0; i<5; i++) ext[i] = (ext[i] ^ 0x17) + 0x17;
      nunaxis[0]->setRelativeValue(       ext[0] );
      nunaxis[1]->setRelativeValue( 255 - ext[1] );

      //Accelerometers
      nunaccl[0]->setRelativeValue( ext[2] );
      nunaccl[1]->setRelativeValue( ext[3] );
      nunaccl[2]->setRelativeValue( ext[4] );

      //Accelerometers (raw data output)
      sliders[3]->setValue(ext[2]);
      sliders[4]->setValue(ext[3]);
      sliders[5]->setValue(ext[4]);

      //Buttons  //TODO: figure out the values based on proper EXT data
      int z = !(ext[5] & 0x01);
      int c = (!((ext[5] >> 1) & 0x01)) ^ z;
      nunbtns[0]->setPress( c );
      nunbtns[1]->setPress( z );


    //Classic Controller extension
    } else if ( extmode == 0xFD) {
      for (int i=0; i<6; i++) ext[i] = (ext[i] ^ 0x17) + 0x17;

      //Buttons
      unsigned short btndata = ext[5] | (ext[4] << 8);
      for (int id=0; id<16; id++) {
        if (cscbtns[id]) cscbtns[id]->setPress( !(btndata & (1<<id)) );
      }

      //X - Left Analog
      cscaxis[0]->setRelativeValue( ext[0] & 0x3F );

      //Y - Left Analog
      cscaxis[1]->setRelativeValue( ext[1] & 0x3F );

      //X - Right Analog
      cscaxis[2]->setRelativeValue( ((ext[0] & 0xC0)>>3) | ((ext[1] & 0xC0)>>5) | ((ext[2] & 0x80)>>7) );

      //Y - Right Analog
      cscaxis[3]->setRelativeValue( ext[2] & 0x1F );

      //L Trigger (Analog Data)
      csctrig[0]->setAbsoluteValue( ((ext[2] & 0x60)>>2) | ((ext[3] & 0xE0)>>5) );

      //R Trigger (Analog Data)
      csctrig[1]->setAbsoluteValue( ext[3] & 0x1F );
    }
  }

}
*/
