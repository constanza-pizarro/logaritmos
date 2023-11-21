#include <stdio.h>
#include <chrono>
#include <thread>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <random> // 
#include <chrono>
#include <ctime> // for clock_t, clock(), CLOCKS_PER_SEC
#include <cmath> // for std::abs
#include "aleatorizado.h"
#include "div_and_con.h"

using namespace chrono;

// Número de consultas
#define L 100
#define U 50000000

/*
Para probar los algoritmos, debera entregarle un set de tamano n de puntos pertenecientes al rango
[0, 1) x [0, 1). Pruebe con n de 5 a 50 millones con un paso de 5 millones. Para cada arreglo de
tamano n, debera ejecutar cada algoritmo una cantidad considerable de veces (> 100).
Luego, debera generar un histograma con los tiempos de ejecucion de cada algoritmo. Debe utilizar
el mismo arreglo en las repeticiones y en ambos algoritmos para poder compararlos. Tambien, deben
graficar como crecen las medias en funcion del tamano del input. Por ultimo, analice el tiempo
promedio para varios inputs aleatorios y muestre que el costo de tiempo en sus implementaciones
se asemejan a lo conocido por la teoria
*/

int main() {

    // Trabajaremos con u ∈ [5000000, ..., 50000000]
    for (int n=5000000; n<=U; n+=5000000) {

        // Repeticiones de los algoritmos
        for (int i=0; i<L; i++) {

        }

    }

    return 0;
}