#include <bits/stdc++.h>
using namespace std;

void solve() {
  int n;
  cin >> n;
  string str;
  cin >> str;
  int count = 0;
  for (int i = 0; i < n; i++) {
    if (count == '?')
      count++;
  }
  int conf = (1 << count);
  vector<int> left, right;
  for (int i = 0; i < n; i++) {
  }
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--) {
    solve();
  }
  return 0;
}
