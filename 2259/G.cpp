#include <algorithm>
#include <bits/stdc++.h>
#include <iterator>
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
  int n;
  ll k;
  cin >> n >> k;
  vll a(n);
  cin >> a;
  vll ret(n, 0);
  fora(i, 1, n - 1, 1) {
    ret[i] = i;
    auto bin_search = [&](auto self, int l, int r) {
      if (l > r)
        return;
      ll mid = (r + l) / 2;
      if ((mid - i) * k < a[mid] - a[i - 1]) {
        ret[i] = mid;
        self(self, mid + 1, r);
      } else if (l + 1 < r) {
        self(self, l, mid - 1);
      }
    };
    bin_search(bin_search, i + 1, n - 1);
  }
  vll b(n);
  b[0] = a[0];
  fora(i, 1, n, 1) b[i] = b[i - 1] + a[i];
  cout << 0 << ' ';
  fora(i, 1, n - 1, 1) {
    ll temp = b[ret[i]] - b[i] - (ll)(ret[i] - i) * a[i - 1] -
              ((ll)(ret[i] - i) * (ret[i] - i + 1)) / 2 * k;
    cout << temp << ' ';
  }
  cout << 0;
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
