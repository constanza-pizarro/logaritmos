#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <random>
#include <limits>

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

    /*for (int i = 0; i < n; ++i) {
        Point p;
        p.x = dis(gen);
        p.y = dis(gen);
        points.push_back(p);
    }*/
    int i=0;
    while (i<n) { 
      double x = dis(gen);
      double y = dis(gen);

      if (x < 1.0 && y < 1.0) {
        Point p;
        p.x = x;
        p.y = y;
        points.push_back(p);
        i++;
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

int main() {
    int n = 100;
    vector<Point> randomPoints;

    // Utilizar el tiempo actual como semilla para mt19937
    unsigned seed = static_cast<unsigned>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    mt19937 randomGenerator(seed);

    cout << "Semilla utilizada: " << seed << endl;

    randomPoints = generateRandomPoints(n, randomGenerator);

    // Imprimir puntos generados
    cout << "Puntos generados:" << endl;
    for (const auto& point : randomPoints) {
        cout << "(" << point.x << ", " << point.y << ")" << endl;
    }

    auto start = high_resolution_clock::now();
    double minDistanceUsingUnorderedMap = findMinDistanceOptimized(randomPoints);
    auto stop = high_resolution_clock::now();

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "N: " << n << ", Distancia minima: " << minDistanceUsingUnorderedMap << ", Tiempo: " << duration.count() << " ms" << endl;

    return 0;
}

