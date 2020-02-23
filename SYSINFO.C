/****************************************************
**                                                 **
** Program: Sysinfo.c                              **
** Author : Gene Backlin                           **
**                                                 **
** Address: CompuServe ID 70401,1574               **
**                                                 **
** Purpose: To give the User as well as the        **
**          Programmer new to OS/2's Presentation  **
**          Manager interface, a generic shell, to **
**          see how the pieces are fit together.   **
**                                                 **
** Written: 12/25/92                               **
** Revised: 12/30/92                               **
**    Version 1.00 - 12/25/92                      **
**       Original Version                          **
**    Version 1.10 - 12/30/92                      **
**       Added Notebook Control                    **
**                                                 **
****************************************************/
#define INCL_32

#define INCL_WIN
#define INCL_GPI

#define INCL_WINHELP                            // Include IPF Header File
#define INCL_VIO

#define INCL_DOS
#define INCL_DOSFILEMGR
#define INCL_DOSMEMMGR
#define INCL_DONMPIPES
#define INCL_ERRORS

#define MAX	24

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <bsedos.h>

#include "sysinfo.h"
#include "notebook.h"

struct _sys_settings
{
  ULONG ulMaxPath; 						
  ULONG ulMaxTextSessions; 			
  ULONG ulMaxPMSessions; 				
  ULONG ulMaxVDMSessions; 				
  ULONG ulBootDrive; 					
  ULONG ulDynamicPriorityVariation; 
  ULONG ulMaxWait_sec; 					
  ULONG ulMinTimeSlice_ms; 			
  ULONG ulMaxTimeSlice_ms; 			
  ULONG ulPageSize_by; 					
  ULONG ulVersionMajor; 				
  ULONG ulVersionMinor; 				
  ULONG ulVersionRevision; 			
  ULONG ulMilliSecCounter;			
  ULONG ulLowOrdTime; 					
  ULONG ulHighOrdTime; 					
  ULONG ulTotalPhysicalMemoryPages; 
  ULONG ulTotalResidentMemoryPages; 
  ULONG ulTotalAvailableMemoryPages;
  ULONG ulTotalMaxPrivateMemory_by;
  ULONG ulTotalMaxSharedMemory_by;
  ULONG ulTimerInterval;
  ULONG ulMaxComponentLength_by; 	
} sys_settings, *psys_settings;

ULONG   StartIndex;   	 	/* Ordinal of 1st variable to return 	*/
ULONG   LastIndex;     		/* Ordinal of last variable to return 	*/
ULONG   DataBuf[MAX];   	/* System information (returned) 		*/
ULONG   DataBufLen;    		/* Data buffer size 							*/
APIRET  rc;            		/* Return code 								*/

CHAR szErrMsg  [79] = "\0";
CHAR szErrTitle[40] = "\0";

HWND hwndFrame;                                 // PM Handle to the Frame

/****************************************************
*                                                   *
*  Function Prototypes                              *
*                                                   *
****************************************************/
MRESULT EXPENTRY ClientWndProc   (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY AboutDlgProc    (HWND, ULONG, MPARAM, MPARAM);
MRESULT EXPENTRY CenterDlg       (HWND hwnd);
INT     EXPENTRY get_system_info (HWND);
INT     EXPENTRY system_info_out (HWND);

BOOL    AddNotebookPages (HWND, PNBC);
FNWP    NotebookDlgProc;

/****************************************************
*                                                   *
*  Required IPF Structure Declarations              *
*                                                   *
****************************************************/
HELPINIT    hmiHelpData;                        // Help Initialization Structure
HWND        hwndHelpInstance;                   // Handle to the Help Window

/**/
/****************************************************
*                                                   *
*  Main Function                                    *
*                                                   *
****************************************************/
INT main(INT argc, CHAR *argv, CHAR *envp)
{
   HAB     hab;                                 // Handle to Application  
   HMQ     hmq;                                 // Hold the Application's Message Queue
   QMSG    qmsg;                                // The actual Queue Message             

   HWND    hwndClient;                          // PM Window Handles
   HWND    hwndMenu;                            // Menu Handle
   HWND    hwndTemp;                            // Temporary Menu Handle

   SWCNTRL SwData;                              // Switch control data block
   HSWITCH hSwitch;                             // Switch entry handle

   ULONG flFrameFlags =                         // Set the Frame-window creation Flags
      FCF_MENU          |                       // Application Menu
      FCF_TITLEBAR      |                       // Application Title
      FCF_SIZEBORDER    |                       // Application Size Border
      FCF_MINMAX        |                       // Minimize and Maximum Buttons
      FCF_SYSMENU       |                       // Application System Menu
      FCF_SHELLPOSITION	|                       // System default size and position
      FCF_ICON          |                       // System default size and position
      FCF_TASKLIST;                             // Add name to TaskList

   hab = WinInitialize(0);                      // Register the Application
   hmq = WinCreateMsgQueue(hab, 0);             // Create the Message Queue

   if (!WinRegisterClass(                       // Register the window class
      hab,                                      // Handle to application
      CLIENT_CLASS,                             // Name of the Class
      ClientWndProc,	                           // Window procedure name
      CS_SIZEREDRAW,	                           // Window style
      0))                                       // Extra window words
         return (FALSE);                        // Terminate if Unsuccessful

/****************************************************
*                                                   *
*  IPF Initialization Structure                     *
*                                                   *
****************************************************/
   hmiHelpData.cb                      = sizeof(HELPINIT);	// size of initialization structure
   hmiHelpData.ulReturnCode            = 0;        // store HM return code from init
   hmiHelpData.pszTutorialName         = 0;        // no tutorial program
   hmiHelpData.phtHelpTable            = (PVOID)
                  (0xffff0000 | MAIN_HELPTABLE);   // help table defined in RC file
   hmiHelpData.hmodAccelActionBarModule= 0;
   hmiHelpData.idAccelTable            = 0;
   hmiHelpData.idActionBar             = 0;
   hmiHelpData.pszHelpWindowTitle      = HELPTITLE;
   hmiHelpData.hmodHelpTableModule     = 0;        // Help Table not in a DLL
   hmiHelpData.fShowPanelId            = 0;        // Help Panels ID is not displayed
   hmiHelpData.pszHelpLibraryName      = HELPFILE; // Library with help panels

/****************************************************
*                                                   *
*  Create Instance of IPF                           *
*                                                   *
****************************************************/
   hwndHelpInstance =                            // Pass Anchor Block handle and address 
      WinCreateHelpInstance(hab, &hmiHelpData);  // of IPF initialization structure

   if(!hwndHelpInstance)
      {
      WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                   (PSZ) "Help Not Available",
                   (PSZ) "Help Creation Error",
                   1,
                   MB_OK | MB_APPLMODAL | MB_MOVEABLE);
      }
   else
      {
      if(hmiHelpData.ulReturnCode)
         {
         WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                      (PSZ) "Help NotAvailable",
                      (PSZ) "Help Creation Error",
                      1,
                      MB_OK | MB_APPLMODAL | MB_MOVEABLE);
         WinDestroyHelpInstance(hwndHelpInstance);
         }
      }

/****************************************************
*                                                   *
*  Create a Top-Level frame window with a client    *
*  window that belongs to the window class          *
*  CLIENT_CLASS. (see sysinfo.h)                    *
*                                                   *
****************************************************/
   hwndFrame = WinCreateStdWindow(              // Create the Frame Window
                      HWND_DESKTOP,	            // Parent is the Desktop
                      0L,                       // Don't make frame window visible
                      &flFrameFlags,            // Frame Controls
                      CLIENT_CLASS,	            // Window class for client
                      TITLE,                    // Window Title
                      0L,                       // Don't make client window visible
                      0,                        // Resources in application model
                      ID_MENU_RESOURCE,         // Resource identifier
                      &hwndClient);	            // Pointer to client window handle
   if (!hwndFrame)
      return (FALSE);

   WinSetWindowPos(
      hwndFrame,                                // Shows and activates frame
      HWND_TOP,                                 // Put the window on top
      55,                                       // Positon x
      350,                                      // Positon y
      525,                                      // New width
      75,                                       // New height
      SWP_SIZE      |                           // Change the size
      SWP_MOVE      |                           // Move the window
      SWP_ACTIVATE  |                           // Make it the active window
      SWP_SHOW                                  // Make it visible
      );

   hwndMenu =
      WinWindowFromID( hwndFrame, FID_MENU);    // Get the Menu handle

   SwData.hwnd          = hwndFrame;            // Set frame Window handle
   SwData.hwndIcon      = 0;                    // Use the default Icon
   SwData.hprog;                                // Use default program handle
   SwData.idProcess     = 0;                    // Use current process id
   SwData.idSession     = 0;                    // Use current session id
   SwData.uchVisibility	= SWL_VISIBLE;          // Make Visible
   SwData.fbJump        = SWL_JUMPABLE;         // Make Jumpable via Alt+Esc
   SwData.szSwtitle[0]  = '\0';                 // Use default Title Test

   hSwitch = WinAddSwitchEntry(&SwData);        // Add task manager entry

/****************************************************
*                                                   *
*  Associate Instance of IPF                        *
*                                                   *
****************************************************/
   if(hwndHelpInstance)
      WinAssociateHelpInstance(hwndHelpInstance, hwndFrame);

/****************************************************
*                                                   *
*  Start the main message loop. Get Messages from   *
*  the queue and dispatch them to the appropriate   *
*  windows.                                         *
*                                                   *
****************************************************/
   while(WinGetMsg(hab, &qmsg, 0, 0, 0))        // Loop until WM_QUIT
      WinDispatchMsg(hab, &qmsg);

/****************************************************
*                                                   *
*  Main Loop has terminated. Destroy all windows and*
*  the message queue; then terminate the application*
*                                                   *
****************************************************/
   if(hwndHelpInstance)
      WinDestroyHelpInstance(hwndHelpInstance);	// Destroy Help Instance

   WinDestroyWindow(hwndFrame);                 // Destroy main window
   WinDestroyMsgQueue(hmq);                     // Destroy message queue
   WinTerminate(hab);                           // Deregister application

   return 0;
}

/**/
/****************************************************
*                                                   *
*  Client Window Procedure                          *
*                                                   *
****************************************************/
MRESULT EXPENTRY ClientWndProc( HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2)
{
   HWND    hwndTemp;                            // Temporary Menu Handle
   RECTL   rcl;                                 // Window rectangle
   HPS     hps;                                 // Handle to the Presentation space
   UCHAR   szMsg[40];                           // Standared Text Message
   UCHAR   szTitle[20];                         // Title...
   ULONG   rc;                                  // Status return code
   static  NBC nbControl;                       // Notebook Control parameters

   switch(msg)
      {
      case WM_CREATE:
         WinSendMsg(                            // is just the
              hwnd,                             // About Box
              WM_COMMAND,                       // Credit
              (MPARAM) IDM_HEL_ABOUT,	         // Display.
              0L);
         break;
      
      case WM_ERASEBACKGROUND:
        return (MRESULT)(TRUE);
      
      case WM_CHAR:
        return (MRESULT)(TRUE);
      
      case WM_PAINT:
        hps = WinBeginPaint(hwnd, 0, &rcl);
        WinEndPaint(hps);
        break;
      
      case WM_COMMAND:
        switch (SHORT1FROMMP(mp1))
          {
          case IDM_DIS_NOTEBOOK:
            WinCreateStdNotebook(hwnd,           // Application Window Handle
                              &nbControl,        // Notebook Control Structure
                              160,               // xLeft Position
                              15,                // yBottom Position
                              350,               // xRight Position
                              325,               // yTop Position
                              ID_NOTEBOOK,       // Notebook Window ID
                              HWND_DESKTOP,      // Parent Window Handle
                              HWND_DESKTOP       // Owner Window Handle
                              );
            AddNotebookPages(hwnd, &nbControl);  // Add pages to the Notebook Control
            WinDisplayNotebook(hwnd,&nbControl); // Display the Notebook Control
            return 0;
      
          case IDM_DIS_NOTEBOOK_EXIT:
            WinDestroyNotebook(hwnd, &nbControl);
            return 0;

          case IDM_HEL_ABOUT:                    // Program Credits
            rc = WinDlgBox(
                    HWND_DESKTOP,                // Desktop is parent
                    hwnd,                        // Current window is owner
                    AboutDlgProc,                // Entry point of dialog proc.
                    0,                           // Resource is in EXE
                    IDD_ABOUTBOX,                // Dialog resource identifier
                    (PVOID)NULL);                // Pointer to initialization dat
            return 0;
      
          case IDM_EXIT:
            WinSendMsg(hwnd, WM_CLOSE, 0, 0);    // Exit the Program
            return 0;
      
          case IDM_HELP_FOR_HELP:
            if(hwndHelpInstance)
              WinSendMsg(hwndHelpInstance, HM_DISPLAY_HELP, 0L, 0L);
            break;
      
          default:
            return WinDefWindowProc(hwnd, msg, mp1, mp2);
          }
        break;
      
      case HM_ERROR:
        if((hwndHelpInstance && (ULONG) mp1) == HMERR_NO_MEMORY)
          {
          WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                       (PSZ) "Help Terminated Due to Error",
                       (PSZ) "Help Error",
                       1,
                       MB_OK | MB_APPLMODAL | MB_MOVEABLE);
            WinDestroyHelpInstance(hwndHelpInstance);
          }
        else
          {
            WinMessageBox(HWND_DESKTOP, HWND_DESKTOP,
                       (PSZ) "Help Error Occurred",
                       (PSZ) "Help Error",
                       1,
                       MB_OK | MB_APPLMODAL | MB_MOVEABLE);
          }
        break;
      
      case WM_CLOSE:
        /*
         * This is the place to put your termination routines
         */
      
        WinPostMsg( hwnd, WM_QUIT, 0L, 0L );	// Cause termination
        break;
      
      default:
        return WinDefWindowProc(hwnd, msg, mp1, mp2); 
      }
  return (MRESULT)FALSE;
}

/**/
/****************************************************
*                                                   *
*  About Window Procedure                           *
*                                                   *
****************************************************/
MRESULT EXPENTRY AboutDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
  switch(msg)
    {
    case WM_INITDLG:
      CenterDlg(hwnd);
      return 0;

    case WM_COMMAND:
      switch( SHORT1FROMMP( mp1 ) )             // Extract the command value
        {
        case DID_OK:
          WinDismissDlg( hwnd, DID_OK );
        break;

        case DID_CANCEL:
          WinDismissDlg( hwnd, DID_CANCEL );
        break;
        }

    default:
      return(WinDefDlgProc(hwnd, msg, mp1, mp2));
      break;
    }
  return(MPVOID);
}   /* AboutDlgProc() */

/**/
/****************************************************
*                                                   *
*  Center Dialog Box Procedure                      *
*                                                   *
****************************************************/
MRESULT EXPENTRY CenterDlg(HWND hwnd)
{
   RECTL rclScreen;
   RECTL rclDialog;
   LONG  sWidth, sHeight, sBLCx, sBLCy;
   
   WinQueryWindowRect(HWND_DESKTOP, &rclScreen);
   WinQueryWindowRect(hwnd, &rclDialog);
   
   sWidth  = (LONG) (rclDialog.xRight - rclDialog.xLeft);
   sHeight = (LONG) (rclDialog.yTop   - rclDialog.yBottom);
   sBLCx   = ((LONG) rclScreen.xRight - sWidth)  / 2;
   sBLCy   = ((LONG) rclScreen.yTop   - sHeight) / 2;
   
   WinSetWindowPos(
      hwnd,                                    // Activates frame
      HWND_TOP,                                // Put the window on top
      sBLCx,                                   // Positon x
      sBLCy,                                   // Positon y
      0,                                       // New width
      0,                                       // New height
      SWP_MOVE);                               // Move the window
                                               
   return 0;                                   
}

/**/
/*******************************************************
**                                                    **
**       AddNotebookPages                             **
**                                                    **
*******************************************************/
BOOL AddNotebookPages (HWND hwnd, PNBC nbControl)
{
   nbControl->nbtDimensions.lMinorTabWidth = 90;

/********************************************
*       Setup Title Page                   **
********************************************/
   nbControl->sPageFlags                   = NB_DEFAULT_MAJOR_FLAGS;
   nbControl->nbtDimensions.szMajorTabText = (PSZ)"System";
   nbControl->szStatusLineText             = (PSZ)"Current Operating System";
   nbControl->pfnwpDlgProc                 = NotebookDlgProc;
   nbControl->ulDlgId                      = IDD_NB_PAGE1;
   
   WinInsertNotebookPage(hwnd,
                      nbControl,                // Notebook Control 
                      TRUE,                     // Are there Major Tabs on Page ?
                      FALSE,                    // Are there Minor Tabs on Page ?
                      TRUE,                     // Is there a Dialog Box associated with Page ?
                      TRUE                      // Is there Status Line Text ?
                      );

/********************************************
*       Setup Page 1A                      **
********************************************/
   nbControl->sPageFlags                   = NB_DEFAULT_MINOR_FLAGS;
   nbControl->nbtDimensions.szMinorTabText = (PSZ)"Values";
   nbControl->szStatusLineText             = (PSZ)"Current Maximum Settings";
   nbControl->pfnwpDlgProc                 = NotebookDlgProc;
   nbControl->ulDlgId                      = IDD_NB_PAGE1A;
   
   WinInsertNotebookPage(hwnd,
                      nbControl,                // Notebook Control 
                      FALSE,                    // Are there Major Tabs on Page ?
                      TRUE,                     // Are there Minor Tabs on Page ?
                      TRUE,                     // Is there a Dialog Box associated with Page ?
                      TRUE                      // Is there Status Line Text ?
                      );

/********************************************
*       Setup Page 1B                      **
********************************************/
   nbControl->sPageFlags                   = NB_DEFAULT_MINOR_FLAGS;
   nbControl->nbtDimensions.szMinorTabText = (PSZ)"Time";
   nbControl->szStatusLineText             = (PSZ)"Current Settings";
   nbControl->pfnwpDlgProc                 = NotebookDlgProc;
   nbControl->ulDlgId                      = IDD_NB_PAGE1B;
   
   WinInsertNotebookPage(hwnd,
                      nbControl,                // Notebook Control 
                      FALSE,                    // Are there Major Tabs on Page ?
                      TRUE,                     // Are there Minor Tabs on Page ?
                      TRUE,                     // Is there a Dialog Box associated with Page ?
                      TRUE                      // Is there Status Line Text ?
                      );

/********************************************
*       Setup Page 2                       **
********************************************/
   nbControl->sPageFlags                   = NB_DEFAULT_MAJOR_FLAGS;
   nbControl->nbtDimensions.szMajorTabText = (PSZ)"Memory";
   nbControl->szStatusLineText             = (PSZ)"Current Operating System";
   nbControl->pfnwpDlgProc                 = NotebookDlgProc;
   nbControl->ulDlgId                      = IDD_NB_PAGE2;
   
   WinInsertNotebookPage(hwnd,
                      nbControl,                // Notebook Control 
                      TRUE,                     // Are there Major Tabs on Page ?
                      FALSE,                    // Are there Minor Tabs on Page ?
                      TRUE,                     // Is there a Dialog Box associated with Page ?
                      TRUE                      // Is there Status Line Text ?
                      );

/********************************************
*       Setup Page 2A                      **
********************************************/
   nbControl->sPageFlags                   = NB_DEFAULT_MINOR_FLAGS;
   nbControl->nbtDimensions.szMinorTabText = (PSZ)"Values";
   nbControl->szStatusLineText             = (PSZ)"Current Maximum Settings";
   nbControl->pfnwpDlgProc                 = NotebookDlgProc;
   nbControl->ulDlgId                      = IDD_NB_PAGE2A;
   
   WinInsertNotebookPage(hwnd,
                      nbControl,                // Notebook Control 
                      FALSE,                    // Are there Major Tabs on Page ?
                      TRUE,                     // Are there Minor Tabs on Page ?
                      TRUE,                     // Is there a Dialog Box associated with Page ?
                      TRUE                      // Is there Status Line Text ?
                      );

/********************************************
*       Setup Page 3                       **
********************************************/
   nbControl->sPageFlags                   = NB_DEFAULT_MAJOR_FLAGS;
   nbControl->nbtDimensions.szMajorTabText = (PSZ)"Timers";
   nbControl->szStatusLineText             = (PSZ)"Current System Settings";
   nbControl->pfnwpDlgProc                 = NotebookDlgProc;
   nbControl->ulDlgId                      = IDD_NB_PAGE3;
   
   WinInsertNotebookPage(hwnd,
                      nbControl,                // Notebook Control 
                      TRUE,                     // Are there Major Tabs on Page ?
                      FALSE,                    // Are there Minor Tabs on Page ?
                      TRUE,                     // Is there a Dialog Box associated with Page ?
                      TRUE                      // Is there Status Line Text ?
                      );

/********************************************
*       Setup Page 3A                      **
********************************************/
   nbControl->sPageFlags                   = NB_DEFAULT_MINOR_FLAGS;
   nbControl->nbtDimensions.szMinorTabText = (PSZ)"Values";
   nbControl->szStatusLineText             = (PSZ)"Current System Settings";
   nbControl->pfnwpDlgProc                 = NotebookDlgProc;
   nbControl->ulDlgId                      = IDD_NB_PAGE3A;
   
   WinInsertNotebookPage(hwnd,
                      nbControl,                // Notebook Control 
                      FALSE,                    // Are there Major Tabs on Page ?
                      TRUE,                     // Are there Minor Tabs on Page ?
                      TRUE,                     // Is there a Dialog Box associated with Page ?
                      TRUE                      // Is there Status Line Text ?
                      );

/********************************************
*       Get SysInfo Help                   **
********************************************/
   nbControl->sPageFlags                   = NB_DEFAULT_MAJOR_FLAGS;
   nbControl->nbtDimensions.szMajorTabText = (PSZ)"Help";
   nbControl->szStatusLineText             = (PSZ)"Use F1 for Detailed Help";
   nbControl->pfnwpDlgProc                 = NotebookDlgProc;
   nbControl->ulDlgId                      = IDD_NB_PAGE_HELP;
   
   WinInsertNotebookPage(hwnd,
                      nbControl,                // Notebook Control 
                      TRUE,                     // Are there Major Tabs on Page ?
                      FALSE,                    // Are there Minor Tabs on Page ?
                      TRUE,                     // Is there a Dialog Box associated with Page ?
                      TRUE                      // Is there Status Line Text ?
                      );

/********************************************
*       Close the Notebook                 **
********************************************/
   nbControl->sPageFlags                   = NB_DEFAULT_MAJOR_FLAGS;
   nbControl->nbtDimensions.szMajorTabText = (PSZ)"Exit";
   nbControl->szStatusLineText             = (PSZ)"Press Exit to Close Notebook";
   nbControl->pfnwpDlgProc                 = NotebookDlgProc;
   nbControl->ulDlgId                      = IDD_NB_EXIT_PAGE;
   
   WinInsertNotebookPage(hwnd,
                      nbControl,                // Notebook Control 
                      TRUE,                     // Are there Major Tabs on Page ?
                      FALSE,                    // Are there Minor Tabs on Page ?
                      TRUE,                     // Is there a Dialog Box associated with Page ?
                      TRUE                      // Is there Status Line Text ?
                      );

   return (TRUE);
}

/**/
/*******************************************************
**                                                    **
**       Notebook Dialog Box Procedure                **
**                                                    **
*******************************************************/
MRESULT EXPENTRY NotebookDlgProc(HWND hwnd, ULONG msg, MPARAM mp1, MPARAM mp2 )
{
   CHAR  szTemp[40];
   CHAR  szCurrentTitle[40];

   switch(msg)
     {
     case WM_INITDLG:
       get_system_info(hwnd);
 
       sprintf(szTemp, "%lu.%02lu.%lu",sys_settings.ulVersionMajor/10,
                                       sys_settings.ulVersionMinor,
                                       sys_settings.ulVersionRevision);
       strcpy(szCurrentTitle, " Version ");
       strcat(szCurrentTitle, szTemp);

       WinSetDlgItemText(hwnd, IDD_VERSION, szCurrentTitle);

       return 0;
   
     case WM_COMMAND:
       switch( SHORT1FROMMP( mp1 ) )             // Extract the command value
         {
         case IDD_NB_NOTEBOOK_CLOSE:
          WinSendMsg(                         
              hwndFrame,                           
              WM_COMMAND,                     
              (MPARAM) IDM_DIS_NOTEBOOK_EXIT, 
              0L);
           break;
   
         case DID_OK:
            break;
   
         case DID_CANCEL:
            break;
         }
   
     default:
       return(WinDefDlgProc(hwnd, msg, mp1, mp2));
       break;
     }
   return(WinDefDlgProc(hwnd, msg, mp1, mp2));
}   /* NotebookDlgProc() */

/***************************************/
/*                                     */
/*       Get System Information        */
/*                                     */
/***************************************/
INT EXPENTRY get_system_info(HWND hwnd)
{
  StartIndex = QSV_MAX_PATH_LENGTH;    /* In this example we will ask for the */
  LastIndex  = QSV_MAX_COMP_LENGTH;    /*   maximum number of Text, PM and    */
                     						/*   DOS sessions on the local system  */
 
  DataBufLen = sizeof(DataBuf);    		/* Size of the supplied data buffer 	*/
 
  rc = DosQuerySysInfo((ULONG)StartIndex, (ULONG)LastIndex,
  						 (PVOID)DataBuf, (ULONG)DataBufLen);
                      /* On successful return, the three    */
                      /*   requested doubleword values will */
                      /*   be contained within the supplied */
                      /*   data buffer                      */
  if (rc != 0)
    {
    sprintf(szErrMsg,"DosQuerySysInfo error\n Return Code = %ld", rc);
    rc = WinMessageBox(HWND_DESKTOP,
							  hwnd,	
      					  (char *)szErrMsg,	
      					  (char *)szErrTitle,	
      					  WinQueryWindowUShort(hwnd, QWS_ID), /* Window Id		*/
      					  MB_OK         |	
      					  MB_ERROR);	
      						
    }
  else
    {													   
    sys_settings.ulMaxPath                   = DataBuf[QSV_MAX_PATH_LENGTH - 1];
    sys_settings.ulMaxTextSessions           = DataBuf[QSV_MAX_TEXT_SESSIONS - 1];
    sys_settings.ulMaxPMSessions             = DataBuf[QSV_MAX_PM_SESSIONS - 1];
    sys_settings.ulMaxVDMSessions            = DataBuf[QSV_MAX_VDM_SESSIONS  - 1];
    sys_settings.ulBootDrive                 = DataBuf[QSV_BOOT_DRIVE - 1];
    sys_settings.ulDynamicPriorityVariation  = DataBuf[QSV_DYN_PRI_VARIATION - 1];
    sys_settings.ulMaxWait_sec               = DataBuf[QSV_MAX_WAIT - 1];
    sys_settings.ulMinTimeSlice_ms           = DataBuf[QSV_MIN_SLICE - 1];
    sys_settings.ulMaxTimeSlice_ms           = DataBuf[QSV_MAX_SLICE - 1];
    sys_settings.ulPageSize_by               = DataBuf[QSV_PAGE_SIZE - 1];
    sys_settings.ulVersionMajor              = DataBuf[QSV_VERSION_MAJOR - 1];
    sys_settings.ulVersionMinor              = DataBuf[QSV_VERSION_MINOR - 1];
    sys_settings.ulVersionRevision           = DataBuf[QSV_VERSION_REVISION - 1];
    sys_settings.ulMilliSecCounter           = DataBuf[QSV_MS_COUNT - 1];
    sys_settings.ulLowOrdTime                = DataBuf[QSV_TIME_LOW - 1];
    sys_settings.ulHighOrdTime               = DataBuf[QSV_TIME_HIGH - 1];
    sys_settings.ulTotalPhysicalMemoryPages  = DataBuf[QSV_TOTPHYSMEM - 1];
    sys_settings.ulTotalResidentMemoryPages  = DataBuf[QSV_TOTRESMEM - 1];
    sys_settings.ulTotalAvailableMemoryPages = DataBuf[QSV_TOTAVAILMEM - 1];
    sys_settings.ulTotalMaxPrivateMemory_by  = DataBuf[QSV_MAXPRMEM - 1];
    sys_settings.ulTotalMaxSharedMemory_by   = DataBuf[QSV_MAXSHMEM - 1];
    sys_settings.ulTimerInterval             = DataBuf[QSV_TIMER_INTERVAL - 1];
    sys_settings.ulMaxComponentLength_by     = DataBuf[QSV_MAX_COMP_LENGTH - 1];

	 rc = system_info_out(hwnd);
    }
}

/***************************************/
/*                                     */
/*       Display System Information    */
/*                                     */
/***************************************/
INT EXPENTRY system_info_out(HWND hwnd)
{
	char pTemp[30]	= "\0";
	short rc;

   ltoa(sys_settings.ulMaxPath, pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_MAX_PATH_FLD, pTemp);
	strcpy(pTemp,"\0");

   ltoa(sys_settings.ulMaxTextSessions, pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_MAX_TEXT_FLD, pTemp);

	ltoa(sys_settings.ulMaxPMSessions 			  	, pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_MAX_PMSESS_FLD, pTemp);

	ltoa(sys_settings.ulMaxVDMSessions 			  	, pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_MAX_VDMSESS_FLD, pTemp);

	ltoa(sys_settings.ulBootDrive 					, pTemp, 10);  
   WinSetDlgItemText(hwnd, IDD_DIS_BOOTDRIVE_FLD, pTemp);

	ltoa(sys_settings.ulDynamicPriorityVariation , pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_DYNPRI_FLD, pTemp);

	ltoa(sys_settings.ulMaxWait_sec 				  	, pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_MAX_WAIT_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulMinTimeSlice_ms 			, pTemp, 10);  
   WinSetDlgItemText(hwnd, IDD_DIS_MIN_TIMESLICE_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulMaxTimeSlice_ms 			, pTemp, 10);  
   WinSetDlgItemText(hwnd, IDD_DIS_MAX_TIMESLICE_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulPageSize_by 				  	, pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_PAGESIZE_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulMilliSecCounter 			, pTemp, 10);  
   WinSetDlgItemText(hwnd, IDD_DIS_MS_COUNTER_FLD, pTemp);

	ltoa(sys_settings.ulLowOrdTime 				  	, pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_LO_TIME_FLD, pTemp);

	ltoa(sys_settings.ulHighOrdTime 				  	, pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_HI_TIME_FLD, pTemp);

	ltoa(sys_settings.ulTotalPhysicalMemoryPages , pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_TOT_PHY_MEM_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulTotalResidentMemoryPages , pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_TOT_RES_MEM_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulTotalAvailableMemoryPages, pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_TOT_AVAIL_MEM_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulTotalMaxPrivateMemory_by , pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_MAX_PRIVATE_MEM_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulTotalMaxSharedMemory_by  , pTemp, 10);
   WinSetDlgItemText(hwnd, IDD_DIS_MAX_SHARED_MEM_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulTimerInterval 			  	, pTemp, 10);
	strcat(pTemp," .10/ms");
   WinSetDlgItemText(hwnd, IDD_DIS_TIME_INTERVAL_FLD, pTemp);
	strcpy(pTemp,"\0");

	ltoa(sys_settings.ulMaxComponentLength_by 	, pTemp, 10);  
   WinSetDlgItemText(hwnd, IDD_DIS_MAX_COMP_LEN_FLD, pTemp);
	strcpy(pTemp,"\0");

  return 0;
}
