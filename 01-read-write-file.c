// hello-native-app.c
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.

#include "common.h"

#define FILENAME L"\\SystemRoot\\01-read-write-file.txt"

INT
__cdecl
_main(
    IN INT argc,
    IN PCHAR argv[],
    IN PCHAR envp[],
    IN ULONG DebugFlag)
{
    HANDLE hFile;
    CHAR abData[256];
    DWORD i, cbData;

    DbgPrint("This is a debug output.\n");

    hFile = DoOpenFile(FILENAME, TRUE);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DbgPrint("hFile == INVALID_HANDLE_VALUE\n");
        return -1;
    }

    DoPutSz(hFile, "ABCDEFG");
    NtFlushBuffersFile(hFile, &IoStatusBlock);
    DoCloseFile(hFile);

    hFile = DoOpenFile(FILENAME, FALSE);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DbgPrint("hFile == INVALID_HANDLE_VALUE\n");
        return -1;
    }

    DoReadFile(hFile, abData, cbData, &cbData);

    DoCloseFile(hFile);

    for (i = 0; i < cbData; ++i)
    {
        abData[i] = abData[i] + 1;
    }

    hFile = DoOpenFile(FILENAME, TRUE);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DbgPrint("hFile == INVALID_HANDLE_VALUE\n");
        return -1;
    }

    DoWriteFile(hFile, abData, cbData);
    DoPutSz(hFile, "\r\n");
    DoCloseFile(hFile);

    return 0;
}
