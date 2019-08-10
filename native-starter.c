// native-starter.c
// Copyright (C) 2019 Katayama Hirofumi MZ <katayama.hirofumi.mz@gmail.com>.
// This file is public domain software.

#define WIN32_NO_STATUS
#include <windef.h>
#include <winbase.h>
#include <wingdi.h>

#define NTOS_MODE_USER
#include <ndk/iofuncs.h>
#include <ndk/rtlfuncs.h>
#include <ndk/mmfuncs.h>
#include <ndk/obfuncs.h>
#include <ndk/psfuncs.h>

#define NDEBUG
#include <debug.h>

BOOL CreateNativeProcess(LPCWSTR file_name, LPCWSTR cmd_line)
{
    NTSTATUS status;
    LPCWSTR file_part;
    UNICODE_STRING nt_file, imgname, imgpath, dllpath, cmdline;
    PRTL_USER_PROCESS_PARAMETERS processparameters;
    RTL_USER_PROCESS_INFORMATION processinformation = {0};
    WCHAR Env[2] = {0, 0};

    RtlDosPathNameToNtPathName_U(file_name, &nt_file, &file_part, NULL);

    RtlInitUnicodeString(&imgpath, nt_file.Buffer);
    RtlInitUnicodeString(&imgname, file_part);
    RtlInitUnicodeString(&dllpath, SharedUserData->NtSystemRoot);
    RtlInitUnicodeString(&cmdline, cmd_line);

    status = RtlCreateProcessParameters(&processparameters, &imgname,
                                        &dllpath, &dllpath, &cmdline,
                                        Env, 0, 0, 0, 0);
    if (!NT_SUCCESS(status))
    {
        return FALSE;
    }

    status = RtlCreateUserProcess(&imgpath, OBJ_CASE_INSENSITIVE,
                                  processparameters, NULL, NULL, NULL,
                                  FALSE, NULL, NULL, &processinformation);
    if (!NT_SUCCESS(status))
    {
        return FALSE;
    }

    status = NtResumeThread(processinformation.ThreadHandle, NULL);
    if (!NT_SUCCESS(status))
    {
        return FALSE;
    }

    return TRUE;
}

int main(int argc, char **argv)
{
    ANSI_STRING FileNameA;
    UNICODE_STRING FileNameW;
    RtlInitAnsiStringEx(&FileNameA, argv[1]);
    RtlAnsiStringToUnicodeString(&FileNameW, &FileNameA, TRUE);

    return CreateNativeProcess(FileNameW.Buffer, FileNameW.Buffer) ? 0 : -1;
}
