#include <iostream>
#include <bits/stdc++.h>
#include <ctime>
#include <math.h>
#include <algorithm>
#include <vector>
#include <cstdio>

using namespace std;
#define MAX_POPULATION 40
#define GENERATIONS 25
/*
1. Definir una representaci?n a ser usada para cada individuo de
manera que una solucion completa puede ser representada BITS

2. Definir las estrategias de substitucion, seleccion, cruzamiento y mutacion
ELEGIR LOS PADRES: Valor actual
CROSSOVER: One Point Crossover
MUTATION: Flip mutation
3. Definir la funcion de aptitud
EJECUTAR LA FUNCION f(x,y) = x^2 - 2xy + y^2
X: 0 Y: 255
MAX = 65025

4. Ajustar los siguientes parametros:
Tamanho de la poblacion: 40 individuos
Probabilidad de cruzamiento: 1.0
Probabilidad de mutacion: 0.1
Numero de generaciones: 25
*/

using namespace std;

struct Genoma {
	bitset<7> valorx;
	bitset<8> valory;
	float aptitud;

	void mutation () {
		int xChanged = 0, yChanged = 0;
		xChanged = rand () % 7;
		yChanged = rand () % 8;

		(valorx[xChanged] == 0) ? valorx.set (xChanged, 1) : valorx.set (xChanged, 0);
		(valory[yChanged] == 0) ? valory.set (yChanged, 1) : valory.set (yChanged, 0);
	}
};

int fitnessFunction (vector<Genoma> &gen);
bool compare_nocase (Genoma gen1, Genoma gen2);
void crossover (vector<Genoma> &gen);
vector<Genoma> proportionSelection (vector<Genoma> gen);
int showBestCase (vector<Genoma> gen);
int showPromCase (vector<Genoma> gen);

int main () {
	srand (time (NULL));
	int bitsX = 7, bitsY = 8;

	/// generate population
	//creando poblacion de X y Y
	vector<Genoma> poblacion (MAX_POPULATION);
	for ( int p = 0; p < MAX_POPULATION; ++p ) {
		for ( int i = 0; i < bitsX; ++i )
			poblacion[p].valorx[i] = rand () % 2;

		for ( int i = 0; i < bitsY; ++i )
			poblacion[p].valory[i] = rand () % 2;
	}
	int generacion = 0;
	/// loop


	FILE* pipe = _popen("C:/gnuplot/bin/gnuplot.exe", "w");
	if (pipe != NULL)
	{
		fprintf(pipe, "set term win\n");
		fprintf(pipe, "array A[100]\n");
		fprintf(pipe, "array B[100]\n");
        fprintf(pipe, "array C[100]\n");
		do {
		fitnessFunction (poblacion);

		/// Parent selection
		vector<Genoma> newPob = proportionSelection (poblacion);
		/// crossover
		crossover (newPob);
		/// Mutation
		//El 10% mutar?
		for ( int i = 0; i < MAX_POPULATION * 0.1; i++ )
			newPob[rand () % MAX_POPULATION].mutation ();
		cout << "Generacion " << generacion << ": " << showBestCase (poblacion) << endl;
        fprintf(pipe, "A[%d] = %d\n",generacion+1,generacion);
		fprintf(pipe, "B[%d] = %d\n",generacion+1,showBestCase (poblacion));
		fprintf(pipe, "C[%d] = %d\n",generacion+1,showPromCase (poblacion));

		poblacion.clear ();
		poblacion = newPob;
		/// aumentar generacion
		++generacion;
	} while ( generacion <= GENERATIONS ); /// termination condition
	cout << "Margen de error: " << ((float)showBestCase (poblacion) / 65025 * 100) -100 << "%" << endl;



		fprintf(pipe, "plot sample [i=1:100] '+' using (A[i]):(B[i]) with linespoints,\\\n");
        fprintf(pipe, "[i=1:100] '+' using (A[i]):(C[i]) with linespoints\n");
		fprintf(pipe, "set term pngcairo\n");
		fprintf(pipe, "set output \"myFile.png\"\n");
		fprintf(pipe, "replot\n");
		fprintf(pipe, "set term win\n");
		fflush(pipe);
		while(1){

		}
	}
	else puts("Could not open the file\n");
	_pclose(pipe);

	return 0;
}

int showBestCase (vector<Genoma> gen) {
	int index = 0;
	for ( int i = 1; i < MAX_POPULATION; ++i ) {
		if ( gen[index].aptitud < gen[i].aptitud )
			index = i;
	}
	return gen[index].aptitud;
}

int showPromCase (vector<Genoma> gen) {
	long index = 0;
	for ( int i = 1; i < MAX_POPULATION; ++i ) {
		index+=gen[i].aptitud;
	}
	return index/MAX_POPULATION;
}

vector<Genoma> proportionSelection (vector<Genoma> gen) {
	vector<Genoma> genNuevo;
	int s = fitnessFunction (gen);
	int r = rand () % s;
	int apuntar = 0;
	for ( int i = 0; i < MAX_POPULATION; i++ ) {
		while ( apuntar < r ) {
			apuntar += gen[i].aptitud;
		}
		genNuevo.push_back (gen[i]);
		apuntar = 0;
		r = rand () % s;
	}
	return genNuevo;
}

/// metodo: cruzamiento en un punto
void crossover (vector<Genoma> &gen) {
	for ( int index = 0; index < MAX_POPULATION; index += 2 ) {
		int xIndex = rand () % 7;
		int yIndex = rand () % 8;
		for ( int i = xIndex; i < 7; ++i ) {
			bitset<1> bit (gen[index + 1].valorx[i]);
			gen[index].valorx[i] = gen[index + 1].valorx[i];
			gen[index].valorx[i] = bit[0];
		}

		for ( int i = yIndex; i < 8; ++i ) {
			bitset<1> bit (gen[index + 1].valorx[i]);
			gen[index].valorx[i] = gen[index + 1].valorx[i];
			gen[index].valorx[i] = bit[0];
		}
	}
}

bool compare_nocase (Genoma gen1, Genoma gen2) {
	return (gen1.aptitud > gen2.aptitud);
}

/// compute fitness of all genomes
int fitnessFunction (vector<Genoma> &gen) {
	int x, y, total = 0;
	for ( int i = 0; i < MAX_POPULATION; ++i ) {
		x = (int)gen[i].valorx.to_ulong ();
		y = (int)gen[i].valory.to_ulong ();
		total += gen[i].aptitud = pow (x, 2) - (2 * x * y) + pow (y, 2);
	}

	for ( int i = 0; i < MAX_POPULATION; i++ )
		gen.push_back (gen[i]);

	sort (gen.begin (), gen.end (), compare_nocase);
	// gen.sort (compare_nocase);

	//list<Genoma>::iterator it;
	return total;
}

