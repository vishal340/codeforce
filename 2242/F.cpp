#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

using mli = list<pair<int, list<int>>>;

void mer_uni(mli &v, mli::iterator s, mli::iterator m, mli::iterator e) {
  mli l1, l2;
  l1.splice(l1.begin(), v, s, m);
  l2.splice(l2.begin(), v, m, e);
  l1.merge(l2, [](const auto &a, const auto &b) { return a.first < b.first; });
  l1.unique([](auto &a, auto &b) {
    if (a.first == b.first) {
      a.second.splice(a.second.end(), b.second);
      return true;
    }
    return false;
  });
  v.splice(e, l1);
}

void solve() {
  int i, n;
  cin >> n;
  vi a(n);
  for (i = 0; i < n; i++) {
    cin >> a[i];
  }
  mli v;
  v.emplace_back(a[0], list<int>{0});
  for (i = 1; i < n; i++) {
    if (v.begin()->first == 0) {
      v.begin()->second.push_back(i);
    } else {
      v.emplace(v.begin(), 0, list<int>{i});
    }
    auto it = v.begin();
    for (; it != v.end(); it++) {
      if (it->first >= a[i]) {
        break;
      }
    }
    auto it1 = it, it2 = v.begin();
    for (; it1 != v.end(); it1++) {
      if (it1->first >= 2 * a[i]) {
        break;
      }
    }
    if (it == v.end()) {
      for (auto it3 = v.begin(); it3 != v.end(); it3++) {
        it3->first += a[i];
      }
      continue;
    }
    for (auto it3 = v.begin(); it3 != it; it3++) {
      it3->first += a[i];
    }
    for (auto it3 = it; it3 != it1; it3++)
      it3->first -= a[i];

    auto range_start = v.begin();
    auto range_end = it;
    v.splice(it1, v, range_start, range_end);
    auto it3 = it1;
    while (it3 != v.end() && it3->first < 3 * a[i]) {
      it3->first -= a[i];
      it3++;
    }
    auto it4 = it3;
    mer_uni(v, range_start, it1, it3);
    while (it4 != v.end()) {
      it4->first -= a[i];
      it4++;
    }
  }
  for (auto &iter : v) {
    for (auto j : iter.second) {
      a[n - j - 1] = iter.first;
    }
  }
  for (i = 0; i < n - 1; i++)
    cout << a[i] << ' ';
  cout << a[n - 1] << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int t = 1;
  // cin >> t;
  while (t--)
    solve();

  return 0;
}
