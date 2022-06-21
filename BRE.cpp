// BRE
//Grzegorz Kowalczyk, D3

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

void ZapiszLog(const string);
void UtworzPlik(const string, const int, const char);
uint8_t HammingDistance(uint8_t, uint8_t);

int main(int argc, char* argv[])
{
	cout << "Kalkulator BRE, v.1.0 \n";
	ZapiszLog("Start programu");

	string nazwaplik1;
	string nazwaplik2;

	if (argc != 3)
	{
		ZapiszLog("Nie podano plików do porównania, program tworzy je sam."); 
		UtworzPlik("plik1.bin", 100000, 0x55);
		UtworzPlik("plik2.bin", 100000, 0x45);
	}
	else
	{
		nazwaplik1 = argv[1];
		nazwaplik1 = argv[2];

		ZapiszLog("Processing test files");
	}
	ZapiszLog("Koniec programu\n");
	return 0;
}

void ZapiszLog(const string wiadomosc)
{
	fstream plikLog;
	plikLog.open("Log.txt", ios::app);
	if (plikLog.good())
	{
		time_t teraz = time(NULL);
		string dt = ctime(&teraz);
		plikLog << dt.substr(0, dt.length() - 1) << ": " << wiadomosc << endl;
		plikLog.close();
	}
	else
	{
		cout << "Błąd otwarcia pliku \n";
	}
}

void UtworzPlik(const string nazwa, const int licznik, const char znak)
{
	fstream plik;
	plik.open(nazwa, ios::binary | ios::out);
	if (plik.good())
	{
		for (int i = 0; i < licznik; i++)
		{
			plik << znak;
		}
		plik.close();
		ZapiszLog("Utworzono plik binarny: " + nazwa);
	}
	else
	{
		cout << "Błąd otwarcia pliku \n";
	}
}

uint8_t HammingDistance(uint8_t n1, uint8_t n2)
{
	uint8_t x = n1 ^ n2; // XOR
	uint8_t setBits = 0;
	while (x > 0)
	{
		setBits += x & 1;
		x >>= 1;
	}
	return setBits;
}

