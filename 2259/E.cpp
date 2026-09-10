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
#define fora(i, l, r, k) for (int(i) = (l); (i) < (r); (i) += (k))
#define forb(i, r, l, k) for (int(i) = (r) - 1; (i) >= (l); (i) -= (k))
#define rep(i, n) fora(i, 0, n, 1)
#define per(i, n) forb(i, n, 0, 1)

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

struct TwoSat {
  int N;
  vector<vi> gr;
  vi values; // 0 = false, 1 = true

  TwoSat(int n = 0) : N(n), gr(2 * n) {}

  void either(int f, int j) {
    f = max(2 * f, -1 - 2 * f);
    j = max(2 * j, -1 - 2 * j);
    gr[f].push_back(j ^ 1);
    gr[j].push_back(f ^ 1);
  }

  void setValue(int x) { either(x, x); }
  vi val, comp, z;
  int time = 0;

  int dfs(int i) {
    int low = val[i] = ++time, x;
    z.push_back(i);
    for (int e : gr[i])
      if (!comp[e])
        low = min(low, val[e] ? val[e] : dfs(e));
    if (low == val[i])
      do {
        x = z.back();
        z.pop_back();
        comp[x] = low;
        if (values[x >> 1] == -1)
          values[x >> 1] = x & 1;
      } while (x != i);
    return val[i] = low;
  }

  bool solve() {
    values.assign(N, -1);
    val.assign(2 * N, 0);
    comp = val;
    time = 0;
    fora(i, 0, 2 * N, 1) if (!comp[i]) dfs(i);
    fora(i, 0, N, 1) if (comp[2 * i] == comp[2 * i + 1]) return false;
    return true;
  }
};

void solve() {
  int n;
  cin >> n;
  vi a(n);
  cin >> a;
  vi vec(n);
  TwoSat ret(n);
  int count = 0;
  rep(i, n) {
    if (a[i] >= 0) {
      if (a[i] > 0) {
        vec[max(0, i - a[i] + 1)]++;
        if (i + a[i] < n)
          vec[i + a[i]]--;
      }
      if (i < a[i] and i + a[i] >= n) {
        cout << -1;
        return;
      }
      if (i >= a[i] and i + a[i] < n) {
        ret.either(i - a[i], i + a[i]);
      } else if (i >= a[i])
        ret.setValue(i - a[i]);
      else {
        ret.setValue(i + a[i]);
      }
    } else {
      count++;
    }
  }
  if (count == n) {
    rep(i, n) cout << 1;
    return;
  }
  if (vec[0] > 0)
    ret.setValue(~0);
  fora(i, 1, n, 1) {
    vec[i] += vec[i - 1];
    if (vec[i] > 0)
      ret.setValue(~i);
  }
  if (ret.solve()) {
    rep(i, n) cout << ret.values[i];
  } else {
    cout << -1;
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
      cout << '\n';
    }
  }
  return 0;
}
