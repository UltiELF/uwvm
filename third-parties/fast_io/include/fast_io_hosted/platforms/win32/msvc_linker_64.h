﻿#pragma once

#pragma comment(linker, "/alternatename:__imp_?GetLastError@win32@fast_io@@YAIXZ=__imp_GetLastError")
#pragma comment(linker, "/alternatename:__imp_?LoadLibraryA@win32@fast_io@@YAPEAXPEBD@Z=__imp_LoadLibraryA")
#pragma comment(linker, "/alternatename:__imp_?LoadLibraryW@win32@fast_io@@YAPEAXPEB_S@Z=__imp_LoadLibraryW")
#pragma comment(linker, "/alternatename:__imp_?LoadLibraryExA@win32@fast_io@@YAPEAXPEBDPEAXI@Z=__imp_LoadLibraryExA")
#pragma comment(linker, "/alternatename:__imp_?LoadLibraryExW@win32@fast_io@@YAPEAXPEB_SPEAXI@Z=__imp_LoadLibraryExW")
#pragma comment(linker, \
				"/alternatename:__imp_?FormatMessageA@win32@fast_io@@YAIIPEBDIIPEADIPEAX@Z=__imp_FormatMessageA")
#pragma comment(linker, \
				"/alternatename:__imp_?FormatMessageW@win32@fast_io@@YAIIPEB_SIIPEA_SIPEAX@Z=__imp_FormatMessageW")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?CreateFileMappingA@win32@fast_io@@YAPEAXPEAXPEAUsecurity_attributes@12@IIIPEBD@Z=__imp_CreateFileMappingA")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?CreateFileMappingW@win32@fast_io@@YAPEAXPEAXPEAUsecurity_attributes@12@IIIPEB_S@Z=__imp_CreateFileMappingW")
#pragma comment(linker, "/alternatename:__imp_?MapViewOfFile@win32@fast_io@@YAPEAXPEAXIII_K@Z=__imp_MapViewOfFile")
#pragma comment(linker, "/alternatename:__imp_?SetEndOfFile@win32@fast_io@@YAHPEAX@Z=__imp_SetEndOfFile")
#pragma comment(linker, "/alternatename:__imp_?UnmapViewOfFile@win32@fast_io@@YAHPEBX@Z=__imp_UnmapViewOfFile")
#pragma comment(linker, \
				"/alternatename:__imp_?WriteFile@win32@fast_io@@YAHPEAXPEBXIPEAIPEAUoverlapped@12@@Z=__imp_WriteFile")
#pragma comment(linker, \
				"/alternatename:__imp_?ReadFile@win32@fast_io@@YAHPEAXPEBXIPEAIPEAUoverlapped@12@@Z=__imp_ReadFile")
#pragma comment(linker, "/alternatename:__imp_?SetFilePointer@win32@fast_io@@YAIPEAXHPEAHI@Z=__imp_SetFilePointer")
#pragma comment(linker, \
				"/alternatename:__imp_?SetFilePointerEx@win32@fast_io@@YAHPEAX_JPEA_JI@Z=__imp_SetFilePointerEx")
#pragma comment(linker, \
				"/alternatename:__imp_?DuplicateHandle@win32@fast_io@@YAHPEAX00PEAPEAXIHI@Z=__imp_DuplicateHandle")
#pragma comment(linker, "/alternatename:__imp_?GetStdHandle@win32@fast_io@@YAPEAXI@Z=__imp_GetStdHandle")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?CreatePipe@win32@fast_io@@YAHPEAPEAX0PEAUsecurity_attributes@12@I@Z=__imp_CreatePipe")
#pragma comment(linker, "/alternatename:__imp_?FreeLibrary@win32@fast_io@@YAHPEAX@Z=__imp_FreeLibrary")
#pragma comment(linker, "/alternatename:__imp_?GetProcAddress@win32@fast_io@@YAP6A_JX_EPEAXPEBD@Z=__imp_GetProcAddress")
#pragma comment(linker, "/alternatename:__imp_?GetModuleHandleA@win32@fast_io@@YAPEAXPEBD@Z=__imp_GetModuleHandleA")
#pragma comment(linker, "/alternatename:__imp_?GetModuleHandleW@win32@fast_io@@YAPEAXPEB_S@Z=__imp_GetModuleHandleW")
#pragma comment(linker, "/alternatename:__imp_?WaitForSingleObject@win32@fast_io@@YAIPEAXI@Z=__imp_WaitForSingleObject")
#pragma comment(linker, "/alternatename:__imp_?CancelIo@win32@fast_io@@YAIPEAX@Z=__imp_CancelIo")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetFileInformationByHandle@win32@fast_io@@YAHPEIAXPEIAUby_handle_file_information@12@@Z=__imp_GetFileInformationByHandle")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetUserDefaultLocaleName@win32@fast_io@@YAHPEA_SH@Z=__imp_GetUserDefaultLocaleName")
#pragma comment(linker, "/alternatename:__imp_?GetUserDefaultLCID@win32@fast_io@@YAIXZ=__imp_GetUserDefaultLCID")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetSystemTimePreciseAsFileTime@win32@fast_io@@YAXPEAUfiletime@12@@Z=__imp_GetSystemTimePreciseAsFileTime")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetSystemTimeAsFileTime@win32@fast_io@@YAXPEAUfiletime@12@@Z=__imp_GetSystemTimeAsFileTime")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?QueryUnbiasedInterruptTime@win32@fast_io@@YAHPEA_K@Z=__imp_QueryUnbiasedInterruptTime")
#pragma comment( \
	linker, "/alternatename:__imp_?QueryPerformanceCounter@win32@fast_io@@YAHPEA_J@Z=__imp_QueryPerformanceCounter")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?QueryPerformanceFrequency@win32@fast_io@@YAHPEA_J@Z=__imp_QueryPerformanceFrequency")
#pragma comment( \
	linker, "/alternatename:__imp_?GetProcessTimes@win32@fast_io@@YAHPEAXPEAUfiletime@12@111@Z=__imp_GetProcessTimes")
#pragma comment( \
	linker, "/alternatename:__imp_?GetThreadTimes@win32@fast_io@@YAHPEAXPEAUfiletime@12@111@Z=__imp_GetThreadTimes")
#pragma comment(linker, \
				"/alternatename:__imp_?GetHandleInformation@win32@fast_io@@YAHPEAXPEAI@Z=__imp_GetHandleInformation")
#pragma comment(linker, \
				"/alternatename:__imp_?SetHandleInformation@win32@fast_io@@YAHPEAXII@Z=__imp_SetHandleInformation")
#pragma comment(linker, "/alternatename:__imp_?GetTempPathA@win32@fast_io@@YAIIPEAD@Z=__imp_GetTempPathA")
#pragma comment(linker, "/alternatename:__imp_?GetTempPathW@win32@fast_io@@YAIIPEA_S@Z=__imp_GetTempPathW")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?CreateFileA@win32@fast_io@@YAPEAXPEBDIIPEAUsecurity_attributes@12@IIPEAX@Z=__imp_CreateFileA")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?CreateFileW@win32@fast_io@@YAPEAXPEB_SIIPEAUsecurity_attributes@12@IIPEAX@Z=__imp_CreateFileW")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?CreateIoCompletionPort@win32@fast_io@@YAPEAXPEAX0_KI@Z=__imp_CreateIoCompletionPort")
#pragma comment(linker, "/alternatename:__imp_?SystemFunction036@win32@fast_io@@YAHPEAXI@Z=__imp_SystemFunction036")
#pragma comment(linker, "/alternatename:__imp_?CloseHandle@win32@fast_io@@YAHPEAX@Z=__imp_CloseHandle")
#pragma comment(linker, \
				"/alternatename:__imp_?LockFileEx@win32@fast_io@@YAHPEAXIIIIPEAUoverlapped@12@@Z=__imp_LockFileEx")
#pragma comment(linker, \
				"/alternatename:__imp_?UnlockFileEx@win32@fast_io@@YAHPEAXIIIPEAUoverlapped@12@@Z=__imp_UnlockFileEx")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?DeviceIoControl@win32@fast_io@@YAHPEAXI0I0I0PEAUoverlapped@12@@Z=__imp_DeviceIoControl")
#pragma comment(linker, "/alternatename:__imp_?GetFileType@win32@fast_io@@YAIPEAX@Z=__imp_GetFileType")
#pragma comment(linker, "/alternatename:__imp_?GetACP@win32@fast_io@@YAIXZ=__imp_GetACP")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetEnvironmentVariableA@win32@fast_io@@YAIPEBDPEADI@Z=__imp_GetEnvironmentVariableA")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetEnvironmentVariableW@win32@fast_io@@YAIPEB_SPEA_SI@Z=__imp_GetEnvironmentVariableW")
#pragma comment(linker, "/alternatename:__imp_?MessageBoxA@win32@fast_io@@YAIPEAXPEBD1I@Z=__imp_MessageBoxA")
#pragma comment(linker, "/alternatename:__imp_?MessageBoxW@win32@fast_io@@YAIPEAXPEB_S1I@Z=__imp_MessageBoxW")
#pragma comment(linker, "/alternatename:__imp_?GetConsoleMode@win32@fast_io@@YAHPEAXPEAI@Z=__imp_GetConsoleMode")
#pragma comment(linker, "/alternatename:__imp_?SetConsoleMode@win32@fast_io@@YAHPEAXI@Z=__imp_SetConsoleMode")
#pragma comment(linker, "/alternatename:__imp_?ReadConsoleA@win32@fast_io@@YAHPEAX0IPEAI0@Z=__imp_ReadConsoleA")
#pragma comment(linker, "/alternatename:__imp_?ReadConsoleW@win32@fast_io@@YAHPEAX0IPEAI0@Z=__imp_ReadConsoleW")
#pragma comment(linker, "/alternatename:__imp_?WriteConsoleA@win32@fast_io@@YAHPEAXPEBXIPEAI0@Z=__imp_WriteConsoleA")
#pragma comment(linker, "/alternatename:__imp_?WriteConsoleW@win32@fast_io@@YAHPEAXPEBXIPEAI0@Z=__imp_WriteConsoleW")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetConsoleScreenBufferInfo@win32@fast_io@@YAHPEAXPEAUconsole_screen_buffer_info@12@@Z=__imp_GetConsoleScreenBufferInfo")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ScrollConsoleScreenBufferA@win32@fast_io@@YAHPEAXPEBUsmall_rect@12@1Ucoord@12@PEBUchar_info@12@@Z=__imp_ScrollConsoleScreenBufferA")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ScrollConsoleScreenBufferW@win32@fast_io@@YAHPEAXPEBUsmall_rect@12@1Ucoord@12@PEBUchar_info@12@@Z=__imp_ScrollConsoleScreenBufferW")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?SetConsoleCursorPosition@win32@fast_io@@YAHPEAXUcoord@12@@Z=__imp_SetConsoleCursorPosition")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?InitializeCriticalSection@win32@fast_io@@YAXPEAX@Z=__imp_InitializeCriticalSection")
#pragma comment(linker, \
				"/alternatename:__imp_?EnterCriticalSection@win32@fast_io@@YAXPEAX@Z=__imp_EnterCriticalSection")
#pragma comment( \
	linker, "/alternatename:__imp_?TryEnterCriticalSection@win32@fast_io@@YAHPEAX@Z=__imp_TryEnterCriticalSection")
#pragma comment(linker, \
				"/alternatename:__imp_?LeaveCriticalSection@win32@fast_io@@YAXPEAX@Z=__imp_LeaveCriticalSection")
#pragma comment(linker, \
				"/alternatename:__imp_?DeleteCriticalSection@win32@fast_io@@YAXPEAX@Z=__imp_DeleteCriticalSection")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?WSADuplicateSocketA@win32@fast_io@@YAHPEAXIPEAUwsaprotocol_infoa@12@@Z=__imp_WSADuplicateSocketA")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?WSADuplicateSocketW@win32@fast_io@@YAXPEAXIPEAUwsaprotocol_infow@12@@Z=__imp_WSADuplicateSocketW")
#pragma comment(linker, "/alternatename:__imp_?WSACleanup@win32@fast_io@@YAHXZ=__imp_WSACleanup")
#pragma comment(linker, "/alternatename:__imp_?WSAStartup@win32@fast_io@@YAHIPEAUwsadata@12@@Z=__imp_WSAStartup")
#pragma comment(linker, "/alternatename:__imp_?WSAGetLastError@win32@fast_io@@YAHXZ=__imp_WSAGetLastError")
#pragma comment(linker, "/alternatename:__imp_?closesocket@win32@fast_io@@YAH_K@Z=__imp_closesocket")
#pragma comment( \
	linker, "/alternatename:__imp_?WSASocketW@win32@fast_io@@YA_KHHHPEAUwsaprotocol_infow@12@II@Z=__imp_WSASocketW")
#pragma comment( \
	linker, "/alternatename:__imp_?WSASocketA@win32@fast_io@@YA_KHHHPEAUwsaprotocol_infoa@12@II@Z=__imp_WSASocketA")
#pragma comment(linker, "/alternatename:__imp_?bind@win32@fast_io@@YAH_KPEBXH@Z=__imp_bind")
#pragma comment(linker, "/alternatename:__imp_?listen@win32@fast_io@@YAH_KH@Z=__imp_listen")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?WSAAccept@win32@fast_io@@YA_K_KPEBXPEAHP6AXPEAUwsabuf@12@3PEAUqualityofservice@12@433PEAI0@_E0@Z=__imp_WSAAccept")
#pragma comment(linker, "/alternatename:__imp_?ioctlsocket@win32@fast_io@@YAH_KJPEAI@Z=__imp_ioctlsocket")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?WSASend@win32@fast_io@@YAH_KPEAUwsabuf@12@IPEAIIPEAUoverlapped@12@P6AXII3I@_E@Z=__imp_WSASend")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?WSASendMsg@win32@fast_io@@YAH_KPEAUwsamsg@12@IPEAIPEAUoverlapped@12@P6AXII3I@_E@Z=__imp_WSASendMsg")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?WSASendTo@win32@fast_io@@YAH_KPEAUwsabuf@12@IPEAIIPEBXHPEAUoverlapped@12@P6AXII4I@_E@Z=__imp_WSASendTo")
#pragma comment(linker, "/alternatename:__imp_?recv@win32@fast_io@@YAH_KPEADHH@Z=__imp_recv")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?WSARecv@win32@fast_io@@YAH_KPEAUwsabuf@12@IPEAI2PEAUoverlapped@12@P6AXII3I@_E@Z=__imp_WSARecv")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?WSARecvFrom@win32@fast_io@@YAH_KPEAUwsabuf@12@IPEAI2PEAXPEAHPEAUoverlapped@12@P6AXII5I@_E@Z=__imp_WSARecvFrom")
#pragma comment(linker, "/alternatename:__imp_?connect@win32@fast_io@@YAH_KPEBXH@Z=__imp_connect")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?WSAConnect@win32@fast_io@@YAH_KPEBXHPEAUwsabuf@12@2PEAUqualityofservice@12@3@Z=__imp_WSAConnect")
#pragma comment(linker, "/alternatename:__imp_?recvfrom@win32@fast_io@@YAH_KPEADHHPEAXPEAH@Z=__imp_recvfrom")
#pragma comment(linker, "/alternatename:__imp_?sendto@win32@fast_io@@YAH_KPEBDHHPEBXH@Z=__imp_sendto")
#pragma comment(linker, "/alternatename:__imp_?shutdown@win32@fast_io@@YAH_KPEBXH@Z=__imp_shutdown")
#pragma comment(linker, "/alternatename:__imp_?GetCurrentProcessId@win32@fast_io@@YAIXZ=__imp_GetCurrentProcessId")
#pragma comment(linker, "/alternatename:__imp_?FlushFileBuffers@win32@fast_io@@YAHPEAX@Z=__imp_FlushFileBuffers")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetQueuedCompletionStatus@win32@fast_io@@YAHPEAXPEAIPEA_KPEAUoverlapped@12@I@Z=__imp_GetQueuedCompletionStatus")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?freeaddrinfo@win32@fast_io@@YAXPEAU?$win32_family_addrinfo@$0A@@12@@Z=__imp_freeaddrinfo")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?FreeAddrInfoW@win32@fast_io@@YAXPEAU?$win32_family_addrinfo@$00@12@@Z=__imp_FreeAddrInfoW")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?getaddrinfo@win32@fast_io@@YAHPEBD0PEBU?$win32_family_addrinfo@$0A@@12@PEAPEAU312@@Z=__imp_getaddrinfo")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetAddrInfoW@win32@fast_io@@YAHPEB_S0PEBU?$win32_family_addrinfo@$00@12@PEAPEAU312@@Z=__imp_GetAddrInfoW")
#pragma comment( \
	linker, "/alternatename:__imp_?CryptAcquireContextA@win32@fast_io@@YAHPEA_KPEB_Q1II@Z=__imp_CryptAcquireContextA")
#pragma comment( \
	linker, "/alternatename:__imp_?CryptAcquireContextW@win32@fast_io@@YAHPEA_KPEB_S1II@Z=__imp_CryptAcquireContextW")
#pragma comment(linker, "/alternatename:__imp_?CryptReleaseContext@win32@fast_io@@YAH_KI@Z=__imp_CryptReleaseContext")
#pragma comment(linker, "/alternatename:__imp_?CryptGenRandom@win32@fast_io@@YAH_KIPEAE@Z=__imp_CryptGenRandom")
#pragma comment(linker, "/alternatename:__imp_?RegOpenKeyA@win32@fast_io@@YAH_KPEB_QPEA_K@Z=__imp_RegOpenKeyA")
#pragma comment(linker, "/alternatename:__imp_?RegOpenKeyW@win32@fast_io@@YAH_KPEB_SPEA_K@Z=__imp_RegOpenKeyW")
#pragma comment(linker, \
				"/alternatename:__imp_?RegQueryValueExA@win32@fast_io@@YAH_KPEB_QPEAI2PEAX2@Z=__imp_RegQueryValueExA")
#pragma comment(linker, \
				"/alternatename:__imp_?RegQueryValueExW@win32@fast_io@@YAH_KPEB_SPEAI2PEAX2@Z=__imp_RegQueryValueExW")
#pragma comment(linker, "/alternatename:__imp_?RegCloseKey@win32@fast_io@@YAH_K@Z=__imp_RegCloseKey")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetTimeZoneInformation@win32@fast_io@@YAIPEAUtime_zone_information@12@@Z=__imp_GetTimeZoneInformation")
#pragma comment( \
	linker, "/alternatename:__imp_?rtl_nt_status_to_dos_error@nt@win32@fast_io@@YAII@Z=__imp_RtlNtStatusToDosError")
#pragma comment(linker, "/alternatename:__imp_?NtClose@nt@win32@fast_io@@YAIPEAX@Z=__imp_NtClose")
#pragma comment(linker, "/alternatename:__imp_?ZwClose@nt@win32@fast_io@@YAIPEAX@Z=__imp_ZwClose")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtCreateFile@nt@win32@fast_io@@YAIPEAPEAXIPEAUobject_attributes@123@PEAUio_status_block@123@PEA_JIIIIPEAXI@Z=__imp_NtCreateFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwCreateFile@nt@win32@fast_io@@YAIPEAPEAXIPEAUobject_attributes@123@PEAUio_status_block@123@PEA_JIIIIPEAXI@Z=__imp_ZwCreateFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtCreateSection@nt@win32@fast_io@@YAIPEIAPEAXIPEIAUobject_attributes@123@PEA_KIIPEIAX@Z=__imp_NtCreateSection")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwCreateSection@nt@win32@fast_io@@YAIPEIAPEAXIPEIAUobject_attributes@123@PEA_KIIPEIAX@Z=__imp_ZwCreateSection")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtQueryInformationProcess@nt@win32@fast_io@@YAIPEIAXW4process_information_class@123@PEAXIPEAI@Z=__imp_NtQueryInformationProcess")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwQueryInformationProcess@nt@win32@fast_io@@YAIPEIAXW4process_information_class@123@PEAXIPEAI@Z=__imp_ZwQueryInformationProcess")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtWriteFile@nt@win32@fast_io@@YAIPEAX0P6AX0PEAUio_status_block@123@I@_E01PEBXIPEA_JPEAI@Z=__imp_NtWriteFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwWriteFile@nt@win32@fast_io@@YAIPEAX0P6AX0PEAUio_status_block@123@I@_E01PEBXIPEA_JPEAI@Z=__imp_ZwWriteFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtReadFile@nt@win32@fast_io@@YAIPEAX0P6AX0PEAUio_status_block@123@I@_E01PEBXIPEA_JPEAI@Z=__imp_NtReadFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwReadFile@nt@win32@fast_io@@YAIPEAX0P6AX0PEAUio_status_block@123@I@_E01PEBXIPEA_JPEAI@Z=__imp_ZwReadFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtQueryDirectoryFile@nt@win32@fast_io@@YAIPEAX0P6AX0PEAUio_status_block@123@I@_E010IW4file_information_class@123@HPEAUunicode_string@123@H@Z=__imp_NtQueryDirectoryFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwQueryDirectoryFile@nt@win32@fast_io@@YAIPEAX0P6AX0PEAUio_status_block@123@I@_E010IW4file_information_class@123@HPEAUunicode_string@123@H@Z=__imp_ZwQueryDirectoryFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtQuerySection@nt@win32@fast_io@@YAIPEAXW4section_information_class@123@0_KPEA_K@Z=__imp_NtQuerySection")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwQuerySection@nt@win32@fast_io@@YAIPEAXW4section_information_class@123@0_KPEA_K@Z=__imp_ZwQuerySection")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtQueryInformationFile@nt@win32@fast_io@@YAIPEIAXPEIAUio_status_block@123@0IW4file_information_class@123@@Z=__imp_NtQueryInformationFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwQueryInformationFile@nt@win32@fast_io@@YAIPEIAXPEIAUio_status_block@123@0IW4file_information_class@123@@Z=__imp_ZwQueryInformationFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtSetInformationFile@nt@win32@fast_io@@YAIPEIAXPEIAUio_status_block@123@0IW4file_information_class@123@@Z=__imp_NtSetInformationFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwSetInformationFile@nt@win32@fast_io@@YAIPEIAXPEIAUio_status_block@123@0IW4file_information_class@123@@Z=__imp_ZwSetInformationFile")
#pragma comment( \
	linker, "/alternatename:__imp_?NtDuplicateObject@nt@win32@fast_io@@YAIPEAX00PEAPEAXIII@Z=__imp_NtDuplicateObject")
#pragma comment( \
	linker, "/alternatename:__imp_?ZwDuplicateObject@nt@win32@fast_io@@YAIPEAX00PEAPEAXIII@Z=__imp_ZwDuplicateObject")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtWaitForSingleObject@nt@win32@fast_io@@YAIPEAXHPEA_K@Z=__imp_NtWaitForSingleObject")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwWaitForSingleObject@nt@win32@fast_io@@YAIPEAXHPEA_K@Z=__imp_ZwWaitForSingleObject")
#pragma comment(linker, "/alternatename:__imp_?NtSetSystemTime@nt@win32@fast_io@@YAIPEA_K0@Z=__imp_NtSetSystemTime")
#pragma comment(linker, "/alternatename:__imp_?ZwSetSystemTime@nt@win32@fast_io@@YAIPEA_K0@Z=__imp_ZwSetSystemTime")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtCreateProcess@nt@win32@fast_io@@YAIPEAPEAXIPEAUobject_attributes@123@PEAXI222@Z=__imp_NtCreateProcess")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwCreateProcess@nt@win32@fast_io@@YAIPEAPEAXIPEAUobject_attributes@123@PEAXI222@Z=__imp_ZwCreateProcess")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?rtl_dos_path_name_to_nt_path_name_u@nt@win32@fast_io@@YAEPEB_SPEAUunicode_string@123@PEAPEB_SPEAUrtl_relative_name_u@123@@Z=__imp_RtlDosPathNameToNtPathName_U")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?rtl_dos_path_name_to_nt_path_name_u_with_status@nt@win32@fast_io@@YAIPEB_SPEAUunicode_string@123@PEAPEB_SPEAUrtl_relative_name_u@123@@Z=__imp_RtlDosPathNameToNtPathName_U_WithStatus")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?rtl_free_unicode_string@nt@win32@fast_io@@YAXPEAUunicode_string@123@@Z=__imp_RtlFreeUnicodeString")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?RtlInitializeCriticalSection@nt@win32@fast_io@@YAXPEAX@Z=__imp_RtlInitializeCriticalSection")
#pragma comment( \
	linker, "/alternatename:__imp_?RtlEnterCriticalSection@nt@win32@fast_io@@YAXPEAX@Z=__imp_RtlEnterCriticalSection")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?RtlTryEnterCriticalSection@nt@win32@fast_io@@YAHPEAX@Z=__imp_RtlTryEnterCriticalSection")
#pragma comment( \
	linker, "/alternatename:__imp_?RtlLeaveCriticalSection@nt@win32@fast_io@@YAXPEAX@Z=__imp_RtlLeaveCriticalSection")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?RtlDeleteCriticalSection@nt@win32@fast_io@@YAXPEAX@Z=__imp_RtlDeleteCriticalSection")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?RtlCreateUserThread@nt@win32@fast_io@@YAIPEAX0HI_K100PEAPEAXPEAUclient_id@123@@Z=__imp_RtlCreateUserThread")
#pragma comment(linker, "/alternatename:__imp_?NtResumeThread@nt@win32@fast_io@@YAIPEAXPEAI@Z=__imp_NtResumeThread")
#pragma comment(linker, "/alternatename:__imp_?ZwResumeThread@nt@win32@fast_io@@YAIPEAXPEAI@Z=__imp_ZwResumeThread")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtLockFile@nt@win32@fast_io@@YAIPEAX0P6AX0PEAUio_status_block@123@I@_E01PEA_J3IEE@Z=__imp_NtLockFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwLockFile@nt@win32@fast_io@@YAIPEAX0P6AX0PEAUio_status_block@123@I@_E01PEA_J3IEE@Z=__imp_ZwLockFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtUnlockFile@nt@win32@fast_io@@YAIPEAXPEAUio_status_block@123@PEA_J2I@Z=__imp_NtUnlockFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwUnlockFile@nt@win32@fast_io@@YAIPEAXPEAUio_status_block@123@PEA_J2I@Z=__imp_ZwUnlockFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtFlushBuffersFile@nt@win32@fast_io@@YAIPEAXPEAUio_status_block@123@@Z=__imp_NtFlushBuffersFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwFlushBuffersFile@nt@win32@fast_io@@YAIPEAXPEAUio_status_block@123@@Z=__imp_ZwFlushBuffersFile")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtFlushBuffersFileEx@nt@win32@fast_io@@YAIPEAXI0IPEAUio_status_block@123@@Z=__imp_NtFlushBuffersFileEx")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwFlushBuffersFileEx@nt@win32@fast_io@@YAIPEAXI0IPEAUio_status_block@123@@Z=__imp_ZwFlushBuffersFileEx")
#pragma comment(linker, "/alternatename:__imp_?DbgPrint@nt@win32@fast_io@@YAIPEBDZZ=__imp_DbgPrint")
#pragma comment(linker, "/alternatename:__imp_?DbgPrintEx@nt@win32@fast_io@@YAIIIPEBDZZ=__imp_DbgPrintEx")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?RtlCreateProcessParameters@nt@win32@fast_io@@YAIPEAPEAUrtl_user_process_parameters@123@PEAUunicode_string@123@111PEAX1111@Z=__imp_RtlCreateProcessParameters")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?RtlCreateProcessParametersEx@nt@win32@fast_io@@YAIPEAPEAUrtl_user_process_parameters@123@PEAUunicode_string@123@111PEAX1111I@Z=__imp_RtlCreateProcessParametersEx")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?RtlDestroyProcessParameters@nt@win32@fast_io@@YAIPEAUrtl_user_process_parameters@123@@Z=__imp_RtlDestroyProcessParameters")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtCreateUserProcess@nt@win32@fast_io@@YAIPEAX0IIPEAUobject_attributes@123@1IIPEAUrtl_user_process_parameters@123@PEAUps_create_info@123@PEAUps_attribute_list@123@@Z=__imp_NtCreateUserProcess")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwCreateUserProcess@nt@win32@fast_io@@YAIPEAX0IIPEAUobject_attributes@123@1IIPEAUrtl_user_process_parameters@123@PEAUps_create_info@123@PEAUps_attribute_list@123@@Z=__imp_ZwCreateUserProcess")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?RtlCreateUserProcess@nt@win32@fast_io@@YAIPEAUunicode_string@123@IPEAUrtl_user_process_parameters@123@PEAUsecurity_descriptor@123@2PEAXE33PEAUrtl_user_process_information@123@@Z=__imp_RtlCreateUserProcess")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtMapViewOfSection@nt@win32@fast_io@@YAIPEAX0PEAPEAX_K2PEBTlarge_integer@123@PEA_KW4section_inherit@123@II@Z=__imp_NtMapViewOfSection")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwMapViewOfSection@nt@win32@fast_io@@YAIPEAX0PEAPEAX_K2PEBTlarge_integer@123@PEA_KW4section_inherit@123@II@Z=__imp_ZwMapViewOfSection")
#pragma comment(linker, \
				"/alternatename:__imp_?NtUnmapViewOfSection@nt@win32@fast_io@@YAIPEAX0@Z=__imp_NtUnmapViewOfSection")
#pragma comment(linker, \
				"/alternatename:__imp_?ZwUnmapViewOfSection@nt@win32@fast_io@@YAIPEAX0@Z=__imp_ZwUnmapViewOfSection")
#pragma comment(linker, "/alternatename:__imp_?SetConsoleCP@win32@fast_io@@YAHI@Z=__imp_SetConsoleCP")
#pragma comment(linker, "/alternatename:__imp_?SetConsoleOutputCP@win32@fast_io@@YAHI@Z=__imp_SetConsoleOutputCP")
#pragma comment(linker, "/alternatename:__imp_?GetConsoleCP@win32@fast_io@@YAIXZ=__imp_GetConsoleCP")
#pragma comment(linker, "/alternatename:__imp_?GetConsoleOutputCP@win32@fast_io@@YAIXZ=__imp_GetConsoleOutputCP")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtReadVirtualMemory@nt@win32@fast_io@@YAIPEAX00_KPEA_K@Z=__imp_NtReadVirtualMemory")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwReadVirtualMemory@nt@win32@fast_io@@YAIPEAX00_KPEA_K@Z=__imp_ZwReadVirtualMemory")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtWriteVirtualMemory@nt@win32@fast_io@@YAIPEAX00_KPEA_K@Z=__imp_NtWriteVirtualMemory")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwWriteVirtualMemory@nt@win32@fast_io@@YAIPEAX00_KPEA_K@Z=__imp_ZwWriteVirtualMemory")
#pragma comment(linker, "/alternatename:__imp_?RtlAcquirePebLock@nt@win32@fast_io@@YAIXZ=__imp_RtlAcquirePebLock")
#pragma comment(linker, "/alternatename:__imp_?RtlReleasePebLock@nt@win32@fast_io@@YAIXZ=__imp_RtlReleasePebLock")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtAllocateVirtualMemory@nt@win32@fast_io@@YAIPEAXPEAPEAX_KPEA_KII@Z=__imp_NtAllocateVirtualMemory")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwAllocateVirtualMemory@nt@win32@fast_io@@YAIPEAXPEAPEAX_KPEA_KII@Z=__imp_ZwAllocateVirtualMemory")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtQueryObject@nt@win32@fast_io@@YAIPEAXW4object_information_class@123@0IPEAI@Z=__imp_NtQueryObject")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwQueryObject@nt@win32@fast_io@@YAIPEAXW4object_information_class@123@0IPEAI@Z=__imp_ZwQueryObject")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?GetSystemInfo@win32@fast_io@@YAXPEAUsystem_info@12@@Z=__imp_GetSystemInfo")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtQuerySystemInformation@nt@win32@fast_io@@YAIW4system_information_class@123@PEAXIPEAI@Z=__imp_NtQuerySystemInformation")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?NtFreeVirtualMemory@nt@win32@fast_io@@YAIPEAXPEAPEAXPEA_KI@Z=__imp_NtFreeVirtualMemory")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?ZwFreeVirtualMemory@nt@win32@fast_io@@YAIPEAXPEAPEAXPEA_KI@Z=__imp_ZwFreeVirtualMemory")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?SetFileInformationByHandle@win32@fast_io@@YAHPEAXW4file_info_by_handle_class@12@0I@Z=__imp_SetFileInformationByHandle")
#pragma comment( \
	linker,      \
	"/alternatename:__imp_?SetFileTime@win32@fast_io@@YAHPEAXPEBUfiletime@12@11@Z=__imp_SetFileTime")

#if defined(_DLL)
    #pragma comment(linker, "/alternatename:__imp_?msvc__RTtypeid@msvc@fast_io@@YAPEAXPEAX@Z=__imp___RTtypeid")
#else
    #pragma comment(linker, "/alternatename:?msvc__RTtypeid@msvc@fast_io@@YAPEAXPEAX@Z=__RTtypeid")
#endif
