#include <iostream>
#include <limits>
#include <iomanip>

using namespace std;

//типизированя функция 
template<typename T> 
//параметр i нужен что бы при выводе типов CHAR выводился диапазон а не символы
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
    //заголовок таблицы
   cout << setw(20) <<"Type" <<setw(9)
        << "SIZE" <<setw(25) 
        << "MIN" <<setw(25) 
        << "MAX" <<endl;
   //построчоный вывод типов данных с учетом форматирования тиблицы
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
