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

constexpr int MOD = 1e9 + 7;

void solve() {
  int n;
  cin >> n;
  vi a(n);
  cin >> a;
  vi bad(n + 1, 0);
  vector<pii> good;
  for (int i = 0; i < n; i++) {
    ll t1 = a[i] * (ll)(i + 1);
    if (t1 < n) {
      bad[t1]++;
      bad[min(n, (a[i] + 1) * (i + 1))]--;
    }
    for (int j = 0; j < min(a[i], (n + i) / (i + 1)); j++) {
      good.pb({(i + 1) * j, min(n - 1, (i + 1) * (j + 1) - 1)});
    }
  }
  sort(all(good),
       [](auto a, auto b) { return a.second - a.first < b.second - b.first; });
  set<pair<int, int>> st;
  for (auto [l, r] : good) {
    auto it = st.lower_bound({l, 0});
    if (it == st.end() or it->second > r)
      st.insert({l, r});
  }
  vector<pair<int, int>> lr(all(st));
  vi ones(n, 0);
  int acc = 0;
  for (int i = 0; i < n; i++) {
    acc += bad[i];
    if (acc == 0) {
      ones[i] = 1;
    }
  }
  acc = 1;
  int l = 0, r = 0;
  vector<int> ret(n + 1, 0);
  ret[0] = 1;
  fora(i, 0, n, 1) {
    if (ones[i] == 0)
      continue;
    while (l < lr.size() and lr[l].second < i) {
      acc = (acc + MOD - ret[l]) % MOD;
      l++;
    }
    while (r < lr.size() and lr[r].first <= i) {
      r++;
    }
    ret[r] = (ret[r] + acc) % MOD;
    acc = (acc * 2) % MOD;
  }
  cout << ret[lr.size()];
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
