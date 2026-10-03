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

constexpr int MOD = 998244353;

struct mint {
  int val = 0;

  mint(long long v = 0) { val = int((v % MOD + MOD) % MOD); }

  mint operator+(const mint &o) const { return mint(val + o.val); }
  mint operator+(long long o) const { return *this + mint(o); }
  mint operator*(const mint &o) const { return mint(1LL * val * o.val); }
  mint operator*(long long o) const { return *this * mint(o); }
  mint operator/(const mint &o) const { return *this * o.inv(); }

  friend mint operator+(long long o, const mint &m) { return mint(o) + m; }
  friend mint operator*(long long o, const mint &m) { return mint(o) * m; }

  mint &operator+=(const mint &o) { return *this = *this + o; }
  mint &operator+=(long long o) { return *this = *this + o; }
  mint &operator*=(const mint &o) { return *this = *this * o; }
  mint &operator*=(long long o) { return *this = *this * o; }

  mint pow(long long p) const {
    mint a = *this, res = 1;
    while (p > 0) {
      if (p & 1)
        res *= a;
      a *= a;
      p >>= 1;
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

struct Fenwick {
  int n;
  vector<int> bit;

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
  vi p(n);
  cin >> p;
  Fenwick sp(n);
  vb c(n, 0);
  map<int, int> msp;
  rep(i, n) {
    auto t1 = sp.sum(p[i] - 1, n - 1);
    if (t1 == 0)
      c[i] = 1;
    else
      msp[t1]++;
    sp.point_set(p[i] - 1, 1);
  }
  auto it = msp.rbegin();
  mint ret = 1;
  int j = 0;
  per(i, n) {
    if (it == msp.rend())
      break;
    if (!c[i]) {
      j++;
    }
    if (it->first == i) {
      rep(k, it->second) {
        ret *= j - k;
        ret = ret / (k + 1);
      }
      j -= it->second;
      it++;
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
