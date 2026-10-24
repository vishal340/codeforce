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

constexpr int MOD = 1e9 + 7;
constexpr int N = 200000;

void solve() {
  int n, m;
  cin >> n >> m;
  vector<vi> g(n);
  rep(i, n - 1) {
    int x, y;
    cin >> x >> y;
    x--, y--;
    g[x].pb(y);
    g[y].pb(x);
  }
  vi b(m);
  cin >> b;
  vb vis(n, 0);
  priority_queue<int> pq;
  pq.push(n - 1);
  int nn = 0;
  bool flag = false;
  vi dist(n, 0);
  while (!pq.empty()) {
    int v = pq.top();
    pq.pop();
    vis[v] = 1;
    for (int i : g[v]) {
      if (i == 0) {
        flag = 1;
        nn = v;
      }
      if (vis[i] == 0) {
        vis[i] = 1;
        pq.push(i);
        dist[i] = dist[v] + 1;
      }
    }
    if (flag)
      break;
  }
  while (!pq.empty())
    pq.pop();
  int ret = dist[0];
  fill(all(dist), 0);
  fill(all(vis), 0);
  vis[0] = 1;
  pq.push(nn);
  while (!pq.empty()) {
    int v = pq.top();
    pq.pop();
    vis[v] = 1;
    for (int i : g[v]) {
      if (vis[i] == 0) {
        vis[i] = 1;
        pq.push(i);
      }
    }
  }
  vb vis1(n, 0);
  while (!pq.empty())
    pq.pop();
  pq.push(n - 1);
  while (!pq.empty()) {
    int v = pq.top();
    pq.pop();
    vis1[v] = 1;
    for (int i : g[v]) {
      if (vis1[i] == 0) {
        vis1[i] = 1;
        pq.push(i);
        if (vis[i])
          dist[i] = dist[v] + 1;
      }
    }
  }
  debug(dist);
  rep(i, m) {
    if (vis[b[i] - 1]) {
      ret = max(ret, dist[b[i] - 1] + 1);
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
