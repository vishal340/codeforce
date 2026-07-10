#include <bits/stdc++.h>
#include <utility>
using namespace std;

using vi = vector<int>;

void solve() {
  int i, n, k;
  cin >> n >> k;
  vi a(n);
  for (i = 0; i < n; i++) {
    cin >> a[i];
  }
  list<pair<int, int>> b;
  auto t = make_pair(a[0], 1);
  for (i = 1; i < n; i++) {
    if (a[i] == t.first) {
      t.second++;
    } else {
      b.push_back(t);
      t.first = a[i];
      t.second = 1;
    }
  }
  b.push_back(t);
  int ret = 0;
  bool changed = true;
  int c = b.size();
  while (c > 0) {
    if (((n == k) || (k > n && !((k - n) % c))) && changed) {
      ret++;
      changed = false;
    } else {
      int l = 0;
      auto it = b.begin();
      while (it != b.end()) {
        it->second--;
        if (it->second == 0) {
          changed = true;
          l++;
        }
        it++;
      }
      n -= c;
      if (l)
        b.remove_if([](const auto &x) { return x.second == 0; });
      c = b.size();
    }
  }
  cout << ret << '\n';
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
