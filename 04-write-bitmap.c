// hello-native-app.c
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.

#include "common.h"

#define WIDTHBYTES(i) (((i) + 31) / 32 * 4)

#define WIDTH 320
#define HEIGHT 200
#define DEPTH 24
#define IMAGE_SIZE (WIDTHBYTES(WIDTH * DEPTH) * HEIGHT)

INT
__cdecl
_main(
    IN INT argc,
    IN PCHAR argv[],
    IN PCHAR envp[],
    IN ULONG DebugFlag)
{
    HANDLE hFile;
    BYTE ab[sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + IMAGE_SIZE];
    LPBYTE pb = ab;
    BITMAPFILEHEADER *pbf;
    BITMAPINFOHEADER *pbi;

    pbf = (BITMAPFILEHEADER *)pb;
    pb += sizeof(*pbf);
    pbi = (BITMAPINFOHEADER *)pb;
    pb += sizeof(*pbi);
    RtlFillMemory(pb, IMAGE_SIZE, 0x88);

    pbf->bfType = 0x4D42;
    pbf->bfSize = sizeof(ab);
    pbf->bfReserved1 = 0;
    pbf->bfReserved2 = 0;
    pbf->bfOffBits = sizeof(*pbf) + sizeof(*pbi);

    RtlZeroMemory(pbi, sizeof(*pbi));
    pbi->biSize = sizeof(*pbi);
    pbi->biWidth = WIDTH;
    pbi->biHeight = HEIGHT;
    pbi->biPlanes = 1;
    pbi->biBitCount = DEPTH;

    hFile = DoOpenFile(L"\\SystemRoot\\04-write-bitmap.bmp", TRUE);
    DoWriteFile(hFile, ab, sizeof(ab));
    DoCloseFile(hFile);

    return 0;
}
