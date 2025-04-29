#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <omp.h>
using namespace std;

void parallelBFS(vector<vector<int>> &graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);
    queue<int> q;

    visited[start] = true;
    q.push(start);

    cout << "BFS Traversal: ";
    while (!q.empty()) {
        int node = q.front();
        q.pop();
        cout << node << " ";

        #pragma omp parallel for
        for (int i = 0; i < graph[node].size(); i++) {
            int neighbor = graph[node][i];
            if (!visited[neighbor]) {
                #pragma omp critical
                {
                    if (!visited[neighbor]) {
                        visited[neighbor] = true;
                        q.push(neighbor);
                    }
                }
            }
        }
    }
    cout << endl;
}

void parallelDFS(vector<vector<int>> &graph, int start) {
    int n = graph.size();
    vector<bool> visited(n, false);
    stack<int> s;

    s.push(start);

    cout << "DFS Traversal: ";
    while (!s.empty()) {
        int node = s.top();
        s.pop();

        if (!visited[node]) {
            visited[node] = true;
            cout << node << " ";

            #pragma omp parallel for
            for (int i = 0; i < graph[node].size(); i++) {
                int neighbor = graph[node][i];
                if (!visited[neighbor]) {
                    #pragma omp critical
                    {
                        if (!visited[neighbor]) {
                            s.push(neighbor);
                        }
                    }
                }
            }
        }
    }
    cout << endl;
}

int main() {
    int n, e;
    cout << "Enter number of nodes and edges: ";
    cin >> n >> e;

    vector<vector<int>> graph(n);
    cout << "Enter edges (u v) for undirected graph:\n";
    for (int i = 0; i < e; i++) {
        int u, v;
        cin >> u >> v;
        graph[u].push_back(v);
        graph[v].push_back(u);
    }

    int choice;
    int ss;
    do {
        cout << "\nMenu:\n1. BFS\n2. DFS\n3. Exit\nEnter choice: ";
        cin >> choice;
        if (choice == 1) {
            cout<<endl<<"Enter starting vextex: ";
            cin>>ss;
            parallelBFS(graph, ss);
        } else if (choice ==2) {
            parallelDFS(graph, 0);
        } else if (choice!=3) {
            cout << "Invalid choice. Try again.\n";
        }
            
    } while (choice != 3);

    return 0;
}
