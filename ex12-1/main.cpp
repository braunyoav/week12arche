#include <windows.h>
#include <iostream>

static long long CountChar(const char* buf, size_t n, char c)
{
    long long cnt = 0;
    for (size_t i = 0; i < n; i++)
    {
        if (buf[i] == c)
        {
            cnt++;
        }
    }
        return cnt;
}

int main()
{
    const char* fileName = "gibrish.bin";
    const char letter = 'A';

    SYSTEM_INFO si{};
    GetSystemInfo(&si);
    const DWORD chunkSize = si.dwAllocationGranularity;

    HANDLE hFile = CreateFileA(
        fileName,
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );
    if (hFile == INVALID_HANDLE_VALUE) 
    {
        std::cerr << "CreateFileA failed: " << GetLastError() << "\n";
        return 1;
    }

    LARGE_INTEGER sz{};
    if (!GetFileSizeEx(hFile, &sz) || sz.QuadPart <= 0)
    {
        std::cerr << "GetFileSizeEx failed or empty file\n";
        CloseHandle(hFile);
        return 1;
    }
    const unsigned long long fileSize = (unsigned long long)sz.QuadPart;

    HANDLE hMap = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READONLY,
        0, 0,
        NULL
    );
    if (!hMap)
    {
        std::cerr << "CreateFileMappingA failed: " << GetLastError() << "\n";
        CloseHandle(hFile);
        return 1;
    }

    long long total = 0;

    for (unsigned long long offset = 0; offset < fileSize; offset += chunkSize)
    {
        DWORD bytesToMap = chunkSize;
        unsigned long long remaining = fileSize - offset;
        if (remaining < bytesToMap)
        {
            bytesToMap = (DWORD)remaining;
        }

        DWORD offsetLow = (DWORD)(offset & 0xFFFFFFFFULL);
        DWORD offsetHigh = (DWORD)(offset >> 32);

        const char* view = (const char*)MapViewOfFile(
            hMap,
            FILE_MAP_READ,
            offsetHigh,
            offsetLow,
            bytesToMap
        );

        if (!view) 
        {
            std::cerr << "MapViewOfFile failed at offset " << offset
                << " err=" << GetLastError() << "\n";
            CloseHandle(hMap);
            CloseHandle(hFile);
            return 1;
        }

        total += CountChar(view, bytesToMap, letter);

        UnmapViewOfFile(view);
    }

    std::cout << "the number of A's is: " << total << "\n";

    CloseHandle(hMap);
    CloseHandle(hFile);
    return 0;
}
