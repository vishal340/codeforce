#include <algorithm>
#include <iostream>
#include <vector>

using namespace std;

using vec = vector<int>;
void solve() {
  int i = 0, n, k;
  cin >> n >> k;
  int c = n / k;
  int t = 0;
  while (c > 0) {
    t++;
    c /= 2;
  }
  if (t > 1)
    t--;
  int ans = t * k;
  n -= k * ((1 << t) - 1);
  int gap = (1 << t);
  if (n > 0) {
    ans += min(n / gap, k);
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
