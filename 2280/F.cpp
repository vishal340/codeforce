#include <bits/stdc++.h>
#include <climits>
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

void solve() {
  int n, m, w, x, y, z;
  cin >> n >> m >> w >> x >> y >> z;
  --w, --x, --y, --z;
  vector<vi> g(n);
  vector<pii> edges;
  edges.reserve(m);
  rep(i, m) {
    int a, b;
    cin >> a >> b;
    --a, --b;
    g[a].pb(b), g[b].pb(a);
    edges.pb({a, b});
  }

  // w_x -> y_z in f(G): only O(n+m) meta-vertices matter
  // (row w, row y, diagonals u_u, and u_v for each edge)
  unordered_map<ll, int> vs;
  vs.reserve((size_t)(2 * n + 2 * m) * 2);
  auto add = [&](int a, int b) {
    ll k = 1LL * a * n + b;
    if (!vs.count(k)) {
      int id = sz(vs);
      vs.emplace(k, id);
    }
  };
  auto id_of = [&](int a, int b) { return vs[1LL * a * n + b]; };

  rep(i, n) {
    add(w, i);
    add(y, i);
    add(i, i);
    for (int e : g[i])
      add(i, e);
  }

  int C = sz(vs);
  vector<vi> g2(C);
  auto link = [&](int u, int v) {
    g2[u].pb(v);
    g2[v].pb(u);
  };
  for (auto [a, b] : edges) {
    link(id_of(w, a), id_of(w, b));
    link(id_of(y, a), id_of(y, b));
    link(id_of(a, a), id_of(a, b));
    link(id_of(b, b), id_of(b, a));
    link(id_of(a, b), id_of(b, a));
  }

  vi dis(C, INT_MAX);
  queue<int> q;
  int s = id_of(w, x), t = id_of(y, z);
  dis[s] = 0;
  q.push(s);
  while (!q.empty()) {
    int u = q.front();
    q.pop();
    for (int v : g2[u])
      if (dis[v] == INT_MAX) {
        dis[v] = dis[u] + 1;
        q.push(v);
      }
  }
  cout << dis[t];
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
