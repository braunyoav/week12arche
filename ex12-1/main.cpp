#include <windows.h>
#include <cstdlib>
#include <iostream>


int main()
{

    HANDLE hFile;

    hFile = CreateFileA
    (
        "gibrish.bin",
        GENERIC_READ,
        FILE_SHARE_READ,
        NULL,
        OPEN_EXISTING,
        FILE_ATTRIBUTE_NORMAL,
        NULL
    );

    if (hFile == INVALID_HANDLE_VALUE)
    {
        return 1;
    }
    LARGE_INTEGER fileSize;
    if (!GetFileSizeEx(hFile, &fileSize))
    {
        CloseHandle(hFile);
        return 1;
    }
    DWORD dwFileSize = fileSize.LowPart;
    if (dwFileSize == 0)
    {
        std::cout << "Error in size";
        return 1;
    }
    char* ptr = (char*)malloc(dwFileSize);
    if (ptr == NULL)
    {
        return 1;
    }
    DWORD dwBytesRead = 0;
    BOOL bSuccess = ReadFile(
        hFile,                
        ptr,        
        dwFileSize,            
        &dwBytesRead,           
        NULL
    );

    if (!bSuccess || dwBytesRead != dwFileSize)
    {
        std::cerr << "Error reading file: " << GetLastError() << std::endl;
        CloseHandle(hFile);
        return 1;
    }
    long int counter = 0;
    for (long int i = 0; i < dwFileSize; i++)
    {
        if (*(ptr + i) == 'A')
        {
            counter++;
        }
    }
    std::cout << "the number of A's is: " << counter << std::endl;
    free(ptr);
    CloseHandle(hFile);
    system("pause");
    getchar();
    return 0;
}