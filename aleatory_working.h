#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <random>
#include <limits>
#include <cstdint>

using namespace std;
using namespace std::chrono;

struct Point {
    double x, y;
};

double calculateDistance(Point p1, Point p2) {
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}


vector<Point> generateRandomPoints(int n, mt19937& gen) {
    vector<Point> points;
    uniform_real_distribution<> dis(0.0, 1.0);
    while (points.size()<n) {
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


double findMinDistance(vector<Point>& points) {
    double minDistance = numeric_limits<double>::infinity();
    int n = points.size();

    for (int i=0; i<n-1; i+=2) {
        minDistance = min(minDistance, calculateDistance(points[i], points[i+1]));
    }

    return minDistance;
}


// Función hash basada en la función de dispersión std::hash
int customHash(double value) {
    return hash<double>{}(value);
}


int customHashMersenne(double value) {
    // Número primo de Mersenne M31 (2^31 - 1)
    const int mersenne_prime = (1 << 31) - 1;

    // Convertir el valor en un entero
    int intValue = static_cast<int>(value);

    // Aplicar la propiedad (a * b) % p = ((a % p) * (b % p)) % p
    int hashValue = ((intValue % mersenne_prime) * (intValue % mersenne_prime)) % mersenne_prime;

    // Asegurarse de que el resultado sea no negativo
    if (hashValue < 0) {
        hashValue += mersenne_prime;
    }

    return hashValue;
}


// Hashing Universal con a en [1, p-1] y b en [0, p-1]
int customUniversalHash(double value, int a, int b, int p, int m) {

    // Aplicar la función de hash
    int hashValue = ((static_cast<int>(a) * static_cast<int>(value) + b) % p) % m;

    return hashValue;
}


// Funciones más rápidas
int customHashFasterFunctions(double value) {
    // Convertir el valor en un entero
    int intValue = static_cast<int>(value);

    const int mersenne_prime = (1 << 31) - 1;
    
    // Calcular el módulo mersenne_prime de manera eficiente
    int result = (intValue & mersenne_prime) + (intValue >> 31);
    result = (result & mersenne_prime) + (result >> 31);

    // Restar mersenne_prime si el valor es mayor o igual a mersenne_prime
    return (result >= mersenne_prime) ? result - mersenne_prime : result;
}


unordered_map<int, unordered_map<int, vector<Point>>> generateGridMersenne(vector<Point>& points) {
    unordered_map<int, unordered_map<int, vector<Point>>> grid;

    for (Point& p : points) {
        int gridX = customHashMersenne(p.x);
        int gridY = customHashMersenne(p.y);
        grid[gridX][gridY].push_back(p);
    }

    return grid;
}


unordered_map<int, unordered_map<int, vector<Point>>> generateGridUniversal(vector<Point>& points, int a, int b, int p, int m) {
    unordered_map<int, unordered_map<int, vector<Point>>> grid;

    for (Point& point : points) {
        int gridX = customUniversalHash(point.x, a, b, p, m);
        int gridY = customUniversalHash(point.y, a, b, p, m);
        grid[gridX][gridY].push_back(point);
    }

    return grid;
}


// Función para generar la cuadrícula utilizando la función de hashing std::hash
unordered_map<int, unordered_map<int, vector<Point>>> generateGrid(vector<Point>& points) {
    unordered_map<int, unordered_map<int, vector<Point>>> grid;

    for (Point& p : points) {
        int gridX = customHash(p.x);
        int gridY = customHash(p.y);
        grid[gridX][gridY].push_back(p);
    }

    return grid;
}


double findMinDistanceOptimizedMersenne(vector<Point>& points) {
    double minDistance = numeric_limits<double>::infinity();

    double d = findMinDistance(points);
    d /= 2.0;

    // Generar la cuadrícula utilizando Mersenne Hashing
    unordered_map<int, unordered_map<int, vector<Point>>> grid = generateGridMersenne(points);

    // Actualizar el valor de d para tener celdas de cuadrícula apropiadas
    for (Point& p : points) {
        int gridX = static_cast<int>(p.x / (d / 2));
        int gridY = static_cast<int>(p.y / (d / 2));
        grid[gridX][gridY].push_back(p);
    }

    // Encontrar la distancia mínima entre puntos vecinos en la cuadrícula
    for (Point& p : points) {
        int gridX = static_cast<int>(p.x / d);
        int gridY = static_cast<int>(p.y / d);

        for (int i = gridX - 1; i <= gridX + 1; ++i) {
            for (int j = gridY - 1; j <= gridY + 1; ++j) {
                if (grid.find(i) != grid.end() && grid[i].find(j) != grid[i].end()) {
                    for (Point& neighbor : grid[i][j]) {
                        // Evitar comparar el mismo punto
                        if (neighbor.x != p.x || neighbor.y != p.y) {
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


double findMinDistanceOptimizedUniversal(vector<Point>& points, int a, int b, int p, int m) {
    double minDistance = numeric_limits<double>::infinity();

    double d = findMinDistance(points);
    d /= 2.0;

    // Generar la cuadrícula utilizando la nueva función de hashing
    unordered_map<int, unordered_map<int, vector<Point>>> grid = generateGridUniversal(points, a, b, p, m);

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


double findMinDistanceOptimizedGeneric(vector<Point>& points) {
    double minDistance = numeric_limits<double>::infinity();

    double d = findMinDistance(points);
    d /= 2.0;

    // Generar la cuadrícula utilizando la nueva función de hashing
    unordered_map<int, unordered_map<int, vector<Point>>> grid = generateGrid(points);


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



double findMinDistanceOptimized(vector<Point>& points) {
    double minDistance = numeric_limits<double>::infinity();
    double d = findMinDistance(points);  // Calcular la distancia mínima inicial
    unordered_map<int, unordered_map<int, vector<Point>>> grid;

    // Actualizar el valor de d para tener celdas de cuadrícula apropiadas
    d /= 2.0;

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



/*// ---------------------GENERAR GRILLAS---------------------

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

double findMinDistanceUniversal(vector<Point>& points, int a, int b, int p, int m) {
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
    //d /= 2.0;

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
}*/
