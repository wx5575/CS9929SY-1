/*********************************************************************
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                           www.segger.com                           *
**********************************************************************
*                                                                    *
* C-file generated by                                                *
*                                                                    *
*        emWin Font Converter (ST) version 5.26                      *
*        Compiled Aug 14 2014, 10:10:08                              *
*                                                                    *
*        (c) 1998 - 2014 Segger Microcontroller GmbH & Co. KG        *
*                                                                    *
**********************************************************************
*                                                                    *
* Source file: ����16.c                                              *
* Font:        ����                                                  *
* Height:      16                                                    *
*                                                                    *
**********************************************************************
*                                                                    *
* Initial font height:  16                                           *
* Range disabled:       0000 - FFFF                                  *
* Character enabled:   0x30 (48)                                     *
*                                                                    *
**********************************************************************
*/

#include "GUI.h"

#ifndef GUI_CONST_STORAGE
  #define GUI_CONST_STORAGE const
#endif

/* The following line needs to be included in any file selecting the
   font.
*/
extern GUI_CONST_STORAGE GUI_FONT GUI_Font16;

/* Start of unicode area <Basic Latin> */
GUI_CONST_STORAGE unsigned char acGUI_Font16_0030[ 11] = { /* code 0030, DIGIT ZERO */
  __XX____,
  _X__X___,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  X____X__,
  _X__X___,
  __XX____};

GUI_CONST_STORAGE GUI_CHARINFO_EXT GUI_Font16_CharInfo[1] = {
   {   6,  11,   1,   3,   8, acGUI_Font16_0030 } /* code 0030, DIGIT ZERO */
};

GUI_CONST_STORAGE GUI_FONT_PROP_EXT GUI_Font16_Prop1 = {
   0x0030 /* first character */
  ,0x0030 /* last character  */
  ,&GUI_Font16_CharInfo[  0] /* address of first character */
  ,(GUI_CONST_STORAGE GUI_FONT_PROP_EXT *)0 /* pointer to next GUI_FONT_PROP_EXT */
};

GUI_CONST_STORAGE GUI_FONT GUI_Font16 = {
   GUI_FONTTYPE_PROP_EXT /* type of font    */
  ,16 /* height of font  */
  ,16 /* space of font y */
  ,1 /* magnification x */
  ,1 /* magnification y */
  ,{&GUI_Font16_Prop1}
  ,14 /* Baseline */
  ,7 /* Height of lowercase characters */
  ,11 /* Height of capital characters */
};

