// FSP.cpp : Ten plik zawiera funkcję „main”. W nim rozpoczyna się i kończy wykonywanie programu.
//

#include "pch.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <vector>
#include <chrono>
using namespace std;


int koszt1;
int koszt2;
int koszt3;
int czas;
int czas2;
int liczba;
int maszyny;
int UP = 10000;
int LP;
vector<vector<int>> P;
vector<vector<int>> Z;
vector<vector<int>> Pomoc;
vector<vector<int>> tab;


/*********************************************************************
Struktura wczytanej tablicy:
lp || czas na 1 maszynie || czas na 2 maszynie
Pamietac o tym ze przez dodanie liczby pozycyjnej indeksy przesuwaja sie o jeden!


**************************************************************************/


void Wczytaj(string Nazwapliku) {
	string linia;
	fstream plik;

	plik.open(Nazwapliku, ios::in);
	if (plik.good() == true)
	{
		plik >> liczba;
		plik >> maszyny;
		string pomoc1, pomoc2;

		for (int i = 0; i < liczba; i++)
		{
			int tmp;

			vector<int> v1;
			v1.push_back(i); // Wrzucamy jaka to kolejnosc
			for (int j = 0; j < maszyny; j++)
			{
				plik >> pomoc1 >> tmp;
				v1.push_back(tmp);

			}
			tab.push_back(v1);

		}
		plik.close();
	}
}

int Oblicz(vector<vector<int>> tabela, int liczba) {
	int koszt = 0;
	int zak = 0;
	int start = 0;
	vector<vector<int>> P(maszyny, vector<int>(liczba, 0));

	for (int i = 0; i < maszyny; ++i)
	{

		for (int j = 0; j < liczba; ++j) {

			if (i == 0)
			{
				start = zak;
				zak = start + tabela[j][i + 1];

			}
			else
			{
				if (j == 0)
				{
					start = P[i - 1][j];
				}
				else
				{
					start = max(P[i - 1][j], zak);
				}
				zak = start + tabela[j][i + 1];
			}

			P[i][j] = zak;
		}

		//P[0][0];
	}
	return P[maszyny - 1][liczba - 1];

}

int Oblicz2(vector<vector<int>> tabela, int liczba) {
	int koszt = 0;
	for (int i = 0; i < liczba; i++)
	{
		int tmp = tabela[0][1];
		vector<int> v1;
		vector<int> v2;
		for (int j = 0; j < maszyny; j++) {
			// Zeby przsunac indeks przez dodanie liczby pozycyjnej
			int przesuwacz = j + 1;
			// Tam gdzie jest odwolanie do poprzedniego wiersza tablicy P lub Z
			// nie musimy przesuwac bo to jest inna tablica

			if (i > 0) {
				v1.push_back(P[i - 1][j] + tabela[i - 1][przesuwacz]);
				v2.push_back(Z[i - 1][j] + tabela[i][przesuwacz]);
			}
			else if (i == 0 && j == 0) {
				v1.push_back(0);
				v2.push_back(tmp);
			}
			else if (i == 0 && j > 0) {
				v1.push_back(tabela[i][j]);
				tmp = tmp + tabela[i][przesuwacz];
				v2.push_back(tmp);
			}
		}
		P.push_back(v1);
		Z.push_back(v2);

	}
	koszt = Z[liczba - 1][maszyny - 1];
	return koszt;
}
void BruteForce(vector<vector<int>>& tabela, int poz = 0) {

	if (poz == liczba) {
		czas = Oblicz(tabela, liczba);

		koszt2 = min(koszt2, czas);

	}
	else {
		for (int i = poz; i < liczba; i++) {
			for (int j = 0; j < maszyny; j++) {
				swap(tabela[i][j], tabela[poz][j]);
			}
			BruteForce(tabela, poz + 1);
			for (int j = 0; j < maszyny; j++) {
				swap(tabela[i][j], tabela[poz][j]);
			}
		}
	}


}





int BruteForce_Iteracja(vector< vector<int> > zadania, vector<int>& PI_permutacja)
{
	int koszt = INT_MAX;
	int iloscZadan = liczba;
	int iloscMaszyn = maszyny;

	sort(zadania.begin(), zadania.end());
	zadania.begin();
	do
	{
		for (int i = 0; i < iloscMaszyn; ++i)
		{
			int koszt_temp = Oblicz(zadania, liczba);
			if (koszt_temp < koszt)
			{
				koszt = koszt_temp;
				PI_permutacja.clear();
				for (int i = 0; i < iloscZadan; ++i)
				{
					PI_permutacja.push_back(zadania[i][0]);
				}
			}
		}
	} while (next_permutation(zadania.begin(), zadania.end()));
	return koszt;
}


void Branch(vector<vector<int>>& tabela, int poz = 0) {
	int Suma = 0;
	if (poz == liczba) {
		czas2 = Oblicz(tabela, liczba);
		if (czas2 <= UP) { UP = czas2; }
		koszt3 = min(koszt3, czas2);

	}
	else {
		int obliczona = 0;
		for (int i = poz; i < liczba; i++) {
			Suma = 0;
			for (int j = 0; j < maszyny; j++) {
				swap(tabela[i][j], tabela[poz][j]);
			}
			//////////////////////////////////
			for (int k = 0; k < i + 1; k++) {
				vector<int> v3;

				for (int l = 0; l < maszyny + 1; l++) {
					v3.push_back(tabela[k][l]);
				}
				Pomoc.push_back(v3);
			}
			if (i > 0) { obliczona = Oblicz(Pomoc, i); }
			for (int m = i; m < tabela.size(); m++) { Suma = Suma + tabela[m][1] + tabela[m][2]; }
		
			LP = Suma + obliczona;

			////////////////////////////
			if (LP <= UP) {

				Branch(tabela, poz + 1);
			}
			for (int j = 0; j < maszyny; j++) {
				swap(tabela[i][j], tabela[poz][j]);
			}
		}
	}


}




vector<std::vector<int>> Johnson(vector<vector<int>>& tabela)
{
	int l = 0;
	int k = liczba - 1;
	int minP = 10000;//duza liczba

	std::vector<std::vector<int> > wynik(
		(k + 1),
		std::vector<int>(2));

	int minN = 0;

	while (tabela.size() != 0)
	{
		for (int i = 0; i < tabela.size(); i++)
		{
			for (int j = 0; j < maszyny; j++)
			{
				minP = min(tabela[i][j], minP);
				if (minP == tabela[i][j])
				{
					minN = i;
				}
			}
		}
		if (tabela[minN][0] < tabela[minN][1]) ///tabela 8:2
		{
			wynik[l] = tabela[minN];
			l += 1;
			minP = 100000;
		}
		else
		{
			wynik[k] = tabela[minN];
			k -= 1;
			minP = 100000;
		}
		tabela.erase(tabela.begin() + minN);
		//tabela.erase(tabela[minN]);
	}
	return wynik;

}

int FunkcjaCeluJohnson(vector<vector<int>> tabela)
{
	int koszt = 0;
	for (int i = 0; i < liczba; i++)
	{
		int tmp = tabela[0][0]; //koniec pierwszego
		vector<int> v1; //poczatek
		vector<int> v2; //koniec
		for (int j = 0; j < maszyny; j++) {
			// Zeby przsunac indeks przez dodanie liczby pozycyjnej
			// Tam gdzie jest odwolanie do poprzedniego wiersza tablicy P lub Z
			// nie musimy przesuwac bo to jest inna tablica

			if (i > 0) {
				v1.push_back(P[i - 1][j] + tabela[i - 1][j]);
				v2.push_back(Z[i - 1][j] + tabela[i][j]);
			}
			else if (i == 0 && j == 0) {
				v1.push_back(0);
				v2.push_back(tmp);
			}
			else if (i == 0 && j > 0) {
				v1.push_back(tabela[i][j]);
				tmp = tmp + tabela[i][j];
				v2.push_back(tmp);
			}
		}
		P.push_back(v1);
		Z.push_back(v2);

	}
	koszt = Z[liczba - 1][maszyny - 1];

	return koszt;

}





int main()
{



	vector<int> PI_permutacja;


	Wczytaj("data006.txt");
	std::vector<std::vector<int> > john(2, std::vector<int>(liczba));
	int wybor;
	cout << "co robić? 1- brute force-it, 2- Branch and Bound, 3- bruteforce-rek, 4- johnson" << endl;
	cin >> wybor;
	if (wybor == 1) {

		auto start2 = std::chrono::system_clock::now();
		koszt1 = Oblicz(tab, liczba);
		cout << "Koszt bez zmiany:  " << koszt1 << endl;
		
		 koszt2 = BruteForce_Iteracja(tab, PI_permutacja);
		cout << "Koszt BF:  " << koszt2 << endl;
		auto end2 = std::chrono::system_clock::now();
		std::chrono::duration<double> cz2 = end2 - start2;
		cout << "Czas wykonywania Bruteforca iteracyjnie " << cz2.count() << endl;

	}
	else if (wybor == 2) {
		koszt1 = Oblicz(tab, liczba);
		cout << "Koszt bez zmiany:  " << koszt1 << endl;
		koszt3 = 2000;
		auto start2 = std::chrono::system_clock::now();
		Branch(tab, 0);
		cout << "Koszt po branchu: " << koszt3 << endl;
		auto end2 = std::chrono::system_clock::now();
		std::chrono::duration<double> cz2 = end2 - start2;
		cout << "Czas wykonywania Brancha " << cz2.count() << endl;
	}
	else if (wybor == 3) {
		auto start2 = std::chrono::system_clock::now();
		koszt1 = Oblicz(tab, liczba);
		cout << "Koszt bez zmiany:  " << koszt1 << endl;
		koszt2 = 1000;
		BruteForce(tab, 0);
		cout << "Koszt BF:  " << koszt2 << endl;
		auto end2 = std::chrono::system_clock::now();
		std::chrono::duration<double> cz2 = end2 - start2;
		cout << "Czas wykonywania Bruteforca rekurencyjnie " << cz2.count() << endl;

	}
	else {

		for (int i = 0; i < tab.size(); i++)
			tab[i].erase(tab[i].begin());	//usuwamy liczbe porzadkowa zadan
		auto start2 = std::chrono::system_clock::now();
		john = Johnson(tab);
		koszt2 = FunkcjaCeluJohnson(john);
		auto end2 = std::chrono::system_clock::now();
		std::chrono::duration<double> cz2 = end2 - start2;
		cout << "Czas wykonywania Johnsona " << cz2.count() << endl;
		for (int i = 0; i < john.size(); i++)
			for (int j = 0; j < john[1].size(); j++)
			{
				cout << john[i][j] << ", ";
				if (j == 1)
					cout << endl;
			}

		cout << koszt2;




	}








}

