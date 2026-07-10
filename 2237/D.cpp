#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int i, n;
  cin >> n;
  string s;
  cin >> s;
  long long ans = 1;
  long long m[3]{0};
  int r = 0, acc = 0;
  if (s[0] == '0') {
    acc = 1;
  } else {
    acc = 2;
  }
  m[acc]++;
  for (i = 1; i < n; i++) {
    if (s[i] == '0') {
      acc = (acc + 1) % 3;
    } else {
      acc = (acc + 2) % 3;
    }
    m[acc]++;
    if (s[i - 1] != s[i]) {
      r++;
    }
    if (s[i - 1] == s[i] && r > 0) {
      ans -= (long long)((r + 1) / 2) * ((r - 1) / 2 + ((r + 1) & 1));
      r = 0;
    }
    if (acc % 3)
      ans++;
    ans += m[0] + m[1] + m[2] - m[acc];
  }
  ans -= (long long)((r + 1) / 2) * ((r - 1) / 2 + ((r + 1) & 1));
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
