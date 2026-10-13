#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vll = vector<long long>;

struct Fenwick2D {
  int n;
  vector<vector<int>> bit;

  Fenwick2D(int n) : n(n), bit(n + 1, vector<int>(n + 1, 0)) {}

  void add(int r, int c, int val) {
    for (int i = r; i <= n; i += i & -i) {
      for (int j = c; j <= n; j += j & -j) {
        bit[i][j] += val;
      }
    }
  }

  int query(int r, int c) {
    int res = 0;
    for (int i = r; i > 0; i -= i & -i) {
      for (int j = c; j > 0; j -= j & -j) {
        res += bit[i][j];
      }
    }
    return res;
  }

  int query_range(int r1, int c1, int r2, int c2) {
    if (r1 > r2 || c1 > c2)
      return 0;
    return query(r2, c2) - query(r1 - 1, c2) - query(r2, c1 - 1) +
           query(r1 - 1, c1 - 1);
  }
};

void change(vector<vll> &arr, Fenwick2D &bit, int n, int r, int c, ll x) {
  if (r < 2 || c < 2 || r > n || c > n)
    return;

  if (arr[r][c] != 0) {
    bit.add(r, c, -1);
  }

  arr[r][c] += x;

  if (arr[r][c] != 0) {
    bit.add(r, c, 1);
  }
}

void solve() {
  int n, q;
  if (!(cin >> n >> q))
    return;

  vector<vll> a(n + 1, vll(n + 1, 0));
  vector<vll> arr(n + 1, vll(n + 1, 0));

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      cin >> a[i][j];
    }
  }

  for (int i = 1; i <= n; i++) {
    for (int j = 1; j <= n; j++) {
      ll b_val;
      cin >> b_val;
      arr[i][j] = b_val - a[i][j];
    }
  }

  // Compute 2D 1st-differences: V[i][j]
  for (int i = n; i >= 2; i--) {
    for (int j = n; j >= 2; j--) {
      arr[i][j] = arr[i][j] - arr[i - 1][j] - arr[i][j - 1] + arr[i - 1][j - 1];
    }
  }

  Fenwick2D bit(n);
  for (int i = 2; i <= n; i++) {
    for (int j = 2; j <= n; j++) {
      if (arr[i][j] != 0) {
        bit.add(i, j, 1);
      }
    }
  }

  while (q--) {
    string type;
    cin >> type;

    int r1, c1, r2, c2;
    cin >> r1 >> c1 >> r2 >> c2;

    if (type == "U") {
      ll x;
      cin >> x;

      // Updates in D = B - A mean adding x to A subtracts x from D
      change(arr, bit, n, r1, c1, -x);
      change(arr, bit, n, r1, c2 + 1, x);
      change(arr, bit, n, r2 + 1, c1, x);
      change(arr, bit, n, r2 + 1, c2 + 1, -x);
    } else {
      // Internal non-zero V checks occur within [r1 + 1, r2] x [c1 + 1, c2]
      int bad_count = bit.query_range(r1 + 1, c1 + 1, r2, c2);
      if (bad_count == 0) {
        cout << "YES\n";
      } else {
        cout << "NO\n";
      }
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
    }
  }
  return 0;
}
