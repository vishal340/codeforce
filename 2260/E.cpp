#include <bits/stdc++.h>
#include <climits>
using namespace std;

using ll = long long;
using ld = long double;
using vi = vector<int>;
using vll = vector<long long>;
#define ai(x) array<int, (x)>
#define al(x) array<long long, (x)>

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

constexpr int N = 2e5;

void solve() {
  int n, q;
  cin >> n >> q;
  string ss;
  cin >> ss;
  vector<ai(3)> pref(n, {0, 0, 0});
  fora(i, 1, n, 1) {
    rep(j, 3) pref[i][j] = pref[i - 1][j];
    if (ss[i] != ss[i - 1] && ss[i] == '0')
      pref[i][2]++;
    else if (ss[i] == ss[i - 1]) {
      pref[i][ss[i] - '0']++;
    }
  }
  rep(i, q) {
    int l, r;
    cin >> l >> r;
    if (l == r) {
      cout << 3 << '\n';
      continue;
    } else if (l == r - 1) {
      cout << 2 << '\n';
      continue;
    }
    l--, r--;
    int t[3];
    rep(j, 3) { t[j] = pref[r][j] - pref[l][j]; }
    if (ss[r] != ss[l] and ss[l] == '0')
      t[2]++;
    else if (ss[r] == ss[l])
      t[ss[r] - '0']++;
    auto bin = [&](auto self, int tl, int tr) -> int {
      if (tl > tr)
        return INT_MAX;
      int mid = (tr + tl) / 2;
      if (t[2] + max(0, t[1] - mid) + max(0, t[0] - mid) <= mid) {
        return min(mid, self(self, tl, mid - 1));
      } else {
        return self(self, mid + 1, tr);
      }
    };
    cout << 4 * bin(bin, (r - l + 4) / 4, (r - l + 2) / 2) - (r - l + 1)
         << '\n';
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  solve();
  return 0;
}
