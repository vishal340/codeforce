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

template <typename T> istream &operator>>(istream &in, vector<T> &vec) {
  for (int i = 0; i < sz(vec); ++i)
    in >> vec[i];
  return in;
}

// cout-only: space-separated (do not use for debug / cerr).
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
} && requires(const T &x) { cerr << x; };
template <DebugAtom T> void __print(const T &x) { cerr << x; }
template <typename T, typename V> void __print(const pair<T, V> &x) {
  cerr << '{';
  __print(x.first);
  cerr << ", ";
  __print(x.second);
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

struct Dsu {
  vector<int> rt, ss;
  int n;
  Dsu() = default;
  Dsu(int _n) : n(_n) {
    rt.resize(n), iota(rt.begin(), rt.end(), 0);
    ss.assign(n, 1);
  }
  int Find(int v) { return rt[v] == v ? v : rt[v] = Find(rt[v]); }
  bool Union(int v, int u) {
    u = Find(u), v = Find(v);
    if (u == v) {
      return false;
    }
    if (ss[u] > ss[v]) {
      swap(u, v);
    }
    rt[u] = v;
    ss[v] += ss[u];
    return true;
  }
  bool same(int u, int v) { return Find(u) == Find(v); }
};

constexpr int MOD = 1e9 + 7;
constexpr int N = 200000;

void solve() {
  int n, m, q;
  cin >> n >> m >> q;
  set<pair<int, pii>> g;
  rep(i, m) {
    int u, v, d;
    cin >> u >> v >> d;
    u--, v--;
    g.insert({d, {u, v}});
  }
  ll res = 0;
  Dsu dsu(n);
  for (auto it = g.begin(); it != g.end();) {
    auto d = it->first;
    auto [u, v] = it->second;
    if (!dsu.Union(u, v)) {
      res += d;
      it = g.erase(it);
    } else {
      it++;
    }
  }
  // per scenario i, find how many edges can be removed and redrawn
  ll cont = ((ll)(n - 1 - g.size()) * (n - g.size())) / 2;
  vector<pii> X(q);
  rep(i, q) {
    int x;
    cin >> x;
    X[i] = {x, i};
  }
  sort(all(X));
  vll ret(q, -cont);
  int t = n - sz(g);
  auto it = g.rbegin();
  ll acc = 0;
  per(i, q) {
    int l = X[i].second;
    ret[l] *= X[i].first;
    for (; it != g.rend(); it++) {
      if (it->first <= (ll)t * X[i].first)
        break;
      else {
        t++;
        acc += it->first;
      }
    }
    ret[l] += acc - (((ll)t * (t - 1)) / 2 - cont) * (ll)X[i].first;
  }
  rep(i, q) ret[i] += res;
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
