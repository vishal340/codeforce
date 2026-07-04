#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int n;
  cin >> n;
  int ans = 0;
  int root = floor(sqrt(n));
  vector<bool> a = vector<bool>(root + 1, true);
  a[1] = false;
  for (int i = 2; i <= root; i++) {
    if (a[i]) {
      for (int j = i * i; j <= root; j += i) {
        a[j] = false;
      }
    }
  }
  vi primes;
  for (int i = 2; i <= root; i++) {
    if (a[i])
      primes.push_back(i);
  }
  // for (auto it : primes)
  //   cout << it << ' ';
  vector<pair<int, int>> fact;
  int t = n;
  for (auto it : primes) {
    if (t < it) {
      break;
    }
    if (!(t % it)) {
      int c = 0;
      while (!(t % it)) {
        c++;
        t /= it;
      }
      fact.emplace_back(it, c);
    }
  }
  if (t != 1)
    fact.emplace_back(t, 1);
  ans += fact.size();
  int lvl = 2;
  int max_lvl = 0;
  for (auto it : fact) {
    max_lvl += it.second;
  }
  cout << max_lvl + ans - 1 << '\n';
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
