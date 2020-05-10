#define _CRT_SECURE_NO_WARNINGS
#define MENU_ADD 1
#define MENU_CHANGE 2
#define MENU_DELETE 3
#define MENU_SHOWALL 4
#define MENU_EXIT 0

#include <iostream>
#include <windows.h>
#include <vector>


using namespace std;
LPCWSTR records = L"records.txt";

struct rec {
	UINT id;
	FILETIME creationTime;
	TCHAR data[81];
	UINT changesCounter;
};
struct header {
	UINT notEmpty;
	UINT sizeFile;
};

header ReadCounts();
vector<rec> ReadRecs();
void ShowAllRecords();
void AddRecord();
void ChangeRecord();
void DeleteRecord();


int main() {
	int key;
	while (true) {
		cout << "What do you want to do?\n" << endl;
		cout << "[" << MENU_ADD << "]Add record" << endl;
		cout << "[" << MENU_CHANGE << "]Change record" << endl;
		cout << "[" << MENU_DELETE << "]Delete record" << endl;
		cout << "[" << MENU_SHOWALL << "]Show all records" << endl;
		cout << "[" << MENU_EXIT << "]Exit" << endl;
		cout << ">> ";
		cin >> key;
		switch (key) {
		case MENU_EXIT:
			exit(EXIT_SUCCESS);
			break;
		case MENU_ADD:
			AddRecord();
			break;
		case MENU_CHANGE:
			ChangeRecord();
			break;
		case MENU_DELETE:
			DeleteRecord();
			break;
		case MENU_SHOWALL:
			ShowAllRecords();
			break;
		default:
			cout << "Input Error" << endl;
		}
		system("pause");
		system("cls");
	}
}


void ShowAllRecords() {
	vector <rec> datas = ReadRecs();
	header count = ReadCounts();
	SYSTEMTIME SysTime;
	cout << "Size of file: " << count.sizeFile << " bytes; Not null records: " << count.notEmpty << endl;
	for (UINT i = 0; i < datas.size(); i++) {
		FileTimeToSystemTime(&datas[i].creationTime, &SysTime);
		cout << "\nRecord ID : " << datas[i].id << "  " << SysTime.wDay << "." << SysTime.wMonth << "." << SysTime.wYear << " " << SysTime.wHour << ":" << SysTime.wMinute << endl;
		wcout << "Data : " << datas[i].data << endl;
		cout << "Of changes : " << datas[i].changesCounter << endl << endl;
	}
}

header ReadCounts() {
	HANDLE hFile1 = CreateFile(records, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	header h; DWORD ReadByte;
	ReadFile(hFile1, &h, sizeof(header), &ReadByte, NULL);
	if (ReadByte == 0) {
		h.notEmpty = 0;
		h.sizeFile = 0;
	}
	CloseHandle(hFile1);
	return h;
}

vector<rec> ReadRecs() {
	HANDLE hFile1 = CreateFile(records, GENERIC_READ, NULL, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	SetFilePointer(hFile1, sizeof(header), NULL, FILE_BEGIN);
	vector<rec> datas(0);
	rec i; DWORD ReadByte = 0;
	for (;;) {
		bool temp = ReadFile(hFile1, &i, sizeof(rec), &ReadByte, NULL);
		if (ReadByte == 0) break;
		datas.push_back(i);
	}
	CloseHandle(hFile1);

	return datas;
}

void AddRecord() {
	header head = ReadCounts();
	vector <rec> data = ReadRecs();
	SYSTEMTIME k, s;
	GetLocalTime(&k);
	FILETIME ft;
	rec f;
	SystemTimeToFileTime(&k, &ft);
	FileTimeToSystemTime(&ft, &s);
	cout << "\nFor empty data enter '*'";
	cout << "\nInput data: ";
	scanf("%80S", f.data);

	if (lstrcmp(f.data, L"*") == 0) {
		f.id = data.size();
		f.creationTime = ft;
		lstrcpy(f.data, L"");
		f.changesCounter = 0;
	}
	else {
		f.id = data.size();
		f.creationTime = ft;
		f.changesCounter = 0;
		head.notEmpty++;
	}

	data.push_back(f);
	head.sizeFile = sizeof(head) + sizeof(rec) * data.size();
	HANDLE hFile1 = CreateFile(records, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LPDWORD temp = NULL;
	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	WriteFile(hFile1, &head, sizeof(header), temp, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_END);
	for (int i = 0; i < data.size(); i++) {
		WriteFile(hFile1, &data[i], sizeof(rec), temp, NULL);
	}
	CloseHandle(hFile1);
}
void ChangeRecord() {
	SYSTEMTIME SysTime;
	header head = ReadCounts();
	vector <rec> data = ReadRecs();
	UINT n;
	cout << "Enter ID record you want to change: " << endl;
	cin >> n;

	FileTimeToSystemTime(&data[n].creationTime, &SysTime);

	cout << "Record ID : " << data[n].id << "  " << SysTime.wDay << "." << SysTime.wMonth << "." << SysTime.wYear << " " << SysTime.wHour << ":" << SysTime.wMinute << endl;
	wcout << "Data : " << data[n].data << endl;
	cout << "Of changes : " << data[n].changesCounter << endl;
	cout << "\nFor empty data enter '*')";
	cout << "\nEnter new data: ";
	TCHAR str[81];

	lstrcpy(str, data[n].data);
	scanf("%80S", data[n].data);
	if (lstrcmp(data[n].data, L"*") == 0) {
		lstrcpy(data[n].data, L"");
	}
	if (lstrcmp(str, L"") == 0 && lstrcmp(data[n].data, L"") != 0) {
		head.notEmpty++;
	}
	if (lstrcmp(str, L"") != 0 && lstrcmp(data[n].data, L"") == 0) {
		head.notEmpty--;
	}
	data[n].changesCounter++;
	HANDLE hFile1 = CreateFile(records, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LPDWORD musor = NULL;
	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	WriteFile(hFile1, &head, sizeof(header), musor, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_END);
	for (int i = 0; i < data.size(); i++) {
		WriteFile(hFile1, &data[i], sizeof(rec), musor, NULL);
	}
	CloseHandle(hFile1);
}
void DeleteRecord() {
	header head = ReadCounts();
	vector <rec> data = ReadRecs();
	int i;
	cout << "Enter ID record to delete: ";
	cin >> i;

	if (lstrcmp(data[i].data, L"") != 0)
		head.notEmpty--;

	data.erase(data.begin() + i);
	vector<rec>(data).swap(data);
	for (int k = 0; k < data.size(); k++) {
		data[k].id = k;
	}
	head.sizeFile = sizeof(head) + sizeof(rec) * data.size();
	HANDLE hFile1 = CreateFile(records, GENERIC_WRITE, NULL, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	LPDWORD musor = NULL;

	SetFilePointer(hFile1, NULL, NULL, FILE_BEGIN);
	WriteFile(hFile1, &head, sizeof(header), musor, NULL);
	SetFilePointer(hFile1, NULL, NULL, FILE_END);

	for (int i = 0; i < data.size(); i++) {
		WriteFile(hFile1, &data[i], sizeof(rec), musor, NULL);
	}
	CloseHandle(hFile1);
}

