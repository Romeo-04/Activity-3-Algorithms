#include <iostream>
#include <vector>
#include <stack>
#include <set>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream> // Added for file handling

using namespace std;

class TaskScheduler {
private:
    int N, M; // Number of tasks and dependencies
    vector<vector<int>> adj; // Adjacency list for the graph
    vector<bool> visited;
    vector<bool> onStack;
    stack<int> topoStack;
    bool hasCycle;
    vector<int> sortedOrder; // Store sorted order for saving

public:
    TaskScheduler(int numTasks, int numDependencies) {
        N = numTasks;
        M = numDependencies;
        adj.resize(N);
        visited.resize(N, false);
        onStack.resize(N, false);
        hasCycle = false;
    }

    // Function to generate a random DAG
    void generateRandomDAG() {
        do {
            adj.assign(N, vector<int>()); // Clear previous graph
            visited.assign(N, false);
            onStack.assign(N, false);
            hasCycle = false;

            srand(time(0));
            set<pair<int, int>> edges;

            while (edges.size() < M) {
                int u = rand() % N;
                int v = rand() % N;
                if (u != v && edges.find({v, u}) == edges.end()) { // Avoid self-loops and back edges
                    adj[u].push_back(v);
                    edges.insert({u, v});
                }
            }

        } while (detectCycle()); // Keep generating until a valid DAG is created

        // Print generated dependencies
        cout << "Generated Random Dependency Graph:\n";
        for (int i = 0; i < N; i++) {
            cout << "Task " << i << " -> ";
            for (int j : adj[i]) {
                cout << j << " ";
            }
            cout << "\n";
        }
    }

    // Check if a cycle is present
    bool detectCycle() {
        visited.assign(N, false);
        onStack.assign(N, false);
        hasCycle = false;

        for (int i = 0; i < N; i++) {
            if (!visited[i]) {
                dfsCycleCheck(i);
                if (hasCycle) return true;
            }
        }
        return false;
    }

    // DFS for cycle detection
    void dfsCycleCheck(int node) {
        visited[node] = true;
        onStack[node] = true;

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfsCycleCheck(neighbor);
            } else if (onStack[neighbor]) { // Cycle detected
                hasCycle = true;
                return;
            }
        }

        onStack[node] = false;
    }

    // DFS-based Topological Sorting
    void dfs(int node) {
        visited[node] = true;
        onStack[node] = true;

        cout << "Processing Task " << node << "\n";

        for (int neighbor : adj[node]) {
            if (!visited[neighbor]) {
                dfs(neighbor);
            }
        }

        onStack[node] = false;
        topoStack.push(node);
    }

    // Perform topological sorting
    void topologicalSort() {
        cout << "\nStarting Topological Sorting:\n";
        sortedOrder.clear();
        visited.assign(N, false);
        onStack.assign(N, false);

        for (int i = 0; i < N; i++) {
            if (!visited[i]) {
                dfs(i);
            }
        }

        cout << "\nTopological Order of Execution:\n";
        while (!topoStack.empty()) {
            int task = topoStack.top();
            cout << "Task " << task << " ";
            sortedOrder.push_back(task);
            topoStack.pop();
        }
        cout << "\n";

        // Save the results to a file
        saveResultsToFile();
    }

    // Save results to a file
    void saveResultsToFile() {
        ofstream outFile("task_scheduling_results.txt", ios::app);
        if (!outFile) {
            cout << "Error opening file for writing!" << endl;
            return;
        }

        outFile << "=====================================\n";
        outFile << "Task Scheduling Attempt:\n";
        outFile << "Number of Tasks: " << N << ", Number of Dependencies: " << M << "\n";
        outFile << "Adjacency List:\n";

        for (int i = 0; i < N; i++) {
            outFile << "Task " << i << " -> ";
            for (int j : adj[i]) {
                outFile << j << " ";
            }
            outFile << "\n";
        }

        outFile << "Topological Order of Execution:\n";
        for (int task : sortedOrder) {
            outFile << "Task " << task << " ";
        }
        outFile << "\n";
        outFile << "=====================================\n\n";

        outFile.close();
        cout << "Results have been saved to 'task_scheduling_results.txt'\n";
    }
};

int main() {
    int N, M;
    char choice;

    do {
        tryAgain1:
        cout << "Enter the value of N: ";

        if (!(cin >> N)) {
            cout << "Invalid input. Please enter a valid integer." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits <streamsize>::max(), '\n'); // Discard invalid input
            goto tryAgain1; // prompt again
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

        tryAgain2:
        cout << "Enter the value of M: ";

        if (!(cin >> M)) {
            cout << "Invalid input. Please enter a valid integer." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits <streamsize>::max(), '\n'); // Discard invalid input
            goto tryAgain2; // prompt again
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

        try {
            if (N <= 0 || M < 0) throw invalid_argument("Invalid number of tasks or dependencies!");

            TaskScheduler scheduler(N, M);
            scheduler.generateRandomDAG(); // This ensures DAG is valid
            scheduler.topologicalSort();
        }
        catch (const exception &e) {
            cout << "Exception: " << e.what() << endl;
        }

        tryAgain3:
        cout << endl << "Do you want to try another? [y for yes and press any key to exit]: ";

        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a valid choice." << endl;
            cin.clear(); // Clear the error flag
            cin.ignore(numeric_limits <streamsize>::max(), '\n'); // Discard invalid input
            goto tryAgain3; //prompt again
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << endl;

    } while (choice == 'y' || choice == 'Y');

    cout << "Exiting the program\n";
    return 0;
}
