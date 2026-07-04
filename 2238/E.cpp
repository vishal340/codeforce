#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  vector<vector<int>> dp(n + 1, vector<int>(n + 1, INT_MAX));
  dp[0][0] = 0;
  for (int i = 0; i < n; i++) {
    vector<vector<int>> ndp(n + 1, vector<int>(n + 1, INT_MAX));
    for (int j = 0; j <= i; j++) {
      for (int k = 0; k <= i; k++) {
        if (s[i] != 'T')
          ndp[j + 1][k + 1] = min(ndp[j + 1][k + 1], max(dp[j][k], k + 1));
        if (s[i] != 'F')
          ndp[j][max(0, k - 1)] = min(ndp[j][max(0, k - 1)], dp[j][k]);
      }
    }
    swap(dp, ndp);
  }
  int ret = 0;
  for (int i = 0; i <= n; i++) {
    for (int j = 0; j <= n; j++) {
      ret = max(ret, i - dp[i][j]);
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
