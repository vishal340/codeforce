#include <bits/stdc++.h>
using namespace std;

#define pb push_back
using ll = long long;
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

constexpr int MOD = 998244353;
constexpr int N = 200000;

struct Dsu {
  int n, num, B = 1;
  vi bel, esum;
  vector<vi> g1, cc;
  Dsu() = default;
  Dsu(int _n) : n(_n), num(_n) {
    bel.resize(n);
    g1.resize(n);
    cc.resize(n);
    esum.assign(n, 0);
    iota(all(bel), 0);
    rep(i, n) cc[i] = {i};
  }
  void set_g1(int x, int y) {
    g1[x].pb(y);
    g1[y].pb(x);
  }
  void finish_g1(int m) {
    for (auto &adj : g1)
      sort(all(adj));
    rep(i, n) esum[i] = sz(g1[i]);
    B = max(1, (int)(sqrt((double)max(m, 1)) / 5));
  }
  void reset(int x) {
    bel[x] = x;
    cc[x].clear();
    cc[x].pb(x);
    esum[x] = sz(g1[x]);
  }
  void explode(int a, int b) {
    vi ma = std::move(cc[a]);
    vi mb = std::move(cc[b]);
    num += sz(ma) + sz(mb) - 2;
    for (int x : ma)
      reset(x);
    for (int x : mb)
      reset(x);
  }
  // sqrt: both small -> pair binary_search; else scan lighter esum side
  // bel[y]==root is O(1) — do NOT mark the heavy side (that is O(|B|) TLE)
  bool conflict(int a, int b) {
    if (!esum[a] || !esum[b])
      return false;
    if (sz(cc[a]) <= B && sz(cc[b]) <= B) {
      for (int x : cc[a]) {
        if (g1[x].empty())
          continue;
        for (int y : cc[b])
          if (binary_search(all(g1[x]), y))
            return true;
      }
      return false;
    }
    if (esum[a] > esum[b])
      swap(a, b);
    for (int x : cc[a])
      for (int y : g1[x])
        if (bel[y] == b)
          return true;
    return false;
  }
  void Union(int u, int v) {
    u = bel[u], v = bel[v];
    if (u == v)
      return;
    if (conflict(u, v)) {
      explode(u, v);
      return;
    }
    if (sz(cc[u]) > sz(cc[v]))
      swap(u, v);
    for (int x : cc[u]) {
      bel[x] = v;
      cc[v].pb(x);
    }
    cc[u].clear();
    esum[v] += esum[u];
    esum[u] = 0;
    num--;
  }
};

void solve() {
  int n, m, q;
  cin >> n >> m >> q;
  Dsu d(n);
  rep(i, m) {
    int x, y;
    cin >> x >> y;
    --x, --y;
    d.set_g1(x, y);
  }
  d.finish_g1(m);
  while (q--) {
    int x, y;
    cin >> x >> y;
    x--, y--;
    d.Union(x, y);
    cout << d.num << '\n';
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
    }
  }
  return 0;
}
