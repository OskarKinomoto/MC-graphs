#ifndef GRAPH_H
#define GRAPH_H

#include <vector>
#include <ostream>

class Graph
{
public:
    Graph(size_t n, double p, bool b = false);

    void genConTrad();
    void genConMC();

    void print(std::ostream& stream);
    void printStats(std::ostream& stream);

    bool get(size_t i, size_t j);
    void set(size_t i, size_t j, bool val);

    size_t getVConCount(size_t vertex);

    size_t getN() const;

    static double genTheta(double p);

    double exceptedK();
    double k();

    size_t E();

    void saveToSif(std::ostream& stream);

    double findAvgD();

private:
    void prepareAdjMat(bool b);
    size_t pos(size_t i, size_t j);

private:
    const size_t n;
    const double p;
    const double theta;

    std::vector<bool> adj; ///< Adjacency matrix
};

#endif // GRAPH_H
