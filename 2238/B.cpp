#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int n;
  cin >> n;
  long long ans = 0;
  for (int i = 1; i <= n; i++) {
    ans += (long long)(n / i) * (n / i);
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
