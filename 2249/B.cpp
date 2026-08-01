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

constexpr int MOD = 998244353;

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
template <typename T> void __print(const vector<T> &x) {
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
  int i, n;
  cin >> n;
  vi a(n - 1);
  for (i = 0; i < n - 1; i++) {
    cin >> a[i];
    a[i]--;
  }
  for (i = 0; i < n - 1; i++) {
    if (a[i] == n - 2)
      break;
    else if (i == n - 2) {
      cout << 0 << '\n';
      return;
    }
  }
  int l, r;
  vector<bool> b(n - 2, false);
  for (i = 0; i < n - 1; i++) {
    if (a[i] == n - 2) {
      break;
    } else if (a[i] == n - 1 || (i > 0 && a[i] < a[i - 1])) {
      cout << 0 << '\n';
      return;
    } else {
      if (i == 0 || a[i] > a[i - 1]) {
        b[a[i]] = true;
      }
    }
  }
  l = i;
  for (i = n - 2; i >= 0; i--) {
    if (a[i] == n - 2) {
      break;
    } else if (a[i] == n - 1 || (i < n - 2 && a[i] < a[i + 1])) {
      cout << 0 << '\n';
      return;
    } else {
      if (i == n - 2 || a[i] > a[i + 1]) {
        if (b[a[i]]) {
          cout << 0 << '\n';
          return;
        }
        b[a[i]] = true;
      }
    }
  }
  r = i;
  for (i = l + 1; i < r; i++) {
    if (a[i] != n - 2) {
      cout << 0 << '\n';
      return;
    }
  }
  ll ret = 2;
  vi c(l + n - 2 - r);
  int j = 0, k = n - 2, loc = 0;
  while (j < l && k > r) {
    if (a[j] <= a[k]) {
      c[loc] = a[j];
      j++;
    } else {
      c[loc] = a[k];
      k--;
    }
    loc++;
  }
  while (j < l) {
    c[loc] = a[j];
    j++;
    loc++;
  }
  while (k > r) {
    c[loc] = a[k];
    loc++;
    k--;
  }
  int acc = 0, t = 0, t1 = 0;
  for (i = 1; i < c.size(); i++) {
    if (c[i] == c[i - 1]) {
      ret = (ret * (c[i] - i + 1)) % MOD;
    }
  }
  t = r - l;
  for (j = 2; j <= t; j++) {
    ret = (ret * j) % MOD;
  }
  cout << ret << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  cout << fixed << setprecision(12);

  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
    }
  }
  return 0;
}
