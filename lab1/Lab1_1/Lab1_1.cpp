
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <tchar.h>

    
using namespace std;

void GetError();


int main(int argc, char* argv[])
{
    //функция локализации, для поддержи работы с русским текстом
    setlocale(LC_ALL, "RUSSIAN");
    //программа может обработать только 1 ключ, первый который ей будет передан,по этому для коректной работы
    //необходимо передавать только один ключ из ниже перечисленых 
    //-e
    //-s
    //-a
    //-u
    //в остальных случаях программа выдаст ошибку и прекратит свою работу
    if (argc == 2) {
        //если был передан ключ -s, тогда на экран будет выведена информация о пк и памяти
        if (strcmp(argv[1], "-s") == 0) {
            SYSTEM_INFO si;
            GetSystemInfo(&si);
            printf("My system info :\n");
            printf("  Number of processors: %u\n", si.dwNumberOfProcessors);
            printf("  Page size: %u\n", si.dwPageSize);
            printf("  Processor type: %u\n", si.dwProcessorType);
            printf("  Minimum application address: %lx\n", si.lpMinimumApplicationAddress);
            printf("  Maximum application address: %lx\n", si.lpMaximumApplicationAddress);
            printf("  Active processor mask: %u\n", si.dwActiveProcessorMask);
            printf("  Active processor mask: %u\n", si.wProcessorLevel);
            printf("Memory information :\n");
            MEMORYSTATUS ms;
            GlobalMemoryStatus(&ms);
            printf("  Количество использованной памяти в процентах : %u\n", ms.dwMemoryLoad);
            printf("  Объем ОЗУ: %u\n", ms.dwTotalPhys);
            printf("  Доступная ОЗУ : %u\n", ms.dwAvailPhys);
            printf("  Общее число байтов виртуальной памяти, используемой в вызывающем процессе : %u\n", ms.dwTotalVirtual);
            printf("  Объем этой памяти, еще доступной для вызывающего процесса: %u\n", ms.dwAvailVirtual);
        }
        //если был передан ключ -e, будет сгенерирована ошибка и выведен текст этой ошибки
        else if (strcmp(argv[1], "-e") == 0) {
            HLOCAL result = LocalAlloc(LHND, INT32_MAX);
            if (result == NULL) {
                GetError();
            }
        }
        else {
            printf("Error key");
        }
       
    } 
    else {
        printf("Error argument");
    }
    return 0;
}

void GetError()
{
    HLOCAL lpMsg;
    DWORD errcode;
    errcode = GetLastError();
    FormatMessage(
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        errcode,
        MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US),
        ((LPTSTR)&lpMsg),
        0,
        NULL);
    wprintf((L"%s\n"), (LPTSTR)lpMsg);
    LocalFree(lpMsg);
}



