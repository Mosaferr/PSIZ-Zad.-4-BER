// BRE
//Grzegorz Kowalczyk, D3

#include <iostream>
#include <fstream>

using namespace std;

void UtworzPlik(const string, const int, const char);

int main(int argc, char* argv[])
{
	cout << "Kalkulator BRE, v.1.0" << endl;
	UtworzPlik("plik1.bin", 100, 0x55);
	UtworzPlik("plik2.bin", 100, 0x45);
	return 0;
}

void UtworzPlik(const string nazwa, const int licznik, const char znak)
{
	ifstream plik;
	plik.open(nazwa);
	if (!plik)
	{
		ofstream plik(nazwa);
			for (int i = 0; i < licznik; ++i)
			{
				plik << znak;
			}
	}
}