#include <bits/stdc++.h>
using namespace std;

#define ll long long
const int N = 50;
void solve() {
  int i, n;
  cin >> n;
  for (i = 1; i <= n - 2; i += 2) {
    cout << i + 1 << ' ' << i << ' ';
  }
  cout << n << ' ' << n - 1 << '\n';
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
