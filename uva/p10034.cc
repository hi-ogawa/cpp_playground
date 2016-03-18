#include <iostream>
#include <iomanip>
#include <vector>
#include <cmath>
#include <map>

using namespace std;

double Distance(vector< pair<double, double> > points, int v, int u) {
  double dx = points[v].first - points[u].first;
  double dy = points[v].second - points[u].second;
  return sqrt(pow(dx, 2) + pow(dy, 2));
}

double Solve(vector< pair<double, double> > points) {

  vector<int> to_from(points.size());
  map<int, int> candidates;

  // 1. start from `points[0]`
  // 2. init candidates based on `points[0]`
  // 3. loop until there's no `candidates`
  //   a. pick closest `candidates`
  //   b. update `candidates` and `to_from` with picked one
  // 4. take sum of `to_from` edges

  // 1, 2
  to_from[0] = -1; // meaning points[0] is root
  for (unsigned v = 1; v < points.size(); v++) { // O(n*log(n))
    candidates.insert(make_pair(v, 0));
  }

  // 3.
  while (!candidates.empty()) { // O(n)
    // a.
    map<int, int>::iterator pick_it, it;
    pick_it = it = candidates.begin();
    for (it++; it != candidates.end(); it++) { // O(n)
      if (Distance(points, (*it).first, (*it).second) <
          Distance(points, (*pick_it).first, (*pick_it).second)
          ) {
        pick_it = it;
      }
    }
    // b.
    for (it = candidates.begin(); it != candidates.end(); it++) { // O(n)
      if (Distance(points, (*it).first, (*pick_it).first) <
          Distance(points, (*it).first, (*it).second)
          &&
          pick_it != it
          ) {
        it->second = (*pick_it).first; // O(1)
      }
    }
    to_from[(*pick_it).first] = (*pick_it).second;
    candidates.erase(pick_it); // O(log(n))
  }

  // 4.
  double sum = 0;
  for (unsigned v = 1; v < points.size(); v++) {
    sum += Distance(points, v, to_from[v]);
  }
  return sum;
}

int main() {

  int num_cases;
  cin >> num_cases;

  for (int i_case = 1; i_case <= num_cases; i_case++) {

    int num_points;
    cin >> num_points;

    vector< pair<double, double> > points;

    for (int i_point = 0; i_point < num_points; i_point++) {
      double x, y;
      cin >> x >> y;
      points.push_back(make_pair(x, y));
    }

    cout << fixed << setprecision(2) << Solve(points) << "\n";

    if (i_case != num_cases) {
      cout << "\n";
    }
  }
  return 0;
}
