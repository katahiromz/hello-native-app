#ifndef COMMON_H_
#define COMMON_H_

#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <wingdi.h>
#include <string.h>

#define NTOS_MODE_USER
#include <ndk/iofuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/mmfuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/kefuncs.h>
#include <ndk/exfuncs.h>

#define NDEBUG
#include <debug.h>

extern IO_STATUS_BLOCK IoStatusBlock;

HANDLE DoOpenFile(LPCWSTR pszFileName, BOOL bWrite);
BOOL DoReadFile(HANDLE hFile, LPVOID pvData, ULONG cbData, ULONG *pcbRead);
BOOL DoWriteFile(HANDLE hFile, LPCVOID pvData, ULONG cbData);
BOOL DoPutSz(HANDLE hFile, LPCSTR psz);
BOOL DoPutDword(HANDLE hFile, ULONG dwNumber);
VOID DoCloseFile(HANDLE hFile);

#endif  /* ndef COMMON_H_ */
