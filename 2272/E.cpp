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
constexpr int N = 200000;

void solve() {
  int n, m;
  cin >> n >> m;
  string s, t;
  cin >> s >> t;
  vi sow, sot, prefs, preft;
  int ls = -1, lt = -1;
  rep(i, m) {
    if (t[i] == 'W') {
      if (lt != -1)
        sot.pb(i - lt - 1);
      lt = i;
    }
  }
  sort(all(sot));
  rep(i, n) {
    if (s[i] == 'W') {
      if (ls != -1)
        sow.pb(i - ls - 1);
      ls = i;
    }
  }
  sort(all(sow));
  preft.pb(0);
  for (auto it : sot) {
    preft.pb(it + preft.back());
  }
  prefs.pb(0);
  for (auto it : sow) {
    prefs.pb(it + prefs.back());
  }

  int flags = -1;
  rep(i, n) {
    if (s[i] == 'R' and flags != -1)
      cout << m + 1 + max(i - flags - m + lt, 0) * (lt != -1) << ' ';
    else if (s[i] == 'W')
      flags = i;
    else {
      if (lt == -1) {
        cout << 1 << ' ';
      } else {
        auto it = upper_bound(all(sot), i);
        int dist = distance(sot.begin(), it);
        cout << preft[dist] + dist + (sz(sot) + 1 - dist) * (i + 1) + 1 << ' ';
      }
    }
  }
  cout << '\n';
  int flagt = -1;
  rep(i, m) {
    if (t[i] == 'R' and flagt != -1)
      cout << n + 1 + max(i - flagt - n + ls, 0) * (ls != -1) << ' ';
    else if (t[i] == 'W')
      flagt = i;
    else {
      if (ls == -1) {
        cout << 1 << ' ';
      } else {
        auto it = upper_bound(all(sow), i);
        int dist = distance(sow.begin(), it);
        cout << prefs[dist] + dist + (sz(sow) + 1 - dist) * (i + 1) + 1 << ' ';
      }
    }
  }
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
