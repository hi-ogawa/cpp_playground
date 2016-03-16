#include <iostream>
#include <vector>

using namespace std;

#define MAXSIZE 10000

static bool bool_array[MAXSIZE][MAXSIZE];
static int int_array[MAXSIZE][MAXSIZE];

void InitBoolArray(int l, int w) {
  for (int i = 0; i <= l; i++) {
    for (int j = 0; j <= w; j++) {
      bool_array[i][j] = true;
    }
  }
}

void Preprocess(int l, int w) {
  for (int j = 0; j <= w; j++) {
    for (int i = 0; i <= l; i++) {
      if (bool_array[i][j]) {
        int_array[i][j] = 1;
        // ignore boundaries: j <- 1 .. (w - 1)
        if (j > 1) {
          int_array[i][j] += int_array[i][j - 1];
        }
      } else {
        int_array[i][j] = 0;
      }
    }
  }
}


int Solve(int l, int w) {
  if (l == 1 || w == 1) {
    return max(l, w);
  }
  Preprocess(l, w);

  int mx = 0;
  // ignore boundaries: 1 .. (w - 1) and 1 .. (l - 1)
  for (int j = 1; j <= w - 1; j++) {

    vector< pair<int, int> > stack;
    stack.push_back(make_pair(0, 0));

    // ad-hoc: loop 1 .. l to deal with remaining stack
    int_array[l][j] = 0;
    for (int i = 1; i <= l; i++) {
      if (stack.back().first > int_array[i][j]) {
        int current_width = stack.back().first;
        pair<int, int> width_idx;
        do {
          stack.pop_back();
          width_idx = stack.back();
          if (width_idx.first < current_width) {
            mx = max(mx, (i - width_idx.second) * (current_width + 1));
            current_width = width_idx.first;
          }
        } while (stack.back().first > int_array[i][j]);
        mx = max(mx, (i - width_idx.second) * (current_width + 1));
      }
      stack.push_back(make_pair(int_array[i][j], i));
    }
  }
  return mx;
}

int main() {

  int num_cases;
  cin >> num_cases;

  for (int i_case = 0; i_case < num_cases; i_case++) {

    int l, w;
    cin >> l >> w;

    InitBoolArray(l, w);

    int k, x, y, dx, dy;
    do {
      cin >> k;

      switch (k) {
      case 0:
        break;
      case 1:
        cin >> x >> y;
        bool_array[x][y] = false;
        break;
      default:
        cin >> x >> y >> dx >> dy;
        for (int u = 0; u < k; u++) {
          bool_array[x + u * dx][y + u * dy] = false;
        }
      }
    } while (k != 0);

    cout << Solve(l, w) << "\n";
  }

  return 0;
}
