#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

void solve() {
  int i, n, k;
  cin >> n >> k;
  vi a(n);
  for (i = 0; i < n; i++) {
    cin >> a[i];
  }
  vector<long long> b(n, 0);
  long long sum = 0;
  for (i = 0; i <= 2 * k; i++) {
    sum -= a[i];
  }
  b[k] = sum + (2 * k + 1) * a[k];
  for (i = k + 1; i < n - k; i++) {
    b[i] = b[i - 1] + a[i - k - 1] - a[i + k] +
           (long long)(2 * k + 1) * (a[i] - a[i - 1]);
  }
  for (i = n - k; i < n; i++)
    b[i] = b[i - 1] - a[i - n + k] + a[i - k - 1] +
           (long long)(2 * k + 1) * (a[i] - a[i - 1]);
  for (i = k - 1; i >= 0; i--)
    b[i] = b[i + 1] + a[i + 1 + k] - a[i - k + n] +
           (long long)(2 * k + 1) * (a[i] - a[i + 1]);

  long long ret = 0;
  for (i = 0; i < n; i++) {
    if (b[i] > 0)
      ret += b[i];
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
