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
int N = 2e5;
bool square(int n) {
  int sqr = floor(sqrt(n));
  if (sqr * sqr != n and (sqr + 1) * (sqr + 1) != n)
    return false;
  return true;
}
void solve() {
  int i = 0, n;
  cin >> n;
  vi a(n);
  cin >> a;
  vector<vi> gr(n);
  for (i = 0; i < n - 1; i++) {
    int x, y;
    cin >> x >> y;
    x--, y--;
    gr[x].push_back(y);
    gr[y].push_back(x);
  }
  ll ret = 0;
  function<int(int, int)> subtree = [&](int u, int root) -> int {
    int acc = 1;
    bool sq = square(a[u]);
    vi count;
    int zeros = 0;
    for (auto v : gr[u]) {
      if (v == root)
        continue;
      int t = subtree(v, u);
      acc += t;
      if (sq) {
        if (t > 1)
          count.push_back(t);
        else {
          zeros++;
        }
      }
    }
    if (sq) {
      ll pa = ((ll)zeros * (zeros - 1)) / 2;
      ret += (pa * (zeros + 1)) / 3;
      count.push_back(n - acc);
      int t = count.size();
      for (int j = 0; j < t; j++) {
        ll temp1 = (ll)count[j] * zeros;
        ret += (temp1 * (zeros + 1)) / 2;
        for (int k = j + 1; k < t; k++) {
          ll temp = (ll)count[j] * count[k];
          ret += temp * (1 + zeros);
          for (int l = k + 1; l < t; l++) {
            ret += temp * count[l];
          }
        }
      }
    }
    return acc;
  };
  subtree(0, -1);
  cout << ret;
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
