#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <limits>

using namespace std;

// Estructura de un Punto
struct Point {
    double x, y;
};

// Funciones auxiliares
bool compare_x(const Point& a, const Point& b) {
    return a.x < b.x;
}

double distance(const Point& a, const Point& b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

double closest_pair_dist(vector<Point>& arr, int left, int right) {
    // Caso base donde el tamaño del conjunto es 0, 1 o 2. 
    if (right - left <= 2) { 
        // si es 0 o 1, se devuelve infinito
        double min = numeric_limits<double>::infinity();
        if (right - left == 2) // si es 2, se devuelve la distancia entre ambos puntos
        min = distance(arr[left], arr[left+1]);
        return min;
    }

    // Ordenamos los puntos según su coordenada x
    sort(arr.begin() + left, arr.begin() + right, compare_x);

    // Si no estamos en el caso base, entonces dividimos el conjunto 
    // en dos partes iguales usando una recta divisoria
    // Calculamos recursivamente la distancia minima en ambas partes
    int mid = (left + right) / 2;

    double Lmin = closest_pair_dist(arr, left, mid);
    double Rmin = closest_pair_dist(arr, mid, right);
    // Calculamos el minimo entre ambas soluciones
    double LRmin = min(Lmin, Rmin);

    for (int i = mid-1; i >= left; --i) {
        if (arr[mid].x - arr[i].x >= LRmin) {
            break;  // Salir si ya estamos más lejos que la distancia mínima
        }

        for (int j = mid; j < right; ++j) {
            double curr_dist = distance(arr[i], arr[j]);
            LRmin = min(LRmin, curr_dist);
        }
    }

    // retorna la distancia mínima
    return LRmin;
}

// Algoritmo
double divide_and_conquer(vector<Point>& arr) {
    return closest_pair_dist(arr, 0, arr.size());
}
