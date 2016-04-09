#include <iostream>
#include <iomanip>
#include <vector>
#include <map>
#include <algorithm>
#include <boost/optional.hpp>

using namespace std;
using namespace boost;

#define NON_REACHABLE 100000

struct Edge {
  int departure_time;
  int arrival_time;
  int stop;
};

// NOTE:
//   - things to learn
//     - lambda expression
//     - algorithm library
//     - boost optional

optional< pair<int, int> > Solve(vector< vector<Edge> > graph,
                                 int start_time,
                                 int first_stop,
                                 int last_stop) {

  // TODO:
  //   - 1. and 2. can be squashed if we use `do while` ?

  // 1. initialize two arrays with `first_stop`
  // 2. loop until step (a) finds anything
  //   - (a) find `v` s.t. minimize `reachable_times[v]`
  //                          with  `v \in non_picked_verteces`
  //   - (b) update `reachable_times` with intermidiating `v` station
  //   - (c) remove v from non_picked_verteces
  // 3. if `last_stop \nin non_picked_verteces`
  //    then run backword to find laziest departure time
  //    else return None

  vector<int> reachable_times = vector<int>(graph.size(), NON_REACHABLE);
  vector<int> non_picked_verteces = vector<int>(graph.size());

  for (auto it = non_picked_verteces.begin();
       it != non_picked_verteces.end();
       it++) {
    *it = distance(non_picked_verteces.begin(), it);
  }

  // 1
  vector<int>::iterator pick_it =
    find(non_picked_verteces.begin(), non_picked_verteces.end(), first_stop);
  reachable_times[*pick_it] = start_time;
  for (vector<Edge>::iterator it = graph[*pick_it].begin();
       it !=  graph[*pick_it].end();
       it++) {
    if (
        reachable_times[*pick_it] <= (*it).departure_time &&
        (*it).arrival_time <= reachable_times[(*it).stop]
        ) {
      reachable_times[(*it).stop] = (*it).arrival_time;
    }
  }
  non_picked_verteces.erase(pick_it);

  // 2
  while (!non_picked_verteces.empty()) {
    // (a)
    vector<int>::iterator pick_it =
      min_element(non_picked_verteces.begin(), non_picked_verteces.end(),
                  [reachable_times](int i, int j) {
                    return (reachable_times[i] < reachable_times[j]);
                  });
    if (reachable_times[*pick_it] == NON_REACHABLE) { break; }

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
  }

  // 3
  if (find(non_picked_verteces.begin(), non_picked_verteces.end(), last_stop)
      == non_picked_verteces.end()) {
    // TODO: go backward to find laziest departure time
    int arrival_time = reachable_times[last_stop];
    return optional< pair<int, int> >(make_pair(0, arrival_time));
  } else {
    return optional< pair<int, int> >();
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
        edge.departure_time = departure_time;
        edge.stop = cities[next_stop];
        edge.arrival_time = arrival_time;
        graph[cities[stop]].push_back(edge);
        stop = next_stop;
        departure_time = arrival_time;
      }
    }

    int start_time;
    string first_stop;
    string last_stop;
    cin >> start_time >> first_stop >> last_stop;
    optional< pair<int, int> > answer = Solve(graph, start_time,
                                              cities[first_stop], cities[last_stop]);
    cout << "Scenario " << i_case << "\n";
    if (answer) {
      cout << "Departure " << setfill('0') << setw(4) << (*answer).first
           << " " << first_stop << "\n";
      cout << "Arrival   " << setfill('0') << setw(4) << (*answer).second
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
