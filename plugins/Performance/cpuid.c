#include <windows.h>
#include "cpuid.h"


// These are the bit flags that get set on calling cpuid
// with register eax set to 1
#define _MMX_FEATURE_BIT    0x00800000
#define _SSE_FEATURE_BIT    0x02000000
#define _SSE2_FEATURE_BIT     0x04000000

// This bit is set when cpuid is called with
// register set to 80000001h (only applicable to AMD)
#define _3DNOW_FEATURE_BIT    0x80000000

int IsCPUID()
{
  __try {
    _asm {
      xor eax, eax
      cpuid
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    return 0;
  }
  return 1;
}


/***
* int _os_support(int feature)
*   - Checks if OS Supports the capablity or not
*
* Entry:
*   feature: the feature we want to check if OS supports it.
*
* Exit:
*   Returns 1 if OS support exist and 0 when OS doesn't support it.
*
****************************************************************/

int _os_support(int feature)
{
  __try {
    switch (feature) {
    case _CPU_FEATURE_SSE:
      __asm {
        xorps xmm0, xmm0    // executing SSE instruction
      }
      break;
    case _CPU_FEATURE_SSE2:
      __asm {
        xorpd xmm0, xmm0    // executing SSE2 instruction
      }
      break;
    case _CPU_FEATURE_3DNOW:
      __asm {
        pfrcp mm0, mm0      // executing 3DNow! instruction
        emms
      }
      break;
    case _CPU_FEATURE_MMX:
      __asm {
        pxor mm0, mm0       // executing MMX instruction
        emms
      }
      break;
    }
  }
  __except (EXCEPTION_EXECUTE_HANDLER) {
    if (_exception_code() == STATUS_ILLEGAL_INSTRUCTION) {
      return 0;
    }
    return 0;
  }
  return 1;
}


/***
*
* void map_mname(_p_info *)
*   - Maps family and model to processor name
*
****************************************************/


void map_mname(_p_info *pinfo)
{
  // Default to name not known
  strcpy (pinfo->model_name, "Unknown");


  //AMD CPUs
  if (!strncmp("AuthenticAMD", pinfo->v_name, 12)) {
    switch (pinfo->family) {

      case 4: // Am486 and AM5x86 series
        switch (pinfo->model) {
          case 3:  strcpy(pinfo->model_name, "Am486 DX/2");    break;
          case 7:  strcpy(pinfo->model_name, "Am486 DX/2-WB"); break;
          case 8:  strcpy(pinfo->model_name, "Am486 DX/4");    break;
          case 9:  strcpy(pinfo->model_name, "Am486 DX/4-WB"); break;
          case 14: strcpy(pinfo->model_name, "Am5x86-WT");     break;
          case 15: strcpy(pinfo->model_name, "Am5x86-WB");     break;
          default: strcpy(pinfo->model_name, "Unknown Am486 or Am5x86");
        }
        break;

      case 5: // K5 and K6 series
        switch (pinfo->model) {
          case 0:  strcpy(pinfo->model_name, "K5/SSA5");             break;
          case 1:  strcpy(pinfo->model_name, "K5 (PR120 or PR133)"); break;
          case 2:  strcpy(pinfo->model_name, "K5 (PR166)");          break;
          case 3:  strcpy(pinfo->model_name, "K5 (PR200)");          break;
          case 6:  strcpy(pinfo->model_name, "K6 (0.30 µm)");        break;
          case 7:  strcpy(pinfo->model_name, "K6 (0.25 µm)");        break;
          case 8:  strcpy(pinfo->model_name, "K6-2");                break;
          case 9:  strcpy(pinfo->model_name, "K6-3");                break;
          case 13: strcpy(pinfo->model_name, "K6-2+ or K6-III+");    break;
          default: strcpy(pinfo->model_name, "Unknown K5 or K6");
        }
        break;

      case 6: // Athlon and Duron series
        switch (pinfo->model) {
          case 0:  //see 1
          case 1:  strcpy(pinfo->model_name, "Athlon (0.25 µm)");                   break;
          case 2:  strcpy(pinfo->model_name, "Athlon (0.18 µm)");                   break;
          case 3:  strcpy(pinfo->model_name, "Duron (Splitfire)");                  break;
          case 4:  strcpy(pinfo->model_name, "Athlon (Thunderbird)");               break;
          case 6:  strcpy(pinfo->model_name, "Athlon (Palamino)");                  break;
          case 7:  strcpy(pinfo->model_name, "Duron (Morgan)");                     break;
          case 8:  strcpy(pinfo->model_name, "Athlon (Thoroughbred or Applebred)"); break;
          case 10: strcpy(pinfo->model_name, "Athlon (Barton)");                    break;
          default: strcpy(pinfo->model_name, "Unknown Athlon or Duron");
        }
        break;
          
      case 15:  //Athlon 64 and Opteron series
        switch (pinfo->family) {
          case 0:
            switch (pinfo->modelex) {
              case 5:  strcpy(pinfo->model_name, "Athlon 64 FX or Opteron (0.13 µm)"); break;
              case 4:  strcpy(pinfo->model_name, "Athlon 64 (0.13 µm 754)"); break;
              case 7:  strcpy(pinfo->model_name, "Athlon 64 (0.13 µm 939)"); break;
              case 8:  strcpy(pinfo->model_name, "Athlon 64 (0.13 µm 754)"); break;
              case 11: strcpy(pinfo->model_name, "Athlon 64 (0.13 µm 939)"); break;
              case 12: strcpy(pinfo->model_name, "Athlon 64 (0.13 µm 754)"); break;
              case 16: strcpy(pinfo->model_name, "Athlon 64 (0.13 µm 939)"); break;
              default: strcpy(pinfo->model_name, "Unknown Athlon 64 or Opteron");
            }
            break;

           default: strcpy(pinfo->model_name, "Unknown Athlon 64 or Opteron");
        }
        break;

      default:
        strcpy(pinfo->model_name, "Unknown AMD");
    }
  }


  //Intel CPUs
  else if (!strncmp("GenuineIntel", pinfo->v_name, 12)) {
    switch (pinfo->brand) {
      /*
      case 0x01: strcpy (pinfo->model_name, "Celeron (0.18 µm)"); break;
      case 0x02: strcpy (pinfo->model_name, "Pentium III (0.18 µm)"); break;
      case 0x03: strcpy (pinfo->model_name, "Pentium III Xeon (0.18 µm) or Celeron (0.13 µm)"); break;
      case 0x04: strcpy (pinfo->model_name, "Pentium III (0.13 µm)"); break;
      case 0x07: strcpy (pinfo->model_name, "Celeron Mobile (0.13 µm)"); break;
      case 0x06: strcpy (pinfo->model_name, "Pentium III Mobile (0.13 µm)"); break;
      case 0x0A: strcpy (pinfo->model_name, "Celeron 4 (0.18 µm)"); break;
      case 0x08: strcpy (pinfo->model_name, "Pentium 4 (0.18 µm) or Celeron 4 Mobile (0.13 µm)"); break;
      case 0x09: strcpy (pinfo->model_name, "Pentium 4 (0.13 µm)"); break;
      case 0x0E: strcpy (pinfo->model_name, "Pentium 4 Xeon (0.18 µm) or Pentium 4 Mobile (0.13 µm)"); break;
      case 0x0B: strcpy (pinfo->model_name, "Pentium 4 Xeon (0.13 µm) or Xeon MP (0.18 µm)"); break;
      case 0x0C: strcpy (pinfo->model_name, "Pentium 4 Xeon MP (0.13 µm)"); break;
      case 0x0F: strcpy (pinfo->model_name, "Celeron 4 Mobile (0.13 µm)"); break;
      case 0x12: strcpy (pinfo->model_name, "Celeron M (0.13 or 0.09 µm)"); break;
      case 0x16: strcpy (pinfo->model_name, "Pentium M (0.13 or 0.09 µm)"); break;
      */
    default:
      switch (pinfo->family) {

      case 4:
        switch (pinfo->model) {
          case 0: strcpy (pinfo->model_name, "i486DX-25/33"); break;
          case 1: strcpy (pinfo->model_name, "i486DX-50");    break;
          case 2: strcpy (pinfo->model_name, "i486SX");       break;
          case 3: strcpy (pinfo->model_name, "i486DX/2");     break;
          case 4: strcpy (pinfo->model_name, "i486SL");       break;
          case 5: strcpy (pinfo->model_name, "i486SX/2");     break;
          case 7: strcpy (pinfo->model_name, "i486DX/2-WB");  break;
          case 8: strcpy (pinfo->model_name, "i486DX/4");     break;
          case 9: strcpy (pinfo->model_name, "i486DX/4-WB");  break;
          default: strcpy (pinfo->model_name, "Unknown i486");
        }
        break;

      case 5:
        switch (pinfo->model) {
          case 0: strcpy (pinfo->model_name, "Pentium (P5 A-step)"); break;
          case 1: strcpy (pinfo->model_name, "Pentium (P5 - 0.60 µm)"); break;
          case 2: strcpy (pinfo->model_name, "Pentium (P54C - 0.60 µm)"); break;
          case 3: strcpy (pinfo->model_name, "Pentium (P24T Overdrive)"); break;
          case 4: strcpy (pinfo->model_name, "Pentium-MMX (P55C - 0.35 µm)"); break;
          case 7: strcpy (pinfo->model_name, "Pentium-MMX (P54C - 0.35 µm)"); break;
          case 8: strcpy (pinfo->model_name, "Pentium-MMX (P55C Tillamook - 0.25 µm)"); break;
          default: strcpy (pinfo->model_name, "Unknown Pentium");
        }
        break;

      case 6:
        switch (pinfo->model) {
          case 0:  strcpy (pinfo->model_name, "Pentium Pro (P6 A-step)"); break;
          case 1:  strcpy (pinfo->model_name, "Pentium Pro (P6)"); break;
          case 3:  strcpy (pinfo->model_name, "Pentium II (0.28 µm)"); break;
          case 5:  strcpy (pinfo->model_name, "Pentium II (0.25 µm)"); break;
          case 6:  strcpy (pinfo->model_name, "Celeron"); break;
          case 7:  strcpy (pinfo->model_name, "Pentium III (0.25 µm)"); break;
          case 8:  strcpy (pinfo->model_name, "Pentium III (256KB L2 Cache - 0.18 µm)"); break;
          case 9:  strcpy (pinfo->model_name, "Pentium M (512KB or 1MB  L2 Cache - 0.18 µm)"); break;
          case 10: strcpy (pinfo->model_name, "Pentium III (1MB or 2MB  L2 Cache - 0.18 µm)"); break;
          case 11: strcpy (pinfo->model_name, "Pentium III (256KB or 512KB  L2 Cache - 0.13 µm)"); break;
          case 13: strcpy (pinfo->model_name, "Pentium M (512KB or 2MB  L2 Cache - 0.09 µm)"); break;
          case 14: strcpy (pinfo->model_name, "Pentium M DC (2MB  L2 Cache - 65 nm)"); break;
          default: strcpy (pinfo->model_name, "Unknown Pentium III or Pentium M");
        }
        break;

      case 7: strcpy (pinfo->model_name, "Itanium (IA-64)"); break;

      case 15:
        switch (pinfo->familyex) {
          case 0:
            switch (pinfo->model) {
              case 0: strcpy (pinfo->model_name, "Pentium 4 (0.18 µm)"); break;
              case 1: strcpy (pinfo->model_name, "Pentium 4 (0.18 µm)"); break;
              case 2: strcpy (pinfo->model_name, "Pentium 4 (0.13 µm)"); break;
              case 3: strcpy (pinfo->model_name, "Pentium 4 (0.09 µm)"); break;
              default: strcpy (pinfo->model_name, "Unknown Pentium 4");
            }
            break;

          case 1:
            switch (pinfo->model) {
              case 0: strcpy (pinfo->model_name, "Itanium 2 (McKinley - 0.18 µm)"); break;
              case 1: strcpy (pinfo->model_name, "Itanium 2 (Madison or Deerfield - 0.13 µm)"); break;
              case 2: strcpy (pinfo->model_name, "Itanium 3 (Madison 9M - 0.13 µm)"); break;
              default: strcpy (pinfo->model_name, "Unknown Itanium 2");
            }
            break;

            case 2: 
            switch (pinfo->model) {
              case 0: strcpy (pinfo->model_name, "Itanium 2 DC (Montecito - 0.9 µm)"); break;
              default: strcpy (pinfo->model_name, "Unknown Itanium 2 DC");
            }
            break;

          default: strcpy (pinfo->model_name, "Unknown Pentium 4 or Itanium");
        }
      }
    }
  }


  //Cyrix CPUs
  else if (!strncmp("CyrixInstead", pinfo->v_name, 12)) {
    switch (pinfo->family) {

      case 4:
        switch (pinfo->model) {
          case 4: strcpy (pinfo->model_name, "MediaGX");        break;
          default: strcpy (pinfo->model_name, "Unknown Cyrix"); break;
        }
        break;

      case 5:
        switch (pinfo->model) {
          case 2: strcpy (pinfo->model_name, "6x86 or 6x86L");        break;
          case 4: strcpy (pinfo->model_name, "MediaGX MMX Enhanced"); break;
          default: strcpy (pinfo->model_name, "Unknown Cyrix");       break;
        }
        break;

      case 6:
        switch (pinfo->model) {
          case 0: strcpy (pinfo->model_name, "mII (6x86MX)");        break;
          case 5: strcpy (pinfo->model_name, "VIA Cyrix M2");        break;
          case 6: strcpy (pinfo->model_name, "WinChip C5A");         break;
          case 8: strcpy (pinfo->model_name, "WinChip C5N");         break;

          case 7:
            if (pinfo->stepping < 8) {
              strcpy (pinfo->model_name, "WinChip C5B");
            } else {
              strcpy (pinfo->model_name, "WinChip C5C");
            }
            break;

          case 9:
            if (pinfo->stepping < 8) {
              strcpy (pinfo->model_name, "WinChip C5XL");
            } else {
              strcpy (pinfo->model_name, "WinChip C5P");
            }
            break;

          default: strcpy (pinfo->model_name, "Unknown Cyrix");      break;
        }
        break;

      default: strcpy (pinfo->model_name, "Unknown Cyrix"); break;
    };
  }


  //CentarHauls CPUs
  else if (!strncmp("CentaurHauls", pinfo->v_name, 12)) {
    switch (pinfo->model) {
      case 5:
        switch (pinfo->family) {
          case 4: strcpy (pinfo->model_name, "Centaur C6"); break;
          case 8: strcpy (pinfo->model_name, "Centaur C2"); break;
          case 9: strcpy (pinfo->model_name, "Centaur C3"); break;
          default: strcpy (pinfo->model_name, "Unknown Centaur");
        }
        break;

      default: strcpy (pinfo->model_name, "Unknown Centaur");
    }
  }


  //UMC CPUs
  else if (!strncmp("UMC UMC UMC ", pinfo->v_name, 12)) {
    switch (pinfo->model) {
      case 4:
        switch (pinfo->family) {
          case 1: strcpy (pinfo->model_name, "UMC U5D"); break;
          case 2: strcpy (pinfo->model_name, "UMC U5S"); break;
          default: strcpy (pinfo->model_name, "Unknown UMC");
        }
        break;

      default: strcpy (pinfo->model_name, "Unknown UMC");
    }
  }


  //NexGen CPUs
  else if (!strncmp("NexGenDriven", pinfo->v_name, 12)) {
    switch (pinfo->model) {
      case 5:
        switch (pinfo->family) {
          case 0: strcpy (pinfo->model_name, "Nx586"); break;
          default: strcpy (pinfo->model_name, "Unknown NexGen");
        }
        break;

      default: strcpy (pinfo->model_name, "Unknown NexGen");
    }
  }


  //Rise CPUs
  else if (!strncmp("RiseRiseRise", pinfo->v_name, 12)) {
    switch (pinfo->model) {
      case 5:
        switch (pinfo->family) {
          case 0: strcpy (pinfo->model_name, "Rise mP6 (0.25 µm)"); break;
          case 2: strcpy (pinfo->model_name, "Rise mP6 (0.18 µm)"); break;
          default: strcpy (pinfo->model_name, "Unknown Rise");
        }
        break;

      default: strcpy (pinfo->model_name, "Unknown Rise");
    }
  }


  //SiS CPUs
  else if (!strncmp("SiS SiS SiS ", pinfo->v_name, 12)) {
    switch (pinfo->model) {
      case 5:
        switch (pinfo->family) {
          case 0: strcpy (pinfo->model_name, "SiS 55x"); break;
          default: strcpy (pinfo->model_name, "Unknown SiS");
        }
        break;

      default: strcpy (pinfo->model_name, "Unknown SiS");
    }
  }


  //Transmeta CPUs
  else if (!strncmp("TransmetaCPU", pinfo->v_name, 12)) {
    switch (pinfo->model) {
      case 5:
        switch (pinfo->family) {
          case 0: strcpy (pinfo->model_name, "Crusoe TM3x00 or TM5x00"); break;
          default: strcpy (pinfo->model_name, "Unknown Transmeta");
        }
        break;

      default: strcpy (pinfo->model_name, "Unknown Transmeta");
    }
  }


  //National Semiconductor CPUs
  else if (!strncmp("Geode by NSC", pinfo->v_name, 12)) {
    switch (pinfo->model) {
      case 5:
        switch (pinfo->family) {
          case 4:  strcpy (pinfo->model_name, "NSC GX1, GXLV, GXm"); break;
          case 5:  strcpy (pinfo->model_name, "NSC GX2"); break;
          case 10: strcpy (pinfo->model_name, "NSC LX"); break;
          default: strcpy (pinfo->model_name, "Unknown National Semiconductor");
        }
        break;

      default: strcpy (pinfo->model_name, "Unknown National Semiconductor");
    }
  }
}


/***
*
* int _cpuid (_p_info *pinfo)
*
* Entry:
*
*   pinfo: pointer to _p_info.
*
* Exit:
*
*   Returns int with capablity bit set even if pinfo = NULL
*
****************************************************/


int _cpuid (_p_info *pinfo)
{
  DWORD dwStandard = 0;
  DWORD dwFeature = 0;
  DWORD dwBrand = 0;
  DWORD dwStandardEx = 0;
  DWORD dwFeatureEx = 0;
  DWORD dwBrandEx = 0;
  DWORD dwMax = 0;
  int feature = 0;
  int os_support = 0;
  union {
    char cBuf[12+1];
    struct {
      DWORD dw0;
      DWORD dw1;
      DWORD dw2;
    } s;
  } Ident;

  if (!IsCPUID()) {
    return 0;
  }

  _asm {
    push ebx
    push ecx
    push edx

    // get the vendor string
    xor eax, eax
    cpuid
    mov dwMax, eax
    mov Ident.s.dw0, ebx
    mov Ident.s.dw1, edx
    mov Ident.s.dw2, ecx

    // get the Standard bits
    mov eax, 1
    cpuid
    mov dwStandard, eax
    mov dwBrand, ebx
    mov dwFeature, edx

    // get AMD-specials
    mov eax, 80000000h
    cpuid
    cmp eax, 80000000h
    jc notamd
    mov eax, 80000001h
    cpuid
    mov dwFeatureEx, edx

notamd:
    pop ecx
    pop ebx
    pop edx
  }

  if (dwFeature & _MMX_FEATURE_BIT) {
    feature |= _CPU_FEATURE_MMX;
    if (_os_support(_CPU_FEATURE_MMX))
      os_support |= _MMX_FEATURE_BIT;//_CPU_FEATURE_MMX;
  }
  if (dwFeatureEx & _3DNOW_FEATURE_BIT) {
    feature |= _CPU_FEATURE_3DNOW;
//    if (_os_support(_CPU_FEATURE_3DNOW))
//      os_support |= _3DNOW_FEATURE_BIT;//_CPU_FEATURE_3DNOW;
  }
  if (dwFeature & _SSE_FEATURE_BIT) {
    feature |= _CPU_FEATURE_SSE;
    if (_os_support(_CPU_FEATURE_SSE))
      os_support |= _SSE_FEATURE_BIT;//_CPU_FEATURE_SSE;
  }
  if (dwFeature & _SSE2_FEATURE_BIT) {
    feature |= _CPU_FEATURE_SSE2;
    if (_os_support(_CPU_FEATURE_SSE2))
      os_support |= _SSE2_FEATURE_BIT;//_CPU_FEATURE_SSE2;
  }

  if (pinfo) {
    memset(pinfo, 0, sizeof(_p_info));

    pinfo->os_support = os_support;
    pinfo->feature = dwFeature; //feature;
    pinfo->extended = dwFeatureEx;
    pinfo->family = (dwStandard >> 8) & 0xF; // retrieve family
    pinfo->familyex = (dwStandard >> 20) & 0xFF; // retrieve extended family
//    if (pinfo->family == 15) {         // retrieve extended family
//      pinfo->family |= (dwStandard >> 16) & 0xFF0;
//    }
    pinfo->model = (dwStandard >> 4) & 0xF;  // retrieve model
    pinfo->modelex = (dwStandard >> 16) & 0xF;
//    if (pinfo->model == 15) {        // retrieve extended model
//      pinfo->model |= (dwStandard >> 12) & 0xF;
//    }
    pinfo->stepping = (dwStandard) & 0xF;  // retrieve stepping
    pinfo->brand = (dwBrand) & 0xFF;

    Ident.cBuf[12] = 0;
    strcpy(pinfo->v_name, Ident.cBuf);

    map_mname(pinfo);

    pinfo->checks = _CPU_FEATURE_MMX |
            _CPU_FEATURE_SSE |
            _CPU_FEATURE_SSE2 |
            _CPU_FEATURE_3DNOW;
  }

  return feature;
}
