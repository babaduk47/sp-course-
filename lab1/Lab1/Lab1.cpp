
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <tchar.h>

#define MAXBYTE 256

using namespace std;


//������� ��� �������������� � ���� � ������
void ANSIToUnicode(HANDLE ReadF, HANDLE WriteF);
//������� ��� �������������� � ������� � ����
void UnicodeToANSI(HANDLE ReadF, HANDLE WriteF);
//����� ��� �������� ����
CHAR DataBuferOnMByte[(MAXBYTE) * sizeof(CHAR)] = { 0 };
//����� ��� �������������� ����
WCHAR DataBuferOnWChar[(MAXBYTE) * sizeof(WCHAR)] = { 0 };
//��������� �������� ���� � �����
DWORD CountReadCh = 0;

DWORD CountChangedCh = 0;
//���������� ��������� ���� � ����
DWORD CountWriteCh = 0;
//�������� ���������� ��� ����� ������
HANDLE ReadF;
//�������� ���������� ��� ����� ������
HANDLE WriteF;
//����� ����� ��������� ������ �� �� ������
void GetError();
//������� ������ � ��������� ��� ��������� � ����
void ClearBuffer();

int main(int argc, char* argv[])
{
    //������� �����������, ��� �������� ������ � ������� �������
    setlocale(LC_ALL, "RUSSIAN");
    //��������� ����� ���������� ������ 1 ����, ������ ������� �� ����� �������,�� ����� ��� ��������� ������
    //���������� ���������� ������ ���� ���� �� ���� ������������ 
    //-e
    //-s
    //-a
    //-u
    //� ��������� ������� ��������� ������ ������ � ��������� ���� ������
    if (argc == 3) {
        //���� ��� ������� ���� -s, ����� �� ����� ����� �������� ���������� � �� � ������
        if (strcmp(argv[2], "-s") == 0) {
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
            printf("  ���������� �������������� ������ � ��������� : %u\n", ms.dwMemoryLoad);
            printf("  ����� ���: %u\n", ms.dwTotalPhys);
            printf("  ��������� ��� : %u\n", ms.dwAvailPhys);
            printf("  ����� ����� ������ ����������� ������, ������������ � ���������� �������� : %u\n", ms.dwTotalVirtual);
            printf("  ����� ���� ������, ��� ��������� ��� ����������� ��������: %u\n", ms.dwAvailVirtual);
        }
        //���� ��� ������� ���� -e, ����� ������������� ������ � ������� ����� ���� ������
        else if (strcmp(argv[2], "-e") == 0) {
            HLOCAL result = LocalAlloc(LHND, INT32_MAX);
            if (result == NULL) {
                GetError();
            }
        }
        //���� ��� ������� ���� -u or -a, ����� ������ ���� ��� ������ ���� � ������ ���� ��� ������ ����������
        //����� ������������ �� ����� ������� ������� 
        else if (strcmp(argv[2], "-u") == 0 || strcmp(argv[2], "-a") == 0) {
            ReadF = CreateFileA(argv[1],
                GENERIC_READ,
                FILE_SHARE_READ,
                NULL,
                OPEN_EXISTING,
                FILE_ATTRIBUTE_NORMAL,
                NULL);
            //���� ��������� ������ �������� ����� ���������(��� �����), ����� ����� �������� ��������� �� ������ � ��������� ��������� ���� ������
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
    for ( int  i = 0; i < MAXBYTE * sizeof(CHAR); i++)
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
    // ��������� ������ 2, ������� �������� ��� LE ��� BE
    if (!ReadFile(InF, DataBuferOnWChar, 2, &CountReadCh, NULL))
    {
        GetError();
    }
    // �������� ��������� ����, ������ �� ��� ��������� �����
    else if (*DataBuferOnWChar == 0xFEFF || *DataBuferOnWChar == 0xFFFE)
    {
        do {
            //�� ����� ��� ������ ������������ ���������� 265 ����, ���� ������ ��� ���������� ������ ����������� ���������������
            //�������� ���� � ������
            if (!ReadFile(InF, DataBuferOnWChar, (MAXBYTE - 1) * sizeof(WCHAR), &CountReadCh, NULL))
            {
                GetError(); return;
            }
            CountChangedCh = WideCharToMultiByte(CP_MACCP, 0, DataBuferOnWChar, -1, DataBuferOnMByte, CountReadCh, NULL, NULL);
            //������ ��������������� ���� � ����, ��� ��������� ������, ����� ������
            if (!WriteFile(OutF, DataBuferOnMByte, CountChangedCh - 1, &CountWriteCh, NULL))
            {
                GetError(); return;
            }
            //���� ���������� ��������  ���� ����� 256 , ����� ��������� ��������� ����������
        } while (CountReadCh == MAXBYTE);
        //������������ ���������� ���� ��������
        printf("Transcoding complete.\n");
    }
    else
    {
        printf("Error:\n");
        printf("It`s not Unicode Text!\n");
    }
}
