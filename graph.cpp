#include "graph.h"

#include <string>
#include <random>

#include <iostream>

Graph::Graph(size_t n, double p, bool b) : n(n), p(p), theta(genTheta(p))
{
    prepareAdjMat(b);
}

void Graph::genConTrad()
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0, 1);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j) {
            if (i >= j)
                continue;

            if (dis(gen) < p)
                set(i, j, true);
        }
    }
}

void Graph::genConMC()
{
    std::random_device rd;  //Will be used to obtain a seed for the random number engine
    std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
    std::uniform_real_distribution<> dis(0, 1);
    std::uniform_int_distribution<> disI(0, n - 1);

    bool stop = false;

    double Esum = 0;
    int ii = 0;

    size_t E = 0;

    if (p >= 0.5) {

        while (!stop) {

            double etheta = exp(-theta);

            for (size_t f = 0; f < n; ++f) {

                size_t i = disI(gen);
                size_t j = disI(gen);

                if (i == j)
                    continue;

                if (!get(i, j)) {
                    set(i, j, true);
                    ++E;
                    continue;
                }

                double r = dis(gen);
                if (r < etheta) {
                    set(i, j, false);
                    --E;
                }

            }

            double newE = E;


            double i = ii;
            double Esum2 = Esum * (i / (i+1)) + newE / (i + 1);

            if (std::abs(Esum - Esum2) < 1e-7)
                stop = true;

            Esum = Esum2;

            ii++;

            if (ii > 1e6)
                break;

        }
    } else {

        while (!stop) {

            double etheta = exp(theta);

            for (size_t f = 0; f < n; ++f) {

                size_t i = disI(gen);
                size_t j = disI(gen);

                if (i == j)
                    continue;

                if (get(i, j)) {
                    set(i, j, false);
                    ++E;
                    continue;
                }

                double r = dis(gen);
                if (r < etheta) {
                    set(i, j, true);
                    --E;
                }

            }

            double newE = E;


            double i = ii;
            double Esum2 = Esum * (i / (i+1)) + newE / (i + 1);

            if (std::abs(Esum - Esum2) < 1e-7)
                stop = true;

            Esum = Esum2;

            ii++;

            if (ii > 1e6)
                break;

        }
    }
}

void Graph::prepareAdjMat(bool b)
{
    if (b || p >= 0.5)
        adj.resize((n*n - n)/ 2, false);
    else
        adj.resize((n*n - n)/ 2, true);
}

size_t Graph::getN() const
{
    return n;
}

double Graph::genTheta(double p)
{
    return std::log(p / (1 - p));
}

double Graph::k()
{
    double sum = 0;

    for(bool b : adj) {
        sum += b ? 2 : 0;
    }

    return sum / n;
}

size_t Graph::E()
{
    size_t sum = 0;
    for (bool b : adj) {
        sum += b ? 1 : 0;
    }
    return sum;
}

void Graph::saveToSif(std::ostream &stream)
{
    for (size_t i = 0; i < n; ++i) {
        stream << i;

        char con[] = "\tx";
        for (size_t j = i + 1; j < n; ++j) {
            if (get(i,j)) {
                stream << con << "\t" << j;
                con[0] = '\0';
            }
        }
        stream << std::endl;
    }
}

double Graph::findAvgD()
{
    double ret = 0;

    for (size_t i = 0; i < n; ++i) {
        std::vector<size_t> m;
        m.resize(n, n+1);
        m[i] = 0;

        for (size_t dist = 1; dist < n; ++dist) {
            for (size_t j = 0; j < n; ++j) {
                if (m[j] == dist - 1) {
                    for (size_t k = 0; k < n; ++k) {
                        if (get(j,k)) {
                            if (dist < m[k]) {
                                m[k] = dist;
                            }
                        }
                    }
                }
            }
        }

        double mid_ret = 0;
        for (size_t j = 0; j < n; ++j) {
            if (m[j] == n)
                return -1;

            mid_ret += m[j];
        }
        ret += mid_ret / (n - 1);

    }

    return ret / n;
}

double Graph::exceptedK()
{
    return p * (n - 1);
}

void Graph::print(std::ostream &stream)
{
    stream << n << std::endl;

    for (size_t i = 0; i < n; ++i) {
        std::string del = "";
        for (size_t j = 0; j < n; ++j) {
            stream << del << (get(i,j) ? "1" : "0");
            del = " ";
        }
        stream << std::endl;
    }

}

void Graph::printStats(std::ostream &stream)
{
    stream << k();

    for (size_t j = 0; j < n; ++j) {
        stream << "\t" << getVConCount(j);
    }

    stream << std::endl;
}

bool Graph::get(size_t i, size_t j)
{
    if (i == j)
        return false;

    return adj.at(pos(i,j));
}

size_t Graph::pos(size_t i, size_t j)
{
    if (i == j)
        throw std::string("wtf?");

    auto x = i > j ? i : j; // większe
    auto y = i > j ? j : i; // mniejsze

    return (x * (x - 1) ) / 2 + y;
}

void Graph::set(size_t i, size_t j, bool val)
{
    if (i == j)
        return;

    adj.at(pos(i,j)) = val;
}

size_t Graph::getVConCount(size_t vertex)
{
    size_t c = 0;

    for (size_t i = 0; i < n; ++i)
        if (get(vertex, i))
            ++c;

    return c;
}
