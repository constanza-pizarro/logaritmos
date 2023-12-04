#include "../algoritmos.h"

int main() {
    vector<Point> arr = {{0, 0.5}, {0, 0}, {0.4, 0.2}, {0.7, 0.1}, {0.8, 0.9}, {0.1, 0.1}, {0.6, 0.5}};
    double min_dist = findMinDistanceDivideAndConquer(arr);

    //double min_dist_2 = findMinDistanceOptimized(arr);

    cout << "Distancia minima con Divide and conquer es : " << min_dist << endl;
    //cout << "Distancia minima con el aleatorizado (generic hashing) es : " << min_dist_2 << endl;


    return 0;
}
