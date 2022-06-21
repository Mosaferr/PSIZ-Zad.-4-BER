// BRE
//Grzegorz Kowalczyk, D3

#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <time.h>

using namespace std;

void ZapiszLog(const string);
void UtworzPlik(const string, const int, const char);

int main(int argc, char* argv[])
{
	cout << "Kalkulator BRE, v.1.0 \n";
	ZapiszLog("Start programu");
	UtworzPlik("plik1.bin", 100, 0x55);
	UtworzPlik("plik2.bin", 100, 0x45);
	ZapiszLog("Koniec programu");
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
	}
	else
	{
		cout << "Błąd otwarcia pliku \n";
	}
}