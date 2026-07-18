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

template <class Fun> class y_combinator_result {
  Fun fun_;

public:
  template <class T>
  explicit y_combinator_result(T &&fun) : fun_(std::forward<T>(fun)) {}
  template <class... Args> decltype(auto) operator()(Args &&...args) {
    return fun_(std::ref(*this), std::forward<Args>(args)...);
  }
};
template <class Fun> decltype(auto) y_combinator(Fun &&fun) {
  return y_combinator_result<std::decay_t<Fun>>(std::forward<Fun>(fun));
}

const int MOD = 1e9 + 7;

struct mint {
  int val = 0;
  mint(long long v = 0) { val = int((v % MOD + MOD) % MOD); }
  mint operator+(const mint &o) const { return mint((val + o.val) % MOD); }
  mint operator+(ll o) const { return mint((val + o) % MOD); }
  mint operator*(const mint &o) const { return mint(1LL * val * o.val % MOD); }
  mint operator*(ll o) const { return mint(1LL * val * o % MOD); }
  mint operator/(const mint &o) const { return (*this) * o.inv(); }
  friend mint operator+(ll o, const mint &m) { return mint((o + m.val) % MOD); }
  friend mint operator*(ll o, const mint &m) {
    return mint(1LL * o * m.val % MOD);
  }
  mint &operator+=(const mint &o) {
    val = (val + o.val) % MOD;
    return *this;
  }
  mint &operator+=(ll o) {
    val = int((val + o) % MOD);
    return *this;
  }
  mint &operator*=(const mint &o) {
    val = int(1LL * val * o.val % MOD);
    return *this;
  }
  mint &operator*=(ll o) {
    val = int(1LL * val * o % MOD);
    return *this;
  }
  mint &operator/=(const mint &o) { return *this *= o.inv(); }
  mint pow(ll p) const {
    mint a = *this, res = 1;
    for (; p; p >>= 1, a *= a) {
      if (p & 1)
        res *= a;
    }
    return res;
  }
  mint inv() const { return pow(MOD - 2); }
};
ostream &operator<<(ostream &os, const mint &m) { return os << m.val; }
istream &operator>>(istream &is, mint &m) {
  long long v;
  is >> v;
  m = mint(v);
  return is;
}

void __print(int x) { cerr << x; }
void __print(long x) { cerr << x; }
void __print(long long x) { cerr << x; }
void __print(unsigned x) { cerr << x; }
void __print(unsigned long x) { cerr << x; }
void __print(unsigned long long x) { cerr << x; }
void __print(float x) { cerr << x; }
void __print(double x) { cerr << x; }
void __print(long double x) { cerr << x; }
void __print(char x) { cerr << '\'' << x << '\''; }
void __print(const string &x) { cerr << '\"' << x << '\"'; }
void __print(bool x) { cerr << (x ? "true" : "false"); }
void __print(const mint &x) { __print(x.val); }

template <typename T, typename V> void __print(const pair<T, V> &x) {
  cerr << '{';
  __print(x.first);
  cerr << ", ";
  __print(x.second);
  cerr << '}';
}
template <typename T> void __print(const T &x) {
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

void solve() {
  int i, n, m;
  cin >> n >> m;
  vector<vector<pair<int, int>>> adj(n);
  vector<bool> done(n, 0);
  vector<vi> b(2, vi(n, 0));
  for (i = 0; i < m; i++) {
    int o, l, r;
    cin >> o >> l >> r;
    o--;
    l--;
    r--;
    b[o][l]++;
    b[o][r]++;
    adj[l].emplace_back(o, r);
    adj[r].emplace_back(o, l);
  }
  vector<pair<int, int>> acc;
  queue<int> q;
  for (i = 0; i < n; i++) {
    if (b[0][i] == 0 || b[1][i] == 0)
      q.push(i), done[i] = 1;
  }
  while (!q.empty()) {
    auto t = q.front();
    q.pop();
    if (b[0][t] == 0)
      acc.emplace_back(t, -1);
    else {
      acc.emplace_back(t, 1);
    }
    for (auto [o, v] : adj[t])
      if (!--b[o][v] && !done[v])
        q.push(v), done[v] = 1;
  }
  if (acc.size() < n) {
    cout << "NO\n";
    return;
  }
  cout << "YES\n";
  vi ret(n);
  for (i = 0; i < n; i++)
    ret[acc[i].first] = i;
  for (i = 0; i < n; i++) {
    cout << acc[ret[i]].second * (1000000000 - ret[i]) << " \n"[i + 1 == n];
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  // cout << fixed << setprecision(12);

  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
    }
  }
  return 0;
}
