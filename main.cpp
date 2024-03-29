
#include <string>
#include <map>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

#include "lib/io.hpp"
#include "lib/graph.hpp"
#include "lib/reduction.hpp"


// First Runner
void run_1(int argc, char const *argv[])
{
    // File name
    string name = string(argv[1])+".graphs";
    // Intantiating graphs
    Graph gs = Graph(); // The graph g
    Graph gl = Graph(); // The graph g'


    // Reading from file and creating the graphs
    read(name, gl, gs);


    // Now pre-processing the graph for isolated nodes
    // Set isolated nodes
    gs.set_isolated();
    gl.set_isolated();
    // Adding candidates for nodes in graph g1 (all nodes of gl)
    for(Node& n_: gs.nodes_()) {
        n_.create_set(gl.nodes_());
    }
    // Return whether all the isolated nodes can be satisfied
    if(gs.isolated().size() > gl.isolated().size()) {
        write_unsat(argv[1]);
        return;
    }
    
    
    // Starting with reductions
    // First reduction (based on degrees)
    bool _sat = unary_reduction(gs);
    // Secondary reduction (based on arc consitency)
    for(int i=0; i<5; i++)
        secondary_reduction(gs, gl, _sat);
    // while(secondary_reduction(gs, gl, _sat));
    // Sorting to keep the nodes in order
    sort(gs.nodes_().begin(), gs.nodes_().end(), [](Node& _1, Node& _2){return _1.index_() < _2.index_();});
    
    // Writing the data created to files
    // 1. Write the singular and variable mappings
    // 2. Write the satinput file
    // ----------OR------------
    // Write the unsatisfiable satinput file
    if (!_sat) {
        write_unsat(argv[1]);
    }
    else {
        write_map_input(argv[1], gs, gl);
    }
}


// Second Runner
void run_2(int argc, char const *argv[])
{
    read_satoutput(argv[1]);
}


// Main function
int main(int argc, char const *argv[])
{
    if(argc == 2) run_1(argc, argv);
    if(strcmp(argv[2], "1") == 0) run_1(argc, argv);
    else run_2(argc, argv);
    return 0;
}
