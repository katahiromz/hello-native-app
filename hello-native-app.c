// hello-native-app.c
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.
//
// NOTE: Copy this folder to $(REACTOS)\base\system and modify CMakeLists.txt.

#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <wingdi.h>

#define NTOS_MODE_USER
#include <ndk/iofuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/mmfuncs.h>
#include <ndk/obfuncs.h>

#define NDEBUG
#include <debug.h>

#define WIDTHBYTES(i) (((i) + 31) / 32 * 4)

BOOL DoWriteFile(LPCWSTR pszFileName, LPCVOID pvData, ULONG_PTR cbData)
{
    NTSTATUS Status;
    UNICODE_STRING FileName;
    OBJECT_ATTRIBUTES ObjectAttributes;
    IO_STATUS_BLOCK IoStatusBlock;
    HANDLE hFile = INVALID_HANDLE_VALUE;
    BOOL ret = FALSE;

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
    if (!NT_SUCCESS(Status))
    {
        return FALSE;
    }

    Status = NtWriteFile(hFile,
                         NULL,
                         NULL,
                         NULL,
                         &IoStatusBlock,
                         (PVOID)pvData,
                         cbData,
                         NULL,
                         NULL);
    if (NT_SUCCESS(Status))
    {
        ret = TRUE;
    }

    NtClose(hFile);

    return ret;
}

INT
__cdecl
_main(
    IN INT argc,
    IN PCHAR argv[],
    IN PCHAR envp[],
    IN ULONG DebugFlag)
{
    BYTE ab[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + WIDTHBYTES(320 * 24) * 200];
    LPBYTE pb = ab;
    BITMAPFILEHEADER *pbf;
    BITMAPINFOHEADER *pbi;

    pbf = (BITMAPFILEHEADER *)pb;
    pb += sizeof(*pbf);
    pbi = (BITMAPINFOHEADER *)pb;
    pb += sizeof(*pbi);
    RtlFillMemory(pb, WIDTHBYTES(320 * 24) * 200, 0x88);

    pbf->bfType = 0x4D42;
    pbf->bfSize = sizeof(ab);
    pbf->bfReserved1 = 0;
    pbf->bfReserved2 = 0;
    pbf->bfOffBits = sizeof(*pbf) + sizeof(*pbi);

    RtlZeroMemory(pbi, sizeof(*pbi));
    pbi->biSize = sizeof(BITMAPINFOHEADER);
    pbi->biWidth = 320;
    pbi->biHeight = 200;
    pbi->biPlanes = 1;
    pbi->biBitCount = 24;

    if (DoWriteFile(L"\\SystemRoot\\a.bmp", ab, sizeof(ab)))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
