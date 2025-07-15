#include <float.h>
#include <iomanip>
#include <fstream>
#include "directedGraph.h"
#include "myStack.h"

using namespace std;

ifstream fin("input.txt");


//the struct responsible for creating the tasks
struct Task {
  unsigned int TaskID;
  float workload;
  float deadline;
};


//the struct responsible for creating the cores
struct Core {
  float CoreSpeed, PowerRate;
};

//a struct created for handling the strongly connected components with one or 2 nodes
struct pairing {
  int first, second;
}scc[1001];


// struct which help us to manage easily the output
struct output {
   unsigned int node;
   int core;
   float start, end, finalEnergy;
}myOutput[1001];


/*
 * N- the number of nodes
 * M - the number of dependencies
 * K - the number of cores
 */
unsigned int N, M, K;
/*
 * The number of elements of the output vector
 */
int cntOut;

bool visited_g[1001] = {false}, visited_gt[1001] = {false}, visited_gt1[1001] = {false};
Task tasks[1001];
Core cores[2001];
int mappingNodes[2001], mappingNodes_reverse[2001], components[2001];
int nodesCycle[1001];
int c;

Stack<unsigned int> S, S1;
Graph<unsigned int, float> *G = NULL;
Graph<unsigned int, float> *Gt = NULL;
int no_nodes, currentComp;

/*Here are 2 dfs which help us to determine the strongly connected components of our graph (Kosaraju's algorithm)
 */

//A basic dfs
void dfs_g (int start) {
  visited_g[start] = 1;

  for (int i = 1; i <= N; i++) {
    if (visited_g[i] == 0 && G->A[start][i] == 1)  {
      dfs_g(i);
    }
  }

  S.push(start);
  S1.push(start);
}

//A dfs on the transpose graph Gt.
//Gt is a graph which has all the dependencies in an opposite way that G has
void dfs_gt (int start) {
  visited_gt[start] = 1;
  no_nodes++;
  components[start] = currentComp;

  for (int i = 1; i <= N; i++) {
    if (visited_gt[i] == 0 && Gt->A[start][i] == 1)  {
      dfs_gt(i);
    }
  }
}

//Here we redo a part from Kosaraju's algorithm in order to store in a vector all the nodes from each scc
void dfs_gt1 (int start) {
  visited_gt1[start] = 1;
  nodesCycle[++c] = start;
  components[start] = currentComp;

  for (int i = 1; i <= N; i++) {
    if (visited_gt1[i] == 0 && Gt->A[start][i] == 1)  {
      dfs_gt1(i);
    }
  }
}

//Function which gives as the execution time for each node and core
float executionTime (float wkload, float crspeed) {
  return wkload /crspeed;
}

//Function which gives as the energy consumed for each node and core
float energyConsumed (float execTime, float CorePowRate) {
  return execTime * CorePowRate;
}

//Here we search for the 2 nodes scc the optimal cores
void wantedCore (int node1, int node2, int &wantedCore1, int &wantedCore2) {
  float minimumEnergy = 1e8;

  //we first consider that we don t have any good core
  wantedCore1 = -1;
  wantedCore2 = -1;

  //we take any pair of 2 cores and we test if they are suitable for an optimal combination
  //we have to search if the execution time for both cores  satisfy the deadline the tasks have
  //and also if the energy consmed is minimum
  for (int i = 0; i < K; i++) {
    int j = 0;
    while (j < K) {
      if (i == j) {
        j++;
      } else {
        float execTime1 = executionTime(tasks[node1].workload, cores[i].CoreSpeed);
        float execTime2 = executionTime(tasks[node2].workload, cores[j].CoreSpeed);
        float energy1 = energyConsumed(execTime1, cores[i].PowerRate);
        float energy2 = energyConsumed(execTime2, cores[j].PowerRate);
        float sum = energy1 + energy2;

        if (sum < minimumEnergy && execTime1 <= tasks[node1].deadline && execTime2 <= tasks[node2].deadline) {
          minimumEnergy = sum;
          wantedCore1 = i;
          wantedCore2 = j;
        }

        j++;
      }
    }
  }
}

bool ok = true;

//This is the function which schedule the nodes if the graph is a cyclic one
//We ll take 2 cases: if we have a single node in the scc and if we have 2 nodes in the scc
//we take every scc and we verify which condition it fullfils
void schedule() {
  float energy = 0.0;
  float freeCore[1001] = {0.0},af1[1001] = {0.0},af2[1001] = {0.0},af3[1001] = {0.0},af4[1001] = {0.0};

  cntOut++;

  cout << "---Scheduling Results---" << endl;

  if (ok == false) {
    cout << "Scheduling failed. No solution found that meets all deadlines" << endl;
    return;
  }

  //the first case: a single node in the scc
  for (int i = 1; i <= currentComp; i++) {
    if (scc[i].first != -1 && scc[i].second == -1)
    {
      int node = scc[i].first;
      int wantedCore = -1;
      float startingPoint = 0.0;
      float minimumEnergy = 1e8;
      float endingPoint = 0.0;
      float minimum_start = 1e8;

      //we search for each single node in the cyclic graph which core forms along with the node the most efficient combination
      for (int j = 0; j < K; j++) {
        float execTime = executionTime(tasks[node].workload, cores[j].CoreSpeed);
        float energyCons = energyConsumed(execTime, cores[j].PowerRate);
        float maxim = freeCore[j];
        float start = maxim;
        float end = start + execTime;

        //here we verify the constraints
        if (end <= tasks[node].deadline) {
          if (energyCons < minimumEnergy || (energyCons == minimumEnergy && start < minimum_start)) {
            minimumEnergy = energyCons;
            wantedCore = j;
            startingPoint = start;
            minimum_start = start;
            endingPoint = end;
          }
        }
      }

      //Now we verify the results
      if (wantedCore == -1) {
        cout << "Scheduling failed. No solution found that meets all deadlines" << endl;
        ok = false;
      } else {
        af3[i] = startingPoint;
        af4[i] = endingPoint;
        myOutput[cntOut].node = tasks[node].TaskID;
        myOutput[cntOut].core = wantedCore;
        myOutput[cntOut].start = af3[i];
        myOutput[cntOut].end = af4[i];
        myOutput[cntOut].finalEnergy = minimumEnergy;
        cntOut++;
        freeCore[wantedCore] = endingPoint;
        energy += minimumEnergy;
      }
    }
  }

  //Time for the second case: we search the cores for the nodes which are part of a scc of size 2
  for (int i = 1; i <= currentComp; i++) {
    int node1 = scc[i].first;
    int node2 = scc[i].second;

    if (node2 == -1) {
      continue;
    }

    int wantedCore1 = -1;
    int wantedCore2 = -1;

    //here is the function where we search the most suitable core
    wantedCore(node1, node2, wantedCore1, wantedCore2);

    //if we find the most suitable cores, we create the output vector
    if (wantedCore1 != -1 && wantedCore2 != -1) {

      // Determine the starting points for both nodes based on the current available time on their respective cores
      float startingPoint1 = freeCore[wantedCore1];
      float startingPoint2 = freeCore[wantedCore2];

      // Calculate the total ending points without considering dependencies
      float endingPoint1_total = startingPoint1 + executionTime(tasks[node1].workload, cores[wantedCore1].CoreSpeed);;
      float endingPoint2_total = startingPoint2 + executionTime(tasks[node2].workload, cores[wantedCore2].CoreSpeed);

      // Determine the actual ending points considering potential dependencies
      // The start time is set as the maximum of the two starting points to handle dependencies correctly
      float endingPoint1 = max(startingPoint2,startingPoint1) + executionTime(tasks[node1].workload, cores[wantedCore1].CoreSpeed);
      float endingPoint2 = max(startingPoint2,startingPoint1) + executionTime(tasks[node2].workload, cores[wantedCore2].CoreSpeed);

      // Calculate the energy consumption for both nodes based on their execution duration and power rate
      float energy1 = energyConsumed(endingPoint1_total - startingPoint1, cores[wantedCore1].PowerRate);
      float energy2 = energyConsumed(endingPoint2_total - startingPoint2, cores[wantedCore2].PowerRate);

      // Update the freeCore array with the new ending points to keep track of core availability
      // Update auxiliary arrays to store start and end times for output purposes
      freeCore[wantedCore1] = endingPoint1;
      af1[i] = endingPoint1;
      af2[i] = endingPoint2;
      af3[i] = max(startingPoint2,startingPoint1);
      af4[i] = max(startingPoint2,startingPoint1);
      freeCore[wantedCore2] = endingPoint2;

      // Accumulate the energy consumption for the entire schedule
      energy += energy1 + energy2;
    } else {
      //If there are no cores found, we output a failure message
      cout << "Scheduling failed. No solution found that meets all deadlines" << endl;
      ok = false;
    }
  }

  //for each component with 2 nodes, we create the output vector
  for (int i = 1; i <= currentComp; i++) {
    int node1 = scc[i].first;
    int node2 = scc[i].second;

    if (node2 == -1) continue;

    int wantedCore1 = -1;
    int wantedCore2 = -1;
    wantedCore(node1, node2, wantedCore1, wantedCore2);

    if (wantedCore1 != -1 && wantedCore2 != -1) {
      float startingPoint1 = freeCore[wantedCore1];
      float endingPoint1 = startingPoint1 + executionTime(tasks[node1].workload, cores[wantedCore1].CoreSpeed);

      float startingPoint2 = freeCore[wantedCore2];
      float endingPoint2 = startingPoint2 + executionTime(tasks[node2].workload, cores[wantedCore2].CoreSpeed);

      float energy1 = energyConsumed(endingPoint1 - startingPoint1, cores[wantedCore1].PowerRate);
      float energy2 = energyConsumed(endingPoint2 - startingPoint2, cores[wantedCore2].PowerRate);

      myOutput[cntOut].node = tasks[node1].TaskID;
      myOutput[cntOut].core = wantedCore1;
      myOutput[cntOut].start = af3[i];
      myOutput[cntOut].end = af1[i];
      myOutput[cntOut].finalEnergy = energy1;
      cntOut++;

      myOutput[cntOut].node = tasks[node2].TaskID;
      myOutput[cntOut].core = wantedCore2;
      myOutput[cntOut].start = af4[i];
      myOutput[cntOut].end = af2[i];
      myOutput[cntOut].finalEnergy = energy2;
      cntOut++;

      freeCore[wantedCore1] = endingPoint1;
      freeCore[wantedCore2] = endingPoint2;

    }
  }

  //If we found a core for each node, we'll proceed to display the output in the proper format
  if (ok) {
    cntOut--;
    cout << "The total minimum energy is: " << fixed << setprecision(2) << energy << endl;
    cout << "Schedule (one optimal):" << endl;
    cout << " Task | Core | Start | End   | Energy" << endl;
    cout << "------|------|-------|-------|--------" << endl;
    for (int i = 1; i <= cntOut; i++) {
      cout << "    " << myOutput[i].node <<" |    " << myOutput[i].core << " |  " << fixed << setprecision(2) <<  myOutput[i].start << " |  " << fixed << setprecision(2) << myOutput[i].end<< " |  " << fixed << setprecision(2) << myOutput[i].finalEnergy << endl;
    }
  }
}

//Now, time to schedule the acyclic graphs

bool viz[100001];
Stack<int> S2;

//In order to do this, we'll do a topological sorting in order to arrange each node in function of its dependencies
void topSort () {
  bool viz[100001] = {false};

  for (int i = 1; i <= N; i++) {
    if (!viz[i]) {
      Stack<int> S_top;
      S_top.push(i);

      while (!S_top.isEmpty()) {
        int node = S_top.peek();

        if (!viz[node]) {
          viz[node] = true;
        }

        for (int j = 1; j <= N; j++) {
          if (!viz[j] && G->A[j][node] == 1) {
            S_top.push(j);
          }
        }

        //order[ind--] = node;
        S2.push(node);

        S_top.pop();
      }
    }
  }
}

//We'll proceed to schedule the task in a similar way like we did when we scheduled the nodes from the cyclic graphs
//The difference consists in the fact that we'll respect the order given by the topological sorting, not the sequentially order of the nodes
void schedule2() {
  float energy = 0.0;
  float freeCore[1001] = {0.0};
  float endTimes[1001] = {0.0};
  cntOut = 1;
  topSort();

  for (int i = 0; i < N; i++) {
    int node = S2.peek();
    S2.pop();
    float maxDependencyEndTime = 0.0;

    float minEnergy = FLT_MAX;
    int wantedCore = -1;
    float startTime = 0.0, endTime = 0.0;

    for (int j = 0; j < K; j++) {
      float execTime = executionTime(tasks[node].workload, cores[j].CoreSpeed);
      float energyCons = energyConsumed(execTime, cores[j].PowerRate);
      float start = max(freeCore[j], maxDependencyEndTime);
      float end = start + execTime;

      if (end <= tasks[node].deadline && (energyCons < minEnergy || (energyCons == minEnergy && start < startTime))) {
        minEnergy = energyCons;
        wantedCore = j;
        startTime = start;
        endTime = end;
      }
    }

    if (wantedCore != -1) {
      freeCore[wantedCore] = endTime;
      endTimes[node] = endTime;
      energy += minEnergy;
      myOutput[cntOut].node = tasks[node].TaskID;
      myOutput[cntOut].core = wantedCore;
      myOutput[cntOut].start = startTime;
      myOutput[cntOut].end = endTime;
      myOutput[cntOut].finalEnergy = minEnergy;
      cntOut++;
    } else {
      ok = false;
      cout << "Scheduling failed. No solution found that meets all deadlines" << endl;
    }
  }

  if (ok) {
    cntOut--;
    cout << "The total minimum energy is: " << fixed << setprecision(2) << energy << endl;
    cout << "Schedule (one optimal):" << endl;
    cout << " Task | Core | Start | End   | Energy" << endl;
    cout << "------|------|-------|-------|--------" << endl;
    for (int i = 1; i <= cntOut; i++) {
      cout << "    " << myOutput[i].node <<" |    " << myOutput[i].core << " |  " << fixed << setprecision(2) <<  myOutput[i].start << " |  " << fixed << setprecision(2) << myOutput[i].end<< " |  " << fixed << setprecision(2) << myOutput[i].finalEnergy << endl;
    }
  }
}

int main () {
  int cnt = 1;

  fin >> N;
  if (N <= 0 || N > 1000) {
    cerr << "Error: Number of tasks (N) must be between 1 and 1000." << endl;
    return 1;
  }

  G = new Graph<unsigned int, float>(N);
  Gt = new Graph<unsigned int, float>(N);

  //We read from the input file all the tasks
  for (int i = 1; i <= N; i++) {
    fin >> tasks[i].TaskID >> tasks[i].workload >> tasks[i].deadline;
    if (tasks[i].workload <= 0) {
      cerr << "Error: Task " << i << " has non-positive workload." << endl;
      return 1;
    }
    if (tasks[i].deadline <= 0) {
      cerr << "Error: Task " << i << " has non-positive deadline." << endl;
      return 1;
    }
    //we'll reorder each task by giving a number between 1 and N
    mappingNodes[tasks[i].TaskID] = cnt++;
    mappingNodes_reverse[cnt - 1] = tasks[i].TaskID;
  }

  fin >> M;
  if (M < 0 || M > 2000) {
    cerr << "Error: Number of dependencies (M) must be between 0 and 2000." << endl;
    return 1;
  }

  for (int i = 0; i < M; i++) {
    int node1, node2;
    fin >> node1 >> node2;
    if (mappingNodes[node1] == 0 || mappingNodes[node2] == 0) {
      cerr << "Error: Invalid task ID in dependency: " << node1 << " -> " << node2 << endl;
      return 1;
    }
    if (node1 == node2) {
      cerr << "Error: Task " << node1 << " cannot depend on itself." << endl;
      return 1;
    }
    //We use the mapping made previously when we create the graph
    node1 = mappingNodes[node1];
    node2 = mappingNodes[node2];
    //We make all the graphs G and G transpose
    G->addEdge(node1, node2);
    Gt->addEdge(node2, node1);
  }

  fin >> K;
  if (K <= 0 || K > 2000) {
    cerr << "Error: Number of cores K must be between 1 and 2000" << endl;
    return 1;
  }

  //We read each core
  for (int i = 0; i < K; i++) {
    fin >> cores[i].CoreSpeed >> cores[i].PowerRate;
    if (cores[i].CoreSpeed <= 0) {
      cerr << "Error: Core " << i << " has non positive speed" << endl;
      return 1;
    }
    if (cores[i].PowerRate <= 0) {
      cerr << "Error: Core " << i << " has non positive power rate." << endl;
      return 1;
    }
  }

  //We apply Kosaraju in order to find if the graph is acyclic or not

  for (int i = 1; i <= N; i++) {
    if (!visited_g[i]) {
      dfs_g(i);
    }
  }


  //We verify the cyclicity of the graph
  while (!S.isEmpty()) {
    int node = S.peek();
    S.pop();

    if (!visited_gt[node]) {
      no_nodes = 0;
      currentComp++;
      dfs_gt(node);

      if (no_nodes == 1) {
        scc[currentComp].first = node;
        scc[currentComp].second = -1;
      }
      else if (no_nodes == 2) {
        int node1 = -1;
        int node2 = -1;
        for (int i = 1; i <= N; i++) {
          if (components[i] == currentComp) {
            if (node1 == -1) {
              node1 = i;
            }
            else {
              node2 = i;
            }
          }
        }

        scc[currentComp].first = node1;
        scc[currentComp].second = node2;
      }
      else if (no_nodes > 2) {
        ok = false;
      }
    }
  }


  //if the graph is cyclic (the number of scc is less than the number of nodes)
  //we'll do again Kosaraju and we will display a warning which tells us that we have cycles
  //After that we'll call the schedule function in order to search for suitable combinations
  if (currentComp < N) {
    schedule();
    for (int i = 1; i <= N; i++) {
      visited_gt[i] = 0;
    }

    while (!S1.isEmpty()) {
      int node = S1.peek();
      S1.pop();

      if (!visited_gt[node]) {
        no_nodes = 0;
        currentComp++;
        dfs_gt(node);


        if (no_nodes != 1) {
          c = 0;
          cerr << "Warning: Cycle detected involving tasks (1-based ID): { ";
          for (int i = 1; i <= N; i++) {
            visited_gt1[i] = 0;
          }

          dfs_gt1(node);

          for (int i = 1; i < no_nodes; i++) {
            cerr << nodesCycle[i] << ", ";
          }

          cerr << nodesCycle[no_nodes] << " }" << endl;
        }
      }
    }
  } else {
    //If the graph is acyclic we'll do directly the schedulling
    schedule2();
  }

  return 0;
}