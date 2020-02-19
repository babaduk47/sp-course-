#include <iostream>
#include <limits>
#include <iomanip>

using namespace std;

//������������ ������� 
template<typename T> 
//�������� i ����� ��� �� ��� ������ ����� CHAR ��������� �������� � �� �������
void ShowType(bool i=false) {
    cout << setw(20) << typeid(T).name() << setw(4)
        << sizeof(T)<< setw(5) <<" Byte" << setw(25);
    if(!i)
    cout << numeric_limits<T>::min()<< setw(25)
        << numeric_limits<T>::max() << endl;
    else     cout << (int)numeric_limits<T>::min() << setw(25)
        << (int)numeric_limits<T>::max() << endl;
}

int main()
{
    //��������� �������
   cout << setw(20) <<"Type" <<setw(9)
        << "SIZE" <<setw(25) 
        << "MIN" <<setw(25) 
        << "MAX" <<endl;
   //����������� ����� ����� ������ � ������ �������������� �������
   ShowType<int>();
   ShowType<bool>();
   ShowType<float>();
   ShowType<double>();
   ShowType<char>(true);
   ShowType<long>();
   ShowType<signed char>(true);
   ShowType<unsigned char>(true);
   ShowType<unsigned short>();
   ShowType<unsigned int>();
   ShowType<unsigned long>();
   ShowType<unsigned long long>();
   ShowType<wchar_t>();
   ShowType<char16_t>();
   ShowType<char32_t>();
   ShowType<short>();
   ShowType<long long>();
}
