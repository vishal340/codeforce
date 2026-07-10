#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int i, n;
  cin >> n;
  vi a(n);
  for (i = 0; i < n; i++) {
    cin >> a[i];
  }
  vector<bool> one(n, 0);
  int acc = 0;
  for (i = 0; i < n; i++) {
    if (a[i] == 1)
      acc++;
    else {
      acc--;
    }
    if (acc >= 0)
      one[i] = true;
  }
  acc = 0;
  for (i = n - 2; i >= 1; i--) {
    if (a[i] == 3) {
      acc--;
      ;
    } else {
      acc++;
    }
    if (acc < 0) {
      acc = 0;
    } else {
      if (one[i - 1]) {
        cout << "YES\n";
        return;
      }
    }
  }
  cout << "NO\n";
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
