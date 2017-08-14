#include <iostream>
#include <fstream>
#include <vector>

#include "graph.h"

using namespace std;

#define HOME "~"

void clasic(double p, double n, size_t N) {
    static std::ofstream sr(HOME "/graphs/graf-klas-sr");
    std::ofstream f(HOME "/graphs/graf-klas-" + std::to_string(n) + "-" + std::to_string(p));

    cout << "Clasic: " << n << " " << p << endl;

    for (size_t i = 0; i < N; ++i) {
        Graph g(n, p, true);
        g.genConTrad();

        if (!i) {
            f << n << "\t" << p << "\t" << g.exceptedK() << "\t" << N << endl;
            sr << n << "\t" << p << "\t" << g.findAvgD() << endl;
            std::ofstream sif(HOME "/graphs/graf-klas-" + std::to_string(n) + "-" + std::to_string(p) + "-" + std::to_string(i) + ".sif");
            g.saveToSif(sif);
        }



        g.printStats(f);
    }
}

void MC(double p, double n, size_t N) {
    static std::ofstream sr(HOME "/graphs/graf-mc-sr");
    std::ofstream f(HOME "/graphs/graf-mc-" + std::to_string(n) + "-" + std::to_string(p));

    cout << "MC: " << n << " " << p << endl;

    for (size_t i = 0; i < N; ++i) {
        Graph g(n, p);
        g.genConMC();

        if (!i) {
            f << n << "\t" << p << "\t" << g.exceptedK() << "\t" << N << endl;
            sr << n << "\t" << p << "\t" << g.findAvgD() << endl;
            std::ofstream sif(HOME "/graphs/graf-mc-" + std::to_string(n) + "-" + std::to_string(p) + "-" + std::to_string(i) + ".sif");
            g.saveToSif(sif);
        }


        g.printStats(f);
    }
}

int main()
{

    std::vector<int> Ns = {5, 10, 20, 50, 100, 200, 400, 1000};
    std::vector<double> ps = {0.01, 0.1, 0.3, 0.5, 0.7, 0.9, 0.99};

    cout << "Step 1" << endl;
    for (auto n : Ns)
        for (auto p : ps)
            clasic(p, n, 20);
/*
    cout << "Step 2" << endl;
    for (auto n : Ns)
        for (auto p : ps)
            MC(p, n, 20);
            */
    return 0;
}
