#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include "C:\Users\Vicente\Desktop\FCFM\Algoritmos\T3\logaritmos\aleatory_working.cpp"

// Definir tus funciones generateRandomPoints() y findMinDistanceOptimized()
// Asumiendo que tienes tus funciones definidas aquí...

std::mutex mtx; // Mutex para controlar la escritura en archivo desde múltiples hilos

void runAttempts(int n, std::mt19937& gen, std::ofstream& archivo) {
    double duration_total = 0;

    for (int intentos = 0; intentos <= 20; ++intentos) {
        std::vector<Point> randomPoints = generateRandomPoints(n, gen);
        auto start = std::chrono::high_resolution_clock::now();
        double result = findMinDistanceOptimized(randomPoints);
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(stop - start);

        std::lock_guard<std::mutex> lock(mtx); // Bloquea el archivo para escritura

        archivo << "N = " << n << ", Intento " << intentos << " duración: " << duration.count() << "ms\n";
        duration_total += duration.count();

        if (intentos == 20) {
            archivo << "N = " << n << ", Promedio de duración: " << duration_total / 20 << "ms\n";
        }
    }
}

int main() {
    unsigned seed = static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    std::mt19937 gen(seed);

    std::ofstream archivo("file.txt"); // Abre el archivo en modo escritura

    const int num_threads = 8; // Número de hilos igual a la cantidad de cores
    std::vector<std::thread> threads;

    for (int n = 5000000; n <= 50000000; n += 5000000) {
        std::cout << "N =" << n << std::endl;
        archivo << "N =" << n << std::endl;

        threads.clear();

        for (int i = 0; i < num_threads; ++i) {
            threads.emplace_back(runAttempts, n, std::ref(gen), std::ref(archivo));
        }

        for (std::thread& t : threads) {
            t.join(); // Espera a que todos los hilos finalicen
        }
    }

    return 0;
}
