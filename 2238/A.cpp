#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int n, c;
  cin >> n >> c;
  vi a1(n), b1(n);
  for (int i = 0; i < n; i++) {
    cin >> a1[i];
  }
  for (int i = 0; i < n; i++) {
    cin >> b1[i];
  }
  int ans = 0;
  for (int i = 0; i < n; i++) {
    if (a1[i] < b1[i]) {
      ans = -1;
      break;
    }
    ans += a1[i] - b1[i];
  }
  int temp = c;
  sort(a1.begin(), a1.end());
  sort(b1.begin(), b1.end());
  for (int i = 0; i < n; i++) {
    if (a1[i] < b1[i]) {
      temp = -1;
      break;
    }
    temp += a1[i] - b1[i];
  }
  if (ans < 0 && temp < 0) {
    cout << -1 << '\n';
  } else if (ans < 0) {
    cout << temp << '\n';
  } else if (temp < 0) {
    cout << ans << '\n';
  } else {
    cout << min(ans, temp) << '\n';
  }
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
