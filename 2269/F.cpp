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

void solve() {
  int n;
  cin >> n;
  vi p(n + 2), R(n + 2), L(n + 2, n + 1);
  vll dp(n + 2);
  vector<vi> vec(n + 2);
  vb mark(n + 2, false);
  fora(i, 1, n + 1, 1) cin >> p[i];
  forb(i, n + 1, 1, 1) {
    for (R[i] = i + 1; R[i] <= n && p[R[i]] < p[i]; R[i] = R[R[i]])
      ;
    L[R[i]] = i;
  }
  auto solve_block = [&](int l, int r) {
    dp[r] = r - l;
    int lst = r, cnt = 0;
    if (L[r] != n + 1) {
      vec[L[r]].pb(r);
      mark[r] = true;
      cnt++;
    }
    forb(i, r, l, 1) {
      while (!mark[lst])
        lst--;
      if (lst == R[i]) {
        dp[i] = dp[R[i]] + (r - i - 1);
      } else {
        int c2 = R[i] - i - 1 + (R[R[i]] != n + 1) + cnt - 1 -
                 (R[R[i]] != n + 1 && L[R[R[i]]] <= i);
        dp[i] = dp[lst] + 2 * (r - i) - 2 - c2;
      }
      for (int j : vec[i]) {
        mark[j] = false;
        cnt--;
      }
      if (L[i] != n + 1) {
        vec[L[i]].pb(i);
        mark[i] = true;
        cnt++;
      }
    }
    fora(i, l, r + 1, 1) dp[i] += l - 1;
  };
  int l = 1;
  fora(r, 1, n + 1, 1) {
    if (R[r] == n + 1) {
      solve_block(l, r);
      l = r + 1;
    }
  }
  ll ans = 0;
  fora(i, 1, n + 1, 1) ans += dp[i];
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
