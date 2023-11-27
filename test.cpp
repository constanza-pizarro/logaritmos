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
#include "a.h" // momentaneo

using namespace chrono;

// Número de consultas
#define L 100
#define U 50000000

int main() {

    vector<double> averageTime_per_n_dpr(U); // dividir para reinar
    vector<int> average_k_per_n_a(U); // aleatorizado

    // Trabajaremos con n ∈ [5000000, ..., 50000000]
    for (int n=5000000; n<=U; n+=5000000) {

        vector<double> times_dpr(L);
        vector<double> times_a(L);

        // Repeticiones de los algoritmos
        for (int i=0; i<=L; i++) { // > 100

            // arreglo 
            vector<Point> arr = generateRandomPoints(n);

            cout << "-------------------------------- Dividir para reinar --------------------------------" << endl;
            auto begin_dpr = high_resolution_clock::now();
            // divide_and_conquer(arr); // recibe como parametro arr
            auto end_dpr = high_resolution_clock::now();

            double time = duration_cast<nanoseconds>(end_dpr - begin_dpr).count() / 1e9;
            times_dpr[i] = time;
            
            
            cout << "-------------------------------- Aleatorizado --------------------------------" << endl;
            auto begin_a = high_resolution_clock::now();
            findMinimumDistanceOptimized(arr);
            auto end_a = high_resolution_clock::now();

            double time = duration_cast<nanoseconds>(end_a - begin_a).count() / 1e9;
            times_a[i] = time;

        }

        // Calcula el tiempo promedio de ejecución y lo imprime
        double averageTime_dpr = accumulate(times_dpr.begin(), times_dpr.end(), 0.0) / L;
        cout << "n: " << n << "  promedio Dividir para reinar: " << averageTime_dpr << endl;
        averageTime_per_n_dpr[n-1] = averageTime_dpr;

        // Calcula el tiempo promedio de ejecución y lo imprime
        double averageTime_a = accumulate(times_a.begin(), times_a.end(), 0.0) / L;
        cout << "n: " << n << "  promedio Aleatorizado: " << averageTime_a << endl;
        average_k_per_n_a[n-1] = averageTime_a;

    }

    return 0;

}