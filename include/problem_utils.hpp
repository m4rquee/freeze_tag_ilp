#ifndef Problem_UTILS_DEFINE
#define Problem_UTILS_DEFINE

#include "mygraphlib.hpp"
#include <chrono>
#include <lemon/dijkstra.h>

#define ELAPSED ((chrono::system_clock::now() - P.start).count() / 1E9)
#define _NEW_UB_MESSAGE(SOL, MSG)                                                                                      \
    {                                                                                                                  \
        PrintSolution(P, SOL, MSG);                                                                                    \
        printf("custo: %05.5f - %02.2f%% ótimo\n", UB, 100 * LB / UB);                                                 \
    }
#define NEW_UB_MESSAGE(SOL) _NEW_UB_MESSAGE(SOL, "\nNovo UB.")

using namespace lemon;
using namespace std;

typedef chrono::time_point<chrono::system_clock> time_point;
typedef vector<DNode> DNodeVector;
typedef vector<Arc> ArcVector;
typedef Dijkstra<Digraph, ArcValueMap> DijkstraSolver;

// Problem_Instance put all relevant information in one class.
class Problem_Instance {
public:
    Problem_Instance(Digraph &graph, DNodeStringMap &vvname, DNodePosMap &posx, DNodePosMap &posy, DNode &sourcenode,
                 int &nnodes, int &time_limit, ArcValueMap &weight, ArcBoolMap &original, double &source_radius);
    ~Problem_Instance();
    void start_counter();

    Digraph &g;
    DNodeStringMap &vname;
    DNodePosMap &px;
    DNodePosMap &py;
    int nnodes;
    DNode &source;
    time_point start;
    const int time_limit;
    ArcValueMap &weight;
    ArcBoolMap &original;
    Arc *solution;
    double &source_radius;
};

void PrintInstanceInfo(Problem_Instance &P);

void PrintSolution(Problem_Instance &P, ArcVector &Sol, const string &msg);

bool ReadProblemGraph(const string &filename, Digraph &g, DNodeStringMap &vname, DNodePosMap &posx, DNodePosMap &posy,
                  DNode &source, int &nnodes, ArcValueMap &weight, ArcBoolMap &original, double &source_radius,
                  bool calc_clojure = false, bool tsplib = false);

bool ViewProblemSolution(Problem_Instance &P, double &LB, double &UB, const string &msg, bool only_active_edges);

#endif// Problem_UTILS_DEFINE