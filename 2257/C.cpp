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

void solve() {
  int i = 0, n, m;
  cin >> n;
  vi a(n + 1);
  vector<vi> g(n + 1);
  for (i = 2; i <= n; i++) {
    cin >> a[i];
    g[i].push_back(a[i]);
    g[a[i]].push_back(i);
  }
  cin >> m;
  vi b(m);
  for (i = 0; i < m; i++) {
    cin >> b[i];
  }
  sort(all(b));
  cout << m - 1 << ' ';
  if (b[0] == 1) {
    for (i = 1; i < m; i++)
      cout << b[i] << ' ';
    cout << '\n';
    return;
  }
  vector<bool> vis(n + 1, false);
  int ex = -1;
  priority_queue<int> pq;
  for (i = 0; i < m; i++) {
    for (auto it : g[b[i]]) {
      if (!vis[it]) {
        pq.push(it);
      }
    }
    while (!pq.empty()) {
      auto t = pq.top();
      pq.pop();
      vis[t] = 1;
      if (binary_search(all(b), t)) {
        ex = b[i];
        goto aa;
      }
      for (auto it1 : g[t]) {
        if (!vis[it1]) {
          pq.push(it1);
        }
      }
    }
  }
aa:;
  if (ex == -1) {
    for (i = 0; i < m - 1; i++)
      cout << b[i] << ' ';
  } else {
    for (i = 0; i < m; i++) {
      if (ex != b[i])
        cout << b[i] << ' ';
    }
  }
  cout << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
    }
  }
  return 0;
}
