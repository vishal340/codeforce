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
  vi a(n);
  cin >> a;
  sort(all(a));
  vi b(m + 1, 0), c(2 * m, 0);
  int cur = 1;
  for (i = 0; i < n; i++) {
    c[a[i]]++;
    for (int j = cur; j <= a[i]; j++) {
      b[j] = n - i;
    }
    cur = a[i] + 1;
  }
  vi ret(m + 1, 0);
  for (i = 1; i <= m; i++) {
    int count = 0;
    int k = 1;
    for (int j = 1; j <= m / i; j++) {
      count += b[i * j];
      if (j == (1 << k) - 1) {
        ret[k] = max(ret[k], count + c[i * (j + 1)]);
        k++;
      }
    }
    if (k <= m)
      ret[k] = max(ret[k], count);
  }
  for (i = 1; i <= m; i++) {
    ret[i] = max(ret[i - 1], ret[i]);
    cout << ret[i] << ' ';
  }
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
