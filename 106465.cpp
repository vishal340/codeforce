#include <bits/stdc++.h>
#include <climits>
using namespace std;

int main() {

  int n;
  cin >> n;
  vector<int> a(n);
  for (int i = 0; i < n; i++) {
    cin >> a[i];
  }
  vector<int> dp(n * n, INT_MAX);
  for (int i = 0; i < n; i++)
    dp[i] = 1;
  for (int i = n; i < 2 * n - 1; i++)
    dp[i] = 2;
  for (int i = 2; i < n; i++) {
    for (int j = i * n; j < (i + 1) * n - i; j++) {
      if (a[j - i * n] == a[j - i * n + i]) {
        dp[j] = dp[j - 2 * n + 1];
      }
      for (int k = 0; k < i; k++) {
        dp[j] = min(dp[j], dp[j - (i - k) * n] + dp[j + k + 1 - (k + 1) * n]);
      }
    }
  }
  cout << dp[n * n - n] << '\n';
  return 0;
}
