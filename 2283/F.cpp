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
#define pii pair<int, int>
#define pll pair<ll, ll>

#define sz(x) static_cast<int>((x).size())
#define all(x) (x).begin(), (x).end()
#define fora(i, l, r, k) for (int i = (l); i < (r); i += (k))
#define forb(i, r, l, k) for (int i = (r) - 1; i >= (l); i -= (k))
#define rep(i, n) fora(i, 0, n, 1)
#define per(i, n) forb(i, n, 0, 1)

template <typename A, typename B>
istream &operator>>(istream &in, pair<A, B> &p) {
  return in >> p.first >> p.second;
}
template <typename... Ts> istream &operator>>(istream &in, tuple<Ts...> &t) {
  apply([&](auto &...xs) { (in >> ... >> xs); }, t);
  return in;
}
template <typename T> istream &operator>>(istream &in, vector<T> &vec) {
  for (int i = 0; i < sz(vec); ++i)
    in >> vec[i];
  return in;
}

// cout-only: space-separated (do not use for debug / cerr).
template <typename A, typename B>
ostream &operator<<(ostream &out, const pair<A, B> &p) {
  return out << p.first << ' ' << p.second;
}
template <typename... Ts>
ostream &operator<<(ostream &out, const tuple<Ts...> &t) {
  apply(
      [&](const auto &x, const auto &...xs) {
        out << x;
        ((out << ' ' << xs), ...);
      },
      t);
  return out;
}
template <typename T> ostream &operator<<(ostream &out, const vector<T> &v) {
  for (int i = 0; i < sz(v); ++i) {
    if (i)
      out << ' ';
    out << v[i];
  }
  return out;
}

// cerr-only debug printers (ignore ostream<< for ranges/containers).
template <typename T>
concept DebugAtom = !is_array_v<remove_cvref_t<T>> && !requires(const T &t) {
  t.begin();
  t.end();
} && !requires {
  typename tuple_size<remove_cvref_t<T>>::type;
} && requires(const T &x) { cerr << x; };
template <DebugAtom T> void __print(const T &x) { cerr << x; }
template <typename T, typename V> void __print(const pair<T, V> &x) {
  cerr << '{';
  __print(x.first);
  cerr << ", ";
  __print(x.second);
  cerr << '}';
}
template <typename... Ts> void __print(const tuple<Ts...> &t) {
  cerr << '{';
  apply(
      [&](const auto &...xs) {
        int f = 0;
        ((cerr << (f++ ? ", " : ""), __print(xs)), ...);
      },
      t);
  cerr << '}';
}
template <typename T>
  requires(!DebugAtom<T>)
void __print(const T &x) {
  int f = 0;
  cerr << '{';
  for (auto &&e : x) {
    cerr << (f++ ? ", " : "");
    __print(e);
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

constexpr int MOD = 1e9 + 7;
constexpr int N = 200000;

struct SegTree {
  int n;
  vector<int> st, lz;

  SegTree(int n_) : n(n_), st(4 * n_), lz(4 * n) {}

  SegTree(const vector<int> &a) : SegTree(sz(a)) { build(1, 0, n - 1, a); }

  void apply(int v, int l, int r, int x) {
    st[v] = x * (r - l + 1);
    lz[v] = x;
  }

  void push(int v, int l, int r) {
    if (lz[v] == -1 || l == r)
      return;
    int m = (l + r) >> 1;
    apply(v << 1, l, m, lz[v]);
    apply(v << 1 | 1, m + 1, r, lz[v]);
    lz[v] = -1;
  }

  void pull(int v) { st[v] = st[v << 1] + st[v << 1 | 1]; }

  void build(int v, int l, int r, const vector<int> &a) {
    if (l == r)
      return void(st[v] = a[l]);
    int m = (l + r) >> 1;
    build(v << 1, l, m, a);
    build(v << 1 | 1, m + 1, r, a);
    pull(v);
  }

  void upd(int v, int l, int r, int ql, int qr, int x) {
    if (ql > qr)
      return;
    if (ql == l && qr == r)
      return apply(v, l, r, x);
    push(v, l, r);
    int m = (l + r) >> 1;
    upd(v << 1, l, m, ql, min(qr, m), x);
    upd(v << 1 | 1, m + 1, r, max(ql, m + 1), qr, x);
    pull(v);
  }

  int qry(int v, int l, int r, int ql, int qr) {
    if (ql > qr)
      return 0;
    if (ql == l && qr == r)
      return st[v];
    push(v, l, r);
    int m = (l + r) >> 1;
    return qry(v << 1, l, m, ql, min(qr, m)) +
           qry(v << 1 | 1, m + 1, r, max(ql, m + 1), qr);
  }

  void set(int v, int l, int r, int p, int x) {
    if (l == r) {
      st[v] = x, lz[v] = -1;
      return;
    }
    push(v, l, r);
    int m = (l + r) >> 1;
    if (p <= m)
      set(v << 1, l, m, p, x);
    else
      set(v << 1 | 1, m + 1, r, p, x);
    pull(v);
  }

  void add(int l, int r, int x) { upd(1, 0, n - 1, l, r, x); }
  int sum(int l, int r) { return qry(1, 0, n - 1, l, r); }
  void point_set(int p, int x) { set(1, 0, n - 1, p, x); }
};

void solve() {
  int n;
  cin >> n;
  vector<pii> g(n);
  vi b(n, 0);
  ll ret = 0;
  map<int, vi> a;
  rep(i, n - 1) {
    int x, y;
    cin >> x >> y;
    x--, y--;
    g[i + 1] = {x, y};
    a[y].pb(i + 1);
  }
  SegTree tree(n);
  fora(i, 1, n, 1) {
    tree.upd(1, 0, n - 1, g[i].first, g[i].second, 1);
    ret += g[i].second - g[i].first + 1;
    if (a.count(i) != 0) {
      for (auto u : a[i]) {
        ret -= tree.sum(g[u].first, g[u].second);
      }
    }
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
