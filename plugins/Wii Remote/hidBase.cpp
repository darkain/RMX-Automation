/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#pragma comment(lib, "hid.lib")
#pragma comment(lib, "setupapi.lib")


#include "../../sdk/rmxBase.h"
#include "hidBase.h"
#include <setupapi.h>


hidBase::hidBase(HANDLE devhandle, HIDD_ATTRIBUTES *devattrib, const char *devpath)
 : path(devpath), vMessageWnd("RMX HID") {

  forcewrite = FALSE;
  hdevice = devhandle;
  
  ZeroMemory(&attrib, sizeof(attrib));
  memcpy(&attrib, devattrib, devattrib->Size);

  rmxUtf8ToWide devicePath(devpath);
  hwrite = CreateFile(devicePath.getBuffer(), GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
  hread  = CreateFile(devicePath.getBuffer(), GENERIC_READ,  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

  hevent = CreateEvent(NULL, TRUE, TRUE, L"");

  ZeroMemory(&overlap, sizeof(overlap));
  overlap.hEvent = hevent;

  ZeroMemory(&caps, sizeof(caps));
  PHIDP_PREPARSED_DATA parsed;
  HidD_GetPreparsedData(hdevice, &parsed);
  HidP_GetCaps(parsed, &caps);
  HidD_FreePreparsedData(parsed);

  HidD_SetNumInputBuffers(hdevice, 250);

  vMessageWnd::init();
  Begin(THREAD_PRIORITY_ABOVE_NORMAL);
}


hidBase::~hidBase() {
  vclose(hwrite );
  vclose(hread  );
  vclose(hdevice);
  vclose(hevent );

  End();
//  disconnect();
}


DWORD hidBase::ThreadProc() {
  PROFILE_STR("Thread");

  unsigned char *buffer = (unsigned char*) malloc(caps.InputReportByteLength);

  while (isRunning()  &&  canRead()) {
    Sleep(1);

    buffer[0] = 0x00;  //see notes at: http://msdn2.microsoft.com/en-us/library/ms790957.aspx
    if (readReport(buffer, caps.InputReportByteLength) &&  buffer[0] != 0x00) {

      beginThreadSafe();

        BOOL needPost = !buffers.hasItems();

        unsigned char *msg = (unsigned char*) malloc(caps.InputReportByteLength);
        memcpy(msg, buffer, caps.InputReportByteLength);
        buffers.appendItem(msg);

      endThreadSafe();

      if (needPost) sendMessage(WM_USER+1);
    }
  }

  PROFILE_STR("Thread");
  free(buffer);
  return FALSE;
}


VUINT hidBase::wndProc(UINT msg, VUINT param1, VUINT param2) {
  if (msg = WM_USER+1) {
//TODO:  re-implement this
//    if (isStarted()) stopTimer();

    beginThreadSafe();

      unsigned char *data = buffers.getFirstItem();
      while (data) {
        onReadReport(data, caps.InputReportByteLength);
        buffers.removeItem(data);
        data = buffers.getFirstItem();
      }

    endThreadSafe();
    return 0;
  }

  return vMessageWnd::wndProc(msg, param1, param2);
}



/*
BOOL hidBase::connect(unsigned short vendor, unsigned short product) {
  HANDLE hinfo = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);

  SP_DEVICE_INTERFACE_DATA devinfo;
  ZeroMemory(&devinfo, sizeof(devinfo));
  devinfo.cbSize =     sizeof(devinfo);

  int index = 0;
  while (SetupDiEnumDeviceInterfaces(hinfo, 0, &guid, index++, &devinfo)) {
    DWORD bufflen = 0;
    SetupDiGetDeviceInterfaceDetail(hinfo, &devinfo, NULL, 0, &bufflen, NULL);

    SP_DEVICE_INTERFACE_DETAIL_DATA *data = (SP_DEVICE_INTERFACE_DETAIL_DATA*) malloc(bufflen);
    ZeroMemory(data, bufflen);
    data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

    SetupDiGetDeviceInterfaceDetail(hinfo, &devinfo, data, bufflen, &bufflen, NULL);

	HANDLE file = CreateFile(data->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

    HIDD_ATTRIBUTES attrib;
    ZeroMemory(&attrib, sizeof(attrib));
	attrib.Size =       sizeof(attrib);

    if (HidD_GetAttributes(file, &attrib)) {
      if (attrib.VendorID == vendor  &&  attrib.ProductID == product) {
        hdevice = file;

        hwrite = CreateFile(data->DevicePath, GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);
        hread  = CreateFile(data->DevicePath, GENERIC_READ,  FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

        hevent = CreateEvent(NULL, TRUE, TRUE, L"");
        ZeroMemory(&overlap, sizeof(overlap));
        overlap.hEvent = hevent;

        PHIDP_PREPARSED_DATA parsed;
        HidD_GetPreparsedData(hdevice, &parsed);
        HidP_GetCaps(parsed, &caps);
        HidD_FreePreparsedData(parsed);

        HidD_SetNumInputBuffers(hdevice, 250);

        SetupDiDestroyDeviceInfoList(hinfo);
        return TRUE;
      }

    } else {
      vclose(file);
    }

  }

  SetupDiDestroyDeviceInfoList(hinfo);
  return FALSE;
}


void hidBase::disconnect() {
  vclose(hwrite );
  vclose(hread  );
  vclose(hdevice);
  ZeroMemory(&hevent, sizeof(hevent));
}
*/

BOOL hidBase::readReport(unsigned char *data, int len) {
  if (!canRead()) return FALSE;

  if (!forcewrite) {
    if (HidD_GetInputReport(hread, data, len)) {
      return TRUE;
    }
  }

  DWORD bytes = 0;
  ReadFile(hread, data, len, &bytes, &overlap);
  
  switch (WaitForSingleObject(hevent, 3000)) {
    case WAIT_OBJECT_0:
      ResetEvent(hevent);
      return TRUE;

    case WAIT_TIMEOUT:
      CancelIo(hread);
      break;
  }

  ResetEvent(hevent);
//  disconnect();  //TODO: do something about this
  return FALSE;
}


BOOL hidBase::writeReport(const unsigned char *data, int len) {
  if (!canWrite()) return FALSE;

  int buflen = caps.OutputReportByteLength;
  void *buffer = malloc(buflen);
  ZeroMemory(buffer, buflen);

  if (len > buflen) len = buflen;
  memcpy(buffer, data, len);


  BOOL ret = FALSE;

  if (!forcewrite) {
    ret = HidD_SetOutputReport(hwrite, buffer, buflen);
  }

  if (!ret) {
    DWORD bytes = 0;
    ret = WriteFile(hwrite, buffer, buflen, &bytes, &overlap);
  }

  free(buffer);
  return ret;
}


BOOL hidBase::reportByte(unsigned char report, unsigned char byte) {
  unsigned char data[2];
  data[0] = report;
  data[1] = byte;
  return writeReport(data, sizeof(data));
}
