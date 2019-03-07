/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/

/*
http://www.paradicesoftware.com/specs/cpuid/
http://www.sandpile.org/ia32/cpuid.htm
*/


#include "performance.h"
#include "cpuid.h"


static vPerformance *PERFORMANCE = new vPerformance();


vPerformance::vPerformance() : plugBase("Performance") {
  tree            = NULL;
  FrapsSharedData = NULL;
  frapsDLL        = NULL;
  fsd             = NULL;
}


void vPerformance::onInit() {
  tree = new cfgTree(getName(), this);
  tree->setSavable(FALSE);
  tree->setEnabled(FALSE);

  
  //FRAPS related stuff
  frapsDLL = GetModuleHandle(L"FRAPS.DLL");
  if (!frapsDLL) {
    CONSOLE_MAIN->printWarning(getName(), "FRAPS 1.9C (or newer) not found");
  } else {
    (FARPROC &) FrapsSharedData = GetProcAddress(frapsDLL, "FrapsSharedData");
    if (!FrapsSharedData) {
      CONSOLE_MAIN->printWarning(getName(), "FRAPS 1.9C (or newer) not found");
    } else {
      new vFrapsData(tree);
    }
  }


  //CPU-ID related stuff
  _p_info cpuinfo;
  char hex[32] = "";
  ZeroMemory(&cpuinfo, sizeof(cpuinfo));
  _cpuid(&cpuinfo);

  cfgText *cpuid = new cfgText("CPUID", tree);
  cpuid->setText(cpuinfo.v_name);
  cpuid->setExpanded(TRUE);

  (new cfgText("CPU Model Name", cpuid))->setText(cpuinfo.model_name);
  (new cfgText("CPU Family",     cpuid))->setValueInt(cpuinfo.family);
  if (cpuinfo.familyex != 0) {
    (new cfgText("CPU Extended Family", cpuid))->setValueInt(cpuinfo.familyex);
  }
  (new cfgText("CPU Model",      cpuid))->setValueInt(cpuinfo.model);
  if (cpuinfo.modelex != 0) {
    (new cfgText("CPU Extended Model",  cpuid))->setValueInt(cpuinfo.modelex);
  }
  (new cfgText("CPU Stepping",   cpuid))->setValueInt(cpuinfo.stepping);

  cfgText *support_cpu = new cfgText("CPU Instruction Support", cpuid);
  VPRINTF(hex, sizeof(hex), "%08X", cpuinfo.feature);
  support_cpu->setText(hex);
  support_cpu->setExpanded(TRUE);

  (new cfgBool("Floating Point Unit",                   support_cpu))->setValue( cpuinfo.feature & (1<< 0) );
  (new cfgBool("Virtual Mode Extensions",               support_cpu))->setValue( cpuinfo.feature & (1<< 1) );
  (new cfgBool("Debugging Extensions",                  support_cpu))->setValue( cpuinfo.feature & (1<< 2) );
  (new cfgBool("Page Size Extensions",                  support_cpu))->setValue( cpuinfo.feature & (1<< 3) );
  (new cfgBool("Time Stamp Counter",                    support_cpu))->setValue( cpuinfo.feature & (1<< 4) );
  (new cfgBool("Model-Specific Registers",              support_cpu))->setValue( cpuinfo.feature & (1<< 5) );
  (new cfgBool("Physical Address Extensions",           support_cpu))->setValue( cpuinfo.feature & (1<< 6) );
  (new cfgBool("Machine Check Exceptions",              support_cpu))->setValue( cpuinfo.feature & (1<< 7) );

  (new cfgBool("Compare and Exchange 8-Byte",           support_cpu))->setValue( cpuinfo.feature & (1<< 8) );
  (new cfgBool("On-chip APIC",                          support_cpu))->setValue( cpuinfo.feature & (1<< 9) );
  (new cfgBool("Reserved",                              support_cpu))->setValue( cpuinfo.feature & (1<<10) );
  (new cfgBool("Fast System Calls",                     support_cpu))->setValue( cpuinfo.feature & (1<<11) );
  (new cfgBool("Memory Type Range Registers",           support_cpu))->setValue( cpuinfo.feature & (1<<12) );
  (new cfgBool("Page Global Enable",                    support_cpu))->setValue( cpuinfo.feature & (1<<13) );
  (new cfgBool("Machine Check Architecture",            support_cpu))->setValue( cpuinfo.feature & (1<<14) );
  (new cfgBool("Conditional Move Instruction",          support_cpu))->setValue( cpuinfo.feature & (1<<15) );

  (new cfgBool("Page Attribute Table",                  support_cpu))->setValue( cpuinfo.feature & (1<<16) );
  (new cfgBool("36-bit Page Size Extensions",           support_cpu))->setValue( cpuinfo.feature & (1<<17) );
  (new cfgBool("Processor Serial Number",               support_cpu))->setValue( cpuinfo.feature & (1<<18) );
  (new cfgBool("CLFLUSH (description pending)",         support_cpu))->setValue( cpuinfo.feature & (1<<19) );
  (new cfgBool("Reserved",                              support_cpu))->setValue( cpuinfo.feature & (1<<20) );
  (new cfgBool("Debug Trace and EMON Store MSRs",       support_cpu))->setValue( cpuinfo.feature & (1<<21) );
  (new cfgBool("Thermal Control MSR",                   support_cpu))->setValue( cpuinfo.feature & (1<<22) );
  (new cfgBool("MMX Instruction Set",                   support_cpu))->setValue( cpuinfo.feature & (1<<23) );

  (new cfgBool("Fast Floating Point Save/Restore",      support_cpu))->setValue( cpuinfo.feature & (1<<24) );
  (new cfgBool("SSE (Streaming SIMD Extensions)",       support_cpu))->setValue( cpuinfo.feature & (1<<25) );
  (new cfgBool("SSE2 (Streaming SIMD Extensions - #2)", support_cpu))->setValue( cpuinfo.feature & (1<<26) );
  (new cfgBool("Selfsnoop",                             support_cpu))->setValue( cpuinfo.feature & (1<<27) );
  (new cfgBool("Hyper-Threading Technology",            support_cpu))->setValue( cpuinfo.feature & (1<<28) );
  (new cfgBool("Thermal Interrupt & Status MSRs.",      support_cpu))->setValue( cpuinfo.feature & (1<<29) );
  (new cfgBool("IA-64 (64-bit Intel CPU)",              support_cpu))->setValue( cpuinfo.feature & (1<<30) );
  (new cfgBool("Pending Break Event",                   support_cpu))->setValue( cpuinfo.feature & (1<<31) );

  cfgText *support_ext = new cfgText("CPU Extended Instruction Support", cpuid);
  VPRINTF(hex, sizeof(hex), "%08X", cpuinfo.extended);
  support_ext->setText(hex);
  support_ext->setExpanded(TRUE);

  cfgText *support_os  = new cfgText("OS Instruction Support", cpuid);
  VPRINTF(hex, sizeof(hex), "%08X", cpuinfo.os_support);
  support_os->setText(hex);
  support_os->setExpanded(TRUE);
}

/*
  printf("Shared data size: %d\r\n", fsd->sizeOfStruct);
  printf("Game name: %s\r\n", fsd->gameName);
  printf("Frame rate: %d\r\n", fsd->currentFPS);
  printf("Total frames: %d\r\n", fsd->totalFrames);
  printf("Time last frame drawn: %d\r\n", fsd->timeOfLastFrame);
  printf("Current time: %d\r\n", GetTickCount());
*/


FRAPS_SHARED_DATA *vPerformance::updateFraps() {
  if (!FrapsSharedData) return NULL;
  fsd = FrapsSharedData();
  return fsd;
}


const char *vPerformance::getFrameName() {
  if (!updateFraps()) return NULL;
  return fsd->gameName;
}


int vPerformance::getFrameRate() {
  if (!updateFraps()) return 0;
  return fsd->currentFPS;
}


int vPerformance::getFrameTime() {
  if (!updateFraps()) return 0;
  return fsd->timeOfLastFrame;
}


//------------------------------------------------------------------------------------------


vFrapsData::vFrapsData(cfgBase *parent) : cfgInt("Frame Rate", parent), vTimer(10, TRUE) {
//  setTooltip("DirectX and OpenGL frame rate as reported by FRAPS (www.fraps.com)");
  setValue(0);
}


void vFrapsData::onTimer(DWORD systemTime) {
  int newdata = PERFORMANCE->getFrameRate();
  if (newdata != getValue()) setValue(newdata);
}
