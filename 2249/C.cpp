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

struct SegTree {
  int n;
  vi t, lz;

  SegTree(int n_) : n(n_), t(4 * n_), lz(4 * n_) {}

  void apply(int v, int x) {
    t[v] += x;
    lz[v] += x;
  }

  void push(int v) {
    if (!lz[v])
      return;
    apply(v << 1, lz[v]);
    apply(v << 1 | 1, lz[v]);
    lz[v] = 0;
  }

  void pull(int v) { t[v] = max(t[v << 1], t[v << 1 | 1]); }

  void upd(int v, int l, int r, int ql, int qr, int x) {
    if (ql >= qr)
      return;
    if (ql == l && qr == r)
      return apply(v, x);
    push(v);
    int m = (l + r) >> 1;
    if (ql < m)
      upd(v << 1, l, m, ql, min(qr, m), x);
    if (qr > m)
      upd(v << 1 | 1, m, r, max(ql, m), qr, x);
    pull(v);
  }

  int qry(int v, int l, int r, int ql, int qr) {
    if (ql >= qr)
      return 0;
    if (ql == l && qr == r)
      return t[v];
    push(v);
    int m = (l + r) >> 1;
    int res = 0;
    if (ql < m)
      res = max(res, qry(v << 1, l, m, ql, min(qr, m)));
    if (qr > m)
      res = max(res, qry(v << 1 | 1, m, r, max(ql, m), qr));
    return res;
  }

  void add(int l, int r, int x) { upd(1, 0, n, l, r, x); }
  int getmax(int l, int r) { return qry(1, 0, n, l, r); }
};

void solve() {
  int n;
  cin >> n;
  vi p(n);
  for (int &x : p)
    cin >> x;

  int pos = find(all(p), 1) - p.begin();
  rotate(p.begin(), p.begin() + pos, p.end());

  vi pos_of(n + 1);
  for (int i = 0; i < n; i++)
    pos_of[p[i]] = i;

  SegTree st(n);
  vi seen(n + 2);
  for (int i = 0; i < n; i++) {
    seen[p[i]] = 1;
    if (!seen[p[i] - 1])
      st.add(i, n, 1);
    if (seen[p[i] + 1])
      st.add(i, n, -1);
  }

  int ans = 0;
  if (st.getmax(0, n) <= 2)
    ans++;

  for (int i = 1; i < n; i++) {
    int x = p[i - 1];
    if (x != 1) {
      int j = pos_of[x - 1];
      if (i - 1 < j) {
        st.add(i - 1, j, -1);
      } else {
        st.add(i - 1, n, -1);
        st.add(0, j, -1);
      }
    } else {
      st.add(1, n, -1);
    }

    if (x < n) {
      int j = pos_of[x + 1];
      if (i - 1 < j) {
        st.add(j, n, 1);
        st.add(0, i - 1, 1);
      } else {
        st.add(j, i - 1, 1);
      }
    }

    st.add(i, i + 1, 1);

    if (st.getmax(0, n) <= 2)
      ans++;
  }

  cout << ans << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
