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

struct SegTree {
  int n;
  vector<ll> t;

  SegTree(int n_) : n(n_), t(2 * n_) {}

  SegTree(const vector<ll> &a) : n(sz(a)), t(2 * n) {
    for (int i = 0; i < n; ++i)
      t[n + i] = a[i];
    for (int i = n - 1; i; --i)
      t[i] = t[i << 1] + t[i << 1 | 1];
  }

  void add(int p, ll x) {
    for (t[p += n] += x; p >>= 1;)
      t[p] = t[p << 1] + t[p << 1 | 1];
  }

  void set(int p, ll x) {
    for (t[p += n] = x; p >>= 1;)
      t[p] = t[p << 1] + t[p << 1 | 1];
  }

  ll sum(int l, int r) {
    ll res = 0;
    for (l += n, r += n + 1; l < r; l >>= 1, r >>= 1) {
      if (l & 1)
        res += t[l++];
      if (r & 1)
        res += t[--r];
    }
    return res;
  }
};

void solve() {
  int n;
  cin >> n;
  vll a(n);
  cin >> a;
  if (n == 1) {
    cout << 0;
    return;
  }
  sort(all(a));
  ll ret = 0;
  SegTree tree((n - 3) / 2);
  fora(i, 0, (n - 3) / 2, 1) {
    tree.set(i, 2 * a[2 * i + 2] - a[2 * i + 1] - a[2 * i + 3]);
  }
  list<ll> b(all(a));
  int l = 0, r = (n - 5) / 2;
  rep(i, (n - 3) / 2) {
    if (tree.sum(l, r) <= 0) {
      auto t = b.begin();
      auto t1 = next(next(t));
      ret += *t1 - *t;
      b.erase(t);
      b.erase(t1);
      l++;
    } else {
      auto t = prev(b.end());
      auto t1 = prev(prev(t));
      ret += *t - *t1;
      b.erase(t);
      b.erase(t1);
      r--;
    }
  }
  cout << ret + *b.rbegin() - *b.begin();
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
