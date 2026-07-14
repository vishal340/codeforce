#include <algorithm>
#include <bits/stdc++.h>
using namespace std;

#define ll long long

const int N = 2e5;
array<int, N + 1> a, b;

void solve() {
  int i, n;
  cin >> n;
  map<int, int> cur;
  cur[0] = 0;
  for (i = 1; i <= n; i++) {
    char t1;
    int t2;
    cin >> t1 >> t2;
    if (t1 == 'p') {
      a[i] = t2;
      b[i] = -1;
    } else {
      b[i] = t2;
      a[i] = -1;
    }
  }
  for (i = 1; i <= N; i++) {
    if (b[i] > 0) {
      b[i] = ceil(sqrt(2 * b[i])) - 1;
    } else if (b[i] > -1) {
      b[i] = 0;
    }
  }
  for (i = 1; i <= N; i++) {
    if (a[i] != -1) {
      cur[a[i]] = 1;
    }
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int t;
  cin >> t;
  while (t--)
    solve();

  return 0;
}
