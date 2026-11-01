#include <bits/stdc++.h>
#include <climits>
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

template <typename A, typename B>
istream &operator>>(istream &in, pair<A, B> &p) {
  return in >> p.first >> p.second;
}
template <typename... Ts> istream &operator>>(istream &in, tuple<Ts...> &t) {
  apply([&](auto &...xs) { (in >> ... >> xs); }, t);
  return in;
}
template <typename T> istream &operator>>(istream &in, vector<T> &vec) {
  for (int i = 0; i < sz(vec); ++i)
    in >> vec[i];
  return in;
}

// cout-only: space-separated (do not use for debug / cerr).
template <typename A, typename B>
ostream &operator<<(ostream &out, const pair<A, B> &p) {
  return out << p.first << ' ' << p.second;
}
template <typename... Ts>
ostream &operator<<(ostream &out, const tuple<Ts...> &t) {
  apply(
      [&](const auto &x, const auto &...xs) {
        out << x;
        ((out << ' ' << xs), ...);
      },
      t);
  return out;
}
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
} && !requires {
  typename tuple_size<remove_cvref_t<T>>::type;
} && requires(const T &x) { cerr << x; };
template <DebugAtom T> void __print(const T &x) { cerr << x; }
template <typename T, typename V> void __print(const pair<T, V> &x) {
  cerr << '{';
  __print(x.first);
  cerr << ", ";
  __print(x.second);
  cerr << '}';
}
template <typename... Ts> void __print(const tuple<Ts...> &t) {
  cerr << '{';
  apply(
      [&](const auto &...xs) {
        int f = 0;
        ((cerr << (f++ ? ", " : ""), __print(xs)), ...);
      },
      t);
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
constexpr int N = 200000;

void solve() {
  int n;
  cin >> n;
  vi a(n);
  cin >> a;

  map<int, int> ma;
  rep(i, n) ma[a[i]]++;

  int k = sz(ma);
  vi x, cnt;

  for (auto [v, c] : ma) {
    x.push_back(v);
    cnt.push_back(c);
  }

  vector<pll> pref(k), suff(k);

  // Prefix: merge from left to right
  ll mass = cnt[0];

  rep(i, k - 1) {
    ll d = x[i + 1] - x[i];

    pref[i + 1] = {pref[i].first + mass * d, pref[i].second + d};

    mass += cnt[i + 1];
  }

  // Suffix: merge from right to left
  mass = cnt[k - 1];

  rep(i, k - 1) {
    int j = k - 2 - i;
    ll d = x[j + 1] - x[j];

    suff[i + 1] = {suff[i].first + mass * d, suff[i].second + d};

    mass += cnt[j];
  }

  ll cost = LLONG_MAX, op = LLONG_MAX;

  rep(i, k) {
    ll c = pref[i].first + suff[k - 1 - i].first;
    ll o = pref[i].second + suff[k - 1 - i].second;

    if (make_pair(c, o) < make_pair(cost, op)) {
      cost = c;
      op = o;
    }
  }

  cout << cost << ' ' << op;
}
// void solve() {
//   int n;
//   cin >> n;
//   vi a(n);
//   cin >> a;
//   ll cost = INT_MAX, op = 0;
//   map<int, int> ma;
//   rep(i, n) { ma[a[i]]++; }
//   auto it1 = ma.begin(), it2 = prev(ma.end());
//   vector<pll> pref(sz(ma)), suff(sz(ma));
//   rep(i, sz(ma) - 1) {
//     ll t2 = (it2->first - prev(it2)->first) * it2->second;
//     suff[i + 1] = {suff[i].first + t2, suff[i].second + it2->second};
//     prev(it2)->second += it2->second;
//     it2--;
//   }
//   rep(i, sz(ma) - 1) {
//     ll t1 = (next(it1)->first - it1->first) * it1->second;
//     pref[i + 1] = {pref[i].first + t1, pref[i].second + it1->second};
//     next(it1)->second += it1->second;
//     it1++;
//   }
//   debug(pref);
//   debug(suff);
//   rep(i, n) {
//     ll t = pref[i].first + suff[n - 1 - i].first;
//     if (cost > t) {
//       cost = t;
//       op = pref[i].second + suff[n - 1 - i].second;
//     }
//   }
//   // while (it1 != it2) {
//   //   int t1 = (next(it1)->first - it1->first) * it1->second;
//   //   int t2 = (it2->first - prev(it2)->first) * it2->second;
//   //   if (t2 >= t1) {
//   //     int t = next(it1)->first - it1->first;
//   //     op += t;
//   //     cost += 1LL * it1->second * t;
//   //     next(it1)->second += it1->second;
//   //     it1++;
//   //   } else {
//   //     int t = it2->first - prev(it2)->first;
//   //     op += t;
//   //     cost += 1LL * it2->second * t;
//   //     prev(it2)->second += it2->second;
//   //     it2--;
//   //   }
//   // }
//   cout << cost << ' ' << op;
// }

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
