#include <iostream>
#include <fstream>

using namespace std;

int findMandelbrot(double cr, double ci, int max_iterations) {
	int i = 0;
	double zr = 0.0, zi = 0.0;
	
	while (i < max_iterations && zr * zr + zi * zi < 4.0) {
		double temp = zr * zr - zi * zi + cr;
		zi = 2.0 * zr * zi + ci;
		zr = temp;
		i++;
	}

	return i;
}

double mapToReal(int x, int imageWidth, double minR, double maxR) {
	double range = maxR - minR;
	// [0, szerokosc]
	// [0, maxR - minR] - val * range / width 
	// [minR, maxR] - ostatniaLinijka + minR
	return x * (range / imageWidth) + minR;
}

double mapToImaginary(int y, int imageHeight, double minI, double maxI) {
	double range = maxI - minI;
	//To samo co dla rzeczywistych
	return y * (range / imageHeight) + minI;
}

int main() {
	//Pobranie wartosci z pliku txt
	ifstream fin("input.txt");

	int imageWidth, imageHeight, maxN;
	double minR, maxR, minI, maxI;

	//Sprawdzenie
	if (!fin) {
		cout << "Nie mozna odczytac pliku!" << endl;
		cin.ignore();
		return 0;
	}

	//FileInput
	fin >> imageWidth >> imageHeight >> maxN;
	fin >> minR >> maxR >> minI >> maxI;
	fin.close(); //zamkniecie buforu

	//FileOutput
	ofstream fout("fraktal.ppm");
	fout << "P3" << endl; //Naglowek .ppm
	fout << imageWidth << " " << imageHeight << endl; //Rozmiar
	fout << "256" << endl; //Max wart. pikseli RGB

	//Dla kazdego pikselu
	for (int y = 0; y < imageHeight; y++) { //Wiersz

		for (int x = 0; x < imageWidth; x++) { //Piksele w wierszu

			//Oblicz rzeczywista i urojona wartosc dla c (wzor Mandelbrota)
			double cr = mapToReal(x, imageHeight, minR, maxR);
			double ci = mapToImaginary(y, imageHeight, minI, maxI);

			//Oblicz liczbe iteracji we wzorze Mandelbrota uzywajac wspomnianego c
			int n = findMandelbrot(cr, ci, maxN);

			//Mapowanie na kolory - można zmienić i pokombinować
			int r = ( (int) sin(n)/n % 256);
			int g = (  28  % 256);
			int b = ( (int) cos(n) * n  % 256);

			//Wyjscie do obrazka
			fout << r << " " << g << " " << b << " ";
		}
		cout << "\033[H" << y << " / " << imageHeight << " px" << " X " << imageWidth << " px";
		fout << endl;
	}

	fout.close();
	cout << endl << "Zakonczono program pomyslnie!" << endl;
	return 0;

}
