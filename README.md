# Smart-Traffic-Management-System-

# 1. Introduction
The Traffic Simulation System is a comprehensive program designed to simulate real-time vehicle movement through a road network while managing traffic signals dynamically. The project uses a graph-based representation of road networks to map intersections and roads, allowing vehicles to traverse the network along the shortest path. Traffic signals are dynamically adjusted based on vehicle density, ensuring efficient traffic flow and minimizing congestion.

This simulation is particularly useful for urban traffic management systems, road design analysis, and understanding the impact of dynamic traffic signal systems.

# 2. Key Features
# 1- Graph-Based Road Network Representation

-Represents roads as directed edges and intersections as nodes.
-Roads have associated travel times for precise simulation.

# 2- Shortest Path Calculation (Dijkstra’s Algorithm)

-Computes the shortest path for vehicles between intersections based on travel time.

3- Real-Time Vehicle Movement Simulation

-Tracks vehicles as they traverse roads and dynamically updates their positions over time.

4- Dynamic Traffic Signal Management

-Adjusts traffic signal timings based on the number of vehicles waiting at each intersection.
-Uses intelligent algorithms to calculate green signal duration based on vehicle density.

5- File-Based Input and Initialization

-Reads road network data from a CSV file (road_network.csv).
-Reads vehicle data, including start and end points, from a CSV file (vehicles.csv).

6- Priority Queue for Road Management

-Prioritizes roads with higher vehicle density for signal management.

7- Visualization of Traffic and Signals

-Displays road usage statistics, vehicle states, and intersection signal timings in real-time.

8- Extensible and Modular Design

-Easily adaptable for additional features like multi-directional traffic, real-world data integration, and visualization tools.
