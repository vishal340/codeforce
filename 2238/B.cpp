#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int n;
  cin >> n;
  vi a(n);
  set<int> b;
  for (int i = 0; i < n; i++)
    cin >> a[i];
  for (int i = 0; i < n; i++) {
    int x;
    cin >> x;
    b.insert(x);
  }
  vi c(n, 0);
  for (int i = 0; i < n; i++) {
    auto it = b.lower_bound(a[i]);
    if (it != b.end()) {
      c[i] = *it;
      b.erase(it);
    } else {
      cout << -1 << '\n';
      return;
    }
  }
  long long ans = 0;
  for (int i = 0; i < n; i++) {
    for (int j = i + 1; j < n; j++) {
      if (c[i] > c[j])
        ans++;
    }
  }
  cout << ans << '\n';
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
