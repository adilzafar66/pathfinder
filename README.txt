Path Optimization in 2D Directed Graphs

This project is aimed at visualizing path finding algorithms particularly A* and Dijkstra. 
It provides a command-line interface (CLI) and a graphical user interface (GUI) to interact with and visualize these algorithms on user-defined graphs.

The output of the graph consists of nodes and edges with each edge connection two different nodes. On top of the edges, the program
builds optimal paths using Dijkstra and A* algorithms. The path traced by A* is represented in green while the path traced by Dijkstra is
shown in blue. The traceable edges are shown in black and the non-traceable edges (cost = -1) are shown in red.

===========================================
Building
===========================================

1. Clone the repository from Github: https://github.com/uvic-seng475-2023-05/cpp_project-pathfinder.git

2. Specify the install directory ($INSTALL_DIR) and run the following commands:
    cmake -H. -Btmp_cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR
    cmake --build tmp_cmake --clean-first --target install

3. Navigate to the $INSTALL_DIR/bin to run the executable.

4. Run the demo script for starter in $INSTALL_DIR/bin.


===========================================
Usage and UI
===========================================

To run the program, use the following CLI arguments:

-f <input file>: Specifies the input file containing graph information.
-a <algorithm>: Choose "astar" for A* algorithm, "dijkstra" for Dijkstra's algorithm, or "all" to run both.
-o <output file> (optional): Use this argument if you want to save path edges to an output file.
-p (optional): Use this argument if you only want to display the paths without showing all the edges.

Upon launching the program, the user interface (UI) will be presented, featuring the graph visualization along with the optimal paths. The UI is designed to be intuitive and interactive, allowing users to explore the graph and its details.

# Node Interaction

Nodes within the graph are interactive. Clicking on a node will reveal its position information. A subsequent click will hide this information.

# Edge Interaction

Edges within the graph can also be interacted with. Clicking on an edge will display its associated cost. A second click will conceal the cost information.

# Zoom and Pan

Zooming in and out can be achieved by scrolling the mouse wheel. Scrolling in will zoom the view in, while scrolling out will zoom the view out.
To pan across the graph, click and hold the mouse button while dragging the screen. This action moves the view in the direction of mouse movement, allowing you to navigate different parts of the graph.

Feel free to explore and experiment with the visualization of pathfinding algorithms on 2D directed graphs using this project!

===========================================
Additional Info
===========================================

Included in the project is a script named random_graph_generator.cpp that serves to generate random graphs. During the build process, this script will also be built. Users have the option to utilize this script to generate random graph input files. These files can be created with user-defined start and end nodes, offering flexibility in experimentation.

Feel free to create your own input files using the random_graph_generator.cpp script and place them in the path_finder directory. This additional functionality allows you to witness intriguing outcomes when processed through the program.