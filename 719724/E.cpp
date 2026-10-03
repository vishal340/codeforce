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

void solve() {
  int n;
  cin >> n;
  string s;
  cin >> s;
  ll ret = 1;
  {
    ai(26) num{};
    rep(i, n) {
      if (i != 0) {
        if (s[i - 1] != s[i]) {
          ret += i - num[s[i] - 'a'];
        }
        int j = s[i] - 'a';
        num[j] = num[j] + 1;
      } else {
        num[s[0] - 'a']++;
      }
    }
  }
  {
    ai(26) num{};
    per(i, n) {
      if (i != n - 1) {
        if (s[i + 1] != s[i]) {
          ret += n - 1 - i - num[s[i] - 'a'];
        }
        int j = s[i] - 'a';
        num[j] = num[j] + 1;
      } else {
        num[s[n - 1] - 'a']++;
      }
    }
  }
  array<ai(26), 26> num1{}, num2{}, num3{};
  fora(i, 1, n, 1) {
    if (s[i] != s[i - 1]) {
      if (i > 1) {
        num3 = num2;
        num2 = array<ai(26), 26>{};
        num2[s[i] - 'a'][s[i - 1] - 'a'] = num1[s[i] - 'a'][s[i - 1] - 'a'] + 1;
        num1 = num3;
        ret -= num2[s[i] - 'a'][s[i - 1] - 'a'];
      } else {
        num2[s[i] - 'a'][s[i - 1] - 'a']++;
        ret--;
      }
    } else {
      num1 = array<ai(26), 26>{};
      num2 = array<ai(26), 26>{};
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
