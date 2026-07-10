#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int i, n;
  cin >> n;
  vi a(n);
  long long ans = 0;
  for (i = 0; i < n; i++) {
    cin >> a[i];
  }
  for (i = 0; i < n; i++) {
    if (ans <= a[i])
      ans = ans = a[i];
    else {
      ans += a[i];
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
