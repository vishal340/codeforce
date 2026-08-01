#include <bits/stdc++.h>
using namespace std;

using ll = long long;
using vi = vector<int>;

#define sz(x) static_cast<int>((x).size())
#define all(x) (x).begin(), (x).end()

void solve() {
  int n, m;
  cin >> n >> m;
  const int RN = 2 * n - 3;
  const int N = 2 * n - 2;
  vector<vector<int>> g(N);

  auto Lid = [&](int v) -> int {
    if (v == n - 1)
      return -1;
    return v == 0 ? 0 : 2 * v - 1;
  };
  auto Rid = [&](int v) -> int {
    if (v == 0)
      return -1;
    return v == n - 1 ? RN : 2 * v;
  };
  auto add = [&](int a, int b) {
    if (a < 0 || b < 0)
      return;
    g[a].push_back(b);
    g[b].push_back(a);
  };
  auto id_to_v = [&](int id) -> int {
    if (id == 0)
      return 0;
    if (id == RN)
      return n - 1;
    if (id & 1)
      return (id + 1) / 2;
    return id / 2;
  };

  for (int i = 0; i < m; i++) {
    int x, y;
    cin >> x >> y;
    --x;
    --y;
    add(Lid(x), Lid(y));
    add(Rid(x), Rid(y));
  }

  vector<bool> vis(N, false);
  vi dis(N, -1), prv(N, -1);
  queue<int> qL, qR;
  vis[0] = true;
  dis[0] = 0;
  for (int to : g[0]) {
    if (vis[to])
      continue;
    vis[to] = true;
    dis[to] = 1;
    prv[to] = 0;
    qL.push(to);
  }

  int distL = 1;
  while (!qL.empty() || !qR.empty()) {
    int distR = distL + 1;
    while (!qL.empty()) {
      int t = qL.front();
      qL.pop();
      if (t != 0 && ((t & 1) == 0 || t + 1 >= N))
        continue;
      int r = t + 1;
      for (int to : g[r]) {
        if (vis[to])
          continue;
        vis[to] = true;
        dis[to] = distR;
        prv[to] = t;
        if (to != RN)
          qR.push(to);
      }
    }
    if (dis[RN] != -1)
      break;
    distL = distR + 1;
    while (!qR.empty()) {
      int t = qR.front();
      qR.pop();
      int l = t - 1;
      if (l < 0)
        continue;
      for (int to : g[l]) {
        if (vis[to])
          continue;
        vis[to] = true;
        dis[to] = distL;
        prv[to] = t;
        qL.push(to);
      }
    }
  }

  int k = dis[RN];
  if (k == -1) {
    cout << -1 << '\n';
    return;
  }

  vi path;
  for (int cur = RN;; cur = prv[cur]) {
    if (cur < 0 || cur >= N) {
      cout << -1 << '\n';
      return;
    }
    path.push_back(id_to_v(cur));
    if (cur == 0)
      break;
  }
  reverse(all(path));
  cout << k << '\n';
  for (int i = 0; i < sz(path); i++)
    cout << path[i] + 1 << " \n"[i + 1 == sz(path)];
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  if (cin >> t) {
    while (t--)
      solve();
  }
  return 0;
}
