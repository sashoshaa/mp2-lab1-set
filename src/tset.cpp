// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tset.cpp - Copyright (c) Гергель В.П. 04.10.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Множество - реализация через битовые поля

#include "tset.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);
static TSet FAKE_SET(1);

TSet::TSet(int mp) : BitField(-1)
{
    if (mp <= 0) {
        throw std::invalid_argument("MaxPower must be positive");
    }
}

// конструктор копирования
TSet::TSet(const TSet& s) : MaxPower(s.MaxPower), BitField(s.BitField) { }

// конструктор преобразования типа
TSet::TSet(const TBitField& bf) : MaxPower(bf.GetLength()), BitField(bf) { }

// преобразование множества в битовое поле
TSet::operator TBitField()
{
    return BitField;
}

int TSet::GetMaxPower(void) const // получить макс. к-во эл-тов
{
    return MaxPower;
}

int TSet::IsMember(const int Elem) const // элемент множества?
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of bounds");
    }
    return BitField.GetBit(Elem);
}

void TSet::InsElem(const int Elem) // включение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of bounds");
    }
    BitField.SetBit(Elem);
}

void TSet::DelElem(const int Elem) // исключение элемента множества
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of bounds");
    }
    BitField.ClrBit(Elem);
}

// теоретико-множественные операции

TSet& TSet::operator=(const TSet& s) // присваивание
{
    if (this != &s) {
        MaxPower = s.MaxPower;
        BitField = s.BitField;
    }
    return *this;
}

int TSet::operator==(const TSet& s) const // сравнение
{
    return (MaxPower == s.MaxPower) && (BitField == s.BitField);
}

int TSet::operator!=(const TSet& s) const // сравнение
{
    return !(*this == s);
}

TSet TSet::operator+(const TSet& s) // объединение
{
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("Sets must have the same MaxPower for union");
    }
    TSet result(MaxPower);
    result.BitField = BitField | s.BitField;
    return result;
}

TSet TSet::operator+(const int Elem) // объединение с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of bounds");
    }
    TSet result(*this);
    result.InsElem(Elem);
    return result;
}

TSet TSet::operator-(const int Elem) // разность с элементом
{
    if (Elem < 0 || Elem >= MaxPower) {
        throw std::out_of_range("Element is out of bounds");
    }
    TSet result(*this);
    result.DelElem(Elem);
    return result;
}

TSet TSet::operator*(const TSet& s) // пересечение
{
    if (MaxPower != s.MaxPower) {
        throw std::invalid_argument("Sets must have the same MaxPower for intersection");
    }
    TSet result(MaxPower);
    result.BitField = BitField & s.BitField;
    return result;
}

TSet TSet::operator~(void) // дополнение
{
    TSet result(MaxPower);
    result.BitField = ~BitField;
    return result;
}

// перегрузка ввода/вывода

istream& operator>>(istream& istr, TSet& s) // ввод
{
    for (int i = 0; i < s.MaxPower; i++) {
        int elem;
        istr >> elem;
        if (elem == 1) {
            s.InsElem(i);
        }
        else {
            s.DelElem(i);
        }
    }
    return istr;
}

ostream& operator<<(ostream& ostr, const TSet& s) // вывод
{
    //ostr << "{";
    //bool first = true;
    for (int i = 0; i < s.MaxPower; i++) {
        ostr << i;
        /*if (s.IsMember(i)) {
            if (!first) {
                ostr << ", ";
            }
            ostr << i;
            first = false;
        }*/
    }
    //ostr << "}";
    return ostr;
}
