from sys import argv

import networkx as nx
from matplotlib import pyplot as plt
from ortools.sat.python import cp_model

from src.cp.utils import *
from src.cp.solvers import solve_ftp
from src.cp.plotting import plot_solution
from src.cp.reading import read_tsplib_graph

MAX_TIME = int(argv[1])

delta = 1E-2

# Setup:
names, coords = read_tsplib_graph()
n = len(names)
source = names[n - 1]
names_to_i = {name: i for i, name in enumerate(names)}
dist = l2_norm(coords, delta)
UB = trivial_ub(n, dist)

# Print instance info:
print('Freeze-Tag instance information:')
print(f'\tTime limit = {MAX_TIME}s')
print('\tNumber of nodes =', n)
print('\tSource =', source)

# FTP solving:
status, model, solver, depth, d_v, x_e = solve_ftp(names, dist, MAX_TIME, UB, True)

if status == cp_model.FEASIBLE or status == cp_model.OPTIMAL:
    # FTP solution:
    print('Full FTP solution:')
    depth = solver.Value(depth)
    print(f'  solution makespan: {delta * depth:.2f}')
    print(f'  d_v = (', end='')
    for v in range(n - 1):
        depth_v = solver.Value(d_v[v])
        print(f'{delta * depth_v:.2f}', end=', ')
    print(f'{delta * solver.Value(d_v[-1])})')

    print('  edges: ', end='')
    sol_edges = []
    for u in range(n):
        for v in range(n - 1):
            if u == v: continue
            if solver.Value(x_e[u][v]):
                print(f'{names[u]}-{names[v]}; ', end='')
                sol_edges.append((names[u], names[v]))
    tree = nx.DiGraph(sol_edges)
    hop_depth = calc_depth(source, names_to_i, tree)
    print(f'\n  hop depth: {hop_depth}')

    node_colors = []
    for v in tree.nodes:
        if v == source:
            node_colors.append('red')
        else:
            node_colors.append('cyan' if solver.Value(d_v[names_to_i[v]]) == depth else 'black')

    # Solution plotting:
    plt.figure(figsize=(10, 6))

    coords_dict = {names[i]: c for i, c in enumerate(coords)}
    plot_solution(tree, sol_edges, coords_dict, names, node_colors, 'green', style='solid', node_size=40)

    plt.gca().set_aspect('equal', adjustable='box')
    plt.show()
else:
    print('No solution found.')
