#include <bits/stdc++.h>
using namespace std;

#define ll long long
const int N = 50;
void solve() {
  int i, n;
  cin >> n;
  if (n == 1) {
    cout << 1 << '\n';
    return;
  }
  if (n == 2) {
    cout << -1 << '\n';
    return;
  }
  cout << 1 << ' ' << 2 << ' ' << 3;
  ll c = 6;
  for (i = 4; i <= n; i++) {
    cout << ' ' << c;
    c *= 2;
  }
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
