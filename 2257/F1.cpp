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

constexpr int arr_max = 10;
constexpr ll INF = (ll)1e18;
using mdarray = array<array<ll, arr_max>, arr_max>;

mdarray make_inf() {
  mdarray m{};
  for (int i = 0; i < arr_max; i++)
    for (int j = 0; j < arr_max; j++)
      m[i][j] = INF;
  return m;
}

// Min-plus identity: 0 on diagonal, INF elsewhere.
mdarray make_id() {
  mdarray m = make_inf();
  for (int i = 0; i < arr_max; i++)
    m[i][i] = 0;
  return m;
}

mdarray merge_mat(const mdarray &A, const mdarray &B, int x) {
  mdarray C = make_inf();
  for (int i = 0; i < x; i++)
    for (int j = 0; j < x; j++)
      for (int k = 0; k < x; k++)
        C[i][j] = min(C[i][j], A[i][k] + B[k][j]);
  return C;
}

struct SegTree {
  int n, x;
  vector<mdarray> st;

  // Closed intervals [l, r]. Leaves when l == r. Tree size 4n (no power-of-2
  // pad).
  SegTree(int n_, int x_) : n(n_), x(x_), st(4 * n_, make_inf()) {}

  SegTree(int x_, const vector<array<ll, 2>> &a) : SegTree(sz(a), x_) {
    build(1, 0, n - 1, a);
  }

  void set_leaf(int v, int i, const vector<array<ll, 2>> &a) {
    st[v] = make_inf();
    ll d = a[i][0], s = a[i][1];
    for (int p = 0; p < min<ll>(x, d); p++)
      for (int q = 0; q < x; q++)
        st[v][p][q] = ((d + q - p + x - 1) / x - 1) * s;
  }

  void pull(int v) { st[v] = merge_mat(st[v << 1], st[v << 1 | 1], x); }

  void build(int v, int l, int r, const vector<array<ll, 2>> &a) {
    if (l == r) {
      set_leaf(v, l, a);
      return;
    }
    int m = (l + r) >> 1;
    build(v << 1, l, m, a);
    build(v << 1 | 1, m + 1, r, a);
    pull(v);
  }

  void upd(int v, int l, int r, int q, const vector<array<ll, 2>> &a) {
    if (l == r) {
      set_leaf(v, l, a);
      return;
    }
    int m = (l + r) >> 1;
    if (q <= m)
      upd(v << 1, l, m, q, a);
    else
      upd(v << 1 | 1, m + 1, r, q, a);
    pull(v);
  }

  mdarray qry(int v, int l, int r, int ql, int qr) {
    if (ql > qr || qr < l || ql > r)
      return make_id();
    if (ql <= l && r <= qr)
      return st[v];
    int m = (l + r) >> 1;
    if (qr <= m)
      return qry(v << 1, l, m, ql, qr);
    if (ql > m)
      return qry(v << 1 | 1, m + 1, r, ql, qr);
    return merge_mat(qry(v << 1, l, m, ql, qr),
                     qry(v << 1 | 1, m + 1, r, ql, qr), x);
  }

  void update(int i, const vector<array<ll, 2>> &a) { upd(1, 0, n - 1, i, a); }
  mdarray query(int l, int r) { return qry(1, 0, n - 1, l, r); }
};

void solve() {
  int i = 0, n, x, q;
  cin >> n >> q >> x;
  vector<array<ll, 2>> a(n);
  for (i = 0; i < n; i++)
    cin >> a[i][0];
  for (i = 0; i < n; i++)
    cin >> a[i][1];
  SegTree tree(x, a);
  for (i = 0; i < q; i++) {
    char c;
    int y, z;
    cin >> c >> y >> z;
    if (c == '?') {
      int l = y - 1, r = z - 1;
      if (l == r) {
        // ceil((d-1)/x) * s
        cout << ((a[l][0] + x - 2) / x) * a[l][1] << '\n';
      } else {
        // matrix on closed [l, r-1], then land on platform r
        auto t = tree.query(l, r - 1)[0];
        ll cur = INF;
        for (int j = 0; j < min<ll>(x, a[r][0]); j++) {
          cur = min(cur, t[j] + ((a[r][0] - j - 1 + x - 1) / x) * a[r][1]);
        }
        cout << cur << '\n';
      }
    } else {
      if (c == '1') {
        a[y - 1][0] = z;
      } else if (c == '2') {
        a[y - 1][1] = z;
      }
      tree.update(y - 1, a);
    }
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  solve();
  return 0;
}
