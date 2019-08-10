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
    HANDLE hSection;
    LARGE_INTEGER FileSize, Offset;
    PVOID BaseAddress;
    SIZE_T ViewSize = 0;
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
                          FILE_CREATE,
                          FILE_SYNCHRONOUS_IO_NONALERT,
                          NULL, 0);
    if (!NT_SUCCESS(Status))
    {
        return FALSE;
    }

    FileSize.QuadPart = cbData;
    Status = NtCreateSection(&hSection,
                             SECTION_ALL_ACCESS,
                             NULL,
                             &FileSize,
                             PAGE_READWRITE,
                             SEC_COMMIT,
                             hFile);
    if (!NT_SUCCESS(Status))
    {
        goto CloseFile;
    }

    BaseAddress = NULL;
    ViewSize = cbData;
    Offset.QuadPart = 0;
    Status = NtMapViewOfSection(hSection,
                                NtCurrentProcess(),
                                &BaseAddress,
                                0,
                                ViewSize,
                                &Offset,
                                &ViewSize,
                                ViewUnmap,
                                0,
                                PAGE_READWRITE);
    if (!NT_SUCCESS(Status) || ViewSize != cbData || !BaseAddress)
    {
        goto CloseSection;
    }

    RtlCopyMemory(BaseAddress, pvData, cbData);

    ret = TRUE;

    NtUnmapViewOfSection(NtCurrentProcess(), BaseAddress);

CloseSection:
    NtClose(hSection);

CloseFile:
    NtClose(hFile);

    return ret;
}

typedef struct MYBITMAPDATA
{
    BITMAPFILEHEADER bf;
    BITMAPINFOHEADER bi;
    BYTE ab[WIDTHBYTES(320 * 24) * 200];
} MYBITMAPDATA;

INT
__cdecl
_main(
    IN INT argc,
    IN PCHAR argv[],
    IN PCHAR envp[],
    IN ULONG DebugFlag)
{
    MYBITMAPDATA data;
    data.bf.bfType = 0x4D42;
    data.bf.bfSize = sizeof(data);
    data.bf.bfReserved1 = 0;
    data.bf.bfReserved2 = 0;
    data.bf.bfOffBits = sizeof(data);

    RtlZeroMemory(&data.bi, sizeof(data.bi));
    data.bi.biSize = sizeof(BITMAPINFOHEADER);
    data.bi.biWidth = 320;
    data.bi.biHeight = 200;
    data.bi.biPlanes = 1;
    data.bi.biBitCount = 24;

    RtlFillMemory(&data.ab, sizeof(data.ab), 0x88);

    if (DoWriteFile(L"\\SystemRoot\\a.bmp", &data, sizeof(data)))
    {
        return 0;
    }
    else
    {
        return -1;
    }
}
