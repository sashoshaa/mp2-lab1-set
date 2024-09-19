// ННГУ, ВМК, Курс "Методы программирования-2", С++, ООП
//
// tbitfield.cpp - Copyright (c) Гергель В.П. 07.05.2001
//   Переработано для Microsoft Visual Studio 2008 Сысоевым А.В. (19.04.2015)
//
// Битовое поле привет

#include "tbitfield.h"

// Fake variables used as placeholders in tests
static const int FAKE_INT = -1;
static TBitField FAKE_BITFIELD(1);

TBitField::TBitField(int len) //конструктор инициализации
{
	if (len < 0) {
		throw std::invalid_argument("Error!");
	}
	BitLen = len;
	MemLen = (BitLen + (sizeof(TELEM) * 8 - 1)) / (sizeof(TELEM) * 8);
	pMem = new TELEM[MemLen];
	for (int i = 0; i < MemLen; i++)
		pMem[i] = 0;
}

TBitField::TBitField(const TBitField& bf) // конструктор копирования
{
	BitLen = bf.BitLen;
	MemLen = bf.MemLen;

	pMem = new TELEM[MemLen];
	std::memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
}

TBitField::~TBitField() // деструктор
{
	delete[] pMem;
}

int TBitField::GetMemIndex(const int n) const // индекс Мем для бита n
{
	if ((n < 0) || (n >= BitLen)) {
		throw std::out_of_range("Error!");
	}
	return n / (sizeof(TELEM) * 8);
}

TELEM TBitField::GetMemMask(const int n) const // битовая маска для бита n
{
	if ((n < 0) || (n >= BitLen)) {
		throw std::out_of_range("Error!");
	}
	return 1 << n % (sizeof(TELEM) * 8); // оператор сдвига слево
}

// доступ к битам битового поля

int TBitField::GetLength(void) const // получить длину (к-во битов)
{
	return BitLen;
}

void TBitField::SetBit(const int n) // установить бит
{
	if ((n < 0) || (n >= BitLen))
		throw std::out_of_range("Error!");
	int i = GetMemIndex(n);
	TELEM t = GetMemMask(n);
	pMem[i] |= t; // a = a | b; a |= b; побитовое или
}

void TBitField::ClrBit(const int n) // очистить бит
{
	if ((n < 0) || (n >= BitLen))
		throw std::out_of_range("Error!");
	int i = GetMemIndex(n);
	TELEM t = GetMemMask(n);
	pMem[i] &= ~t; // побитовое отрицание
}

int TBitField::GetBit(const int n) const // получить значение бита
{
	if ((n < 0) || (n >= BitLen))
		throw std::out_of_range("Error!");
	int i = GetMemIndex(n);
	TELEM t = GetMemMask(n);
	return (pMem[i] & t) != 0; // побитовое и
}

// битовые операции

TBitField& TBitField::operator=(const TBitField& bf) // присваивание
{
	if (this != &bf) {
		delete[] pMem;
		BitLen = bf.BitLen;
		MemLen = bf.MemLen;
		pMem = new TELEM[MemLen];
		std::memcpy(pMem, bf.pMem, MemLen * sizeof(TELEM));
	}
	return *this;
}

int TBitField::operator==(const TBitField& bf) const // сравнение - ?
{
	if (BitLen != bf.BitLen)
		return 0;
	for (int i = 0; i < MemLen; ++i)
		if (pMem[i] != bf.pMem[i]) return 0;
	for (int i = MemLen; i < (MemLen - 1) * sizeof(TELEM) * 8; i++)
		if (this->GetBit(i) != bf.GetBit(i))
			return 0;
	return 1;
}

int TBitField::operator!=(const TBitField& bf) const // сравнение
{
	int r = (bf == *this);
	if (r == 0)
		return 1;
	else
		return 0;
}

TBitField TBitField::operator|(const TBitField& bf) // операция побитовое "или"
{
	int len;
	int len1;
	if (BitLen > bf.BitLen) {
		len = BitLen;
		len1 = bf.BitLen;
	}
	else {
		len = bf.BitLen;
		len1 = BitLen;
	}
	TBitField res(len);
	int i = 0;
	for (i = 0; i < len1; i++)
		res.pMem[i] = pMem[i] | bf.pMem[i];
	while (i < len)
	{
		if (BitLen > bf.BitLen)
			res.pMem[i] = pMem[i];
		else
			res.pMem[i] = bf.pMem[i];
		i++;
	}
	return res;
}

TBitField TBitField::operator&(const TBitField& bf) // операция побитовое "и"
{
	int len;
	int len1;
	if (BitLen > bf.BitLen) {
		len = BitLen;
		len1 = bf.BitLen;
	}
	else {
		len = bf.BitLen;
		len1 = BitLen;
	}
	TBitField res(len);
	for (int i = 0; i < len1; i++)
		res.pMem[i] = pMem[i] & bf.pMem[i];
	return res;
}

TBitField TBitField::operator~(void) // отрицание
{
	TBitField res(BitLen);
	for (int i = 0; i < MemLen; ++i) {
		res.pMem[i] = ~pMem[i];
	}
	return res;
}

// ввод/вывод

istream& operator>>(istream& in, TBitField& bf) // ввод
{
	int bit;
	for (int i = 0; i < bf.BitLen; i++) {
		in >> bit;
		if (bit)
			bf.SetBit(i);
		else
			bf.ClrBit(i);
	}
	return in;
}

ostream& operator<<(ostream& out, const TBitField& bf) // вывод
{
	for (int i = 0; i < bf.BitLen; ++i) {
		if (bf.GetBit(i) == 0)
			out << 0;
		else
			out << 1;
	}
	return out;
}