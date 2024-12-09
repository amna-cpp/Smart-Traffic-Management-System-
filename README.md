# **Traffic Simulation System**  
**Developed by:**  
- Amna Hameed  
- Faryal Siddique  
- Khadija Sohail  

---

## **Overview**  
This project simulates a dynamic traffic management system that leverages advanced data structures to model roads, intersections, and vehicle movement. The system ensures efficient traffic flow by dynamically updating road congestion, prioritizing vehicles, and managing adaptive traffic signals.

---

## **Features**  
### **1. Vehicle Management**  
- **Dynamic Vehicle Updates**:  
  Tracks vehicles entering and exiting roads and intersections, updating counts in real time.  

- **Emergency Vehicle Prioritization**:  
  Emergency vehicles override signals and extend green durations to ensure unimpeded movement.  

### **2. Traffic Signal Management**  
- **Adaptive Signal Timings**:  
  Green and red signal durations are calculated based on real-time vehicle counts.  

- **Emergency Signal Overrides**:  
  Priority is given to emergency vehicles by dynamically changing signals to green.  

### **3. Road and Intersection Network**  
- **Graph Representation**:  
  The road network is modeled as a directed graph with:  
  - **Nodes**: Represent intersections.  
  - **Edges**: Represent roads, weighted by congestion levels (vehicle count).  

- **Real-Time Congestion Updates**:  
  Vehicle movements dynamically adjust road weights to reflect congestion.  

- **Priority-Based Road Selection**:  
  Roads and signals are prioritized using a **priority queue**, ensuring optimal vehicle routing and management.  

---

## **Data Structures Used**  
1. **Graphs**:  
   - Represent the road network with intersections as nodes and roads as edges.  
   - Enable efficient traversal and updates for routing and congestion tracking.  

2. **Hash Tables**:  
   - Used in the `RoadMap` class to quickly look up road and intersection data.  
   - Tracks vehicle counts to determine real-time congestion levels.  

3. **Priority Queue**:  
   - Used to prioritize road selection and traffic signal management.  
   - Ensures emergency vehicles and critical intersections are handled with higher priority.  

---

## **How to Use**  
### **Simulation Menu**  
When running the simulation, you will be presented with a menu that offers several options for managing the simulation. Here’s a list of available options:

1. **Add or Remove Vehicles**:  
   - Add vehicles to specific roads or intersections using a simple input prompt.  
   - Remove vehicles to simulate congestion reduction or vehicle exit from the system.

2. **Start the "Traffic Frenzy" Simulation Loop**:  
   - Begin the main simulation loop, dubbed **"Traffic Frenzy"**. This loop dynamically manages vehicle movement, congestion updates, and traffic signal changes.  
   - The loop will run continuously, updating road conditions, vehicle counts, and intersection signal statuses until stopped manually.

3. **Display Main Road Network**:  
   - View the road network with all intersections and their respective connections, showing which roads are currently congested based on vehicle counts.

4. **Show Vehicle Destinations**:  
   - Display the current destination of each vehicle in the system. This provides a snapshot of where vehicles are headed, helping with route optimizations and future planning.

5. **Monitor System Performance**:  
   - Check real-time performance statistics, such as road congestion levels, signal cycle times, and emergency vehicle movement efficiency.  
   - This helps track the overall effectiveness of the traffic management system and its adaptability to different conditions.

6. **Display Intersection Signals**:  
   - View the current state of all intersections, including:
     - Vehicle count.
     - Traffic signal status (GREEN or RED).
     - Remaining green and red times.

 ### **Algorithms Used**

#### **A* Search Algorithm**  
The **A* search algorithm** is employed for emergency vehicles in this simulation. It is an advanced version of Dijkstra's algorithm, which not only considers the shortest path but also incorporates heuristics (estimates) to guide the search process efficiently. The A* algorithm is particularly useful for emergency vehicles because it accounts for:

- **Shortest Path**: It finds the fastest route from the starting intersection to the destination, minimizing travel time.
- **Congestion Awareness**: It considers real-time congestion (vehicle count) on each road, which helps in avoiding overcrowded routes.
- **Travel Time Optimization**: In addition to distance, A* incorporates travel time on roads, ensuring that emergency vehicles take the most optimal route during critical moments.

By using A* for emergency vehicles, the simulation ensures that they receive priority and can navigate the road network as quickly as possible, even in congested conditions.

#### **Dijkstra's Algorithm**  
For regular vehicles, **Dijkstra's algorithm** is used to determine the shortest path through the road network. It is highly effective in graphs where edge weights (travel times) vary, and it efficiently handles the following:

- **Shortest Path Calculation**: Dijkstra’s algorithm calculates the shortest path between intersections based solely on road travel times.
- **Graph-Based Search**: It uses a **priority queue** to prioritize roads based on the minimum accumulated travel time, ensuring vehicles take the most efficient route available.
- **Dynamic Updates**: As the road network changes, Dijkstra continuously finds the best path for vehicles, considering updated travel times due to congestion or changes in traffic signals.

By utilizing Dijkstra's algorithm, the system ensures that regular vehicles are routed in the most efficient manner, minimizing delays and optimizing traffic flow across the network. 

Both algorithms are integral to the smooth operation of the traffic simulation, helping to manage emergency vehicle priority while optimizing travel for regular vehicles.

---

### **Code Architecture**  
### **Classes and Their Functions**

- **Graph Class**: Represents the road network as a graph, with intersections and roads. It includes functions to add roads, find shortest paths, and manage vehicle movement across intersections.

- **RoadMap Class**: Manages the road network and intersection signals. Functions include adding roads, managing vehicle counts, updating intersection signals, calculating green/red times, and handling emergency vehicle signal overrides.

- **Vehicle Class**: Represents vehicles in the simulation. It stores vehicle properties such as type (emergency/regular), current location, destination, and priority. It handles routing based on vehicle type.

- **realTimeMovement Function**: Simulates the movement of vehicles in real-time. It updates the positions of vehicles, adjusts their routes based on congestion and signal states, and handles emergency vehicle prioritization.

- **Main**: The entry point of the simulation. It initializes the simulation, sets up the road network, processes user inputs through the menu, and runs the main simulation loop (e.g., updating vehicle movements and traffic signals).
---

### **Example Usage**  
Below is an example flow of how the simulation might look when interacting with the menu:

```text
Welcome to the Traffic Simulation System!
------------------------------------------
1. Add or Remove Vehicles
2. Start "Traffic Frenzy" Simulation
3. Display Main Road Network
4. Show Vehicle Destinations
5. Monitor System Performance
6. Display Intersection Signals
------------------------------------------

Choose an option (1-6):
```

Depending on the selected option, the user can interact with the simulation and view real-time traffic data and system status.  

---

## **Conclusion**  
The **Traffic Simulation System** is a robust tool for modeling and managing traffic flow in a dynamic environment. By leveraging **graphs**, **hash tables**, and **priority queues**, the system efficiently handles vehicle management, congestion control, and adaptive signal management.

**Future Enhancements**
Accident and road closure simulations.
Real-time data input from external systems.
Enhanced visualization with graphical output.
**Contributors**
Amna Hameed: Lead developer for signal management and dynamic updates , traffic signals and emergency vehicle updates 
Faryal Siddique: Designed emergency handling and adaptive timing logic.
Khadija Sohail: Initial making of road networks , vehicles management shortest path algorithms 

