/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


//todo:  add recusion check, so we cannot start the screen saver if it is already started


#pragma comment(lib, "setupapi.lib")

#include "pwr.h"

//for battery stuff
#include <batclass.h>
#include <setupapi.h>


static vPowerManagement *power = new vPowerManagement();


vPowerManagement::vPowerManagement() : plugBase("Power Management") {
  functions = NULL;
  battery   = NULL;
  tree      = NULL;
}


void vPowerManagement::onInit() {
  if (!GetShutdownPrivilege()) {
    CONSOLE_ACTIVE->printError(getName(), "FAILED GETTING PERMISSIONS");
    return;
  }

  functions = new funcGroup(getName(), "power");
  new vSystemShutdown(  "Log Off",   "logoff",    functions, EWX_LOGOFF);
  new vSystemShutdown(  "Shutdown",  "shutdown",  functions, EWX_SHUTDOWN);
  new vSystemShutdown(  "Restart",   "restart",   functions, EWX_REBOOT);
  new vSystemPowerState("Sleep",     "sleep",     functions, TRUE);
  new vSystemPowerState("Hibernate", "hibernate", functions, FALSE);
//  if (RMXOS::isWinNT()) {
    new vRemoteShutdown("Remote System Shutdown", NULL, functions, FALSE);
    new vRemoteShutdown("Remote System Restart",  NULL, functions, TRUE);
//  }
//if (RMXOS::isWinNT()) new rmxLockWorkstation("Lock Workstation", functions);
//  new vMonitor("Screen Saver",     "screensaver", SC_SCREENSAVE,   0, functions);
//  new vMonitor("Turn off Monitor", "monitor",     SC_MONITORPOWER, 2, functions);

  if (!RMXOS::isWinNT4()) {
    SYSTEM_POWER_STATUS s;
    ZeroMemory(&s, sizeof(SYSTEM_POWER_STATUS));
    if (GetSystemPowerStatus(&s)) {
      if (s.BatteryLifePercent != 255) {
        battery = new vBattery();
      }
    }
  }

  tree = new vDetectScreenSaver();

//  cfgTree *tree = new cfgTree(getName());
//  (new cfgInt("Num CPUs", tree))->setValue(get_num_cpus());
//  (new cfgInt("CPU Usage", tree))->setValue(get_cpu_usage(1));
}


void vPowerManagement::onQuit() {
  vdelnull(functions);
  vdelnull(battery);
  vdelnull(tree);
}


BOOL vPowerManagement::GetShutdownPrivilege() {
  HANDLE hToken;
  TOKEN_PRIVILEGES tkp;

  if (RMXOS::isWinNT()) {
    if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY, &hToken) ) {
      LookupPrivilegeValue(L"", L"SeShutdownPrivilege", &tkp.Privileges[0].Luid);
      tkp.PrivilegeCount = 1;
      tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
      return (AdjustTokenPrivileges(hToken, 0, &tkp, 0, (PTOKEN_PRIVILEGES)NULL, 0) != 0);
    }
    return FALSE;
  }
  return TRUE;
}


//--------------------------------------------------------------------------------------------


vSystemPowerState::vSystemPowerState(const char *function_name, const char *script_name, funcGroup *parent, BOOL suspendState)
 : funcSingle(function_name, script_name, parent) {
  pwrstate = suspendState;
}

void vSystemPowerState::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  SetSystemPowerState(pwrstate, FALSE);
}


//--------------------------------------------------------------------------------------------


vSystemShutdown::vSystemShutdown(const char *function_name, const char *script_name, funcGroup *parent, BOOL powerState)
 : funcSingle(function_name, script_name, parent) {
  pwrstate = powerState;
}

void vSystemShutdown::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  ExitWindowsEx(pwrstate | EWX_FORCEIFHUNG, 0);
//  return InitiateSystemShutdown(NULL, NULL, 0, FALSE, state);
}


//--------------------------------------------------------------------------------------------


vRemoteShutdown::vRemoteShutdown(const char *function_name, const char *script_name, funcGroup *parent, BOOL powerState) 
 : funcSingle(function_name, script_name, parent) {
  pwrstate = powerState;
  new funcParam("Computer Name", "f_addr", "string", this);
}

void vRemoteShutdown::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  const char *machine = source->list_getParam(input, "f_addr");
  if (machine) {
    rmxUtf8ToWide str(machine);
    InitiateSystemShutdown((LPWSTR)str.getBuffer(), NULL, 0, FALSE, pwrstate);
  }
}


//--------------------------------------------------------------------------------------------


vBattery::vBattery() : cfgTree("Batteries"), vTimer(1000, TRUE) {
  setSavable(FALSE);

  ac        = new cfgText("AC Power Status",           this);
  totaltime = new cfgText("Total Battery Time",        this);
  time      = new cfgText("Battery Time Remaining",    this);
  percent   = new cfgInt( "Battery Percent Remaining", this);
  status    = new cfgText("Battery Power Status",      this);
  charge    = new cfgBool("Charging Battery",          this);

  ac->setEnabled(false);
  charge->setEnabled(false);
  percent->setEnabled(false);
  time->setEnabled(false);
  totaltime->setEnabled(false);
  status->setEnabled(false);

#ifdef _DEBUG  //i dont want this stuff appearing in the main dev tree yet
  GUID guid = {0x72631e54L, 0x78A4, 0x11d0, 0xbc, 0xf7, 0x00, 0xaa, 0x00, 0xb7, 0xb3, 0x2a};
  HDEVINFO devinfo = SetupDiGetClassDevs(&guid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
  if (devinfo) {
    SP_DEVICE_INTERFACE_DATA data;
    ZeroMemory(&data, sizeof(data));
    data.cbSize = sizeof(data);

    SetLastError(0);
    int count = 0;
    while (true) {
      if (SetupDiEnumDeviceInterfaces(devinfo, NULL, &guid, count, &data)) {
        char tmp[256];
        SP_DEVICE_INTERFACE_DETAIL_DATA *detaildata = (SP_DEVICE_INTERFACE_DETAIL_DATA*)tmp;
        ZeroMemory(detaildata, sizeof(tmp));
        detaildata->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA);

        SetupDiGetDeviceInterfaceDetail(devinfo, &data, detaildata, sizeof(tmp) - detaildata->cbSize, NULL, NULL);

        rmxToUtf8 devname(detaildata->DevicePath);
        CONSOLE_MAIN->print(power->getName(), devname.getBuffer());
        // CreateFile(detaildata->DevicePath)
      }

      else {
        break;
      }
      count++;
    }
  }
#endif
}


vBattery::~vBattery() {
}


void vBattery::onTimer(DWORD systemTime) {
  SYSTEM_POWER_STATUS s;
  ZeroMemory(&s, sizeof(SYSTEM_POWER_STATUS));
  if (!GetSystemPowerStatus(&s)) return;

  charge->setValue(s.BatteryFlag & 0x08);
  percent->setValue(s.BatteryLifePercent);

  time->setText(getFormattedTime(s.BatteryLifeTime));
  totaltime->setText(getFormattedTime(s.BatteryFullLifeTime));

  switch (s.ACLineStatus) {
    case AC_LINE_ONLINE: ac->setText("Connected"); break;
    case AC_LINE_OFFLINE: ac->setText("Disconnected"); break;
    case AC_LINE_BACKUP_POWER: ac->setText("Backup Power"); break;
    default: ac->setText("Unknown"); break;
  }

  switch (s.BatteryFlag & 0x07) {
    case 0: status->setText("Normal"); break;
    case BATTERY_FLAG_HIGH: status->setText("High"); break;
    case BATTERY_FLAG_LOW: status->setText("Low"); break;
    case BATTERY_FLAG_CRITICAL: status->setText("Critical"); break;
    default: status->setText("Unknown"); break;
//    default: status->setUserDataInt(s.BatteryFlag); break;
  }
}


const char *vBattery::getFormattedTime(int sec) {
  if (sec == -1) return "Unknown";

  int hour = sec / 3600;
  sec -= (hour * 3600);
  int min = sec / 60;
  sec -= (min *60);
  
  VPRINTF(buffer, sizeof(buffer), "%d:%02d", hour, min);
  return buffer;
}



//--------------------------------------------------------------------------------------------

/*
rmxLockWorkstation::rmxLockWorkstation(const char *name, funcGroup *parent) 
 : funcSingle(name, parent) {
}

UINT rmxLockWorkstation::onActivated(HPLUGIN source, HLIST input, HLIST output) {
  return LockWorkStation();
}
*/



//--------------------------------------------------------------------------------------------

/*
vMonitor::vMonitor(const char *function_name, const char *script_name, UINT moncommand, UINT monparam, funcGroup *parent)
 : funcSingle(function_name, script_name, parent) {
  command = moncommand;
  param = monparam;
}


void vMonitor::onActivated(HPLUGIN source, HLIST input, HLIST output, BOOL state) {
  HPLUGIN master = vHPLUGIN::getMaster();
  HWND    hwnd   = (HWND) master->list_getParamLong(master->root_getIdent(), "window");
  SendMessage(hwnd, WM_SYSCOMMAND, command, param);

  PROFILE_INT(hwnd);
}
*/

//--------------------------------------------------------------------------------------------



vDetectScreenSaver::vDetectScreenSaver() : cfgTree("Screen Saver"), vTimer(16, TRUE) {
//  enable  = new evntPress("Screen Saver Started", this);
//  disable = new evntPress("Screen Saver Stopped", this);
  event = new evntPress("Screen Saver Activated", this);  //todo: find out why this needs an entire tree to itself
  last = FALSE;
}


void vDetectScreenSaver::onTimer(DWORD systemTime) {
  BOOL en = !!getScreenSaver();
  if (last != en) {
    last = en;
    event->setPress(last);
  }
}


BOOL vDetectScreenSaver::getScreenSaver() {
  if (!RMXOS::isWinNT4()) {
    BOOL active = FALSE;
    SystemParametersInfo(SPI_GETSCREENSAVERRUNNING, 0, &active, 0);
    return active;
  }

  HDESK desktop;
  SetLastError(0);
  desktop = OpenDesktop(TEXT("screen-saver"), 0, FALSE, MAXIMUM_ALLOWED);
  if (desktop == NULL) {
    if (GetLastError() == ERROR_ACCESS_DENIED) {
      return TRUE;
    }
    return FALSE;
  }
  CloseDesktop(desktop);
  return TRUE;
}


//--------------------------------------------------------------------------------------------

/*
    HWINSTA  hwinsta = OpenWindowStation("winsta0", FALSE,
                              WINSTA_ACCESSCLIPBOARD   |
                              WINSTA_ACCESSGLOBALATOMS |
                              WINSTA_CREATEDESKTOP     |
                              WINSTA_ENUMDESKTOPS      |
                              WINSTA_ENUMERATE         |
                              WINSTA_EXITWINDOWS       |
                              WINSTA_READATTRIBUTES    |
                              WINSTA_READSCREEN        |
                              WINSTA_WRITEATTRIBUTES);
    if (hwinsta == NULL) {
      MessageBox(0, "!", "", 0);
        return FALSE;
    }
    // 
    // Set the windowstation to be winsta0
    // 
    if (!SetProcessWindowStation(hwinsta)) {
      MessageBox(0, "2", "", 0);
     return FALSE;
    }

    // 
    // Get the default desktop on winsta0
    // 
    HDESK hdesk = OpenDesktop("Winlogon", 0, FALSE,
                        DESKTOP_CREATEMENU |
              DESKTOP_CREATEWINDOW |
                        DESKTOP_ENUMERATE    |
                        DESKTOP_HOOKCONTROL  |
                        DESKTOP_JOURNALPLAYBACK |
                        DESKTOP_JOURNALRECORD |
                        DESKTOP_READOBJECTS |
                        DESKTOP_SWITCHDESKTOP |
                        DESKTOP_WRITEOBJECTS);
    if (hdesk == NULL) {
            MessageBox(0, "3", "", 0);
       return FALSE;
    }

    // 
    // Set the desktop to be "default"
    // 
    if (!SetThreadDesktop(hdesk)) {
            MessageBox(0, "4", "", 0);
       return FALSE;
    }
    return PostMessage(HWND_BROADCAST,WM_HOTKEY,0,
        MAKELPARAM(MOD_ALT|MOD_CONTROL,VK_DELETE));
        */
