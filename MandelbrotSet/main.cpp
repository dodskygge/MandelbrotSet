#include <iostream>
#include <fstream>

//using namespace std;

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
	std::ifstream fin("input.txt");

	int imageWidth, imageHeight, maxN;
	double minR, maxR, minI, maxI;
	std::string fileName;

	//Sprawdzenie
	if (!fin) {
		std::cout << "Nie mozna odczytac pliku!" << std::endl;
		std::cin.ignore();
		return 0;
	}

	//FileInput
	fin >> imageWidth >> imageHeight >> maxN;
	fin >> minR >> maxR >> minI >> maxI;
	fin >> fileName;
	fin.close();

	//Sprawdzenie czy plik istnieje, jesli tak to zmiana nazwy
	if (std::ifstream(fileName).good()) {
		fileName = fileName.substr(0, fileName.size() - 4) + "_nowy.ppm";
	}

	//FileOutput
	std::ofstream fout(fileName);
	fout << "P3" << std::endl; //Naglowek .ppm
	fout << imageWidth << " " << imageHeight << std::endl; //Rozmiar
	fout << "256" << std::endl; //Max wart. pikseli RGB

	//Dla kazdego pikselu
	for (int y = 0; y < imageHeight; y++) { //Wiersz

		for (int x = 0; x < imageWidth; x++) { //Piksele w wierszu

			//Oblicz rzeczywista i urojona wartosc dla c (wzor Mandelbrota)
			double cr = mapToReal(x, imageHeight, minR, maxR);
			double ci = mapToImaginary(y, imageHeight, minI, maxI);

			//Oblicz liczbe iteracji we wzorze Mandelbrota uzywajac wspomnianego c
			int n = findMandelbrot(cr, ci, maxN);

			//Mapowanie na kolory - mo¿na zmieniæ i pokombinowaæ
			//int r = ( (int) sin(n)*3 % 256);
			//int g = ( (int) n/2  % 256 );
			//int b = ( (int) cos(n)* n  % 256);
			// 
			int r = ( n*n % 256);
			int g = ( 28 % 256);
			int b = ( n % 256);
			// 
			//int r = ((int)sin(n) * n % 256);
			//int g = ((int)n * 2 % 256);
			//int b = ((int)cos(n) % 256);
			//
			//int r = ((int)n * n % 256);
			//int g = (n * n % 256);
			//int b = ((n * n) % 256);
			// 
			//int r = ((int)n * n % 256);
			//int g = (n * n % 256);
			//int b = ((n * n) % 256);

			//Wyjscie do obrazka
			fout << r << " " << g << " " << b << " ";
		}
		std::cout << "\033[H" << y << " / " << imageHeight << " px" << " X " << imageWidth << " px";
		fout << std::endl;
	}

	fout.close();
	std::cout << std::endl << "Zakonczono program pomyslnie!" << std::endl;
	return 0;
}