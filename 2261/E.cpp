#include <bits/stdc++.h>
#include <climits>
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

constexpr int N = 2e5;
int lim = floor(sqrt(N)) + 1;
ai(N + 1) dp;
vi P;
void solve() {
  int n, k;
  cin >> n >> k;
  vi a(n);
  cin >> a;
  sort(all(a));
  ll acc = 0;
  if (k > 1) {
    vi ndp(a[n - 1] + 1);
    rep(i, k + 1) ndp[i] = 0;
    fora(i, k + 1, a[n - 1] + 1, 1) ndp[i] = dp[i];
    int first = 0;
    while (P[first] < k + 1) {
      first++;
    }
    first--;
    fora(i, k + 1, a[n - 1] + 1, 1) {
      if (first < P.size() - 1 and P[first + 1] == i)
        first++;
      int pos = first;
      while (pos >= 0 and i <= k * P[pos]) {
        if (i % P[pos] == 0) {
          ndp[i] = min(ndp[i], 1 + P[pos] * ndp[i / P[pos]]);
          cout << ndp[i] << ' ';
        }
        pos--;
      }
    }
    // debug(ndp);
    per(i, n) {
      if (a[i] <= k)
        break;
      acc += ndp[a[i]];
    }
  } else {
    per(i, n) {
      if (a[i] <= k)
        break;
      acc += dp[a[i]];
    }
  }
  cout << acc;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);
  vi prime(N + 1, 1);
  prime[0] = prime[1] = 0;

  for (int i = 2; i < N + 1; i++) {
    if (!prime[i])
      continue;

    if (1LL * i * i > N)
      break;
    for (int j = i * i; j <= N; j += i) {
      prime[j] = 0;
    }
  }
  rep(i, N + 1) {
    if (prime[i] == 1)
      P.pb(i);
  }
  dp[0] = 0, dp[1] = 0;
  int last = P.size() - 1;
  fora(i, 2, N + 1, 1) {
    int pos = 0;
    dp[i] = INT_MAX;
    while (pos <= last and P[pos] <= i) {
      if (i % P[pos] == 0) {
        dp[i] = min(dp[i], 1 + P[pos] * dp[i / P[pos]]);
      }
      pos++;
    }
  }
  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
      cout << '\n';
    }
  }
  return 0;
}
