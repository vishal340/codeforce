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

#define sz(x) static_cast<int>((x).size())
#define all(x) (x).begin(), (x).end()
#define fora(i, l, r, k) for (int(i) = (l); (i) < (r); (i) += (k))
#define forb(i, r, l, k) for (int(i) = (r) - 1; (i) >= (l); (i) -= (k))
#define rep(i, n) fora(i, 0, n, 1)
#define per(i, n) forb(i, n, 0, 1)

#define defret(x)                                                              \
  {                                                                            \
    cout << (x);                                                               \
    return;                                                                    \
  }

template <typename T> istream &operator>>(istream &in, vector<T> &vec) {
  for (int i = 0; i < sz(vec); ++i)
    in >> vec[i];
  return in;
}

template <typename T> ostream &operator<<(ostream &out, const vector<T> &v) {
  for (int i = 0; i < sz(v); ++i) {
    if (i)
      out << ' ';
    out << v[i];
  }
  return out;
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

constexpr ll MOD = 998244353;

struct mll {
  ll val = 0;
  mll(long long v = 0) { val = ll((v % MOD + MOD) % MOD); }
  mll operator+(const mll &o) const { return mll((val + o.val) % MOD); }
  mll operator+(ll o) const { return mll((val + o) % MOD); }
  mll operator*(const mll &o) const { return mll(1LL * val * o.val % MOD); }
  mll operator*(ll o) const { return mll(1LL * val * (o % MOD) % MOD); }
  mll operator/(const mll &o) const { return (*this) * o.inv(); }
  friend mll operator+(ll o, const mll &m) { return mll((o + m.val) % MOD); }
  friend mll operator*(ll o, const mll &m) {
    return mll(1LL * (o % MOD) * m.val % MOD);
  }
  mll &operator+=(const mll &o) {
    val = (val + o.val) % MOD;
    return *this;
  }
  mll &operator+=(ll o) {
    val = ll((val + o) % MOD);
    return *this;
  }
  mll &operator*=(const mll &o) {
    val = 1LL * val * o.val % MOD;
    return *this;
  }
  mll &operator*=(ll o) {
    val = 1LL * val * (o % MOD) % MOD;
    return *this;
  }
  mll &operator/=(const mll &o) { return *this *= o.inv(); }
  mll pow(ll p) const {
    mll a = *this, res = 1;
    for (; p; p >>= 1, a *= a) {
      if (p & 1)
        res *= a;
    }
    return res;
  }
  mll inv() const { return pow(MOD - 2); }
};
ostream &operator<<(ostream &os, const mll &m) { return os << m.val; }
istream &operator>>(istream &is, mll &m) {
  long long v;
  is >> v;
  m = mll(v);
  return is;
}
constexpr int N = 2e5;

void solve() {
  int n;
  cin >> n;
  vi a(n);
  vll b(n);
  cin >> a;
  sort(all(a));
  b[n - 1] = a[n - 1];
  per(i, n - 1) { b[i] = a[i] + b[i + 1]; }
  vector<mll> pref(n);
  pref[0] = 1;
  rep(i, n - 1) { pref[i + 1] = pref[i] * (i + 1); }
  vector<mll> suff(n);
  suff[n - 1] = 1;
  per(i, n - 1) { suff[i] = suff[i + 1] * (i + 1); }
  mll ret = 0;
  rep(i, n - 1) {
    ret +=
        pref[n - 2 - i] * suff[n - 1 - i] * (b[i + 1] - (n - 1 - i) * (ll)a[i]);
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
