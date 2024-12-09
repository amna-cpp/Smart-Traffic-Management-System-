# **Traffic Simulation System**  
**Developed by:**  
- **Amna Hameed**  
- **Faryal Siddique**  
- **Khadija Sohail**  

---  

## **Objective**  
In today's fast-paced world, efficient traffic management is critical for urban development. This project aims to simulate a **realistic, dynamic traffic system** that not only ensures smoother traffic flow but also prioritizes emergency scenarios. By integrating cutting-edge algorithms and data structures, this simulation models real-world challenges like congestion, vehicle prioritization, and adaptive signal management. **Let's dive into this amazing traffic simulation!**

---  

## **Overview**  
The **Traffic Simulation System** combines advanced computing techniques to replicate the complexities of traffic management. From **adaptive traffic signals** to **real-time congestion monitoring**, this simulation serves as a versatile framework for exploring urban traffic solutions.  

- **Emergency Vehicle Prioritization:** Smooth passage for emergency vehicles with dynamic signal overrides.  
- **Congestion Control:** Tracks and mitigates traffic bottlenecks using intelligent algorithms.  
- **Adaptive Road Network:** Responds dynamically to changes like blocked roads or vehicle surges.  

---  

## **Features**  
### **1. Vehicle Management**  
- **Dynamic Vehicle Updates:**  
  Tracks vehicles entering and exiting roads/intersections, dynamically updating congestion levels.  

- **Emergency Vehicle Prioritization:**  
  Guarantees smooth passage by overriding traffic signals and extending green durations.  

### **2. Traffic Signal Management**  
- **Adaptive Signal Timings:**  
  Calculates green and red durations based on real-time vehicle counts.  

- **Emergency Signal Overrides:**  
  Priority is given to emergency vehicles, ensuring uninterrupted travel.  

### **3. Road and Intersection Network**  
- **Graph Representation:**  
  The road network is modeled as a directed graph with:  
  - **Nodes:** Represent intersections.  
  - **Edges:** Represent roads, weighted by congestion levels (vehicle count).  

- **Real-Time Congestion Updates:**  
  Adjusts road weights dynamically based on vehicle movements.  

- **Priority-Based Road Selection:**  
  Uses a **priority queue** to optimize routing decisions and traffic flow.  

---  

## **Data Structures Used**  
1. **Graphs:**  
   - Represent the road network (intersections and roads).  
   - Enable efficient traversal for routing and congestion tracking.  

2. **Hash Tables:**  
   - Implemented in the `RoadMap` class for fast lookups of road and intersection data.  
   - Track vehicle counts and congestion levels in real-time.  

3. **Priority Queue:**  
   - Optimizes traffic signal handling and emergency vehicle routing.  
   - Ensures high-priority tasks (e.g., emergency scenarios) are addressed first.  

---  

## **How to Use**  
### **Simulation Menu**  
When running the simulation, you will encounter an interactive menu with the following options:  

1. **Add or Remove Vehicles:**  
   - Add vehicles to roads/intersections or remove them to simulate changes in traffic.  

2. **Start the "Traffic Frenzy" Simulation Loop:**  
   - Engage in the main loop, **"Traffic Frenzy,"** which dynamically updates vehicle movements, congestion levels, and signal states.  

3. **Display Main Road Network:**  
   - View the entire road network, including intersections, roads, and congestion levels.  

4. **Show Vehicle Destinations:**  
   - Track each vehicle's current destination and monitor their progress in real-time.  

5. **Monitor System Performance:**  
   - View real-time stats, such as road congestion levels, signal timings, and emergency vehicle efficiency.  

6. **Display Intersection Signals:**  
   - Check current intersection statuses, including:  
     - Vehicle count.  
     - Traffic signal state (GREEN/RED).  
     - Remaining green and red durations.  

---  

## **Algorithms Used**  

### **A* Search Algorithm**  
The **A* algorithm** is implemented for **emergency vehicles** to prioritize their movement across the network:  

- **Shortest Path:** Finds the fastest route by considering both distance and travel time.  
- **Congestion Awareness:** Avoids overcrowded routes by factoring in vehicle counts.  
- **Travel Time Optimization:** Ensures emergency vehicles take the most efficient paths.  

### **Dijkstra's Algorithm**  
Used for **regular vehicles**, ensuring efficient traffic flow:  

- **Shortest Path Calculation:** Determines the shortest path based on road travel times.  
- **Priority-Based Traversal:** Handles road updates dynamically using a **priority queue.**  
- **Efficiency:** Optimizes traffic flow by continuously adapting to congestion changes.  

---  

## **Code Architecture**  

### **Classes and Their Functions**  
- **Graph Class:**  
  Models the road network with intersections and roads. Includes functions for adding roads, finding paths, and managing vehicle movement.  

- **RoadMap Class:**  
  Handles the road network, intersection signals, and congestion tracking. Functions include adding roads, updating signal states, and managing vehicle counts.  

- **Vehicle Class:**  
  Represents vehicles with properties such as type (emergency/regular), current location, destination, and priority.  

- **realTimeMovement Function:**  
  Simulates vehicle movement across the network, updating routes dynamically and prioritizing emergencies.  

- **Main:**  
  The starting point for the simulation, initializing the system, processing user inputs, and running the simulation loop.  

---

## **Example Usage**  
Here’s how the simulation might look during interaction:  

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

---  

## **Future Enhancements**  
- **Accident and Road Closure Simulation:**  
  Model accidents and dynamically reroute vehicles around closed roads.  

- **Real-Time Data Integration:**  
  Incorporate data from external systems for real-world applicability.  

- **Enhanced Visualization:**  
  Introduce graphical outputs for a more engaging simulation experience.  

---  

## **Contributors**  
- **Amna Hameed:** Lead developer for signal management and dynamic updates.  
- **Faryal Siddique:** Developed road networks, vehicle management, and shortest path algorithms.  
- **Khadija Sohail:** Focused on blocked roads, rerouting vehicles, and implementing BFS algorithms.  

---  

**Join us on this thrilling journey to revolutionize traffic management! 🚦**
