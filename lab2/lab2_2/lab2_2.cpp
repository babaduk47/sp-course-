#define _CRT_SECURE_NO_WARNINGS
#define MENU_PRINT 1
#define MENU_COPY 2
#define MENU_CREATE 3
#define MENU_INFO_MORE 4
#define MENU_DELETE 5
#define MENU_SET 0
#include <windows.h>;
#include <iostream>
#include <locale.h>

using namespace std;

void SetDir(TCHAR* buffer);
void PrintDir(TCHAR* buffer);
void CreateDir(TCHAR* buffer);
void FullInfo(TCHAR* buffer);
void DeleteDir(TCHAR* buffer);
void CopyFileInCurDir(TCHAR* buffer);

int main() {

	setlocale(LC_ALL, "rus");

	while (true) {
		TCHAR buffer[MAX_PATH];
		GetCurrentDirectory(sizeof(buffer), buffer);

		int key;

		system("cls");
		TCHAR dir[105] = { L"Your current directory: " };
		lstrcat(dir, buffer);

		SetConsoleTitle(dir);

		cout << "What do you want to do?\n" << endl;
		cout << "[" << MENU_PRINT << "]Print directory" << endl;
		cout << "[" << MENU_COPY << "]Copy File in current directory" << endl;
		cout << "[" << MENU_CREATE << "]Create directory in current directory" << endl;
		cout << "[" << MENU_INFO_MORE << "]More information about file" << endl;
		cout << "[" << MENU_DELETE << "]Delete file or empty directory" << endl;
		cout << "[" << MENU_SET << "]Set directory" << endl;
		cout << ">> ";

		cin >> key;

		switch (key) {
		case MENU_SET:
			SetDir(buffer);
			break;
		case MENU_PRINT:
			PrintDir(buffer);
			break;
		case MENU_COPY:
			CopyFileInCurDir(buffer);
			break;
		case MENU_CREATE:
			CreateDir(buffer);
			break;
		case MENU_INFO_MORE:
			FullInfo(buffer);
			break;
		case MENU_DELETE:
			DeleteDir(buffer);
			break;
		}
		system("pause");
	}
}

void SetDir(TCHAR* buffer) {
	for (;;) {
		cout << "Enter full way to the directory" << endl;
		TCHAR temp[MAX_PATH];
		cin.get();
		scanf("%80S", temp);
		if (!SetCurrentDirectory(temp))
			cout << "\nNot correct name of directory" << endl;
		else {
			GetCurrentDirectory(sizeof(temp), temp);
			wcout << "Your current directory is : " << temp << endl;
			lstrcpy(buffer, L"");
			lstrcpy(buffer, temp);
			return;
		}
	}
}

void PrintDir(TCHAR* buffer) {
	cout << "Files in current directory: " << endl;
	WIN32_FIND_DATA FindFileData;
	HANDLE hf;
	TCHAR find[MAX_PATH];
	lstrcpy(find, buffer);
	lstrcat(find, L"\\*");
	hf = FindFirstFile((LPCWSTR)find, &FindFileData);
	if (hf != INVALID_HANDLE_VALUE)
	{
		do
		{
			SYSTEMTIME systime;
			FILETIME localfiletime;
			FileTimeToLocalFileTime(&FindFileData.ftCreationTime, &localfiletime);
			FileTimeToSystemTime(&localfiletime, &systime);
			LONGLONG size = (FindFileData.nFileSizeHigh * (MAXDWORD + 1)) + FindFileData.nFileSizeLow;
			wcout << FindFileData.cFileName << "\t" << size << " bytes " << "\t" << systime.wDay << "." << systime.wMonth << "." << systime.wYear << " " << systime.wHour << ":" << systime.wMinute << endl;
		} while (FindNextFile(hf, &FindFileData) != 0);
		FindClose(hf);
	}
}

bool FileExists(LPCTSTR fname)
{
	return::GetFileAttributes(fname) != DWORD(-1);
}

void CopyFileInCurDir(TCHAR* buffer) {
	cout << "Input file name in current directory: ";
	TCHAR CurFile[MAX_PATH], s[MAX_PATH], newDirectory[MAX_PATH];
	scanf("%80S", s);
	lstrcpy(CurFile, buffer);
	lstrcat(CurFile, L"\\");
	lstrcat(CurFile, s);
	if (!FileExists(CurFile)) {
		cout << "File does not exist: ";
		wcout<< CurFile;
		cout << "\n";
	}
	cout << "\nInput new copy file name: ";
	scanf("%80S", s);
	lstrcpy(newDirectory, buffer);
	lstrcat(newDirectory, L"\\");
	lstrcat(newDirectory, s);
	BOOL temp = CopyFile(CurFile, newDirectory, false);
	if (!temp) cout << "Copy error\n"; 
	else cout << "Copied\n";
}
void CreateDir(TCHAR* buffer) {
	TCHAR newDir[MAX_PATH], path[MAX_PATH];
	cout << "Input new directory name: ";
	cin.get();
	scanf("%80S", newDir);
	lstrcpy(path, buffer);
	lstrcat(path, L"\\");
	lstrcat(path, newDir);
	if (CreateDirectory(path, NULL))
		cout << "Directory created" << endl;
	else
		cout << "Error create directory" << endl;
}

void FullInfo(TCHAR* buffer) {
	TCHAR file[MAX_PATH], way[MAX_PATH];
	cout << "Input file name: ";
	cin.get();
	scanf("%80S", file);
	lstrcpy(way, buffer);
	lstrcat(way, L"\\");
	lstrcat(way, file);
	HANDLE hFile1 = CreateFile(way, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	BY_HANDLE_FILE_INFORMATION infa;
	BOOL f = GetFileInformationByHandle(hFile1, &infa);
	CloseHandle(hFile1);
	if (!f) {
		cout << "Not correct name of file\n";
	}
	else {
		SYSTEMTIME systime;
		FILETIME localfiletime;
		FileTimeToLocalFileTime(&infa.ftCreationTime, &localfiletime);
		FileTimeToSystemTime(&localfiletime, &systime);
		cout << "Creation time - " << systime.wDay << "." << systime.wMonth << "." << systime.wYear << " " << systime.wHour << ":" << systime.wMinute << endl;

		FileTimeToLocalFileTime(&infa.ftLastWriteTime, &localfiletime);
		FileTimeToSystemTime(&localfiletime, &systime);
		cout << "Last Write time - " << systime.wDay << "." << systime.wMonth << "." << systime.wYear << " " << systime.wHour << ":" << systime.wMinute << endl;
	}
}
void DeleteDir(TCHAR* buffer) {
	TCHAR file[MAX_PATH], way[MAX_PATH];
	cout << "Input file name to delete: ";
	cin.get();
	scanf("%80S", file);
	lstrcpy(way, buffer);
	lstrcat(way, L"\\");
	lstrcat(way, file);
	BOOL g = DeleteFile(way), k;
	if (!g) {
		k = RemoveDirectory(way);
	}
	if (!g && !k) {
		DWORD lastErrorCode = GetLastError();
		void* cstr;
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
			NULL,
			GetLastError(),
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&cstr,
			0,
			NULL
		);
		PTCHAR res((LPTSTR)cstr);
		printf("%s", res);
	}
	else {
		cout << "Deleted : ";
		wcout << way << endl;
	}
}


