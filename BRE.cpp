// BER
//Grzegorz Kowalczyk, D3

#define _CRT_SECURE_NO_WARNINGS
#include "pch.h"

using namespace std;

struct ObliczeniaBer
{
	double tot; //Liczba wszystkich bitów
	double err; //Liczba różnych bitów
	float ber;  //Wynik obliczeń BER
	clock_t t1; //czas startu obliczeń
	clock_t t2; //czas zakończenia obliczeń
};
void ZapiszLog(const string);
void UtworzPlik(const string, const int, const char);
uint8_t HammingDistance(uint8_t, uint8_t);
ObliczeniaBer ObliczBer(string, string);
void ZapiszWyniki(ObliczeniaBer wynik);

int main(int argc, char* argv[])
{
	cout << " Kalkulator Bit Error Rate, v.1.0 \n";
	cout << "-----------------------------------\n";
	ZapiszLog("Start programu");

	string nazwapliku1;
	string nazwapliku2;
	ObliczeniaBer wynik;

	if (argc != 3)
	{
		int opcja = {};
		bool koniec = false;
		do
		{
			cout << "\n Program stworzy dwa pliki binarne do porownania i obliczenia BER \n"
				"[1] - dwa identyczne pliki, kazdy po 100 bajtow, zawierajace binarnie zapisana wartosc 0x55 \n"
				"[2] - dwa rozne pliki po 100B, pierwszy z binarnie zapisana wartoscia 0x55, drugi z 0x50 \n"
				"[3] - dwa rozne po 400MB, pierwszy z binarnie zapisana wartoscia 0x55, drugi z 0x50 \n";
			cout << "\n Wybierz opcje do testu: [1], [2], albo [3]: ";
			cin >> opcja;
			cout << endl;
			//ZapiszLog("Nie podano plików do porównania, program tworzy je sam.");
			switch (opcja)
			{
			case 1:
			{
				UtworzPlik("plik1.bin", 100, 0x55);
				UtworzPlik("plik2.bin", 100, 0x55);
				ZapiszLog("Program stworzyl dwa identyczne pliki, kazdy po 100 bajtow, \n"
					"zawierajace binarnie zapisana wartosc 0x55");
				koniec = true;
			} break;
			case 2:
			{
				UtworzPlik("plik1.bin", 100, 0x55);
				UtworzPlik("plik2.bin", 100, 0x50);
				ZapiszLog("Program stworzyl dwa rozne pliki, kazdy po 100B, \n"
					"pierwszy z binarnie zapisana wartoscia 0x55, drugi z 0x50");
				koniec = true;
			} break;
			case 3:
			{
				UtworzPlik("plik1.bin", 4 * 1024 * 1024, 0x55);
				UtworzPlik("plik2.bin", 4 * 1024 * 1024, 0x50);
				//UtworzPlik("plik1.bin", 400 * 1024 * 1024, 0x55);
				//UtworzPlik("plik2.bin", 400 * 1024 * 1024, 0x50);
				ZapiszLog("Program stworzyl dwa rozne pliki, kazdy po 400MB, \n"
					"pierwszy z binarnie zapisana wartoscia 0x55, drugi z 0x50");
				koniec = true;
			} break;
			default:
				cout << "Podano zla wartosc \n";
				system("pause");
				system("cls");
				cin.clear();
				cin.ignore(numeric_limits<streamsize>::max(), '\n');
			}
		} while (!koniec);

		cout << "\n Uruchom program jeszcze raz z argumentami: ./BRE2test.exe plik1.bin plik2.bin\n\n";
		system("pause");
		ZapiszLog("Zamknięcie programu\n");
		system("cls");
	}
	else
	{
		nazwapliku1 = argv[1];
		nazwapliku2 = argv[2];
		ZapiszLog("Pobranie dwoch plikow do obliczen");
		wynik = ObliczBer(nazwapliku1, nazwapliku2);
		ZapiszWyniki(wynik);
		ZapiszLog("Koniec programu\n");
	}
	return 0;
}

void ZapiszLog(const string wiadomosc)
{
	time_t teraz = time(NULL);
	string dt = ctime(&teraz);
	cout << dt.substr(0, dt.length() - 1) << ": " << wiadomosc << endl;
	fstream plikLog;
	plikLog.open("Log.txt", ios::app);
	if (plikLog.good())
	{
		plikLog << dt.substr(0, dt.length() - 1) << ": " << wiadomosc << endl;
		plikLog.close();
	}
	else
	{
		cerr << "Blad otwarcia pliku log\n";
	}
}
void UtworzPlik(const string nazwa, const int licznik, const char znak)
{
	ZapiszLog("Tworzenie pliku binarnego: " + nazwa);
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
		//cerr << "Blad otwarcia pliku testowego \n";
		ZapiszLog("Blad otwarcia pliku testowego");
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
			wynik.err += HammingDistance(a, b);
			wynik.tot += 8;
		}
	}
	wynik.ber = (float)wynik.err / wynik.tot;
	wynik.t2 = clock();
	ZapiszLog("Zakonczenie obliczen");
	return wynik;
}
void ZapiszWyniki(ObliczeniaBer wynik)
{
	stringstream message;
	message << "Podanie wynikow" << endl;
	message << "   Bitowa stopa bledu    - BER: " << wynik.ber << endl;
	message << "   Liczba wszystkich bitow - Tot: " << wynik.tot << endl;
	message << "   Liczba wszystkich bitow - Err: " << wynik.err << endl;
	message << "   Czas obliczeń: " << ((float)wynik.t2 - wynik.t1) / CLOCKS_PER_SEC << " sec ";
	ZapiszLog(message.str());
}
