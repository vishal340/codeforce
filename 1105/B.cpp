#include <iostream>

using namespace std;

void solve() {
  long long n, m, r, c;
  cin >> n >> m >> r >> c;
  long long mod = 998244353;
  long long s = r * c - 1 + (m - c) * (r - 1) + (n - r) * (c - 1);
  long long ans = 1;
  long long base = 2;
  while (s > 0) {
    if (s % 2) {
      ans = (ans * base) % mod;
    }
    s /= 2;
    base = (base * base) % mod;
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
