#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
#include<string>
#include<sstream>
using namespace std;

class Graph {
    struct Road {
        char intersection;
        int length;
        Road* next;
    };

    struct Intersection {
        char intersection;
        Road* head;
    };

    Intersection* adjLists;
    int intersectionCount;
    int capacity;

    int findIndex(char intersection) const {
        for (int i = 0; i < intersectionCount; ++i) {
            if (adjLists[i].intersection == intersection)
                return i;
        }
        return -1;
    }

    Road* createRoad(char intersection, int length) {
        Road* newRoad = new Road;
        newRoad->intersection = intersection;
        newRoad->length = length;
        newRoad->next = nullptr;
        return newRoad;
    }

    void resizeIfNeeded() {
        if (intersectionCount == capacity) {
            capacity = (capacity == 0) ? 1 : capacity * 2;
            Intersection* newAdjLists = new Intersection[capacity];
            for (int i = 0; i < intersectionCount; ++i) {
                newAdjLists[i] = adjLists[i];
            }
            delete[] adjLists;
            adjLists = newAdjLists;
        }
    }

public:
    Graph() : adjLists(nullptr), intersectionCount(0), capacity(0) {}

    ~Graph() {
        for (int i = 0; i < intersectionCount; ++i) {
            Road* current = adjLists[i].head;
            while (current) {
                Road* toDelete = current;
                current = current->next;
                delete toDelete;
            }
        }
        delete[] adjLists;
    }

    void addIntersection(char intersection) {
        if (findIndex(intersection) == -1) {
            resizeIfNeeded();
            adjLists[intersectionCount].intersection = intersection;
            adjLists[intersectionCount].head = nullptr;
            intersectionCount++;
        }
    }

    void addRoad(char from, char to, int length) {
        int fromIndex = findIndex(from);
        if (fromIndex == -1) {
            addIntersection(from);
            fromIndex = findIndex(from);
        }

        Road* newRoad = createRoad(to, length);
        newRoad->next = adjLists[fromIndex].head;
        adjLists[fromIndex].head = newRoad;
    }

    void displayGraph() const {
        for (int i = 0; i < intersectionCount; ++i) {
            cout << "Intersection " << adjLists[i].intersection << ": ";
            Road* temp = adjLists[i].head;
            while (temp) {
                cout << "-> (To: " << temp->intersection << ", Travel Time: " << temp->length << " mins) ";
                temp = temp->next;
            }
            cout << endl;
        }
    }

    // Dijkstra's Algorithm
    string findShortestPath(char start, char end) const {
    int* distances = new int[intersectionCount];
    bool* visited = new bool[intersectionCount];
    char* predecessors = new char[intersectionCount];
    string path;

    for (int i = 0; i < intersectionCount; ++i) {
        distances[i] = 99999999; // Replacing INT_MAX
        visited[i] = false;
        predecessors[i] = '\0';
    }

    int startIndex = findIndex(start);
    distances[startIndex] = 0;

    for (int i = 0; i < intersectionCount; ++i) {
        int minDistance = 99999999, minIndex = -1;
        for (int j = 0; j < intersectionCount; ++j) {
            if (!visited[j] && distances[j] < minDistance) {
                minDistance = distances[j];
                minIndex = j;
            }
        }

        if (minIndex == -1) break;

        visited[minIndex] = true;

        Road* current = adjLists[minIndex].head;
        while (current) {
            int neighborIndex = findIndex(current->intersection);
            if (!visited[neighborIndex] && distances[minIndex] + current->length < distances[neighborIndex]) {
                distances[neighborIndex] = distances[minIndex] + current->length;
                predecessors[neighborIndex] = adjLists[minIndex].intersection;
            }
            current = current->next;
        }
    }

    int endIndex = findIndex(end);
    if (distances[endIndex] == 99999999) {
        cout << "No path exists from " << start << " to " << end << "." << endl;
        // Try to find a different route, for example, by trying other neighboring nodes
        // You can implement retry logic or fallback paths here, depending on your graph structure.
        return "No Path";
    } else {
        // Backtrack to find the path
        char tempPath[intersectionCount];
        int pathLength = 0;
        for (char at = end; at != '\0'; at = predecessors[findIndex(at)]) {
            tempPath[pathLength++] = at;
        }

        for (int i = pathLength - 1; i >= 0; --i) {
            path += tempPath[i];
        }
    }

    delete[] distances;
    delete[] visited;
    delete[] predecessors;
    return path;
}

    int getTravelTime(char from, char to) const {
        int fromIndex = findIndex(from);
        if (fromIndex == -1) return 0;

        Road* current = adjLists[fromIndex].head;
        while (current) {
            if (current->intersection == to) {
                return current->length;
            }
            current = current->next;
        }
        return 0;
    }

   string findFastestPathAStar(char start, char end) {
    char openSet[100]; // Store open set (we'll use a simple list for this)
    char cameFrom[100]; // To store the path
    int gScore[100]; // Actual travel cost to each node
    int fScore[100]; // Estimated total cost from start to goal

    for (int i = 0; i < intersectionCount; ++i) {
        gScore[i] = 99999999; // Initialize to large value
        fScore[i] = 99999999;
    }

    int startIndex = findIndex(start);
    gScore[startIndex] = 0;
    fScore[startIndex] = calculateHeuristic(start, end);

    int openSetSize = 0;
    openSet[openSetSize++] = start;

    while (openSetSize > 0) {
        // Find the node with the lowest fScore
        int currentIndex = -1;
        int lowestFScore = 99999999;
        for (int i = 0; i < openSetSize; ++i) {
            int idx = findIndex(openSet[i]);
            if (fScore[idx] < lowestFScore) {
                lowestFScore = fScore[idx];
                currentIndex = idx;
            }
        }

        char current = openSet[currentIndex];
        if (current == end) {
            // Reconstruct the path
            string path = "";
            while (current != start) {
                path = current + path;
                current = cameFrom[findIndex(current)];
            }
            path = start + path;
            return path;
        }

        // Remove current from open set
        openSet[currentIndex] = openSet[--openSetSize];

        // Check neighbors
        Road* currentRoad = adjLists[findIndex(current)].head;
        while (currentRoad) {
            int tentativeGScore = gScore[findIndex(current)] + currentRoad->length;
            int neighborIndex = findIndex(currentRoad->intersection);
            if (tentativeGScore < gScore[neighborIndex]) {
                cameFrom[neighborIndex] = current;
                gScore[neighborIndex] = tentativeGScore;
                fScore[neighborIndex] = gScore[neighborIndex] + calculateHeuristic(currentRoad->intersection, end);
                openSet[openSetSize++] = currentRoad->intersection;
            }
            currentRoad = currentRoad->next;
        }
    }

    // If no path is found, return a fallback message
    cout << "No path exists from " << start << " to " << end << "." << endl;
    return "No Path";
}

    int calculateHeuristic(char startIntersection, char endIntersection) {
        return 0 ;
    }


};

class Vehicle {
private:
    char vehicle[100];
    char start;
    char end;
    bool isEmergency; // New boolean for emergency vehicles
    int priority;     // New priority field (low or high)

public:
    Vehicle() : start('\0'), end('\0'), isEmergency(false), priority(0) {
        vehicle[0] = '\0';
    }

    Vehicle(const char* vehicleId, char startIntersection, char endIntersection, bool isEmergencyVehicle) 
        : start(startIntersection), end(endIntersection), isEmergency(isEmergencyVehicle), priority(isEmergencyVehicle ? 1 : 0) {
        int i = 0;
        while (vehicleId[i] != '\0') {
            vehicle[i] = vehicleId[i];
            ++i;
        }
        vehicle[i] = '\0';
    }

    const char* getId() const { return vehicle; }
    char getStart() const { return start; }
    char getEnd() const { return end; }
    bool getIsEmergency() const { return isEmergency; }
    int getPriority() const { return priority; }

    void setPriority(int newPriority) { priority = newPriority; }

    void print() const {
        cout << "Vehicle: " << vehicle 
             << ", Start: " << start 
             << ", End: " << end 
             << ", Priority: " << priority 
             << ", Is Emergency: " << (isEmergency ? "Yes" : "No") 
             << endl;
    }
};


// Function to load vehicles into a single array
void loadVehicles(const char* fileName, Vehicle* vehicles, int& vehicleCount) {
    ifstream file(fileName);
    if (!file.is_open()) {
        cout << "Error opening file: " << fileName << endl;
        return;
    }

    // Skip the first line
    string line;
    if (!getline(file, line)) {
        cout << "Error reading the header line or file is empty." << endl;
        return;
    }

    while (getline(file, line)) {
        stringstream ss(line);
        string vehicleId, start, end;

        getline(ss, vehicleId, ',');  // Vehicle ID
        getline(ss, start, ',');      // Start intersection
        getline(ss, end, ',');        // End intersection

        // Determine if the vehicle is an emergency vehicle based on its ID
        bool isEmergency = (vehicleId.substr(0, 2) == "EV"); // IDs starting with "EV" are emergency vehicles

        // Set priority based on whether it's an emergency vehicle
        int priority = (isEmergency) ? 1 : 0; // 1 for emergency (high priority), 0 for regular (low priority)

        vehicles[vehicleCount] = Vehicle(vehicleId.c_str(), start[0], end[0], priority);
        vehicleCount++;
    }

    file.close();
}


void printVehicles(const Vehicle* vehicles, int count) {
    for (int i = 0; i < count; i++) {
        vehicles[i].print();
    }
}

void readCSVAndBuildGraph(const char* filename, Graph& graph) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        return;
    }

    char line[100];
    file.getline(line, 100); // Skip the header

    while (file.getline(line, 100)) {
        char from = line[0];
        char to = line[2];
        int travelTime = 0;
        for (int i = 4; line[i] != '\0'; ++i) {
            travelTime = travelTime * 10 + (line[i] - '0');
        }

        graph.addIntersection(from);
        graph.addIntersection(to);
        graph.addRoad(from, to, travelTime);
    }

    file.close();
}

class PriorityQueue {
    struct Node {
        char from;
        char to;
        int vehicleCount;
        Node* next;
    };

    Node* head;

public:
    PriorityQueue() : head(nullptr) {}

    ~PriorityQueue() {
        while (head) {
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void enqueue(char from, char to, int count) {
        Node* newNode = new Node{from, to, count, nullptr};
        if (!head || head->vehicleCount < count) {
            newNode->next = head;
            head = newNode;
        } else {
            Node* current = head;
            while (current->next && current->next->vehicleCount >= count) {
                current = current->next;
            }
            newNode->next = current->next;
            current->next = newNode;
        }
    }

    Node* dequeue() {
        if (!head) return nullptr;
        Node* temp = head;
        head = head->next;
        return temp;
    }

    bool isEmpty() const {
        return head == nullptr;
    }

    void displayQueue() const {
        Node* current = head;
        cout << "Priority Queue (Vehicle Count): ";
        while (current) {
            cout << "(" << current->from << current->to << ": " << current->vehicleCount << ") ";
            current = current->next;
        }
        cout << endl;
    }

    // Checks if the road from 'from' to 'to' is in the queue
    bool containsRoad(char from, char to) const {
        Node* current = head;
        while (current) {
            if (current->from == from && current->to == to) {
                return true;
            }
            current = current->next;
        }
        return false;
    }

    // Removes the road from 'from' to 'to' from the queue
    void removeRoad(char from, char to) {
        if (!head) return;

        if (head->from == from && head->to == to) {
            Node* temp = head;
            head = head->next;
            delete temp;
            return;
        }

        Node* current = head;
        while (current->next) {
            if (current->next->from == from && current->next->to == to) {
                Node* temp = current->next;
                current->next = current->next->next;
                delete temp;
                return;
            }
            current = current->next;
        }
    }
};

// In the RoadMap class, we add an additional method to ensure all intersections are printed

class RoadMap {
public:
    struct RoadNode {
        char from;
        char to;
        int vehicleCount;
        RoadNode* next;

        RoadNode(char f, char t, int c) : from(f), to(t), vehicleCount(c), next(nullptr) {}
    };

    struct IntersectionNode {
        char intersection;
        int vehicleCount;
        string signalState; // "GREEN" or "RED"
        int greenTime;      // Countdown for green signal
        int redTime;        // Countdown for red signal
        int greenTimeBackup; // Original green time duration
        int redTimeBackup;   // Original red time duration
        IntersectionNode* next;
        bool emergencyVehicle ; 

        IntersectionNode(char intersection, int count, int green, int red)
        : intersection(intersection),
        vehicleCount(count),
        signalState(determineInitialSignalState(count)), // Determine initial signal state
        greenTime(green),
        redTime(red),
        greenTimeBackup(green),
        redTimeBackup(red),
        next(nullptr) {}

        // Helper function to determine the initial signal state
        static std::string determineInitialSignalState(int vehicleCount) {
            if (vehicleCount >= 2) {
                return "GREEN";  // If there are 2 or more vehicles, start with green
            }
            return "RED";  // Otherwise, start with red
        }

        void setGreenTime(int newGreenTime) {
            greenTime = newGreenTime;
        }

        // Set new red time and update redTimeBackup
        void setRedTime(int newRedTime) {
            redTime = newRedTime;
        }
    };

private:
    RoadNode* head;
    IntersectionNode* intersectionHead;

public:
    RoadMap() : head(nullptr), intersectionHead(nullptr) {}

    // Add or increment a road
   void addOrIncrementRoad(char from, char to) {
    RoadNode* current = head;

    // Traverse the list to find if the road already exists
    while (current) {
        if (current->from == from && current->to == to) {
            current->vehicleCount++;
            return;
        }
        current = current->next;
    }

    // Add a new road node if not found
    if (from != '\0' && to != '\0') { // Ensure valid `from` and `to` values
        RoadNode* newNode = new RoadNode(from, to, 1);
        newNode->next = head;
        head = newNode;

        // Update intersection vehicle count for the destination intersection
        incrementIntersectionVehicleCount(to);
    }
}


    void decrementRoad(char from, char to) {
        RoadNode* current = head;
        while (current) {
            if (current->from == from && current->to == to) {
                if (current->vehicleCount > 0) {
                    current->vehicleCount--;
                }
                return;
            }
            current = current->next;
        }
    }

    // Increment the count of vehicles for a given intersection
    void incrementIntersectionVehicleCount(char intersection) {
        IntersectionNode* current = intersectionHead;
        while (current) {
            if (current->intersection == intersection) {
                current->vehicleCount++;
                return;
            }
            current = current->next;
        }

        // Calculate green and red times based on initial rules
        int greenTime = calculateGreenTime(1); // Default green time for 1 vehicle
        int redTime = calculateRedTime(1);

        IntersectionNode* newNode = new IntersectionNode(intersection, 1, greenTime, redTime);
        newNode->next = intersectionHead;
        intersectionHead = newNode;
    }

    // Print all intersection signals (including those with no vehicles)
    void printIntersectionSignals() {
        // Print all intersections A-Z
        for (char intersection = 'A'; intersection <= 'Z'; ++intersection) {
            IntersectionNode* current = intersectionHead;
            bool found = false;
            while (current) {
                if (current->intersection == intersection) {
                    found = true;
                    cout << "Intersection " << intersection
                         << " | Vehicles: " << current->vehicleCount
                         << " | Signal: " << current->signalState
                         << " | Green Time Left: " << current->greenTime
                         << "s | Red Time Left: " << current->redTime << "s\n";
                    break;
                }
                current = current->next;
            }

            // Default for intersections with no vehicles
            if (!found) {
                cout << "Intersection " << intersection
                     << " | Vehicles: 0 | Signal: GREEN | Green Time Left: 15s | Red Time Left: 5s\n";
            }
        }
    }

    IntersectionNode* findIntersection(char intersection) {
        IntersectionNode* current = intersectionHead;
        while (current) {
            if (current->intersection == intersection) {
                return current; // Found the intersection
            }
            current = current->next;
        }
        return nullptr; // Intersection not found
    }

    // Update intersection signals and manage countdowns
    void updateIntersectionSignals() {
        IntersectionNode* current = intersectionHead;
        while (current) {
            // Handle green signal
            if (current->signalState == "GREEN") {
                current->greenTime--;
                if (current->greenTime <= 0) {
                    // Reset green time based on vehicle count
                    current->greenTime = calculateGreenTime(current->vehicleCount);
                    current->redTime = calculateRedTime(current->vehicleCount);
                    current->signalState = "RED";  // Switch to red signal
                }
            }
            // Handle red signal
            else if (current->signalState == "RED") {
                current->redTime--;
                if (current->redTime <= 0) {
                    // Reset red time based on vehicle count
                    current->redTime = calculateRedTime(current->vehicleCount);
                    current->greenTime = calculateGreenTime(current->vehicleCount);
                    current->signalState = "GREEN";  // Switch to green signal
                }
            }
            current = current->next;
        }
    }

    // Calculate green time based on vehicle count at the intersection
    int calculateGreenTime(int vehicleCount) {
        if (vehicleCount >= 3) {
            return 45 + rand() % 6; // 45 to 50 seconds
        } else if (vehicleCount == 2) {
            return 30 + rand() % 6; // 30 to 35 seconds
        } else if (vehicleCount == 1) {
            return 15 + rand() % 11; // 15 to 25 seconds
        }
        return 15; // Default for no vehicles
    }

    // Calculate red time based on green time
    int calculateRedTime(int vehicleCount) {
        if (vehicleCount >= 3) {
            return 5 + rand() % 6; // 45 to 50 seconds
        } else if (vehicleCount == 2) {
            return 10 + rand() % 6; // 30 to 35 seconds
        } else if (vehicleCount == 1) {
            return 20 + rand() % 11; // 15 to 25 seconds
        }
        return 30;
    }

    void overrideTrafficSignal(char Intersection){
        IntersectionNode* emergencyIntersection = findIntersection(Intersection);
        emergencyIntersection->signalState = "GREEN" ; 
        emergencyIntersection->greenTime=emergencyIntersection->greenTime + 20; 
    }

    // Accessor for IntersectionNode head
    IntersectionNode* getIntersectionHead() const {
        return intersectionHead;
    }

    RoadNode* getHead() const {
        return head;
    }

    ~RoadMap() {
        RoadNode* current = head;
        while (current) {
            RoadNode* toDelete = current;
            current = current->next;
            delete toDelete;
        }

        IntersectionNode* intersectionCurrent = intersectionHead;
        while (intersectionCurrent) {
            IntersectionNode* toDelete = intersectionCurrent;
            intersectionCurrent = intersectionCurrent->next;
            delete toDelete;
        }
    }
};

void realTimeMovementWithSignals(Vehicle* vehicles, int vehicleCount, const string* paths, Graph& graph, Vehicle* EVehicles, int evCount) {
    cout << "\nReal-Time Vehicle Movement with Traffic Signal Management:\n";

    int* currentPositions = new int[vehicleCount + evCount]();  // Tracks the position of each vehicle (both regular and emergency)
    int* travelTimes = new int[vehicleCount + evCount]();       // Travel time for each vehicle
    bool* completed = new bool[vehicleCount + evCount]();       // Completion status for each vehicle
    RoadMap roadMap;

    int timeElapsed = 0;
    bool allCompleted = false;

    // Initialize the roadMap by tracking the roads taken by vehicles (both regular and emergency)
    for (int i = 0; i < vehicleCount; ++i) {
        const string& path = paths[i];
        if (path.length() > 1) {
            char from = path[0];
            char to = path[1];
            roadMap.addOrIncrementRoad(from, to);
        }
    }

    for (int i = vehicleCount; i < vehicleCount + evCount; ++i) {
        const string& path = paths[i];
        if (path.length() > 1) {
            char from = path[0];
            char to = path[1];
            roadMap.addOrIncrementRoad(from, to);
        }
    }

    // Main simulation loop
    while (!allCompleted) {
        allCompleted = true;

        // Update traffic signals at intersections
        RoadMap::IntersectionNode* intersectionCurrent = roadMap.getIntersectionHead();
        while (intersectionCurrent) {
            if (intersectionCurrent->signalState == "GREEN") {
                if (--intersectionCurrent->greenTime <= 0) {
                    intersectionCurrent->signalState = "RED";
                    intersectionCurrent->redTime = roadMap.calculateRedTime(intersectionCurrent->vehicleCount);
                }
            } else if (intersectionCurrent->signalState == "RED") {
                if (--intersectionCurrent->redTime <= 0) {
                    intersectionCurrent->signalState = "GREEN";
                    intersectionCurrent->greenTime = roadMap.calculateGreenTime(intersectionCurrent->vehicleCount);
                }
            }
            intersectionCurrent = intersectionCurrent->next;
        }

        // Process regular vehicles
        for (int i = 0; i < vehicleCount; ++i) {
            if (!completed[i]) {
                const string& path = paths[i];

                if (currentPositions[i] < path.length() - 1) {
                    char from = path[currentPositions[i]];
                    char to = path[currentPositions[i] + 1];

                    if (travelTimes[i] == 0) {
                        travelTimes[i] = graph.getTravelTime(from, to);
                    }

                    if (travelTimes[i] > 0) {
                        travelTimes[i]--;
                        allCompleted = false;  // Vehicle still moving
                    }

                    if (travelTimes[i] == 0 && currentPositions[i] < path.length() - 1) {
                        // Check the signal at the intersection
                        RoadMap::IntersectionNode* intersection = roadMap.findIntersection(to);
                        if (intersection) {
                            if (vehicles[i].getIsEmergency()) {
                                // Emergency vehicle bypasses red light
                                cout << "Emergency vehicle " << vehicles[i].getId() << " is overriding signals at intersection " << to << ".\n";
                                roadMap.overrideTrafficSignal(to);  // Override the signal at the intersection
                            } else if (intersection->signalState == "RED") {
                                // Non-emergency vehicles wait at red lights
                                cout << "The signal is red, Vehicle " << vehicles[i].getId() 
                                     << " is waiting at intersection " << to 
                                     << " | Wait time: " << intersection->redTime << " seconds.\n";
                                allCompleted = false;  // Vehicle stays stationary
                            }
                        }

                        if (intersection == nullptr || intersection->signalState == "GREEN" || vehicles[i].getIsEmergency()) {
                            // Move the vehicle to the next road (either the signal is green or it's an emergency vehicle)
                            roadMap.decrementRoad(from, to);
                            currentPositions[i]++;
                            if (currentPositions[i] < path.length() - 1) {
                                from = path[currentPositions[i]];
                                to = path[currentPositions[i] + 1];
                                roadMap.addOrIncrementRoad(from, to);
                            }
                        }
                    }
                }

                if (currentPositions[i] == path.length() - 1 && travelTimes[i] == 0) {
                    completed[i] = true;
                }
            }
        }

        // Process emergency vehicles (separate logic to avoid interference with regular vehicles)
        for (int i = 0; i < evCount; ++i) {
            if (!completed[vehicleCount + i]) {
                const string& path = paths[vehicleCount + i];

                if (currentPositions[vehicleCount + i] < path.length() - 1) {
                    char from = path[currentPositions[vehicleCount + i]];
                    char to = path[currentPositions[vehicleCount + i] + 1];

                    if (travelTimes[vehicleCount + i] == 0) {
                        travelTimes[vehicleCount + i] = graph.getTravelTime(from, to);
                    }

                    if (travelTimes[vehicleCount + i] > 0) {
                        travelTimes[vehicleCount + i]--;
                        allCompleted = false;  // Emergency vehicle still moving
                    }

                    if (travelTimes[vehicleCount + i] == 0 && currentPositions[vehicleCount + i] < path.length() - 1) {
                        // Check the signal at the intersection
                        RoadMap::IntersectionNode* intersection = roadMap.findIntersection(to);
                        if (intersection) {
                            // Emergency vehicles bypass red light
                            cout << "Emergency vehicle " << EVehicles[i].getId() << " is overriding signals at intersection " << to << ".\n";
                            roadMap.overrideTrafficSignal(to);  // Override the signal at the intersection
                        }

                        // Move the emergency vehicle to the next road
                        roadMap.decrementRoad(from, to);
                        currentPositions[vehicleCount + i]++;
                        if (currentPositions[vehicleCount + i] < path.length() - 1) {
                            from = path[currentPositions[vehicleCount + i]];
                            to = path[currentPositions[vehicleCount + i] + 1];
                            roadMap.addOrIncrementRoad(from, to);
                        }
                    }
                }

                if (currentPositions[vehicleCount + i] == path.length() - 1 && travelTimes[vehicleCount + i] == 0) {
                    completed[vehicleCount + i] = true;
                }
            }
        }

        // Print updates every 5 seconds
        if (timeElapsed % 5 == 0) {
            cout << "\n--- Traffic Signal Management at " << timeElapsed << " seconds ---\n";

            // Display vehicle count on roads
            RoadMap::RoadNode* roadCurrent = roadMap.getHead();
            while (roadCurrent) {
                cout << "Road " << roadCurrent->from << "->" << roadCurrent->to
                     << " | Vehicles: " << roadCurrent->vehicleCount << endl;
                roadCurrent = roadCurrent->next;
            }

            // Display intersection signal information (including A-Z)
            roadMap.printIntersectionSignals();

            // Display vehicle states
            cout << "\n\n\n";
            // Regular Vehicles:
            for (int i = 0; i < vehicleCount; ++i) {
                if (!completed[i]) {
                    char from = currentPositions[i] < paths[i].length() ? paths[i][currentPositions[i]] : '\0';
                    char to = (currentPositions[i] + 1) < paths[i].length() ? paths[i][currentPositions[i] + 1] : '\0';

                    if (travelTimes[i] > 0) {
                        cout << "Vehicle " << vehicles[i].getId() << " on road " << from << "->" << to
                             << " | Time left: " << travelTimes[i] << " seconds.\n";
                    } else if (to != '\0') {
                        cout << "Vehicle " << vehicles[i].getId() << " has reached intersection " << to << ".\n";
                    } else {
                        cout << "Vehicle " << vehicles[i].getId() << " has invalid road data. Please check the path.\n";
                    }
                } else {
                    cout << "Vehicle " << vehicles[i].getId() << " has reached its destination.\n";
                }
            }

            // Emergency Vehicles:
            for (int i = 0; i < evCount; ++i) {
                if (!completed[vehicleCount + i]) {
                    char from = currentPositions[vehicleCount + i] < paths[vehicleCount + i].length() ? paths[vehicleCount + i][currentPositions[vehicleCount + i]] : '\0';
                    char to = (currentPositions[vehicleCount + i] + 1) < paths[vehicleCount + i].length() ? paths[vehicleCount + i][currentPositions[vehicleCount + i] + 1] : '\0';

                    if (travelTimes[vehicleCount + i] > 0) {
                        cout << "Emergency Vehicle EV" << EVehicles[i].getId() << " on road " << from << "->" << to
                             << " | Time left: " << travelTimes[vehicleCount + i] << " seconds.\n";
                    } else if (to != '\0') {
                        cout << "Emergency Vehicle EV" << EVehicles[i].getId() << " has reached intersection " << to << ".\n";
                    } else {
                        cout << "Emergency Vehicle EV" << EVehicles[i].getId() << " has invalid road data. Please check the path.\n";
                    }
                } else {
                    cout << "Emergency Vehicle EV" << EVehicles[i].getId() << " has reached its destination.\n";
                }
            }
        }

        this_thread::sleep_for(chrono::seconds(1));
        timeElapsed++;
    }

    delete[] currentPositions;
    delete[] travelTimes;
    delete[] completed;
}


int main() {
    Graph roadMap;

    const char* roadFile = "road_network.csv";
    const char* vehicleFile = "vehicles.csv";
    const char* emergencyFile = "emergency_vehicles.csv";

    readCSVAndBuildGraph(roadFile, roadMap);

    cout << "Road Map:" << endl;
    roadMap.displayGraph();

    int vehicleCount = 0;      // Total count of regular vehicles
    int emergencyCount = 0;    // Total count of emergency vehicles

    int totalVehicles = 0;     // Combined count of vehicles (regular + emergency)
    Vehicle* allVehicles = new Vehicle[1000]; // Assume max 1000 vehicles for simplicity

    // Load vehicles and emergency vehicles into the combined array
    loadVehicles(vehicleFile, allVehicles, totalVehicles);
    loadVehicles(emergencyFile, allVehicles, totalVehicles);

    // Count regular and emergency vehicles
    for (int i = 0; i < totalVehicles; i++) {
        if (allVehicles[i].getIsEmergency()) {
            emergencyCount++;
        } else {
            vehicleCount++;
        }
    }

    // Allocate separate arrays for regular and emergency vehicles
    Vehicle* vehicles = new Vehicle[vehicleCount];
    Vehicle* emergencyVehicles = new Vehicle[emergencyCount];

    // Separate the vehicles into respective arrays
    int vIndex = 0, evIndex = 0;
    for (int i = 0; i < totalVehicles; i++) {
        if (allVehicles[i].getIsEmergency()) {
            emergencyVehicles[evIndex++] = allVehicles[i];
        } else {
            vehicles[vIndex++] = allVehicles[i];
        }
    }

    // Display vehicles
    cout << "\nRegular Vehicles:" << endl;
    printVehicles(vehicles, vehicleCount);

    cout << "\nEmergency Vehicles:" << endl;
    printVehicles(emergencyVehicles, emergencyCount);

    // Find shortest paths for each vehicle
    string* paths = new string[totalVehicles];
    for (int i = 0; i < totalVehicles; i++) {
        paths[i] = roadMap.findShortestPath(allVehicles[i].getStart(), allVehicles[i].getEnd());
        cout << "Vehicle " << allVehicles[i].getId() << " route: " << paths[i] << endl;
    }

    // Call real-time movement function
    realTimeMovementWithSignals(vehicles, vehicleCount, paths, roadMap, emergencyVehicles, emergencyCount);

    // Clean up memory
    delete[] paths;
    delete[] allVehicles;
    delete[] vehicles;
    delete[] emergencyVehicles;

    return 0;
}


