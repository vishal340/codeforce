#include <bits/stdc++.h>
#include <cstddef>
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

constexpr int MOD = 998244353;
constexpr int N = 200000;

bool query(vi::iterator a, vi::iterator b) {
  cout << "? 1 " << b - a << ' ';
  auto it = a;
  while (it != b) {
    cout << (*it) + 1 << ' ';
    it++;
  }
  cout << endl;
  bool ans;
  cin >> ans;
  return 1 ^ ans;
}
bool query(vi::iterator a1, vi::iterator b1, vi::iterator a2, vi::iterator b2) {
  cout << "? 1 " << b1 - a1 + b2 - a2 << ' ';
  auto it = a1;
  while (it != b1) {
    cout << (*it) + 1 << ' ';
    it++;
  }
  it = a2;
  while (it != b2) {
    cout << (*it) + 1 << ' ';
    it++;
  }
  cout << endl;
  bool ans;
  cin >> ans;
  return 1 ^ ans;
}

void solve() {
  int n, m;
  cin >> n >> m;
  vector<vi> g(n);
  rep(i, m) {
    int x, y;
    cin >> x >> y;
    x--, y--;
    g[x].pb(y), g[y].pb(x);
  }
  map<int, int> comp;
  vector<vi> cc;
  {
    vb vis(n, 0);
    vi q;
    rep(i, n) {
      if (!vis[i]) {
        cc.pb(vi());
        cc.back().pb(i);
        vis[i] = 1;
        q.pb(i);
        while (!q.empty()) {
          int u = q.back();
          q.pop_back();
          for (auto v : g[u]) {
            if (vis[v])
              continue;
            vis[v] = 1;
            q.pb(v);
            cc.back().pb(v);
          }
        }
      }
    }
  }
  queue<int> q;
  vb vis(n, 0);
  auto bfs = [&](vi &gr, int beg) {
    vi temp;
    temp.pb(beg);
    q.push(beg);
    vis[beg] = 1;
    while (!q.empty()) {
      int u = q.front();
      q.pop();
      for (auto v : g[u]) {
        if (vis[v])
          continue;
        vis[v] = 1;
        q.push(v);
        temp.pb(v);
      }
    }
    gr = temp;
  };
  int k = 0;
  for (auto &gr : cc) {
    for (auto u : gr) {
      comp[u] = k;
    }
    bfs(gr, gr[0]);
    k++;
  }
  vi inp;
  for (auto gr : cc) {
    for (auto u : gr)
      inp.pb(u);
  }
  int l = 0, r = n - 1;
  while (l < r) {
    int mid = (l + r) / 2;
    if (query(inp.begin(), inp.begin() + mid + 1)) {
      r = mid;
    } else {
      l = mid + 1;
    }
  }
  int ep = l;
  int j = comp[inp[l]];
  vi &iv = cc[j];
  int ov = 0;
  vis.assign(n, 0);
  bfs(iv, inp[l]);
  rep(i, j) { ov += sz(cc[i]); }
  l = 0, r = ov - 1;
  while (l < r) {
    int mid = (l + r) / 2;
    if (query(inp.begin(), inp.begin() + mid + 1, inp.begin() + ov,
              inp.begin() + ov + sz(iv))) {
      r = mid;
    } else {
      l = mid + 1;
    }
  }
  int sp = l;
  l = 0, r = sz(iv) - 1;
  while (l < r) {
    int mid = (l + r) / 2;
    if (query(iv.begin(), iv.begin() + mid + 1, inp.begin(),
              inp.begin() + ov)) {
      r = mid;
    } else {
      l = mid + 1;
    }
  }
  int mp = l;
  cout << "? 2 " << inp[ep] + 1 << ' ' << inp[sp] + 1 << endl;
  bool ans;
  cin >> ans;
  if (ans)
    cout << "! " << inp[ep] + 1 << ' ' << inp[sp] + 1 << endl;
  else
    cout << "! " << iv[mp] + 1 << ' ' << inp[sp] + 1 << endl;
}

int main() {
  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
    }
  }
  return 0;
}
