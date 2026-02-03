#include <windows.h>
#include <iostream>

int main()
{
    const char* mapName = "MySharedFileMap";

    HANDLE hMap = OpenFileMappingA(FILE_MAP_WRITE, FALSE, mapName);
    if (!hMap) 
    {
        std::cerr << "OpenFileMappingA failed (run mapper first): " << GetLastError() << "\n";
        return 1;
    }

    char* view = (char*)MapViewOfFile(hMap, FILE_MAP_WRITE, 0, 0, 1);
    if (!view) 
    {
        std::cerr << "MapViewOfFile failed: " << GetLastError() << "\n";
        CloseHandle(hMap);
        return 1;
    }

    std::cout << "Writer: changing first byte '" << view[0] << "' -> 'Z'\n";
    view[0] = 'Z';
    FlushViewOfFile(view, 1);

    UnmapViewOfFile(view);
    CloseHandle(hMap);
    return 0;
}
