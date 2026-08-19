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
  int i = 0, q;
  ll S;
  cin >> S >> q;
  vll d;
  for (ll x = 1; x <= S / x; x++) {
    if (S % x == 0) {
      d.push_back(x);
      if (x != S / x)
        d.push_back(S / x);
    }
  }
  sort(all(d));
  int n = sz(d);
  vll pref(n);
  pref[0] = d[0] * (S / d[0]);
  for (i = 1; i < n; i++)
    pref[i] = pref[i - 1] + (d[i] - d[i - 1]) * (S / d[i]);

  auto area_upto = [&](ll x) -> ll {
    if (x <= 0)
      return 0;
    if (x >= S)
      return pref[n - 1];
    int j = int(upper_bound(all(d), x) - d.begin()) - 1;
    if (j < 0)
      return x * (S / d[0]);
    ll ans = pref[j];
    if (j + 1 < n)
      ans += (x - d[j]) * (S / d[j + 1]);
    return ans;
  };

  while (q--) {
    ll x, y;
    cin >> x >> y;
    if (x <= 0 || y <= 0) {
      cout << 0 << '\n';
      continue;
    }
    x = min(x, S);
    if (y >= S) {
      cout << area_upto(x) << '\n';
      continue;
    }
    int k = int(partition_point(all(d), [&](ll dv) { return S / dv > y; }) -
                d.begin());
    if (k == 0) {
      cout << area_upto(x) << '\n';
      continue;
    }
    ll cut = d[k - 1];
    if (x <= cut)
      cout << x * y << '\n';
    else
      cout << cut * y + area_upto(x) - pref[k - 1] << '\n';
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
