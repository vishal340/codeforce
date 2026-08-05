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

constexpr int MOD = 1e9 + 7;

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

template <typename T>
concept Printable = requires(ostream &os, const T &value) {
  { os << value } -> same_as<ostream &>;
} && !is_array_v<T>;
template <Printable T> void __print(const T &x) { cerr << x; }
template <typename T, typename V> void __print(const pair<T, V> &x) {
  cerr << '{';
  __print(x.first);
  cerr << ", ";
  __print(x.second);
  cerr << '}';
}
template <typename T>
  requires(!Printable<T>)
void __print(const T &x) {
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

const int N = 2e5;

void solve() {
  int i, n, m, k;
  cin >> n >> m >> k;
  vll a(n * m);
  vll b(n, 0), c(m, 0);
  for (i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      cin >> a[i * m + j];
      b[i] += a[i * m + j];
      c[j] += a[i * m + j];
    }
  if (n > 1 && m > 1) {
    for (i = 0; i < n; i++)
      for (int j = 0; j < m; j++) {
        a[i * m + j] = 3 * a[i * m + j] - b[i] - c[j];
      }
    sort(all(a));
    if (a[n * m - k] >= 0) {
      cout << 0 << '\n';
      return;
    } else {
      cout << (n + m - 4 - a[n * m - k]) / (n + m - 3) << '\n';
      return;
    }
  } else {
    if (n * m == 1 && a[0] < 0) {
      cout << -1 << '\n';
      return;
    }
    ll total = 0;
    for (i = 0; i < n * m; i++)
      total += a[i];
    vector<pair<ll, int>> sa(n * m);
    for (i = 0; i < n * m; i++)
      sa[i].first = a[i], sa[i].second = i;
    sort(sa.begin(), sa.end(),
         [](auto a, auto b) { return a.first < b.first; });
    if (2 * sa[n * m - k].first >= total) {
      cout << 0 << '\n';
      return;
    }
    if (sa[n * m - k].second == k - 1 || sa[n * m - k].second == (n * m - k)) {
      int temp = (n + m - 3 + total - 2 * sa[n * m - k].first) / (n + m - 2);
      if (k > 1 && sa[n * m - k + 1].first - temp < sa[n * m - k].first) {
        temp = (n + m - 4 + total - 2 * sa[n * m - k].first) / (n + m - 3);
      }
      cout << temp << '\n';
    } else {
      cout << (n + m - 4 + total - 2 * sa[n * m - k].first) / (n + m - 3)
           << '\n';
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
    }
  }
  return 0;
}
