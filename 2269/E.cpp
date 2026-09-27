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
  vi a(n), L(n), R(n);
  cin >> a;
  rep(i, n) {
    L[i] = i - 1;
    while (L[i] != -1 and a[i] > a[L[i]])
      L[i] = L[L[i]];
  }
  per(i, n) {
    R[i] = i + 1;
    while (R[i] != n and a[i] >= a[R[i]])
      R[i] = R[R[i]];
  }
  int cur_iter = 0;
  ai(1 << 18) pos{}, iter{};
  auto la = [&](int mask) -> bool {
    // Exists [l,r] with max a[i] (i in [l,r]), (a[i]&mask)==mask,
    // and XOR_{j=l..r}(a[j]&mask) == mask  iff  XOR of others == 0.
    // Iterate the shorter side; lookup matching XOR on the longer side.
    {
      // pref = XOR of a[0..i-1] & mask; pos[p] = index i with that pref
      int pref = 0;
      cur_iter++;
      rep(i, n) {
        if ((a[i] & mask) == mask) {
          // empty right: need XOR[l..i-1] == 0
          if (iter[pref] == cur_iter and pos[pref] > L[i] and pos[pref] < i)
            return true;
          if (i - L[i] > R[i] - i) {
            int rx = 0;
            fora(r, i + 1, R[i], 1) {
              rx ^= a[r] & mask;
              if (rx == 0)
                return true; // empty left
              int need = pref ^ rx; // want XOR[0..l-1] == need
              if (iter[need] == cur_iter and pos[need] > L[i] and
                  pos[need] < i)
                return true;
            }
          }
        }
        pos[pref] = i;
        iter[pref] = cur_iter;
        pref ^= a[i] & mask;
      }
    }
    {
      // pref = XOR of a[i+1..n-1] & mask; pos[p] = index i with that pref
      int pref = 0;
      cur_iter++;
      per(i, n) {
        if ((a[i] & mask) == mask) {
          // empty left: need XOR[i+1..r] == 0
          if (iter[pref] == cur_iter and pos[pref] < R[i] and pos[pref] > i)
            return true;
          if (i - L[i] <= R[i] - i) {
            int lx = 0;
            forb(l, i, L[i] + 1, 1) {
              lx ^= a[l] & mask;
              if (lx == 0)
                return true; // empty right
              int need = pref ^ lx; // want XOR[r+1..n-1] == need
              if (iter[need] == cur_iter and pos[need] < R[i] and
                  pos[need] > i)
                return true;
            }
          }
        }
        pos[pref] = i;
        iter[pref] = cur_iter;
        pref ^= a[i] & mask;
      }
    }
    return 0;
  };
  int ret = 0;
  per(i, 18) {
    if (la(ret | (1 << i)))
      ret |= (1 << i);
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
