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
#include "dump.h"

using namespace chrono;

// Número de consultas
#define L 101
#define U 50000000

int main() {

    vector<double> averageTime_per_n_dpr(U); // dividir para reinar
    vector<int> average_k_per_n_a(U); // aleatorizado
    

    // Utilizar el tiempo actual como semilla para mt19937
    //unsigned seed = static_cast<unsigned>(chrono::high_resolution_clock::now().time_since_epoch().count());
    //mt19937 randomGenerator(seed);
    // Trabajaremos con n ∈ [5000000, ..., 50000000]
    for (int n=100/*5000000*/; n<=U; n+=100/*5000000*/) {

        vector<double> times_dpr(L);
        vector<double> times_a(L);

        // Utilizar el tiempo actual como semilla para mt19937
        unsigned seed = static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
        mt19937 randomGenerator(seed);
        
        vector<Point> arr = generateRandomPoints(n, randomGenerator);
        vector<Point> arr2(arr.size());

        copy(arr.begin(), arr.end(), arr2.begin());

        // Repeticiones de los algoritmos
        for (int i=0; i<L; i++) { // > 100
            
            cout << "Consulta numero " << i+1 << endl;


            cout << "-------------------------------- Dividir para reinar --------------------------------" << endl;
            auto begin_dpr = high_resolution_clock::now();
            double res = divide_and_conquer(arr2);
            auto end_dpr = high_resolution_clock::now();

            cout << res << endl;
            double time1 = duration_cast<milliseconds>(end_dpr - begin_dpr).count();
            times_dpr[i] = time1;
            
            cout << "-------------------------------- Aleatorizado --------------------------------" << endl;
            auto begin_a = high_resolution_clock::now();
            double res2 = findMinDistanceOptimized(arr);
            auto end_a = high_resolution_clock::now();
            cout << res2 << endl;
            double time2 = duration_cast<milliseconds>(end_a - begin_a).count();
            times_a[i] = time2;
    
            cout << endl;

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