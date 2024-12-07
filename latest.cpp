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
void realTimeMovement(Vehicle* vehicles, int vehicleCount, const string* paths, Graph& graph) {
    cout << "\nReal-Time Vehicle Movement:\n";

    int* currentPositions = new int[vehicleCount]();
    int* travelTimes = new int[vehicleCount]();
    bool* completed = new bool[vehicleCount]();

    int timeElapsed = 0;
    bool allCompleted = false;

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
                        currentPositions[i]++;
                    }
                }

                if (currentPositions[i] == path.length() - 1 && travelTimes[i] == 0) {
                    completed[i] = true;
                }
            }
        }

        if (timeElapsed % 5 == 0) {
            cout << "-At " << timeElapsed << "th second\n";
            for (int i = 0; i < vehicleCount; ++i) {
                if (!completed[i]) {
                    const string& path = paths[i];
                    char from = path[currentPositions[i]];
                    char to = path[currentPositions[i] + 1];
                    cout << vehicles[i].getId() << " is on road " << from << to
                         << " (remaining time: " << travelTimes[i] << " seconds)\n";
                } else {
                    cout << vehicles[i].getId() << " has reached its destination.\n";
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
    const char* roadFile = "C:\\Users\\HP\\Documents\\DS_PROJECT\\road_network.csv";
    readCSVAndBuildGraph(roadFile, roadMap);

    cout << "Road Map:" << endl;
    roadMap.displayGraph();

    int vehicleCount = 0;
    const char* vehicleFile = "C:\\Users\\HP\\Documents\\DS_PROJECT\\vehicles.csv";
    Vehicle* vehicles = readVehicles(vehicleFile, vehicleCount);

    if (vehicles) {
        cout << "\nVehicles List:" << endl;
        printVehicles(vehicles, vehicleCount);

        // Generate shortest paths for all vehicles
        string* paths = new string[vehicleCount];
        for (int i = 0; i < vehicleCount; ++i) {
            paths[i] = roadMap.findShortestPath(vehicles[i].getStart(), vehicles[i].getEnd());
        }

        // Real-time movement simulation
        realTimeMovement(vehicles, vehicleCount, paths, roadMap);

        // Cleanup
        delete[] paths;
        delete[] vehicles;
    }

    return 0;
}
