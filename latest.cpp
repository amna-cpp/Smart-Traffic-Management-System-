#include <iostream>
#include <fstream>
#include <chrono>
#include <thread>
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

};

class Vehicle {
private:
    char vehicle[20];
    char start;
    char end;

public:
    Vehicle() : start('\0'), end('\0') { vehicle[0] = '\0'; }
    Vehicle(const char* vehicleId, char startIntersection, char endIntersection) {
        int i = 0;
        while (vehicleId[i] != '\0') {
            vehicle[i] = vehicleId[i];
            ++i;
        }
        vehicle[i] = '\0';
        start = startIntersection;
        end = endIntersection;
    }

    const char* getId() const { return vehicle; }
    char getStart() const { return start; }
    char getEnd() const { return end; }

    void print() const {
        cout << "Vehicle: " << vehicle << ", Start: " << start << ", End: " << end << endl;
    }
};

Vehicle* readVehicles(const char* filename, int& count) {
    ifstream file(filename);
    if (!file.is_open()) {
        cout << "Failed to open file: " << filename << endl;
        count = 0;
        return nullptr;
    }

    char line[100];
    file.getline(line, 100); // Skip the header

    int capacity = 10;
    Vehicle* vehicles = new Vehicle[capacity];
    count = 0;

    while (file.getline(line, 100)) {
        if (count == capacity) {
            capacity *= 2;
            Vehicle* temp = new Vehicle[capacity];
            for (int i = 0; i < count; i++) {
                temp[i] = vehicles[i];
            }
            delete[] vehicles;
            vehicles = temp;
        }

        char vehicleId[20], startIntersection, endIntersection;
        //the overrr
        sscanf(line, "%[^,],%c,%c", vehicleId, &startIntersection, &endIntersection);
        vehicles[count++] = Vehicle(vehicleId, startIntersection, endIntersection);
    }

    file.close();
    return vehicles;
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
        IntersectionNode* next;

        IntersectionNode(char intersection, int count) : intersection(intersection), vehicleCount(count), next(nullptr) {}
    };

private:
    RoadNode* head;
    IntersectionNode* intersectionHead;

public:
    RoadMap() : head(nullptr), intersectionHead(nullptr) {}

    // Add or increment a road
    void addOrIncrementRoad(char from, char to) {
        RoadNode* current = head;
        while (current) {
            if (current->from == from && current->to == to) {
                current->vehicleCount++;
                return;
            }
            current = current->next;
        }

        // Add new road node if not found
        RoadNode* newNode = new RoadNode(from, to, 1);
        newNode->next = head;
        head = newNode;

        // Update intersection vehicle count for incoming intersection
        incrementIntersectionVehicleCount(to);
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

        IntersectionNode* newNode = new IntersectionNode(intersection, 1);
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
                    int greenTime = calculateGreenTime(current->vehicleCount);
                    cout << "Intersection " << intersection
                         << " | Vehicles: " << current->vehicleCount
                         << " | Green Signal: " << greenTime << " seconds.\n";
                    break;
                }
                current = current->next;
            }
            
            // If no vehicles coming to this intersection, set to minimum green signal time (15 seconds)
            if (!found) {
                cout << "Intersection " << intersection
                     << " | Vehicles: 0"
                     << " | Green Signal: 15 seconds.\n";
            }
        }
    }

    // Calculate green time based on vehicle count at the intersection
    int calculateGreenTime(int vehicleCount) {
        if (vehicleCount >= 3) {
            return 45 + rand() % 6;  // 45 to 50 seconds
        } else if (vehicleCount == 2) {
            return 30 + rand() % 6;  // 30 to 35 seconds
        } else if (vehicleCount == 1) {
            return 15 + rand() % 11;  // 15 to 25 seconds
        }
        return 15;  // Default for no vehicles
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




class TrafficSignalManager {
    PriorityQueue queue;

public:
    void addRoadToQueue(char from, char to, int vehicleCount) {
        queue.enqueue(from, to, vehicleCount);
    }

    void manageSignals(int simulationTime, RoadMap& roadMap) {
        cout << "--- Traffic Signal Management ---\n";
        RoadMap::RoadNode* current = roadMap.getHead();
        while (current) {
            int greenTime = calculateGreenTime(current->vehicleCount);
            cout << "Road " << current->from << "->" << current->to
                 << " | Vehicles: " << current->vehicleCount
                 << " | Green Signal: " << greenTime << " seconds.\n";

            current = current->next;
        }
        cout << endl;
    }

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
};


void realTimeMovementWithSignals(Vehicle* vehicles, int vehicleCount, const string* paths, Graph& graph, TrafficSignalManager& signalManager) {
    cout << "\nReal-Time Vehicle Movement with Traffic Signal Management:\n";

    int* currentPositions = new int[vehicleCount]();
    int* travelTimes = new int[vehicleCount]();
    bool* completed = new bool[vehicleCount]();
    RoadMap roadMap;

    int timeElapsed = 0;
    bool allCompleted = false;

    // Initialize the roadMap by tracking the roads taken by vehicles
    for (int i = 0; i < vehicleCount; ++i) {
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
                        allCompleted = false;
                    }

                    if (travelTimes[i] == 0 && currentPositions[i] < path.length() - 1) {
                        roadMap.decrementRoad(from, to);
                        currentPositions[i]++;
                        if (currentPositions[i] < path.length() - 1) {
                            from = path[currentPositions[i]];
                            to = path[currentPositions[i] + 1];
                            roadMap.addOrIncrementRoad(from, to);
                        }
                    }
                }

                if (currentPositions[i] == path.length() - 1 && travelTimes[i] == 0) {
                    completed[i] = true;
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
            for (int i = 0; i < vehicleCount; ++i) {
                if (!completed[i]) {
                    char from = paths[i][currentPositions[i]];
                    char to = paths[i][currentPositions[i] + 1];
                    cout << "Vehicle " << vehicles[i].getId() << " on road " << from << "->" << to
                         << " | Time left: " << travelTimes[i] << " seconds.\n";
                } else {
                    cout << "Vehicle " << vehicles[i].getId() << " has reached its destination.\n";
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

// Your main function remains unchanged except for function calls as needed
int main() {
    Graph roadMap;
    const char* roadFile = "road_network.csv";
    readCSVAndBuildGraph(roadFile, roadMap);

    cout << "Road Map:" << endl;
    roadMap.displayGraph();

    int vehicleCount = 0;
    const char* vehicleFile = "vehicles.csv";
    Vehicle* vehicles = readVehicles(vehicleFile, vehicleCount);

    if (vehicles) {
        cout << "\nVehicles List:" << endl;
        printVehicles(vehicles, vehicleCount);

        string* paths = new string[vehicleCount];
        for (int i = 0; i < vehicleCount; ++i) {
            paths[i] = roadMap.findShortestPath(vehicles[i].getStart(), vehicles[i].getEnd());
        }

        TrafficSignalManager signalManager;
        realTimeMovementWithSignals(vehicles, vehicleCount, paths, roadMap, signalManager);

        delete[] paths;
        delete[] vehicles;
    }

    return 0;
}
