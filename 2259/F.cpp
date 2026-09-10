#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;
using vi = vector<int>;
using vll = vector<long long>;
#define ai(x) array<int, (x)>
#define al(x) array<long long, (x)>

#define sz(x) static_cast<int>((x).size())
#define all(x) (x).begin(), (x).end()
#define fora(i, l, r, k) for (int(i) = (l); (i) < (r); (i) += (k))
#define forb(i, r, l, k) for (int(i) = (r) - 1; (i) >= (l); (i) -= (k))
#define rep(i, n) fora(i, 0, n, 1)
#define per(i, n) forb(i, n, 0, 1)

#define defret(x)                                                              \
  {                                                                            \
    cout << (x);                                                               \
    return;                                                                    \
  }

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

void solve() {
  int n;
  cin >> n;
  vi a(n);
  cin >> a;
  string s;
  cin >> s;
  deque<int> cons1, cons0;
  bool f = false;
  int cnt = 0;
  rep(i, n) {
    if (a[i] == 0) {
      if (f) {
        if (a[i - 1] == 0)
          cnt++;
        else {
          cons1.push_back(cnt);
          cnt = 1;
        }
      }
    } else {
      if (i == 0)
        cnt = 1;
      else if (i > 0 and a[i - 1] == 1) {
        cnt++;
      } else if (a[i - 1] == 0) {
        if (cnt > 0)
          cons0.push_back(cnt);
        cnt = 1;
      }
      f = true;
    }
  }
  if (a[n - 1] == 0)
    cons0.push_back(cnt);
  ll sum1 = 0, sum0 = 0;
  for (int x : cons1)
    sum1 += x;
  for (int x : cons0)
    sum0 += x;
  if (sum0 == 0 or sum1 == 0) {
    rep(i, n + 1) { cout << 0 << ' '; }
    return;
  }
  vll ret(n + 1);
  {
    vll acc(cons0.size());
    ll temp = 0;
    per(i, cons0.size()) {
      temp += cons0[i];
      acc[i] = temp;
    }
    temp = 0;
    fora(i, 0, cons1.size(), 1) { temp += cons1[i] * acc[i]; }
    ret[0] = temp;
  }
  rep(i, n) {
    if (s[i] == '0') {
      if (cons0.size() == 0) {
        ret[i + 1] = 0;
        continue;
      }
      ret[i + 1] = ret[i] - sum1;
      cons0.back()--;
      --sum0;
      if (cons0.back() == 0) {
        sum1 -= cons1.back();
        cons1.pop_back();
        cons0.pop_back();
      }
    } else {
      if (cons1.size() == 0) {
        ret[i + 1] = 0;
        continue;
      }
      ret[i + 1] = ret[i] - sum0;
      cons1.front()--;
      --sum1;
      if (cons1.front() == 0) {
        sum0 -= cons0.front();
        cons0.pop_front();
        cons1.pop_front();
      }
    }
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
