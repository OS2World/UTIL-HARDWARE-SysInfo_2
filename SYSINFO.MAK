# IBM Developer's Workframe/2 Make File Creation run at 16:01:52 on 01/01/93

# Make File Creation run in directory:
#   D:\DEV\SYSINFO;

.SUFFIXES:

.SUFFIXES: .c .rc .ipf

ALL: SYSINFO.EXE \
     SYSINFO.HLP \
     SYSINFO.RES

SYSINFO.EXE:  \
  SYSINFO.OBJ \
  SYSINFO.RES \
  SYSINFO.DEF \
  SYSINFO.MAK
   @REM @<<SYSINFO.@0
     /CO /PM:PM +
     SYSINFO.OBJ
     SYSINFO.EXE
     
     NOTEBOOK.LIB
     SYSINFO.DEF;
<<
   LINK386.EXE @SYSINFO.@0
   RC SYSINFO.RES SYSINFO.EXE

{.}.rc.res:
   RC -r .\$*.RC

{.}.ipf.hlp:
   IPFC .\$*.IPF

{.}.c.obj:
   ICC.EXE /Sm /Ss /Fl /Ls /Lt"Sysinfo - (C)opyright 1992, Gene Backlin" /Lu"A System Information Viewer" /Ti /Gm /C .\$*.c

!include SYSINFO.DEP
