#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int i, n, l, r;
  cin >> l >> r >> n;
  vector<bool> a(n, 0);
  a[0] = 1;
  int p1 = 0, t = l, p2 = 0;
  while (t) {
    t >>= 1;
    p1++;
  }
  t = r;
  while (t) {
    t >>= 1;
    p2++;
  }
  p2--;
  if (p1 > p2) {
    for (i = p1; i < n; i += p1)
      a[i] = 1;
    t = p1 - 1;
    for (i = p1 - 2; i >= 0; i--) {
      if (r & (1 << i)) {
        if (!(l & (1 << i))) {
          int t1 = l & (-(1 << t));
          break;
        }
        for (int j = p1 - 1 - i; j < n; j += p1)
          a[j] = 1;
        t = i;
      }
    }
  } else if (p1 == p2) {
    for (int j = (p2 + 1) * p1; j < n; j += (p2 + 1) * p1)
      a[j] = 1;
    for (i = p1 - 2; i >= 0; i--) {
      if (l & (1 << i)) {
        for (int j = (p2 + 1) * (p1 - 1 - i); j < n; j += (p2 + 1) * p1)
          a[j] = 1;
      }
    }
  } else {
    for (i = p2 * (p2 + 1); i < n; i += p2 * (p2 + 1)) {
      a[i] = 1;
    }
  }
  for (i = 0; i < n; i++)
    cout << a[i];
  cout << '\n';
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
