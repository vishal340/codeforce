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
constexpr int N = 500000;

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

void solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  int z = count(all(s), '0');
  deque<int> dq;
  vector<mint> p(n + 1), ip(n + 1), inv2(n + 1);
  inv2[0] = 1;
  rep(i, n) { inv2[i + 1] = inv2[i] / 2; }
  p[0] = ip[0] = 1;
  rep(i, n) {
    p[i + 1] = mint(1LL - inv2[i + 1].val);
    ip[i + 1] = p[i + 1].inv();
  }
  mint P = 1, S = 0;
  rep(i, n) {
    int x = (s[i] == '0');
    dq.push_back(x);
    mint q = p[x];
    S += P * q;
    P *= q;
  }
  mint scale = mint(2).pow(z);
  mint ans = 0;
  rep(i, n - 1) {
    int a = dq.front();
    int b = dq.back();
    int c = (a && b) ? a + b : 0;
    mint ratio = p[c] * ip[a];
    S *= ratio;
    P *= ratio;
    dq.front() = c;
    S += MOD - P.val;
    P *= ip[dq.back()];
    dq.pop_back();
    int x = dq.back();
    S += MOD - P.val;
    P *= ip[x];
    dq.pop_back();
    S = p[x] * (1 + S);
    P *= p[x];
    dq.push_front(x);
    ans += scale * S;
  }
  cout << ans;
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
