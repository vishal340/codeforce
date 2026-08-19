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
  int i = 0, n;
  ll x;
  cin >> n >> x;
  vi m(n);
  vector<vi> a(n), b(n);
  for (i = 0; i < n; i++) {
    cin >> m[i];
    a[i].resize(m[i]);
    b[i].resize(m[i]);
    for (int j = 0; j < m[i]; j++)
      cin >> a[i][j];
    for (int j = 0; j < m[i]; j++) {
      cin >> b[i][j];
      b[i][j] -= a[i][j];
    }
  }
  vector<queue<array<ll, 4>>> acc(n);
  vi pos(n, 0);
  for (i = 0; i < n; i++) {
    array<ll, 4> t{i, 0, 0, 0};
    int j = 0;
    while (j < m[i]) {
      t[1]++;
      t[2] = max(t[2], a[i][j] - t[3]);
      t[3] += b[i][j];
      if (t[3] >= 0) {
        acc[i].push(t);
        t[2] = 0;
        t[3] = 0;
      }
      j++;
    }
  }
  priority_queue<array<ll, 4>, vector<array<ll, 4>>,
                 decltype([](auto a, auto b) { return a[2] > b[2]; })>
      pq;
  for (i = 0; i < n; i++) {
    if (!acc[i].empty()) {
      pq.push(acc[i].front());
      acc[i].pop();
    }
  }
  while (!pq.empty()) {
    auto t = pq.top();
    pq.pop();
    if (t[2] > x) {
      break;
    } else {
      x += t[3];
      pos[t[0]] = t[1];
    }
    if (!acc[t[0]].empty()) {
      pq.push({acc[t[0]].front()});
      acc[t[0]].pop();
    }
  }
  for (i = 0; i < n; i++) {
    auto t = x;
    int j = pos[i];
    while (j < m[i] && t >= a[i][j]) {
      t += b[i][j];
      j++;
      pos[i] = j;
    }
  }
  pii ret{pos[0], 1};
  for (i = 1; i < n; i++) {
    if (ret.first < pos[i])
      ret = make_pair(pos[i], i + 1);
  }
  cout << ret.first << ' ' << ret.second << '\n';
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
