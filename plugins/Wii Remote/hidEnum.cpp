/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


#include "../../sdk/rmxBase.h"
#include "hidEnum.h"
#include <setupapi.h>


hidEnum::hidEnum() {
  HidD_GetHidGuid(&guid);
}


hidEnum::~hidEnum() {
  hidBase *device = devices.getFirstItem();
  while (device) {
    delete device;
    devices.removeItem(device);
    device = devices.getFirstItem();
  }
}


hidBase *hidEnum::getDevice(unsigned short vendor, unsigned short product) const {
  hidBase *device = devices.getFirstItem();
  while (device) {
    if (device->getVendor() == vendor  &&  device->getProduct() == product) {
      return device;
    }
    device = devices.getNextItem(device);
  }
  return NULL;
}


hidBase *hidEnum::getDevice(const char *path) const {
  hidBase *device = devices.getFirstItem();
  while (device) {
    if (!VSTRCMP(device->getPath(), path)) {
      return device;
    }
    device = devices.getNextItem(device);
  }
  return NULL;
}


void hidEnum::enumerate() {
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

	HANDLE devhandle = CreateFile(data->DevicePath, 0, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

    HIDD_ATTRIBUTES attrib;
    ZeroMemory(&attrib, sizeof(attrib));
	attrib.Size =       sizeof(attrib);

    if (HidD_GetAttributes(devhandle, &attrib)) {
      rmxToUtf8 path(data->DevicePath);
      hidBase *item = getDevice(path.getBuffer());

      if (!item) {
        item = enumDevice(devhandle, &attrib, path.getBuffer());

        if (item) {
          devices.appendItem(item);

        } else {
          vclose(devhandle);
        }

      } else {
        vclose(devhandle);
      }
    }
/*
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
    }*/

  }

  SetupDiDestroyDeviceInfoList(hinfo);
}
