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
  if (n < 7) {
    fora(i, 0, (n / 2) * 2, 2) { cout << "10"; }
    if (n & 1)
      cout << "1";
    return;
  } else {
    int z2 = n - 2;
    int div2 = z2 / 3;
    int rem2 = z2 % 3;
    ll two = 0;
    al(3) arr2{div2, div2, div2};
    rep(i, rem2) arr2[i]++;
    if (rem2 == 0 and div2 % 2 == 0) {
      arr2[0]--, arr2[1]++;
    }
    rep(i, 3) two += (arr2[i] * (arr2[i] + 1)) / 2;
    int z3 = n - 3;
    int div3 = z3 / 3;
    int rem3 = z3 % 3;
    ll thr = 0;
    al(3) arr3{div3, div3, div3};
    if (rem3 == 0 and div3 % 2 == 0) {
      arr3[1]--, arr3[2]++;
    } else if (rem3 == 0 and div3 % 2 == 1) {
      arr3[0]--, arr3[2]++;
    } else {
      rep(i, rem3) arr3[i]++;
      if (div3 % 2 == 0) {
        swap(arr3[0], arr3[2]), swap(arr3[1], arr3[2]);
      } else {
        swap(arr3[1], arr3[2]);
      }
    }
    rep(i, 3) thr += (arr3[i] * (arr3[i] + 1)) / 2;
    thr += arr3[1] + 1;
    if (two < thr) {
      rep(i, arr2[0]) cout << '0';
      cout << '1';
      rep(i, arr2[1]) cout << '0';
      cout << '1';
      rep(i, arr2[2]) cout << '0';
    } else {
      cout << '1';
      rep(i, arr3[0]) cout << '0';
      cout << '1';
      rep(i, arr3[1]) cout << '0';
      cout << '1';
      rep(i, arr3[2]) cout << '0';
    }
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
