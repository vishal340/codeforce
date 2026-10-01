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
  int n, q;
  cin >> n >> q;
  int p = 32 - __builtin_clz(n);
  rep(i, q) {
    int x, y;
    cin >> x >> y;
    if (x == y) {
      cout << 0 << '\n';
      continue;
    }
    int z = x & y;
    if (z == 0)
      cout << x + y << '\n';
    else {
      int s = -1, t = -1;
      rep(j, p) {
        if (s != -1 and t != -1)
          break;
        if ((z & (1 << j)) == 0) {
          if ((x & (1 << j)) == 0 and (y & (1 << j)) == 0) {
            cout << x + (ll)(2 * (1 << j)) + y << '\n';
            goto next;
          } else {
            if ((x & (1 << j)) == 0 and s == -1)
              s = (1 << j);
            else if ((y & (1 << j)) == 0 and t == -1) {
              t = (1 << j);
            }
          }
        }
      }
      int k = max(s, t);
      if (s == -1 or t == -1) {
        cout << -1 << '\n';
      } else if ((x & k) == 0 and (y & k) == 0) {
        cout << x + ll(2 * k) + y << '\n';
      } else {
        cout << x + ll(2 * s) + (ll)(2 * t) + y << '\n';
      }
    }
  next:
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  solve();
  return 0;
}
