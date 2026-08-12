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

void solve2() {
  int i = 0, n;
  cin >> n;
  vector<string> s(n);
  for (i = 0; i < n; i++)
    cin >> s[i];
  int sx = 0, sy = 0;
  int c = 0;
  for (i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      if (s[i][j] == '#') {
        sx += i + 1, sy += j + 1, c++;
      }
    }
  int inv = 1;
  while (1LL * inv * c % n != 1)
    ++inv;
  int rx = 1LL * inv * (sx % n) % n;
  int ry = 1LL * inv * (sy % n) % n;
  if (rx == 0)
    rx = n;
  if (ry == 0)
    ry = n;
  cout << rx << ' ' << ry << '\n' << flush;
}
void solve1() {
  int i = 0, n;
  cin >> n;
  vector<string> s(n);
  for (i = 0; i < n; i++)
    cin >> s[i];
  int x, y;
  cin >> x >> y;
  int sx = 0, sy = 0;
  int c = 0;
  for (i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (s[i][j] == '#')
        sx += i + 1, sy += j + 1, c++;
    }
  }
  x *= c, y *= c;
  sx = (x + c * n - sx) % n, sy = (y + c * n - sy) % n;
  if (sx == 0 && sy == 0) {
    for (i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (s[i][j] == '#') {
          cout << i + 1 << ' ' << j + 1 << ' ' << i + 1 << ' ' << j + 1 << '\n'
               << flush;
          return;
        }
      }
    }
  }
  for (i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (s[i][j] == '#' and s[(i + sx) % n][(j + sy) % n] != '#') {
        cout << i + 1 << ' ' << j + 1 << ' ' << (i + sx) % n + 1 << ' '
             << (j + sy) % n + 1 << '\n'
             << flush;
        return;
      }
    }
  }
}

int main() {
  // ios_base::sync_with_stdio(false);
  // cin.tie(nullptr);

  string s;
  cin >> s;
  if (s == "second") {
    int t = 1;
    if (cin >> t) {
      while (t--) {
        solve2();
      }
    }
  }
  if (s == "first") {
    int t = 1;
    if (cin >> t) {
      while (t--) {
        solve1();
      }
    }
  }
  return 0;
}
