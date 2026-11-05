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
vector<mint> p(5001, 1), fact(5001, 1), ifact(5001, 1);
int till = 1;
void solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  if (n == 1) {
    cout << 1 << ' ' << 1;
    return;
  }
  if (n == 2) {
    if (s[0] == s[1]) {
      cout << 2 << ' ' << 2;
    } else
      cout << 4 << ' ' << 1;
    return;
  }
  int l = n;
  vi b(n, 0);
  mint ret = 1;
  int cnt = 0;
  rep(i, n - 1) {
    if (s[i] != s[i + 1])
      b[i] = b[i + 1] = 1, cnt++;
    if (cnt > 0 and (i == n - 2 or s[i] == s[i + 1])) {
      cnt++;
      l += cnt;
      int j = 0;
      mint ret1 = 0;
      if (cnt % 2 == 0)
        ret1 = 1;
      if (cnt == 3)
        ret1 = 1;
      while (cnt > 4) {
        if (cnt & 1) {
          cnt -= 3;
          j++;
        } else {
          cnt -= 6;
          j += 2;
        }
        int t = (cnt + 2 * j) / 2;
        ret1 += fact[t] * ifact[j] * ifact[t - j];
      }
      ret *= ret1;
      cnt = 0;
    }
  }
  ai(2) a{0, 0};
  rep(i, n) {
    if (b[i] == 0)
      a[s[i] - '0']++;
  }
  int n1 = max(a[0], a[1]) + 1;
  fora(i, till + 1, n1, 1) {
    rep(j, i - 1) {
      p[i] += p[i - j - 1] * fact[i - 1] * ifact[j] * ifact[i - j - 1];
    }
  }
  till = max(till, n1 - 1);
  ret *= p[a[0]] * p[a[1]];
  cout << l << ' ' << ret;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  rep(i, 5000) { fact[i + 1] = fact[i] * (i + 1); }
  ifact[5000] = fact[5000].inv();

  per(i, 5000) ifact[i] = ifact[i + 1] * (i + 1);

  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
      cout << '\n';
    }
  }
  return 0;
}
