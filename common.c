#include "common.h"

IO_STATUS_BLOCK IoStatusBlock;

HANDLE DoOpenFile(LPCWSTR pszFileName, BOOL bWrite)
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

    if (bWrite)
    {
        Status = NtCreateFile(&hFile,
                              FILE_GENERIC_WRITE,
                              &ObjectAttributes,
                              &IoStatusBlock,
                              NULL,
                              FILE_ATTRIBUTE_NORMAL,
                              FILE_SHARE_READ,
                              FILE_OVERWRITE_IF,
                              FILE_SYNCHRONOUS_IO_NONALERT,
                              NULL, 0);
    }
    else
    {
        Status = NtOpenFile(&hFile,
                            FILE_GENERIC_READ,
                            &ObjectAttributes,
                            &IoStatusBlock,
                            FILE_SHARE_READ,
                            FILE_SYNCHRONOUS_IO_NONALERT);
    }
    if (NT_SUCCESS(Status))
        return hFile;

    return INVALID_HANDLE_VALUE;
}

BOOL DoReadFile(HANDLE hFile, LPVOID pvData, ULONG cbData, ULONG *pcbRead)
{
    NTSTATUS Status;
    HANDLE hEvent;
    Status = NtCreateEvent(&hEvent, EVENT_ALL_ACCESS, NULL, SynchronizationEvent, FALSE);
    if (!NT_SUCCESS(Status))
        return FALSE;

    Status = NtReadFile(hFile,
                        hEvent,
                        NULL,
                        NULL,
                        &IoStatusBlock,
                        pvData,
                        cbData,
                        NULL,
                        NULL);
    NtWaitForSingleObject(hEvent, FALSE, NULL);
    if (pcbRead)
    {
        *pcbRead = IoStatusBlock.Information;
    }
    NtClose(hEvent);
    return NT_SUCCESS(Status);
}

BOOL DoWriteFile(HANDLE hFile, LPCVOID pvData, ULONG cbData)
{
    NTSTATUS Status;
    Status = NtWriteFile(hFile,
                         NULL,
                         NULL,
                         NULL,
                         &IoStatusBlock,
                         (LPVOID)pvData,
                         cbData,
                         NULL,
                         NULL);
    return NT_SUCCESS(Status);
}

BOOL DoPutSz(HANDLE hFile, LPCSTR psz)
{
    return DoWriteFile(hFile, psz, (DWORD)strlen(psz));
}

BOOL DoPutDword(HANDLE hFile, ULONG dwNumber)
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

VOID DoCloseFile(HANDLE hFile)
{
    NtClose(hFile);
}
