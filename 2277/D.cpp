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

template <typename T> ostream &operator<<(ostream &out, const vector<T> &v) {
  for (int i = 0; i < sz(v); ++i) {
    if (i)
      out << ' ';
    out << v[i];
  }
  return out;
}

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
constexpr int INV2 = (MOD + 1) / 2;
constexpr int MAXM = 61;

ll addmod(ll a, ll b) { return (a + b) % MOD; }
ll submod(ll a, ll b) { return (a - b % MOD + MOD) % MOD; }
ll mulmod(ll a, ll b) { return (__int128)a * b % MOD; }

// least m with 2^m - 1 >= n (= ceil(log2(n+1)))
int cover(ll n) {
  int m = 0;
  while (m < 62 && (1LL << m) - 1 < n)
    ++m;
  return m;
}

ll f_exact(ll n) {
  if (n <= 1)
    return n;
  static unordered_map<ll, ll> memo;
  if (auto it = memo.find(n); it != memo.end())
    return it->second;
  int m = cover(n);
  ll ans = m + (1LL << m) - 1 - n;
  if (m >= 3) {
    ll n2 = n - (1LL << (m - 1)) + 2;
    ans = min(ans, m + f_exact(n2));
  }
  return memo[n] = ans;
}

ll S[MAXM];
ll U[MAXM];
map<ll, ll> pref_memo;

ll pref(ll k); // sum f(1)+...+f(k) mod MOD

void build() {
  S[0] = 0;
  S[1] = 1;
  S[2] = 6;
  U[0] = 0;
  U[1] = 0;
  U[2] = 1; // g(1)=2 <= 2^{1}=2
  fora(m, 3, MAXM, 1) {
    ll bound = 1LL << (m - 1);
    ll lo = 0, hi = bound;
    while (lo < hi) {
      ll mid = (lo + hi + 1) >> 1;
      if (f_exact(mid) + mid <= bound)
        lo = mid;
      else
        hi = mid - 1;
    }
    U[m] = lo;
    ll half = 1LL << (m - 1);
    ll hi_n = (1LL << m) - 1;
    ll R = U[m] + half - 2;
    ll sum = S[m - 1];
    if (R >= half) {
      ll cnt = R - half + 1;
      sum = addmod(sum, mulmod(cnt % MOD, m));
      sum = addmod(sum, submod(pref(U[m]), 1));
    }
    ll T = R + 1;
    if (T < half)
      T = half;
    if (T <= hi_n) {
      ll cnt = hi_n - T + 1;
      // sum_{n=T}^{hi} (m + hi_n - n) = cnt*m + cnt*(cnt-1)/2
      sum = addmod(sum, mulmod(cnt % MOD, m));
      sum = addmod(sum, mulmod(mulmod(cnt % MOD, (cnt - 1) % MOD), INV2));
    }
    S[m] = sum;
  }
}

ll pref(ll k) {
  if (k <= 0)
    return 0;
  if (auto it = pref_memo.find(k); it != pref_memo.end())
    return it->second;
  int m = 0;
  while (m < 62 && (1LL << m) - 1 < k)
    ++m;
  if (k == (1LL << m) - 1)
    return pref_memo[k] = S[m];
  ll half = 1LL << (m - 1);
  ll ans = S[m - 1];
  ll R = U[m] + half - 2;
  ll hi_rec = min(k, R);
  if (hi_rec >= half) {
    ll cnt = hi_rec - half + 1;
    ll up = hi_rec - half + 2; // n' max
    ans = addmod(ans, mulmod(cnt % MOD, m));
    ans = addmod(ans, submod(pref(up), 1));
  }
  ll T = R + 1;
  if (T < half)
    T = half;
  if (k >= T) {
    ll c = m + (1LL << m) - 1; // f(n) = c - n
    ll cnt = k - T + 1;
    ans = addmod(ans, mulmod(cnt % MOD, c % MOD));
    ll pair_sum =
        mulmod(addmod(T % MOD, k % MOD), cnt % MOD);
    pair_sum = mulmod(pair_sum, INV2);
    ans = submod(ans, pair_sum);
  }
  return pref_memo[k] = ans;
}

void solve() {
  ll l, r;
  cin >> l >> r;
  cout << submod(pref(r), pref(l - 1));
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  build();
  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
      cout << '\n';
    }
  }
  return 0;
}
