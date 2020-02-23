:userdoc.
:title.Sysinfo Help
:body.
.*
.*
:h1 res=1900. Help for Sysinfo Help
:p.Use these choices to find out how to use help, to
get extended help.
:dl tsize=5 break=all.
:dt.:hp2.Help for Help:ehp2.
:dd.Provides detailed information on th kinds of help
available and how to use help.
:dt.:hp2.Extended help:ehp2.
:dd.Displays general Help.
:edl.
.*
.*
:h1 res=1910. Help for About
:p.Use this choice to obtain information on the
Author(s) of the Program.
.*
.*
:h1 res=1911. Help for Help
:p.Use this choice to obtain information on how to
use the Help Facility.
.*
.*
:h1 res=1912. Help For Extended Help
:p.Use this choice to obtain general information on the 
tasks you can perform while you are viewing a help window.
.*
.*
:h1 res=110. Extended Help for Sysinfo
:p.The :hp2.Sysinfo:ehp2. program is a program that will enable the user
to see their current system operating parameters.
:p.The user has two Program Options.
:dl tsize=5 break=all.
:dt.:hp2.Display:ehp2.
:dd.They can :hp2.Display:ehp2. System Information.
:dt.:hp2.Exit:ehp2.
:dd.They can :hp2.Exit:ehp2. the Sysinfo Program.
:edl.
:p.:hp2.Sysinfo:ehp2. is a Presentation Manager Utility
that will display the current system operating parameters
using the Notebook Control Style. Sysinfo will display
the following:
:dl tsize=5 break=all.
:dt.:hp2.System:ehp2.
:dd.The user can see parameters that are concerned with the 
overall opertion of the system.
:dt.:hp2.Memory:ehp2.
:dd.The user can see how the system memory is allocated.
:dt.:hp2.Timers:ehp2.
:dd.The user can see the current status of the system's Timers are.
:edl.
.*
.*
:h1 res=1000. Help for the Display Option
:p.Use this option to bring up the Notebook Control
to Display System Information.
.*
.*
:h1 res=1001. Help for System Information
:p.
 Each system variable is a doubleword value.  The following list 
 gives the ordinal index, name, and description of the system 
 variables. 
:dl tsize=5 break=all.
:dt.:hp2.System Information:ehp2.
:dd.      They can see the current settings.
:dt.:hp2.1.QSV_MAX_PATH_LENGTH:ehp2. 
:dd.Maximum length, in bytes, of a path name. 

:dt.:hp2.2.QSV_MAX_TEXT_SESSIONS:ehp2.  
:dd.Maximum number of text sessions. 

:dt.:hp2.3.QSV_MAX_PM_SESSIONS:ehp2.  
:dd.Maximum number of PM sessions. 

:dt.:hp2.4.QSV_MAX_VDM_SESSIONS:ehp2.  
:dd.Maximum number of DOS sessions. 

:dt.:hp2.5.QSV_BOOT_DRIVE:ehp2.  
:dd.Drive from which the system was started (1 means drive A, 
2 means drive B, and so on). 

:dt.:hp2.6.QSV_DYN_PRI_VARIATION:ehp2.  
:dd.Dynamic priority variation flag (0 means absolute priority, 
1 means dynamic priority). 

:dt.:hp2.7.QSV_MAX_WAIT:ehp2.  
:dd.Maximum wait in seconds. 

:dt.:hp2.8.QSV_MIN_SLICE:ehp2.  
:dd.Minimum time slice in milliseconds. 

:dt.:hp2.9.QSV_MAX_SLICE:ehp2.  
:dd.Maximum time slice in milliseconds. 

:dt.:hp2.10.QSV_PAGE_SIZE:ehp2.  
:dd.Memory page size in bytes.  This value is 4096 for the 80386 processor. 

:dt.:hp2.11.QSV_VERSION_MAJOR:ehp2.  
:dd.Major version number. 

:dt.:hp2.12.QSV_VERSION_MINOR:ehp2.  
:dd.Minor version number. 

:dt.:hp2.13.QSV_VERSION_REVISION:ehp2.  
:dd.Revision letter. 

:dt.:hp2.14.QSV_MS_COUNT:ehp2.  
:dd.Value of a 32-bit, free-running millisecond counter.  
This value is zero when the system is started. 

:dt.:hp2.15.QSV_TIME_LOW:ehp2.  
:dd.Low-order 32 bits of the time in seconds since January 1, 
1970 at 0-00-00. 

:dt.:hp2.16.QSV_TIME_HIGH:ehp2.  
:dd.High-order 32 bits of the time in seconds since January 1, 1970 
at 0-00-00. 

:dt.:hp2.17.QSV_TOTPHYSMEM:ehp2.  
:dd.Total number of pages of physical memory in the system.  
One page is 4KB. 

:dt.:hp2.18.QSV_TOTRESMEM:ehp2.  
:dd.Total number of pages of resident memory in the system. 

:dt.:hp2.19.QSV_TOTAVAILMEM:ehp2.  
:dd.Maximum number of pages of memory that can be allocated by 
all processes in the system.  This number is advisory and is not 
guaranteed, since system conditions change constantly. 

:dt.:hp2.20.QSV_MAXPRMEM:ehp2.  
:dd.Maximum number of bytes of memory that this process can allocate 
in its private arena.  This number is advisory and is not guaranteed, 
since system conditions change constantly. 

:dt.:hp2.21.QSV_MAXSHMEM:ehp2.  
:dd.Maximum number of bytes of memory that a process can allocate in 
the shared arena.  This number is advisory and is not guaranteed, 
since system conditions change constantly. 

:dt.:hp2.22.QSV_TIMER_INTERVAL:ehp2.  
:dd.Timer interval in tenths of a millisecond. 

:dt.:hp2.23.QSV_MAX_COMP_LENGTH:ehp2.  
:dd.Maximum length, in bytes, of one component in a path name. 
:edl.
:p. An application can specify file objects managed by an 
 installable file system that supports long file names.  Because 
 some installable file systems support longer names than others, 
 the application should issue DosQuerySysInfo upon 
 initialization. 
:p. DosQuerySysInfo returns the maximum path length 
 (QSV_MAX_PATH_LENGTH) supported by the installed file 
 system.  The path length includes the drive specifier (d), the 
 leading backslash ( \ ), and the trailing null character.  The 
 value returned by DosQuerySysInfo can be used to allocate 
 buffers for path names returned by other functions, for example, 
 DosFindFirst and DosFindNext. 
.*
.*
:h1 res=1800. Help for Exiting Sysinfo
:p.This option will terminate the Sysinfo program.
.*
.*
:h1 res=3000. Help for AboutBox
:p.:hp2.Thank You:ehp2. for Using Sysinfo.
:p.Comments and/or Suggestions can be sent to the CompuServe 
address displayed in this About Box.
:p.I hope this program has helped you
in some way.
.*
.*
:index.
:euserdoc.



