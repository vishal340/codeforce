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
#define fora(i, l, r, k) for (int(i) = (l); (i) < (r); (i) += (k))
#define forb(i, r, l, k) for (int(i) = (r) - 1; (i) >= (l); (i) -= (k))
#define rep(i, n) fora(i, 0, n, 1)
#define per(i, n) forb(i, n, 0, 1)

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

constexpr int N = 2e5;

struct Fenwick {
  int n;
  vector<ll> b1, b2;

  Fenwick(int n_) : n(n_), b1(n + 1), b2(n + 1) {}

  Fenwick(const vector<ll> &a) : Fenwick(sz(a)) {
    for (int i = 0; i < n; ++i)
      add(i, i, a[i]);
  }

  void bit_add(vector<ll> &bit, int i, ll x) {
    for (++i; i <= n; i += i & -i)
      bit[i] += x;
  }

  ll bit_sum(const vector<ll> &bit, int i) {
    ll res = 0;
    for (++i; i; i -= i & -i)
      res += bit[i];
    return res;
  }

  void add(int l, int r, ll x) {
    bit_add(b1, l, x);
    bit_add(b1, r + 1, -x);
    bit_add(b2, l, x * (l - 1));
    bit_add(b2, r + 1, -x * r);
  }

  ll prefix_sum(int i) { return bit_sum(b1, i) * i - bit_sum(b2, i); }

  ll sum(int l, int r) { return prefix_sum(r) - (l ? prefix_sum(l - 1) : 0); }

  void point_set(int i, ll x) { add(i, i, x - sum(i, i)); }
};

void solve() {
  int n;
  cin >> n;
  vi a(n), c(n);
  multiset<int> b;
  cin >> a >> c;
  rep(i, n) { b.insert(c[i]); }
  rep(i, n) {
    auto it = b.lower_bound(a[i]);
    if (it == b.end()) {
      cout << -1;
      return;
    } else {
      a[i] = *it;
      b.erase(it);
    }
  }
  ll cost = 0;
  rep(i, n) {
    fora(j, i + 1, n, 1) {
      if (a[i] > a[j])
        cost++;
    }
  }
  cout << cost;
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
