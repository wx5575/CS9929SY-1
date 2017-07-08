/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.26                          *
*        Compiled Aug  8 2014, 14:49:54                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/
#define ID_WINDOW_0     (GUI_ID_USER + 0x00)
#define ID_EDIT_0     (GUI_ID_USER + 0x01)
#define ID_DROPDOWN_0     (GUI_ID_USER + 0x02)
#define ID_TEXT_0     (GUI_ID_USER + 0x03)
#define ID_LISTVIEW_0     (GUI_ID_USER + 0x04)
#define ID_BUTTON_0     (GUI_ID_USER + 0x05)
#define ID_BUTTON_1     (GUI_ID_USER + 0x06)


// USER START (Optionally insert additional defines)
// USER END

/*********************************************************************
*
*       Static data
*
**********************************************************************
*/

// USER START (Optionally insert additional static data)
// USER END

/*********************************************************************
*
*       _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect, "Window", ID_WINDOW_0, 0, 1, 800, 480, 0, 0x0, 0 },
  { EDIT_CreateIndirect, "Edit", ID_EDIT_0, 186, 185, 80, 20, 0, 0x64, 0 },
  { DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 188, 228, 80, 19, 0, 0x0, 0 },
  { TEXT_CreateIndirect, "Text", ID_TEXT_0, 189, 262, 80, 20, 0, 0x64, 0 },
  { LISTVIEW_CreateIndirect, "Listview", ID_LISTVIEW_0, 4, 1, 545, 159, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_0, 49, 243, 80, 20, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_1, 56, 274, 80, 20, 0, 0x0, 0 },
  // USER START (Optionally insert additional widgets)
  // USER END
};
