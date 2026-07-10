#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int i, n;
  cin >> n;
  vi a(n);
  int c = 0;
  for (i = 0; i < n; i++) {
    cin >> a[i];
  }
  for (i = 0; i < n; i++) {
    if (a[i] > 1) {
      c++;
    }
    if (a[i] > 2) {
      c++;
    }
    if (c > 1) {
      cout << "YES\n";
      return;
    }
  }
  cout << "NO\n";
  return;
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
