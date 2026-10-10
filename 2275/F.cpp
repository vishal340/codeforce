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

constexpr int MOD = 1e9 + 7;
constexpr int N = 1e6;
vector<int> primes;
void solve() {
  int n;
  cin >> n;
  vi a(n);
  cin >> a;
  map<int, int> cnt;
  rep(i, n) {
    int t = a[i];
    int num = 1;
    for (auto it : primes) {
      if (it * it > t)
        break;
      int t1 = 0;
      if (t % it == 0) {
        do {
          t1++;
          t /= it;
        } while (t > 1 and t % it == 0);
        if (t1 & 1) {
          num *= it;
        }
      }
    }
    num *= t;
    cnt[num]++;
  }
  ll ret = 0;
  set<int> P;
  rep(i, n) {
    int t = a[i];
    for (auto it : primes) {
      if (it * it > t)
        break;
      int t1 = 0;
      if (t % it == 0) {
        do {
          t1++;
          t /= it;
        } while (t > 1 and t % it == 0);
        if (t1 & 1) {
          if (P.find(it) != P.end())
            P.erase(it);
          else {
            P.insert(it);
          }
        }
      }
    }
    if (t > 1) {
      if (P.find(t) != P.end())
        P.erase(t);
      else {
        P.insert(t);
      }
    }
    ll B = 1;
    for (auto it : P) {
      if (B * it <= N) {
        B *= it;
      } else {
        B = -1;
        break;
      }
    }
    if (B == -1)
      continue;
    else {
      ret += cnt[B];
    }
  }
  cout << ret;
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.tie(nullptr);

  vb temp(1000, 1);
  temp[0] = temp[1] = 0;
  int i = 2;
  while (i < 1000) {
    for (int j = i * i; j < 1000; j += i)
      temp[j] = 0;
    i++;
    while (!temp[i])
      i++;
  }
  rep(i, 1000) {
    if (temp[i])
      primes.pb(i);
  }
  int t = 1;
  if (cin >> t) {
    while (t--) {
      solve();
      cout << '\n';
    }
  }
  return 0;
}
