#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/optional.hpp>

using namespace std;
using namespace boost;

#define NON_REACHABLE 2401
#define NON_DEPARTABLE -1

struct Edge {
  int departure_time;
  int arrival_time;
  int stop;
};

// backward variant of `Solve`
int BackwardSolve(vector< vector<Edge> > graph,
                  int end_time,
                  int first_stop,
                  int last_stop) {

  vector<int> departable_times = vector<int>(graph.size(), NON_DEPARTABLE);
  vector<int> non_picked_verteces = vector<int>(graph.size());

  for (auto it = non_picked_verteces.begin();
       it != non_picked_verteces.end();
       it++) {
    *it = distance(non_picked_verteces.begin(), it);
  }

  // 1. loop

  // (a)
  vector<int>::iterator pick_it = find(non_picked_verteces.begin(),
                                       non_picked_verteces.end(),
                                       last_stop);
  departable_times[*pick_it] = end_time;

  while (true) {
    // (b)
    for (vector<Edge>::iterator it = graph[*pick_it].begin();
         it != graph[*pick_it].end();
         it++) {
      if (
          departable_times[*pick_it] >= (*it).arrival_time &&
          (*it).departure_time >= departable_times[(*it).stop]
          ) {
        departable_times[(*it).stop] = (*it).departure_time;
      }
    }

    // (c)
    non_picked_verteces.erase(pick_it);

    // (d)
    if (non_picked_verteces.empty()) { break; }
    pick_it =
      max_element(non_picked_verteces.begin(),
                  non_picked_verteces.end(),
                  [departable_times](int i, int j) {
                    return (departable_times[i] < departable_times[j]);
                  });
    if (departable_times[*pick_it] == NON_DEPARTABLE) { break; }
  }

  // 2
  return departable_times[first_stop];
}

optional<int> Solve(vector< vector<Edge> > graph,
                                 int start_time,
                                 int first_stop,
                                 int last_stop) {

  // 1. loop until step (d) finds nothing
  //   - (a) start with `first_stop` vertex
  //   - (b) update `reachable_time` with intermidiate vertex `pick_it`
  //   - (c) remove `pick_it` from `non_picked_verteces`
  //   - (d) find `v` s.t. minimize `reachable_times[v]`
  //                          with  `v \in non_picked_verteces`
  // 2. if `non_picked_verteces` includes `last_stop`
  //    then return None
  //    else run `BackwordSolve` to find laziest departure time (with almost same algorithm)

  vector<int> reachable_times = vector<int>(graph.size(), NON_REACHABLE);
  vector<int> non_picked_verteces = vector<int>(graph.size());

  for (auto it = non_picked_verteces.begin();
       it != non_picked_verteces.end();
       it++) {
    *it = distance(non_picked_verteces.begin(), it);
  }

  // 1. loop

  // (a) choose first vertex
  vector<int>::iterator pick_it = find(non_picked_verteces.begin(),
                                       non_picked_verteces.end(),
                                       first_stop);
  reachable_times[*pick_it] = start_time;

  while (true) {
    // (b)
    for (vector<Edge>::iterator it = graph[*pick_it].begin();
         it != graph[*pick_it].end();
         it++) {
      if (
          reachable_times[*pick_it] <= (*it).departure_time &&
          (*it).arrival_time <= reachable_times[(*it).stop]
          ) {
        reachable_times[(*it).stop] = (*it).arrival_time;
      }
    }

    // (c)
    non_picked_verteces.erase(pick_it);

    // (d)
    if (non_picked_verteces.empty()) { break; }
    pick_it =
      min_element(non_picked_verteces.begin(),
                  non_picked_verteces.end(),
                  [reachable_times](int i, int j) {
                    return (reachable_times[i] < reachable_times[j]);
                  });
    if (reachable_times[*pick_it] == NON_REACHABLE) { break; }
  }

  // 2
  if (find(non_picked_verteces.begin(), non_picked_verteces.end(), last_stop)
      != non_picked_verteces.end()) {
    return optional<int>();
  } else {
    return optional<int>(reachable_times[last_stop]);
  }
}

int main() {
  int num_cases;
  cin >> num_cases;

  for (int i_case = 0; i_case < num_cases; i_case++) {

    int num_cities;
    cin >> num_cities; // 2..100

    // read cities
    map<string, int> cities;
    for (int i = 0; i < num_cities; i++) {
      string city_name;
      cin >> city_name;
      cities.insert(make_pair(city_name, i));
    }

    // read train description
    vector< vector<Edge> > graph(num_cities, vector<Edge>());
    vector< vector<Edge> > backward_graph(num_cities, vector<Edge>());
    int num_train_descs;
    cin >> num_train_descs; // ..1000
    for (int i = 0; i < num_train_descs; i++) {
      int num_stops; // 2..100
      cin >> num_stops;
      int departure_time;
      string stop;
      cin >> departure_time >> stop;
      for (int j = 1; j < num_stops; j++) {
        int arrival_time;
        string next_stop;
        cin >> arrival_time >> next_stop;
        Edge edge;
        Edge backward_edge;
        edge.departure_time = backward_edge.departure_time = departure_time;
        edge.arrival_time = backward_edge.arrival_time = arrival_time;
        edge.stop = cities[next_stop];
        backward_edge.stop = cities[stop];
        graph[cities[stop]].push_back(edge);
        backward_graph[cities[next_stop]].push_back(backward_edge);
        stop = next_stop;
        departure_time = arrival_time;
      }
    }

    int start_time;
    string first_stop;
    string last_stop;
    cin >> start_time >> first_stop >> last_stop;

    optional<int> o_arrival_time = Solve(graph, start_time,
                                       cities[first_stop], cities[last_stop]);
    int departure_time;
    if (o_arrival_time) {
      departure_time = BackwardSolve(backward_graph, *o_arrival_time,
                                     cities[first_stop], cities[last_stop]);
    }

    cout << "Scenario " << i_case + 1 << "\n";
    if (o_arrival_time) {
      cout << "Departure " << setfill('0') << setw(4) << departure_time
           << " " << first_stop << "\n";
      cout << "Arrival   " << setfill('0') << setw(4) << *o_arrival_time
           << " " << last_stop << "\n";
    } else {
      cout << "No connection" << "\n";
    }
    if (i_case + 1 != num_cases) {
      cout << "\n";
    }
  }
  return 0;
}
