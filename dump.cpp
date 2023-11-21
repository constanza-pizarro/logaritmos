#include <iostream>
#include <vector>
#include <cmath>
#include <unordered_map>
#include <unordered_set>
#include <random>

using namespace std;

struct Point {
    double x, y;
};

double calculateDistance(const Point& p1, const Point& p2) {
    return sqrt(pow((p2.x - p1.x), 2) + pow((p2.y - p1.y), 2));
}

vector<Point> generateRandomPoints(int n) {
    vector<Point> points;
    random_device rd;
    mt19937 gen(rd());
    uniform_real_distribution<> dis(0.0, 100.0); // Rango del plano

    for (int i = 0; i < n; ++i) {
        Point p;
        p.x = dis(gen);
        p.y = dis(gen);
        points.push_back(p);
    }
    return points;
}

double findMinimumDistance(const vector<Point>& points) {
    double minDistance = numeric_limits<double>::infinity();
    int n = points.size();

    for (int i = 0; i < n - 1; ++i) {
        for (int j = i + 1; j < n; ++j) {
            double distance = calculateDistance(points[i], points[j]);
            minDistance = min(minDistance, distance);
        }
    }

    return minDistance;
}

double findMinimumDistanceOptimized(const vector<Point>& points) {
    double minDistance = numeric_limits<double>::infinity();
    double d = findMinimumDistance(points);
    unordered_map<pair<int, int>, unordered_set<Point>, hash<pair<int, int>>> grid;

    for (const Point& p : points) {
        int gridX = p.x / d;
        int gridY = p.y / d;
        grid[{gridX, gridY}].insert(p);
    }

    for (const Point& p : points) {
        int gridX = p.x / d;
        int gridY = p.y / d;

        for (int i = gridX - 1; i <= gridX + 1; ++i) {
            for (int j = gridY - 1; j <= gridY + 1; ++j) {
                if (grid.find({i, j}) != grid.end()) {
                    for (const Point& neighborPoint : grid[{i, j}]) {
                        if (!(neighborPoint.x == p.x && neighborPoint.y == p.y)) {
                            double distance = calculateDistance(p, neighborPoint);
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

    double minDistance = findMinimumDistanceOptimized(randomPoints);

    cout << "La distancia mínima entre los puntos es: " << minDistance << endl;

    return 0;

}