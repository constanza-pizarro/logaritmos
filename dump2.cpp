#include <iostream>
#include <vector>
#include <unordered_map>
#include <cmath>
#include <chrono>
#include <random>

using namespace std;
using namespace std::chrono;

struct Point {
    double x, y;
};

double calculateDistance(Point p1, Point p2) {
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

vector<Point> generateRandomPoints(int n) {
    vector<Point> points;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 1.0); // Rango del plano [0, 1)

    for (int i = 0; i < n; ++i) {
        Point p;
        p.x = dis(gen);
        p.y = dis(gen);
        points.push_back(p);
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
    double d = findMinDistance(points);
    unordered_map<int, unordered_map<int, vector<Point>>> grid;

    for (Point& p : points) {
        int gridX = static_cast<int>(p.x / d);
        int gridY = static_cast<int>(p.y / d);
        grid[gridX][gridY].push_back(p);
    }

    for (Point& p : points) {
        int gridX = static_cast<int>(p.x / d);
        int gridY = static_cast<int>(p.y / d);

        for (int i = gridX - 1; i <= gridX + 1; ++i) {
            for (int j = gridY - 1; j <= gridY + 1; ++j) {
                if (grid.find(i) != grid.end() && grid[i].find(j) != grid[i].end()) {
                    for (Point& neighbor : grid[i][j]) {
                        if (!(neighbor.x == p.x && neighbor.y == p.y)) {
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
        vector<Point> randomPoints = generateRandomPoints(n);

        auto start = high_resolution_clock::now();
        double minDistanceUsingUnorderedMap = findMinDistanceOptimized(randomPoints);
        auto stop = high_resolution_clock::now();

        auto duration = duration_cast<milliseconds>(stop - start);
        cout << "N: " << n << ", Distancia minima: " << minDistanceUsingUnorderedMap << ", Tiempo: " << duration.count() << " ms" << endl;
    

    return 0;
}
