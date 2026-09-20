#include <bits/stdc++.h>
using namespace std;

#define pb push_back
using ll = long long;
using ld = long double;
using vi = vector<int>;
using vb = vector<bool>;
using vll = vector<long long>;
#define ai(x) array<int, (x)>
#define al(x) array<ll, (x)>
#define ap(x) array<bool, (x)>

#define sz(x) static_cast<int>((x).size())
#define all(x) (x).begin(), (x).end()
#define fora(i, l, r, k) for (int(i) = (l); (i) < (r); (i) += (k))
#define forb(i, r, l, k) for (int(i) = (r) - 1; (i) >= (l); (i) -= (k))
#define rep(i, n) fora(i, 0, n, 1)
#define per(i, n) forb(i, n, 0, 1)

#define defret(x)                                                              \
  {                                                                            \
    cout << (x);                                                               \
    return;                                                                    \
  }

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

struct Fenwick {
  int n;
  vector<ll> bit;

  Fenwick(int n_) : n(n_), bit(n + 1) {}

  Fenwick(const vector<ll> &a) : Fenwick(sz(a)) {
    for (int i = 0; i < n; ++i)
      add(i, a[i]);
  }

  void add(int i, ll x) {
    for (++i; i <= n; i += i & -i)
      bit[i] += x;
  }

  ll prefix_sum(int i) {
    ll res = 0;
    for (++i; i; i -= i & -i)
      res += bit[i];
    return res;
  }

  ll sum(int l, int r) { return prefix_sum(r) - (l ? prefix_sum(l - 1) : 0); }

  void point_set(int i, ll x) { add(i, x - sum(i, i)); }
};

void solve() {
  int n;
  cin >> n;
  vi a(n), b(n);
  cin >> a >> b;
  rep(i, n) b[i]--;
  Fenwick tree(n);

  set<int> pivots;
  vi ret(n, 0);
  int pos = 0;
  per(i, n) {
    int t = b[i];
    auto it = pivots.upper_bound(t);
    if (it == pivots.begin() or
        tree.prefix_sum(t - 1) - tree.prefix_sum(*prev(it) - 1) < a[t]) {
      pivots.insert(t);
      pos = t;
    } else {
      pos = *prev(it);
    }
    tree.add(t, a[t]);
    while (pivots.upper_bound(t) != pivots.end()) {
      it = pivots.upper_bound(t);
      if (tree.prefix_sum((*it) - 1) - tree.prefix_sum(pos - 1) >= a[(*it)]) {
        pivots.erase(it);
      } else
        break;
    }
    ret[i] = pivots.size() - 1;
  }
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
