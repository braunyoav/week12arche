#include <windows.h>
#include <iostream>

int main()
{
    const char* fileName = "gibrish.bin";
    const char* mapName = "MySharedFileMap";

    HANDLE hFile = CreateFileA(
        fileName,
        GENERIC_READ | GENERIC_WRITE,          
        FILE_SHARE_READ | FILE_SHARE_WRITE,
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

    HANDLE hMap = CreateFileMappingA(
        hFile,
        NULL,
        PAGE_READWRITE,
        0, 0,
        mapName
    );
    if (!hMap) 
    {
        std::cerr << "CreateFileMappingA failed: " << GetLastError() << "\n";
        CloseHandle(hFile);
        return 1;
    }

    char* view = (char*)MapViewOfFile(hMap, FILE_MAP_READ, 0, 0, 1);
    if (!view)
    {
        std::cerr << "MapViewOfFile failed: " << GetLastError() << "\n";
        CloseHandle(hMap);
        CloseHandle(hFile);
        return 1;
    }

    std::cout << "Mapper running.\n";
    std::cout << "First byte before: '" << view[0] << "'\n";
    std::cout << "Run ex11-2-write.exe in another console, then press Enter...\n";
    std::cin.get();

    std::cout << "First byte after:  '" << view[0] << "'\n";

    UnmapViewOfFile(view);
    CloseHandle(hMap);
    CloseHandle(hFile);
    return 0;
}
