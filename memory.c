// hello-native-app.c
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.

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

#define NDEBUG
#include <debug.h>

IO_STATUS_BLOCK IoStatusBlock;

HANDLE DoOpenFile(LPCWSTR pszFileName)
{
    NTSTATUS Status;
    UNICODE_STRING FileName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    HANDLE hFile = INVALID_HANDLE_VALUE;

    RtlInitUnicodeString(&FileName, pszFileName);

    InitializeObjectAttributes(&ObjectAttributes,
                               &FileName,
                               OBJ_CASE_INSENSITIVE,
                               NULL, NULL);

    Status = NtCreateFile(&hFile,
                          GENERIC_READ | GENERIC_WRITE | SYNCHRONIZE,
                          &ObjectAttributes,
                          &IoStatusBlock,
                          NULL,
                          FILE_ATTRIBUTE_NORMAL,
                          0,
                          FILE_OVERWRITE_IF,
                          FILE_SYNCHRONOUS_IO_NONALERT,
                          NULL, 0);
    if (NT_SUCCESS(Status))
        return hFile;

    return INVALID_HANDLE_VALUE;
}

BOOL DoWriteFile(HANDLE hFile, LPCVOID pvData, DWORD cbData)
{
    NTSTATUS Status;
    Status = NtWriteFile(hFile,
                         NULL,
                         NULL,
                         NULL,
                         &IoStatusBlock,
                         (PVOID)pvData,
                         cbData,
                         NULL,
                         NULL);
    return NT_SUCCESS(Status);
}

BOOL DoPutSz(HANDLE hFile, LPCSTR psz)
{
    return DoWriteFile(hFile, psz, (DWORD)strlen(psz));
}

BOOL DoPutDword(HANDLE hFile, DWORD dwNumber)
{
    CHAR *pch, szBuf[MAX_PATH];
    SIZE_T i, len;

    pch = szBuf;
    if (dwNumber == 0)
    {
        *pch++ = '0';
        *pch = 0;
    }
    else
    {
        while (dwNumber)
        {
            *pch++ = (CHAR)('0' + (dwNumber % 10));
            dwNumber /= 10;
        }
        *pch = 0;

        // _strrev(szBuf);
        len = strlen(szBuf);
        for (i = 0; i < len / 2; ++i)
        {
            CHAR ch = szBuf[i];
            szBuf[i] = szBuf[len - 1 - i];
            szBuf[len - 1 - i] = ch;
        }
    }

    return DoPutSz(hFile, szBuf);
}

void DoCloseFile(HANDLE hFile)
{
    NtClose(hFile);
}

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

    hFile = DoOpenFile(L"\\SystemRoot\\memory.txt");
    if (hFile == INVALID_HANDLE_VALUE)
    {
        DbgPrint("hFile == INVALID_HANDLE_VALUE\n");
        return -1;
    }

    pbData = RtlAllocateHeap(RtlGetProcessHeap(),
                             HEAP_ZERO_MEMORY,
                             256);
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
