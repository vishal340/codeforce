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

constexpr int MOD = 998244353;
constexpr int N = 200000;

void solve(string ss) {
  if (ss == "first") {
    int n;
    cin >> n;
    string inp;
    cin >> inp;
    int z = 0, o = 0;
    rep(i, n - 1) {
      if (inp[i] == '0')
        z++;
      else
        o++;
    }
    if (z == 0) {
      cout << n << '\n';
      if (n & 1) {
        rep(i, n - 1) { cout << i + 1 << ' ' << i + 2 << '\n'; }
        cout << n << ' ' << 1 << '\n';
      } else {
        rep(i, n - 2) { cout << i + 1 << ' ' << i + 2 << '\n'; }
        cout << n - 1 << ' ' << 1 << '\n';
        cout << n - 1 << ' ' << n << '\n';
      }
    } else {
      cout << n - 1 << '\n';
      int v;
      rep(i, n - 1) {
        if (inp[i] == '0') {
          v = i + 1;
          cout << n << ' ' << i + 1 << '\n';
        }
      }
      rep(i, n - 1) {
        if (inp[i] == '1')
          cout << v << ' ' << i + 1 << '\n';
      }
    }
  } else {
    int n, m;
    cin >> n >> m;
    vector<vi> g(n);
    if (m == n - 1) {
      rep(i, n - 1) {
        int x, y;
        cin >> x >> y;
        x--, y--;
        g[x].pb(y);
        g[y].pb(x);
      }
      vi a(n);
      vb vis(n, 0);
      vi q, nq;
      q.pb(n - 1);
      vis[n - 1] = 1;
      bool fl = 1;
      while (!q.empty()) {
        int u = q.back();
        q.pop_back();
        if (!fl) {
          a[u] = 0;
        } else {
          a[u] = 1;
        }
        for (auto v : g[u]) {
          if (vis[v])
            continue;
          vis[v] = 1;
          nq.pb(v);
        }
        if (q.empty()) {
          fl = 1 ^ fl;
          q = nq, nq.clear();
        }
      }
      rep(i, n - 1) cout << a[i];
    } else {
      rep(i, n) {
        int x, y;
        cin >> x >> y;
        x--, y--;
        g[x].pb(y);
        g[y].pb(x);
      }
      rep(i, n - 1) cout << '1';
    }
    cout << '\n';
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  string ss;
  cin >> ss;
  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve(ss);
    }
  }
  return 0;
}
