#include "../aleatory_working.h"
#include <chrono>
#include <random>
#include <iostream>
#include <fstream>
#include <unistd.h>  // Para getpid()

using namespace std;
using namespace std::chrono;

int main() {
    unsigned seed = static_cast<unsigned>(high_resolution_clock::now().time_since_epoch().count()) +
                    static_cast<unsigned>(getpid());
    mt19937 gen(seed);

    cout << "Semilla utilizada: " << seed << endl;

    ofstream archivo("file.txt");

    for (int n = 50000; n <= 50000000; n = n + 5000) {
        cout << "N =" << n << endl;
        archivo << "N =" << n << std::endl;

        double duration_total2 = 0;
        double duration_total3 = 0;
        double duration_total4 = 0;

        for (int intentos = 1; intentos <= 3; intentos++) {
            vector<Point> randomPoints = generateRandomPoints(n, gen);
            vector<Point> randomPointsCpy = randomPoints;
            
            cout << "-------------------------------- Hashing generico --------------------------------" << endl;
            archivo << "Hashing method = Native" << endl;
            auto start3 = high_resolution_clock::now();
            double result3 = findMinDistanceOptimized(randomPointsCpy);
            auto stop3 = high_resolution_clock::now();
            auto duration3 = duration_cast<milliseconds>(stop3 - start3);
            archivo << "duration " << duration3.count() << "ms" << std::endl;
            duration_total3 += duration3.count();
            cout << "distancia = " << result3 << endl;

            cout << "-------------------------------- Hashing Mersenne --------------------------------" << endl;
            archivo << "Hashing method = Mersenne Hashing" << endl;
            auto start2 = high_resolution_clock::now();
            double result2 = findMinDistanceOptimizedMersenne(randomPoints);
            auto stop2 = high_resolution_clock::now();
            auto duration2 = duration_cast<milliseconds>(stop2 - start2);
            archivo << "duration " << duration2.count() << "ms" << std::endl;
            duration_total2 += duration2.count();
            cout << "distancia = " << result2 << endl;

            cout << "-------------------------------- Hashing Universal --------------------------------" << endl;
            uniform_int_distribution<int> dist_a(1, (1 << 31) - 2);
            uniform_int_distribution<int> dist_b(0, (1 << 31) - 1);
            int a = dist_a(gen);
            int b = dist_b(gen);
            int m = 1 << 12;
            archivo << "Hashing method = Universal Hashing" << endl;
            auto start4 = high_resolution_clock::now();
            double result4 = findMinDistanceOptimizedUniversal(randomPoints, a, b, (1 << 31) - 1, m);
            auto stop4 = high_resolution_clock::now();
            auto duration4 = duration_cast<milliseconds>(stop4 - start4);
            archivo << "duration " << duration4.count() << "ms" << std::endl;
            duration_total4 += duration4.count();
            cout << "distancia = " << result4 << endl;

            cout << "Intento numero " << intentos << " terminado" << endl;
            archivo << "Intento numero " << intentos << " terminado" << endl;
            archivo << "\n" << endl;
        }

        archivo << "Results for N=" << n << endl;
        archivo << "duration mean Mersenne " << duration_total2 / 3 << " ms" << endl;
        archivo << "duration mean Universal " << duration_total4 / 3 << " ms" << endl;
        archivo << "duration mean Native " << duration_total3 / 3 << " ms" << endl;
        archivo << "----------------------------------------------" << endl;
    
        //gen.seed(static_cast<unsigned>(high_resolution_clock::now().time_since_epoch().count()) + static_cast<unsigned>(getpid()));
    }

    archivo.close();
    return 0;
}
