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
    LPBYTE pbData;

    DbgPrint("This is a debug output.\n");

    hFile = DoOpenFile(L"\\SystemRoot\\03-alloc-memory.txt", TRUE);
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DbgPrint("hFile == INVALID_HANDLE_VALUE\n");
        return -1;
    }

    pbData = RtlAllocateHeap(RtlGetProcessHeap(), HEAP_ZERO_MEMORY, 256);
    if (pbData)
    {
        SIZE_T i, Size;

        for (i = 0; i < 256; ++i)
        {
            pbData[i] = (BYTE)i;
        }

        Size = RtlSizeHeap(RtlGetProcessHeap(), HEAP_NO_SERIALIZE, pbData);

        DoPutSz(hFile, "Size: ");
        DoPutDword(hFile, Size);
        DoPutSz(hFile, "\r\n");

        DoWriteFile(hFile, &pbData[0x20], 0x7F - 0x20);
        DoPutSz(hFile, "\r\n");

        RtlFreeHeap(RtlGetProcessHeap(), 0, pbData);
    }
    else
    {
        DbgPrint("!pbData\n");
    }

    DoCloseFile(hFile);
    return 0;
}
