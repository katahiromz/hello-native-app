// hello-native-app.c
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.

#include "common.h"

INT
__cdecl
_main(
    IN INT argc,
    IN PCHAR argv[],
    IN PCHAR envp[],
    IN ULONG DebugFlag)
{
    HANDLE hFile;

    DbgPrint("This is a debug output.\n");

    hFile = DoOpenFile(L"\\SystemRoot\\02-tick-count.txt", TRUE);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DbgPrint("hFile == INVALID_HANDLE_VALUE\n");
        return -1;
    }

    DoPutSz(hFile, "TEST\r\n");

    DoPutSz(hFile, "NtGetTickCount: ");
    DoPutDword(hFile, NtGetTickCount());
    DoPutSz(hFile, "\r\n");

    DoPutSz(hFile, "NtGetTickCount: ");
    DoPutDword(hFile, NtGetTickCount());
    DoPutSz(hFile, "\r\n");

    DoPutSz(hFile, "NtGetTickCount: ");
    DoPutDword(hFile, NtGetTickCount());
    DoPutSz(hFile, "\r\n");

    DoCloseFile(hFile);
    return 0;
}
