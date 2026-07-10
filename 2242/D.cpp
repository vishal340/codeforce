#include <bits/stdc++.h>
#include <climits>
using namespace std;

using vi = vector<int>;

string s1, s2;
int l1, l2;

int dp[5000][5000];

int fn(int i1, int i2) {
  if (l1 == i1 && l2 == i2) {
    return 0;
  }
  if (l1 == i1 || l2 == i2) {
    return -1;
  }
  if (dp[i1][i2] != -2)
    return dp[i1][i2];
  int acc = 0;
  int ret = INT_MIN;
  for (int i = i1; i < l1; i++) {
    acc = (acc + s1[i] - '0') % 10;
    int acc1 = 0;
    for (int j = i2; j < l2; j++) {
      acc1 = (acc1 + s2[j] - '0') % 10;
      if (acc1 == acc) {
        int t = fn(i + 1, j + 1);
        if (t != -1) {
          ret = max(ret, t + 1);
        }
      }
    }
  }
  dp[i1][i2] = max(-1, ret);
  return dp[i1][i2];
}

void solve() {
  int i;
  cin >> s1 >> s2;
  l1 = s1.size(), l2 = s2.size();
  for (i = 0; i < l1; i++)
    for (int j = 0; j < l2; j++)
      dp[i][j] = -2;
  cout << fn(0, 0) << '\n';
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
