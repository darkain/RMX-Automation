/******************************  RMX SDK  ******************************\
*  Copyright (c) 2007 Vincent E. Milum Jr., All rights reserved.        *
*                                                                       *
*  See license.txt for more information                                 *
*                                                                       *
*  Latest SDK versions can be found at:  http://rmx.sourceforge.net     *
\***********************************************************************/


/*  X10 House Code Definitions  */
#define HOUSE_A 0x60
#define HOUSE_B 0xE0
#define HOUSE_C 0x20
#define HOUSE_D 0xA0
#define HOUSE_E 0x10
#define HOUSE_F 0x90
#define HOUSE_G 0x50
#define HOUSE_H 0xD0
#define HOUSE_I 0x70
#define HOUSE_J 0xF0
#define HOUSE_K 0x30
#define HOUSE_L 0xB0
#define HOUSE_M 0x00
#define HOUSE_N 0x80
#define HOUSE_O 0x40
#define HOUSE_P 0xC0

/*  X10 Unit Code Definitions  */
#define UNIT_1  0x06
#define UNIT_2  0x0E
#define UNIT_3  0x02
#define UNIT_4  0x0A
#define UNIT_5  0x01
#define UNIT_6  0x09
#define UNIT_7  0x05
#define UNIT_8  0x0D
#define UNIT_9  0x07
#define UNIT_10 0x0F
#define UNIT_11 0x03
#define UNIT_12 0x0B
#define UNIT_13 0x00
#define UNIT_14 0x08
#define UNIT_15 0x04
#define UNIT_16 0x0C

/*	CM11 Command Header Definitions  */
#define STANDARD_ADDRESS  0x04
#define STANDARD_FUNCTION 0x06
#define ENHANCED_ADDRESS  0x05
#define ENHANCED_FUNCTION 0x07

/*  X10 Function Codes  */
#define ALL_UNITS_OFF      0x00
#define ALL_LIGHTS_ON      0x01
#define ON                 0x02
#define OFF                0x03
#define DIM                0x04
#define BRIGHT             0x05
#define ALL_LIGHTS_OFF     0x06
#define EXTENDED_CODE      0x07
#define HAIL_REQ           0x08
#define HAIL_ACK           0x09
#define PRESET_DIM_1       0x0A
#define PRESET_DIM_2       0x0B
#define EXTENDED_DATA_XFER 0x0C
#define STATUS_REQUEST     0x0F

/*  X10 Status Request response codes  */
#define STATUS_ON          0x0D
#define STATUS_OFF         0x0E

/*  CM11 Handshaking codes  */
#define SET_TIME        0x9B
#define INFERFACE_READY 0x55
#define COMPUTER_READY  0xC3
#define ACK             0x00

/*  CM11 Hail codes  */
#define INTERFACE_CQ    0x5A
#define CM11_CLOCK_REQ  0xA5
#define CP10_CLOCK_REQ  0xA6

/*  CM11 Command codes  */
#define MACRO_DNLD      0xFB
#define SI_ENABLE       0xEB
#define SI_DISABLE      0xDB
#define STATUS_REQ_CM11 0x8B
#define STATUS_REQ_CP10 0x6B
