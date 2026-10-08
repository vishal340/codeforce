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

void solve() {
  int n;
  cin >> n;
  vector<vi> a(2 * n, vi(2 * n));
  vector<vi> b(2 * n, vi(2 * n));
  cin >> a;
  bool flag = true;
  rep(i, 2 * n) rep(j, 2 * n) {
    a[i][j]--;
    b[a[i][j]][j] = i;
  }
  vector<pii> ret;
  auto fn = [&](int k, int l) {
    swap(a[k][l], a[k + 1][l]);
    swap(a[k][l + 1], a[k + 1][l + 1]);
    ret.pb({k, l});
    b[a[k][l]][l] = k;
    b[a[k + 1][l]][l] = k + 1;
    b[a[k][l + 1]][l + 1] = k;
    b[a[k + 1][l + 1]][l + 1] = k + 1;
  };
  rep(i, 2 * n - 2) {
    int j = 0;
    if (b[i][j + 1] == i) {
      if (b[i][j] == i + 1)
        fn(i + 1, j);
      if (b[i][j] > i)
        fn(i, j);
    }
    if (b[i][j] > b[i][j + 1]) {
      while (b[i][j] > b[i][j + 1] + 1) {
        int l = b[i][j] - 1;
        fn(l, j);
      }
      fn(b[i][j + 1], j + 1);
    } else if (b[i][j] < b[i][j + 1]) {
      while (b[i][j] + 1 < b[i][j + 1]) {
        int l = b[i][j + 1] - 1;
        if (b[i][j + 1] == b[i][j + 2])
          fn(l, j + 1);
        else
          fn(l, j);
      }
      fn(b[i][j + 1] - 1, j + 1);
    }
    while (b[i][j] != i) {
      int l = b[i][j] - 1;
      fn(l, j);
    }
    fora(j, 2, 2 * n - 2, 2) {
      if (b[i][j + 1] == i) {
        if (b[i][j] == i + 1)
          fn(i + 1, j);
        if (b[i][j] > i)
          fn(i, j);
      }
      if (b[i][j] > b[i][j + 1])
        while (b[i][j] > b[i][j + 1]) {
          int l = b[i][j] - 1;
          fn(l, j - 1);
        }
      else if (b[i][j] < b[i][j + 1]) {
        while (b[i][j] + 1 < b[i][j + 1]) {
          int l = b[i][j + 1] - 1;
          if (b[i][j + 1] == b[i][j + 2])
            fn(l, j + 1);
          else
            fn(l, j);
        }
        fn(b[i][j + 1] - 1, j + 1);
      }
      while (b[i][j] != i) {
        int l = b[i][j] - 1;
        fn(l, j);
      }
    }
    j = 2 * n - 1;
    if (b[i][j] == i) {
      if (b[i][j - 1] == i + 1)
        fn(i + 1, j - 1);
      if (b[i][j - 1] > i)
        fn(i, j - 1);
    } else if (b[i][j - 1] == i) {
      if (b[i][j] == i + 1)
        fn(i + 1, j - 1);
      fn(i, j - 1);
    }
    if (b[i][j] > b[i][j - 1]) {
      while (b[i][j] > b[i][j - 1] + 1) {
        int l = b[i][j] - 1;
        fn(l, j - 1);
      }
      fn(b[i][j - 1], j - 2);
    } else if (b[i][j] < b[i][j - 1]) {
      while (b[i][j] + 1 < b[i][j - 1]) {
        int l = b[i][j - 1] - 1;
        if (b[i][j - 1] == b[i][j - 2])
          fn(l, j - 2);
        else
          fn(l, j - 1);
      }
      fn(b[i][j - 1] - 1, j - 2);
    }
    while (b[i][j] != i) {
      int l = b[i][j] - 1;
      fn(l, j - 1);
    }
  }
  int t = 0;
  int i = 2 * n - 2;
  rep(j, 2 * n) {
    if (a[i][j] != i)
      t++;
  }
  if (t & 1) {
    cout << -1 << '\n';
    return;
  }
  rep(j, 2 * n - 1) {
    if (a[i][j] != i) {
      fn(i, j);
    }
  }
  cout << sz(ret) << '\n';
  for (auto it : ret) {
    cout << it.first + 1 << ' ' << it.second + 1 << '\n';
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
    }
  }
  return 0;
}
