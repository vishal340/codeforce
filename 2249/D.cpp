#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using ld = long double;
using vi = vector<int>;
using vll = vector<long long>;
using pii = pair<int, int>;
using pll = pair<long long, long long>;
using ci= const int;
using cll = const ll;
using cld = const ld;
using cvi = const vi;
using cvll = const vll;
using cpii = const pii;
using cpll = const pll;

#define sz(x) static_cast<int>((x).size())
#define all(x) (x).begin(), (x).end()

template<typename T>
istream &operator>>(istream &in, vector<T> &vec) {
  for (int i = 0; i < sz(vec); ++i) in >> vec[i];
  return in;
}

template<typename T>
ostream &operator<<(ostream &out, const vector<T> &v) {
  for (int i = 0; i < sz(v); ++i) {
    if (i) out << ' ';
    out << v[i];
  }
  return out;
}

template<typename T, typename U>
bool mini(T &a, const U &b) {
  if (a > b) { a = b; return true; }
  return false;
}

template<typename T, typename U>
bool maxi(T &a, const U &b) {
  if (a < b) { a = b; return true; }
  return false;
}

template<typename T>
concept Printable = requires(ostream &os, const T &value) {
  { os << value } -> same_as<ostream&>;
} && !is_array_v<T>;
template<Printable T>
void __print(const T &x) { cerr << x; }
template<typename T, typename V>
void __print(const pair<T, V> &x) { cerr << '{'; __print(x.first); cerr << ", "; __print(x.second); cerr << '}'; }
template<typename T>
  requires (!Printable<T>)
void __print(const T &x) { int f = 0; cerr << '{'; for (auto &i: x) { cerr << (f++ ? ", " : ""); __print(i); } cerr << '}'; }
void _print() { cerr << "]\n"; }
template <typename T, typename... V>
void _print(T t, V... v) { __print(t); if (sizeof...(v)) cerr << ", "; _print(v...); }

#ifndef ONLINE_JUDGE
#define debug(x...) cerr << "[" << #x << "] = ["; _print(x)
#else
#define debug(x...)
#endif

void solve() {
int i = 0, n;
cin >> n;
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
