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
constexpr int B = 125000;

struct SegTree {
  int n, x, base; // covers platforms [base, base + n)
  vector<mdarray> st;

  SegTree(int x_, const vector<array<int, 2>> &a, int base_, int len)
      : n(len), x(x_), base(base_), st(4 * max(len, 1), make_inf()) {
    if (len > 0)
      build(1, base, base + n - 1, a);
  }

  void set_leaf(int v, int i, const vector<array<int, 2>> &a) {
    st[v] = make_inf();
    ll d = a[i][0], s = a[i][1];
    for (int p = 0; p < min<ll>(x, d); p++)
      for (int q = 0; q < x; q++)
        st[v][p][q] = ((d + q - p + x - 1) / x - 1) * s;
  }

  void pull(int v) { st[v] = merge_mat(st[v << 1], st[v << 1 | 1], x); }

  void build(int v, int l, int r, const vector<array<int, 2>> &a) {
    if (l == r) {
      set_leaf(v, l, a);
      return;
    }
    int mid = (l + r) >> 1;
    build(v << 1, l, mid, a);
    build(v << 1 | 1, mid + 1, r, a);
    pull(v);
  }

  void upd(int v, int l, int r, int q, const vector<array<int, 2>> &a) {
    if (l == r) {
      set_leaf(v, l, a);
      return;
    }
    int mid = (l + r) >> 1;
    if (q <= mid)
      upd(v << 1, l, mid, q, a);
    else
      upd(v << 1 | 1, mid + 1, r, q, a);
    pull(v);
  }

  mdarray qry(int v, int l, int r, int ql, int qr) {
    if (ql > qr || qr < l || ql > r)
      return make_id();
    if (ql <= l && r <= qr)
      return st[v];
    int mid = (l + r) >> 1;
    if (qr <= mid)
      return qry(v << 1, l, mid, ql, qr);
    if (ql > mid)
      return qry(v << 1 | 1, mid + 1, r, ql, qr);
    return merge_mat(qry(v << 1, l, mid, ql, qr),
                     qry(v << 1 | 1, mid + 1, r, ql, qr), x);
  }

  bool covers(int i) const { return i >= base && i < base + n; }

  void update(int i, const vector<array<int, 2>> &a) {
    if (!covers(i))
      return;
    upd(1, base, base + n - 1, i, a);
  }

  mdarray query(int l, int r) {
    if (l > r)
      return make_id();
    return qry(1, base, base + n - 1, l, r);
  }
};

struct queries {
  char c;
  int x, y;
};

void solve() {
  int i = 0, n, x, q;
  cin >> n >> q >> x;
  vector<array<int, 2>> a(n), b(n);
  for (i = 0; i < n; i++)
    cin >> a[i][0];
  for (i = 0; i < n; i++)
    cin >> a[i][1];
  vector<queries> Q(q);
  for (i = 0; i < q; i++) {
    char c;
    int y, z;
    cin >> c >> y >> z;
    Q[i] = {c, y, z};
  }

  vector<mdarray> ret(q, make_id());
  vector<array<int, 2>> last(q); // platform r at query time
  vector<char> got_last(q, 0);

  copy(all(a), b.begin());
  for (int base = 0; base < n; base += B) {
    int len = min(B, n - base);
    copy(all(b), a.begin());
    SegTree tree(x, a, base, len);

    for (i = 0; i < q; i++) {
      if (Q[i].c == '?') {
        int L = Q[i].x - 1, R = Q[i].y - 1; // inclusive
        if (!got_last[i]) {
          last[i] = a[R];
          got_last[i] = 1;
        }
        // matrix on [L, R-1] ∩ [base, base+len)
        if (L < R) {
          int lo = max(L, base);
          int hi = min(R - 1, base + len - 1);
          if (lo <= hi)
            ret[i] = merge_mat(ret[i], tree.query(lo, hi), x);
        }
      } else {
        int id = Q[i].x - 1;
        if (Q[i].c == '1')
          a[id][0] = Q[i].y;
        else
          a[id][1] = Q[i].y;
        tree.update(id, a);
      }
    }
  }

  for (i = 0; i < q; i++) {
    if (Q[i].c != '?')
      continue;
    ll d = last[i][0], s = last[i][1];
    ll cur = INF;
    for (int j = 0; j < min<ll>(x, d); j++)
      cur = min(cur, ret[i][0][j] + ((d - j + x - 2) / x) * s);
    if (d == 0)
      cur = ret[i][0][0]; // identity / reach end with no cells
    cout << cur << '\n';
  }
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  solve();
  return 0;
}
