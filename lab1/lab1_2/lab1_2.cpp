
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <tchar.h>

#define MAXBYTE 256

using namespace std;


//функция для преобразования с Анси в Юникод
void ANSIToUnicode(HANDLE ReadF, HANDLE WriteF);
//функция для преобразования с Юникода в Анси
void UnicodeToANSI(HANDLE ReadF, HANDLE WriteF);
//буфер для считаных байт
CHAR DataBuferOnMByte[(MAXBYTE) * sizeof(CHAR)] = { 0 };
//буфер для преобразованых байт
WCHAR DataBuferOnWChar[(MAXBYTE) * sizeof(WCHAR)] = { 0 };
//колчиство считаных байт с файла
DWORD CountReadCh = 0;

DWORD CountChangedCh = 0;
//количество записаных байт в файл
DWORD CountWriteCh = 0;
//файловый дескриптор для файла чтения
HANDLE ReadF;
//файловый дескриптор для файла записи
HANDLE WriteF;
//вывод теста последней ошибки по ее номеру
void GetError();
//очистка буфера и установка все счетчиков в ноль
void ClearBuffer();

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
    if (argc == 3) {
        //если был передан ключ -u or -a, будут открыт файл для чтения байт и создан файл для записи результата
        //затем взависимости от ключа вызвана функция 
         if (strcmp(argv[2], "-u") == 0 || strcmp(argv[2], "-a") == 0) {
            ReadF = CreateFileA(argv[1],
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);
            //если переданый первый параметр будет ошибочным(имя файла), тогда будет выведено сообщение об ошибке и программа прекратит свою работу
            if (ReadF == INVALID_HANDLE_VALUE) {
                cout << "Error input file : "; GetError(); return 0;
            }
            WriteF = CreateFile(L"Out.txt",
                GENERIC_WRITE,
                FILE_SHARE_WRITE,
                NULL,
                CREATE_ALWAYS,
                FILE_ATTRIBUTE_NORMAL,
                NULL);
            if (WriteF == INVALID_HANDLE_VALUE) {
                cout << "Error out file: "; GetError(); return 0;
            }
            switch (argv[2][1])
            {
            case 'u':
                UnicodeToANSI(ReadF, WriteF);
                break;
            case 'a':
                ANSIToUnicode(ReadF, WriteF);
                break;
            default:
                cout << "Key is wrong:  " << argv[2];
                break;
            }
            CloseHandle(ReadF);
            CloseHandle(WriteF);
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

void ClearBuffer()
{
    for (int i = 0; i < MAXBYTE * sizeof(CHAR); i++)
    {
        DataBuferOnMByte[i] = 0;
    }
    for (int i = 0; i < MAXBYTE * sizeof(WCHAR); i++)
    {
        DataBuferOnWChar[i] = 0;
    }
    CountReadCh = 0;
    CountChangedCh = 0;
    CountWriteCh = 0;
}

void ANSIToUnicode(HANDLE ReadF, HANDLE WriteF)
{
    ClearBuffer();
    do {
        if (!ReadFile(ReadF, DataBuferOnMByte, (MAXBYTE - 1) * sizeof(CHAR), &CountReadCh, NULL))
        {
            GetError(); return;
        }
        CountChangedCh = MultiByteToWideChar(CP_ACP, 0, DataBuferOnMByte, -1, DataBuferOnWChar, CountReadCh * sizeof(WCHAR));
        if (!WriteFile(WriteF, DataBuferOnWChar, (CountChangedCh - 1) * sizeof(WCHAR), &CountWriteCh, NULL))
        {
            GetError(); return;
        }
    } while (CountReadCh == MAXBYTE);
    printf("Transcoding complete.\n");
}

void UnicodeToANSI(HANDLE InF, HANDLE OutF)//"/u"
{
    ClearBuffer();
    // получение первых 2, которые отвечают это LE или BE
    if (!ReadFile(InF, DataBuferOnWChar, 2, &CountReadCh, NULL))
    {
        GetError();
    }
    // проверка полученых байт, нужная ли это кодировка файла
    else if (*DataBuferOnWChar == 0xFEFF || *DataBuferOnWChar == 0xFFFE)
    {
        do {
            //из файла для чтение производится считывание 265 байт, если ошибки при считывание небыло выполняется перекодирования
            //считаных байт в буфере
            if (!ReadFile(InF, DataBuferOnWChar, (MAXBYTE - 1) * sizeof(WCHAR), &CountReadCh, NULL))
            {
                GetError(); return;
            }
            CountChangedCh = WideCharToMultiByte(CP_MACCP, 0, DataBuferOnWChar, -1, DataBuferOnMByte, CountReadCh, NULL, NULL);
            //запись перекодированых байт в файл, при ошибочной записи, вывод ошибки
            if (!WriteFile(OutF, DataBuferOnMByte, CountChangedCh - 1, &CountWriteCh, NULL))
            {
                GetError(); return;
            }
            //если количество считаных  байт равно 256 , нужно проивести повторное считывание
        } while (CountReadCh == MAXBYTE);
        //подтвержение успешности всех операций
        printf("Transcoding complete.\n");
    }
    else
    {
        printf("Error:\n");
        printf("It`s not Unicode Text!\n");
    }
}
