/****************************************************
**                                                 **
**    notebook.h                                   **
**                                                 **
****************************************************/
#define NB_DEFAULT_MAJOR_FLAGS     (BKA_MAJOR | BKA_STATUSTEXTON | \
                                    BKA_AUTOPAGESIZE)

#define NB_DEFAULT_MINOR_FLAGS     (BKA_MINOR | BKA_STATUSTEXTON | \
                                    BKA_AUTOPAGESIZE)

#define NB_DEFAULT_NOTABS_FLAGS    (BKA_STATUSTEXTON | \
                                    BKA_AUTOPAGESIZE)

#define NB_DEFAULT_STYLES          (BKS_SOLIDBIND      |   \ 
                                    BKS_BACKPAGESBR    |   \ 
                                    BKS_MAJORTABRIGHT  |   \ 
                                    BKS_SQUARETABS     |   \ 
                                    BKS_TABTEXTCENTER  |   \ 
                                    BKS_STATUSTEXTLEFT)    \ 

#define NB_DEFAULT_ROUNDTAB_STYLES (BKS_SOLIDBIND      |   \ 
                                    BKS_BACKPAGESBR    |   \ 
                                    BKS_MAJORTABRIGHT  |   \ 
                                    BKS_ROUNDEDTABS    |   \ 
                                    BKS_TABTEXTCENTER  |   \ 
                                    BKS_STATUSTEXTLEFT)    \ 

#define NB_DEFAULT_SPIRAL_STYLES   (BKS_SPIRALBIND     |   \ 
                                    BKS_BACKPAGESBR    |   \ 
                                    BKS_MAJORTABRIGHT  |   \ 
                                    BKS_ROUNDEDTABS    |   \ 
                                    BKS_TABTEXTCENTER  |   \ 
                                    BKS_STATUSTEXTLEFT)    \ 

#define  NB_DEFAULT_MAJOR_TAB_WIDTH          70
#define  NB_DEFAULT_MAJOR_TAB_HEIGHT         25
#define  NB_DEFAULT_MINOR_TAB_WIDTH          70
#define  NB_DEFAULT_MINOR_TAB_HEIGHT         25

#define  MAJOR_TAB                           BKA_MAJORTAB
#define  MINOR_TAB                           BKA_MINORTAB

#define  NB_CLR_BGND                         SYSCLR_FIELDBACKGROUND
#define  NB_DEFAULT_BGND_COLOR               BKA_BACKGROUNDPAGECOLORINDEX

/****************************************************
*                                                   *
*  Structure Definitions                            *
*                                                   *
****************************************************/
typedef struct _NOTEBOOK_TAB_DIMENSIONS
{
  LONG lMajorTabWidth;
  LONG lMajorTabHeight;
  PSZ  szMajorTabText;
  LONG lMinorTabWidth;
  LONG lMinorTabHeight;
  PSZ  szMinorTabText;
}NBTAB, *PNBTAB;

typedef struct _NOTEBOOK_CONTROL
{
  HWND   hwndNoteBook;
  ULONG  ulStyles;
  PFNWP  pfnwpDlgProc;
  ULONG  ulDlgId;
  HWND   hwndParent;
  RECTL  rclPos;                                // xLeft, yBottom, xRight, yTop
  HWND   hwndOwner;
  HWND   hwndPosition;
  ULONG  ulWinId;
  USHORT usTabType;
  NBTAB  nbtDimensions;
  PSZ    szStatusLineText;
  SHORT  sPageFlags;
  SHORT  sPagePlacementFlags;
}NBC, *PNBC;

/****************************************************
*                                                   *
*  Function Prototypes                              *
*                                                   *
****************************************************/
BOOL    APIENTRY WinCreateStdNotebook(
                     HWND  hwnd,
                     PNBC  nbControl,
                     LONG  xLeft,
                     LONG  yBottom,
                     LONG  xRight,
                     LONG  yTop,
                     ULONG ulWinId,
                     HWND  hwndParentNB,
                     HWND  hwndOwnerNB
                     );

BOOL APIENTRY CreateNotebook (HWND hwnd, PNBC nbControl);

BOOL APIENTRY SetTabDimensions  (
                     HWND hwnd,
                     PNBC nbControl,
                     LONG lMajorTabWidth,
                     LONG lMajorTabHeight,
                     LONG lMinorTabWidth,
                     LONG lMinorTabHeight
                     );

BOOL APIENTRY WinDestroyNotebook  (HWND hwnd, PNBC nbControl);

BOOL APIENTRY WinDisplayNotebook  (HWND hwnd, PNBC nbControl);

BOOL APIENTRY WinInsertNotebookPage  (
                     HWND hwnd,
                     PNBC nbControl,
                     BOOL MajorTabs,
                     BOOL MinorTabs,
                     BOOL DialogBoxIncluded,
                     BOOL StatusLine
                     );

BOOL APIENTRY WinCreateNotebook   (
                     HWND  hwnd,
                     PNBC  nbControl,
                     LONG  xLeft,
                     LONG  yBottom,
                     LONG  xRight,
                     LONG  yTop,
                     ULONG ulWinId,
                     HWND  hwndParentNB,
                     HWND  hwndOwnerNB,
                     ULONG ulNotebookStyles,
                     SHORT sPagePlacement
                     );
