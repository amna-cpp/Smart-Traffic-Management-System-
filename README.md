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

---

### **Code Architecture**  
### **Classes and Structures**  
1. **`RoadMap` Class**:  
   - Core class managing roads and intersections using graphs and hash tables.  
   - Synchronizes road congestion data with intersection states.  

2. **`RoadNode` Structure**:  
   - Represents a road between two intersections, holding vehicle count and next road node reference.  

3. **`IntersectionNode` Structure**:  
   - Represents an intersection, including the number of vehicles waiting, signal state, and signal timing.  

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

--- 

Let me know if you need further modifications or additional details!
