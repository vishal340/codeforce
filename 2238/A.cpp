#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int n;
  cin >> n;
  vi vec(n);
  for (int i = 0; i < n; i++)
    cin >> vec[i];
  int ans = vec[0];
  for (int i = 0; i < n - 1; i++) {
    if (vec[i + 1] > vec[i]) {
      vec[i + 1] = vec[i];
    }
    ans += vec[i + 1];
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
