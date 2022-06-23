// BER
//Grzegorz Kowalczyk, D3

#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"

using namespace std;

struct ObliczeniaBer
{
	double tot; //Liczba wszystkich bitów
	double err; //Błędne bity (różne)
	float ber;  //Wynik obliczeń BER
	clock_t t1; //czas startu obliczeń
	clock_t t2; //czas zakończenia obliczeń
};

ObliczeniaBer ObliczBer(string, string);
void ZapiszWyniki(ObliczeniaBer wynik);

void ZapiszLog(const string);
void UtworzPlik(const string, const int, const char);
uint8_t HammingDistance(uint8_t, uint8_t);

int main(int argc, char* argv[])
{
	cout << "Kalkulator Bit Error Rate, v.1.0 \n";
	ZapiszLog("Start programu");

	string nazwapliku1;
	string nazwapliku2;
	ObliczeniaBer wynik;

	if (argc != 3)
	{
		ZapiszLog("Nie podano plików do porównania, program tworzy je sam.");
		UtworzPlik("plik1.bin", 100000, 0x55);
		UtworzPlik("plik2.bin", 100000, 0x45);
		ZapiszLog("Utworzono dwa pliki binarne");
		ZapiszLog("Uruchom jeszcze raz z argumentami: ./BRE.exe plik1.bin plik2.bin\n");
	}
	else
	{
		nazwapliku1 = argv[1];
		nazwapliku2 = argv[2];
		ZapiszLog("Pobranie dwóch plików do obliczeń");
		wynik = ObliczBer(nazwapliku1, nazwapliku2);
		ZapiszWyniki(wynik);
		ZapiszLog("Koniec programu\n");
	}
	return 0;
}

void ZapiszLog(const string wiadomosc)
{
	fstream plikLog;
	plikLog.open("Log.log", ios::app);
	if (plikLog.good())
	{
		time_t teraz = time(NULL);
		string dt = ctime(&teraz);
		plikLog << dt.substr(0, dt.length() - 1) << ": " << wiadomosc << endl;
		plikLog.close();
	}
	else
	{
		cerr << "Błąd otwarcia pliku log\n";
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
		cerr << "Błąd otwarcia testowego \n";
		ZapiszLog("Błąd otwarcia pliku testowego");
	}
}
uint8_t HammingDistance(uint8_t n1, uint8_t n2)
{
	uint8_t x = n1 ^ n2;
	uint8_t setBits = 0;
	while (x > 0)
	{
		setBits += x & 1;
		x >>= 1;
	}
	return setBits;
}
ObliczeniaBer ObliczBer(string nazwapliku1, string nazwapliku2)
{
	fstream f1, f2;
	ObliczeniaBer wynik;
	wynik.t1 = 0;
	wynik.t2 = 0;
	wynik.ber = 0;
	wynik.err = 0;
	wynik.tot = 0;

	ZapiszLog("Obliczanie BER...");
	f1.open(nazwapliku1, ios::binary | ios::in);
	f2.open(nazwapliku2, ios::binary | ios::in);
	char a = 0x00;
	char b = 0x00;
	wynik.t1 = clock();

	while (!f1.eof())
	{
		f1 >> a;
		f2 >> b;
		if (!f1.eof())
		{
			wynik.err += HammingDistance(a, b); //add to the .err the number of different bits
			wynik.tot += 8; //add to the .tot the number of compared bits
		}
	}
	wynik.ber = (float)wynik.err / wynik.tot; // calculate ber
	wynik.t2 = clock();
	ZapiszLog("Zakończenie obliczeń");
	return wynik;
}
void ZapiszWyniki(ObliczeniaBer wynik)
{
	stringstream message;
	message << "Podanie wyników" << endl;
	message << "   Wynik obliczeń błędu    - BER: " << wynik.ber << endl;
	message << "   Liczba wszystkich bitów - Tot: " << wynik.tot << endl;
	message << "   Błędne bity (różne)     - Err: " << wynik.err << endl;
	message << "   Czas obliczeń: " << ((float)wynik.t2 - wynik.t1) / CLOCKS_PER_SEC << " sec ";
	ZapiszLog(message.str());
}
