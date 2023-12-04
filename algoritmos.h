#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <random>
#include <limits>
#include <cstdint>

using namespace std;

struct Point {
    double x, y;
};

double calculateDistance(Point p1, Point p2) {
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

vector<Point> generateRandomPoints(int n, mt19937& gen) {
    vector<Point> points;
    uniform_real_distribution<> dis(0.0, 1.0);

    while (points.size() < n) {
        double x = dis(gen);
        double y = dis(gen);

        if (x < 1.0 && y < 1.0) {
            Point p;
            p.x = x;
            p.y = y;
            points.push_back(p);
        }
    }
    return points;
}

double minDistancePairs(vector<Point>& points) {
    double minDistance = numeric_limits<double>::infinity();
    int n = points.size();

    for (int i=0; i<n-1; i+=2) {
        minDistance = min(minDistance, calculateDistance(points[i], points[i+1]));
    }

    return minDistance;
}

bool compareX(const Point& a, const Point& b) {
    return a.x < b.x;
}

double closestPairDist(vector<Point>& arr, int left, int right) {
    // Caso base donde el tamaño del conjunto es 0, 1 o 2. 
    if (right - left <= 2) { 
        // si es 0 o 1, se devuelve infinito
        double min = numeric_limits<double>::infinity();
        if (right - left == 2) // si es 2, se devuelve la distancia entre ambos puntos
        min = calculateDistance(arr[left], arr[left+1]);
        return min;
    }

    // Ordenamos los puntos según su coordenada x
    sort(arr.begin() + left, arr.begin() + right, compareX);

    // Si no estamos en el caso base, entonces dividimos el conjunto 
    // en dos partes iguales usando una recta divisoria
    // Calculamos recursivamente la distancia minima en ambas partes
    int mid = (left + right) / 2;

    double Lmin = closestPairDist(arr, left, mid);
    double Rmin = closestPairDist(arr, mid, right);
    // Calculamos el minimo entre ambas soluciones
    double LRmin = min(Lmin, Rmin);

    for (int i = mid-1; i >= left; --i) {
        if (arr[mid].x - arr[i].x >= LRmin) {
            break;  // Salir si ya estamos más lejos que la distancia mínima
        }

        for (int j = mid; j < right; ++j) {
            double currDist = calculateDistance(arr[i], arr[j]);
            LRmin = min(LRmin, currDist);
        }
    }

    // retorna la distancia mínima
    return LRmin;
}

double findMinDivideAndConquer(vector<Point>& arr) {
    return closestPairDist(arr, 0, arr.size());
}


// ------------------FUNCIONES DE HASHING------------------

int universalHash(double value, int a, int b, int p, int m) {
    return ((static_cast<int>(a) * static_cast<int>(value) + b) % p) % m;
}

int mersennePrimeHash(double value, int p) {
    return static_cast<int>(value) % p;
}

// Funciones más rápidas
int fasterFunctionsHash(double value, int p, int k) {
    // Convertir el valor en un entero
    int intValue = static_cast<int>(value);

    // Calcular el módulo mersenne_prime de manera eficiente
    int result = (intValue & p) + (intValue >> k);

    if (result >= p) result -= p;

    return result;
}

// Función hash basada en la función de dispersión std::hash
int genericHash(double value) {
    return hash<double>{}(value); // Utilizando la función de dispersión std::hash
}

// ---------------------GENERAR GRILLAS---------------------

unordered_map<int, unordered_map<int, vector<Point>>> generateUniversalGrid(vector<Point>& points, int a, int b, int p, int m) {
    unordered_map<int, unordered_map<int, vector<Point>>> grid;
    grid.reserve(m);

    for (Point& point : points) {
        int gridX = universalHash(point.x, a, b, p, m);
        int gridY = universalHash(point.y, a, b, p, m);
        grid[gridX][gridY].push_back(point);
    }

    return grid;
}

unordered_map<int, unordered_map<int, vector<Point>>> generateMersenneGrid(vector<Point>& points, int p, int m) {
    unordered_map<int, unordered_map<int, vector<Point>>> grid;
    grid.reserve(m);

    for (Point& point : points) {
        int gridX = mersennePrimeHash(point.x, p);
        int gridY = mersennePrimeHash(point.y, p);
        grid[gridX][gridY].push_back(point);
    }

    return grid;
}

unordered_map<int, unordered_map<int, vector<Point>>> generateFasterFunctionsGrid(vector<Point>& points, int p, int k, int m) {
    unordered_map<int, unordered_map<int, vector<Point>>> grid;
    grid.reserve(m);

    for (Point& point : points) {
        int gridX = fasterFunctionsHash(point.x, p, k);
        int gridY = fasterFunctionsHash(point.y, p, k);
        grid[gridX][gridY].push_back(point);
    }

    return grid;
}

// Función para generar la cuadrícula utilizando la función de hashing std::hash
unordered_map<int, unordered_map<int, vector<Point>>> generateGenericGrid(vector<Point>& points, int m) {
    unordered_map<int, unordered_map<int, vector<Point>>> grid;
    grid.reserve(m);

    for (Point& point : points) {
        int gridX = genericHash(point.x);
        int gridY = genericHash(point.y);
        grid[gridX][gridY].push_back(point);
    }

    return grid;
}

// -------------------------HASHING-------------------------

double findMindDistanceUniversal(vector<Point>& points, int a, int b, int p, int m) {
    double minDistance = numeric_limits<double>::infinity();

    double d = minDistancePairs(points);
    d/= 2.0;

    // Generar la cuadrícula 
    unordered_map<int, unordered_map<int, vector<Point>>> grid = generateUniversalGrid(points, a, b, p, m);

    // Actualizar el valor de d para tener celdas de cuadrícula apropiadas
    for (Point& p : points) {
        int gridX = static_cast<int>(p.x / (d / 2));
        int gridY = static_cast<int>(p.y / (d / 2));
        grid[gridX][gridY].push_back(p);
    }

    for (Point& p : points) {
        int gridX = static_cast<int>(p.x / d);
        int gridY = static_cast<int>(p.y / d);

        for (int i = gridX - 1; i <= gridX + 1; ++i) {
            for (int j = gridY - 1; j <= gridY + 1; ++j) {
                if (grid.find(i) != grid.end() && grid[i].find(j) != grid[i].end()) {

                    for (Point& neighbor : grid[i][j]) {
                        // Evitar comparar el mismo punto
                        if (fabs(neighbor.x - p.x) > 1e-9 || fabs(neighbor.y - p.y) > 1e-9) {
                            double distance = calculateDistance(p, neighbor);
                            minDistance = min(minDistance, distance);
                        }
                    }
                    
                }
            }
        }
    }

    return minDistance;
}

double findMinDistanceMersenne(vector<Point>& points, int p, int m) {
    double minDistance = numeric_limits<double>::infinity();

    double d = minDistancePairs(points);
    d /= 2.0;

    // Generar la cuadrícula utilizando Mersenne Hashing
    unordered_map<int, unordered_map<int, vector<Point>>> grid = generateMersenneGrid(points, p, m);

    // Actualizar el valor de d para tener celdas de cuadrícula apropiadas
    for (Point& point : points) {
        int gridX = static_cast<int>(point.x / (d / 2));
        int gridY = static_cast<int>(point.y / (d / 2));
        grid[gridX][gridY].push_back(point);
    }

    // Encontrar la distancia mínima entre puntos vecinos en la cuadrícula
    for (Point& point : points) {
        int gridX = static_cast<int>(point.x / d);
        int gridY = static_cast<int>(point.y / d);

        for (int i = gridX - 1; i <= gridX + 1; ++i) {
            for (int j = gridY - 1; j <= gridY + 1; ++j) {
                if (grid.find(i) != grid.end() && grid[i].find(j) != grid[i].end()) {
                    for (Point& neighbor : grid[i][j]) {
                        // Evitar comparar el mismo punto
                        if (neighbor.x != point.x || neighbor.y != point.y) {
                            double distance = calculateDistance(point, neighbor);
                            minDistance = min(minDistance, distance);
                        }
                    }
                }
            }
        }
    }

    return minDistance;
}

double findMinDistanceFasterFunctions(vector<Point>& points, int p, int k, int m) {
    double minDistance = numeric_limits<double>::infinity();

    double d = minDistancePairs(points);
    d /= 2.0;

    // Generar la cuadrícula
    unordered_map<int, unordered_map<int, vector<Point>>> grid = generateFasterFunctionsGrid(points, p, k, m);

    // Actualizar el valor de d para tener celdas de cuadrícula apropiadas
    for (Point& point : points) {
        int gridX = static_cast<int>(point.x / (d / 2));
        int gridY = static_cast<int>(point.y / (d / 2));
        grid[gridX][gridY].push_back(point);
    }

    // Encontrar la distancia mínima entre puntos vecinos en la cuadrícula
    for (Point& point : points) {
        int gridX = static_cast<int>(point.x / d);
        int gridY = static_cast<int>(point.y / d);

        for (int i = gridX - 1; i <= gridX + 1; ++i) {
            for (int j = gridY - 1; j <= gridY + 1; ++j) {
                if (grid.find(i) != grid.end() && grid[i].find(j) != grid[i].end()) {
                    for (Point& neighbor : grid[i][j]) {
                        // Evitar comparar el mismo punto
                        if (neighbor.x != point.x || neighbor.y != point.y) {
                            double distance = calculateDistance(point, neighbor);
                            minDistance = min(minDistance, distance);
                        }
                    }
                }
            }
        }
    }

    return minDistance;
}

double findMinDistanceGeneric(vector<Point>& points, int m) {
    double minDistance = numeric_limits<double>::infinity();

    double d = minDistancePairs(points);
    d /= 2.0;

    // Generar la cuadrícula
    unordered_map<int, unordered_map<int, vector<Point>>> grid = generateGenericGrid(points, m);

    // Actualizar el valor de d para tener celdas de cuadrícula apropiadas
    for (Point& p : points) {
        int gridX = static_cast<int>(p.x / (d / 2));
        int gridY = static_cast<int>(p.y / (d / 2));
        grid[gridX][gridY].push_back(p);
    }

    for (Point& p : points) {
        int gridX = static_cast<int>(p.x / d);
        int gridY = static_cast<int>(p.y / d);

        for (int i = gridX - 1; i <= gridX + 1; ++i) {
            for (int j = gridY - 1; j <= gridY + 1; ++j) {
                if (grid.find(i) != grid.end() && grid[i].find(j) != grid[i].end()) {

                    for (Point& neighbor : grid[i][j]) {
                        // Evitar comparar el mismo punto
                        if (fabs(neighbor.x - p.x) > 1e-9 || fabs(neighbor.y - p.y) > 1e-9) {
                            double distance = calculateDistance(p, neighbor);
                            minDistance = min(minDistance, distance);
                        }
                    }
                    
                }
            }
        }
    }

    return minDistance;
}