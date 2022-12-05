#ifndef FTP_UTILS_DEFINE
#define FTP_UTILS_DEFINE

#include "mygraphlib.hpp"
#include <chrono>
#include <lemon/min_cost_arborescence.h>
#include <queue>

#define ELAPSED ((chrono::system_clock::now() - P.start).count() / 1E9)
#define _NEW_UB_MESSAGE(SOL, MSG)                                      \
    {                                                                  \
        PrintSolution(P, SOL, MSG);                                    \
        printf("custo: %05.5f - %02.2f%% ótimo\n", UB, 100 * LB / UB); \
    }
#define NEW_UB_MESSAGE(SOL) _NEW_UB_MESSAGE(SOL, "\nNovo UB.")

using namespace lemon;
using namespace std;

typedef chrono::time_point<chrono::system_clock> time_point;
typedef vector<DNode> DNodeVector;

// Compares two arcs based upon their weights:
struct ArcCmp : public binary_function<Arc, Arc, bool> {
    ArcValueMap &weight;

    explicit ArcCmp(ArcValueMap &weight) : weight(weight) {}

    inline bool operator()(const Arc &x, const Arc &y) const {
        return weight[x] > weight[y];// acceding order
    }
};

typedef MinCostArborescence<Digraph, ArcValueMap> MinCostArb;
typedef priority_queue<double, vector<double>, greater<>> min_heap;
typedef priority_queue<Arc, vector<Arc>, ArcCmp> min_arc_heap;

// FTP_Instance put all relevant information in one class.
class FTP_Instance {
public:
    FTP_Instance(Digraph &graph, DNodeStringMap &vvname,
                 DNodePosMap &posx, DNodePosMap &posy,
                 ArcValueMap &eweight, DNode &sourcenode,
                 DNode &targetnode, int &npairs, DNodeVector &pickup,
                 DNodeVector &delivery,
                 Digraph::NodeMap<DNode> &del_pickup,
                 DNodeBoolMap &is_pickup, int time_limit);
    void start_counter();

    double eps_min = 2.0, eps_max = 2.0;
    Digraph &g;
    DNodeStringMap &vname;
    DNodePosMap &px;
    DNodePosMap &py;
    ArcValueMap &weight;
    const int nnodes;
    DNode &source;
    DNode &target;
    const int npairs;
    DNodeVector &pickup;
    DNodeVector &delivery;
    Digraph::NodeMap<DNode> &del_pickup; // maps a delivery to its pickup
    map<DNode, vector<Arc>> ordered_arcs;// out arcs of a node ordered by weight
    DNodeBoolMap &is_pickup;
    time_point start;
    const int time_limit;
    map<DNode, map<DNode, double>> weight_map;// used for fast weight lookup
};

void PrintInstanceInfo(FTP_Instance &P);
void PrintSolution(FTP_Instance &P, DNodeVector &Sol,
                   const string &msg);

bool ReadFTPGraph(const string &filename, Digraph &g,
                  DNodeStringMap &vname, DNodePosMap &posx,
                  DNodePosMap &posy, ArcValueMap &weight,
                  DNode &source, DNode &target, int &npairs,
                  DNodeVector &pickup, DNodeVector &delivery,
                  Digraph::NodeMap<DNode> &del_pickup,
                  DNodeBoolMap &is_pickup);

double route_cost(FTP_Instance &P, const DNodeVector &Sol);

bool ViewFTPSolution(FTP_Instance &P, double &LB,
                     double &UB, DNodeVector &Sol,
                     const string &msg);

bool local_search(FTP_Instance &P, double &LB, double &UB,
                  DNodeVector &Sol);

bool arborescence_heuristic(FTP_Instance &P, double &LB, double &UB,
                            DNodeVector &Sol, MinCostArb &solver);

#endif// FTP_UTILS_DEFINE
