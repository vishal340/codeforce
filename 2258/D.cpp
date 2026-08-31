#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;
using vi = vector<int>;
using vll = vector<long long>;
using pii = pair<int, int>;
using pll = pair<long long, long long>;
using ci = const int;
using cll = const ll;
using cld = const ld;
using cvi = const vi;
using cvll = const vll;
using cpii = const pii;
using cpll = const pll;

#define sz(x) static_cast<int>((x).size())
#define all(x) (x).begin(), (x).end()

template <typename T> istream &operator>>(istream &in, vector<T> &vec) {
  for (int i = 0; i < sz(vec); ++i)
    in >> vec[i];
  return in;
}

template <typename T> ostream &operator<<(ostream &out, const vector<T> &v) {
  for (int i = 0; i < sz(v); ++i) {
    if (i)
      out << ' ';
    out << v[i];
  }
  return out;
}

template <typename T>
concept Printable = requires(ostream &os, const T &value) {
  { os << value } -> same_as<ostream &>;
} && !is_array_v<T>;
template <Printable T> void __print(const T &x) { cerr << x; }
template <typename T, typename V> void __print(const pair<T, V> &x) {
  cerr << '{';
  __print(x.first);
  cerr << ", ";
  __print(x.second);
  cerr << '}';
}
template <typename T>
  requires(!Printable<T>)
void __print(const T &x) {
  int f = 0;
  cerr << '{';
  for (auto &i : x) {
    cerr << (f++ ? ", " : "");
    __print(i);
  }
  cerr << '}';
}
void _print() { cerr << "]\n"; }
template <typename T, typename... V> void _print(T t, V... v) {
  __print(t);
  if (sizeof...(v))
    cerr << ", ";
  _print(v...);
}

#ifndef ONLINE_JUDGE
#define debug(x...)                                                            \
  cerr << "[" << #x << "] = [";                                                \
  _print(x)
#else
#define debug(x...)
#endif

void solve() {
  int i = 0, n, m;
  cin >> n >> m;
  vector<array<ll, 2>> a(n), b(m);
  for (i = 0; i < n; i++)
    cin >> a[i][0] >> a[i][1];
  for (i = 0; i < m; i++)
    cin >> b[i][0] >> b[i][1];
  ll l1 = a[0][0], r1 = a[0][1], l2 = b[0][0], r2 = b[0][1];
  i = 0;
  int j = 0;
  multiset<ll, greater<ll>> ret;
  while (i < n && j < m) {
    ll g1 = r1 - l1 + 1;
    ll g2 = r2 - l2 + 1;
    if (g1 < g2 && l1 < l2) {
      ret.insert(min(l2 - 1, r1) - l1 + 1);
    } else if (g1 < g2 && l1 > l2 && r1 >= r2) {
      l1 = max(l1, r2 + 1);
      ret.insert(r2 - l2 + 1);
    } else if (g2 < g1 && l2 < l1) {
      ret.insert(min(l1 - 1, r2) - l2 + 1);
    } else if (g2 < g1 && l2 > l1 && r2 >= r1) {
      l2 = max(l2, r1 + 1);
      ret.insert(r1 - l1 + 1);
    } else if (g1 == g2) {
      ret.insert(g1);
      if (r1 < r2)
        l2 = max(l2, r1 + 1);
      if (r2 < r1)
        l1 = max(l1, r2 + 1);
    }
    auto t = r1;
    if (r1 <= r2) {
      i++;
      if (i < n)
        l1 = a[i][0], r1 = a[i][1];
    }
    if (r2 <= t) {
      j++;
      if (j < m)
        l2 = b[j][0], r2 = b[j][1];
    }
  }
  while (i < n) {
    ret.insert(r1 - l1 + 1);
    i++;
    if (i < n)
      l1 = a[i][0], r1 = a[i][1];
  }
  while (j < m) {
    ret.insert(r2 - l2 + 1);
    j++;
    if (j < m)
      l2 = b[j][0], r2 = b[j][1];
  }
  cout << ret.size() << '\n';
  for (auto it : ret)
    cout << it << ' ';
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
      cout << '\n';
    }
  }
  return 0;
}
