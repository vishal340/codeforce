#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;

constexpr int N = 1000;
constexpr int V = 2 * N;
constexpr int B = 2 * V;
constexpr int INF = 0x3f3f3f3f;
constexpr int BIG = 10000;

struct Edge {
  int u, v, w;
};

int n, m;
int idL[N + 10], idR[N + 10], ori[V + 10];
int init_u[N + 10], init_v[N + 10], init_cnt;
char sx[V + 10][V + 10];
vi adj[V + 10];

struct BlossomTree {
  int n, nx;
  Edge g[B + 10][B + 10];
  int lab[B + 10], match[B + 10], slack[B + 10], st[B + 10], pa[B + 10];
  int flower_from[B + 10][V + 10], flower[B + 10][V + 10];
  int S[B + 10], vis[B + 10], flower_cnt[B + 10], tmp[V + 10];
  queue<int> q;

  void init(int _n) {
    n = _n;
    nx = n;
    for (int i = 0; i <= 2 * n + 2; ++i) {
      lab[i] = match[i] = slack[i] = st[i] = pa[i] = S[i] = vis[i] = 0;
      flower_cnt[i] = 0;
      for (int j = 0; j <= n; ++j)
        flower_from[i][j] = 0;
    }
    for (int i = 1; i <= 2 * n + 2; ++i) {
      for (int j = 1; j <= 2 * n + 2; ++j)
        g[i][j] = {i, j, 0};
    }
  }

  void add_edge(int u, int v, int w) {
    if (u == v)
      return;
    if (w > g[u][v].w) {
      g[u][v] = {u, v, w};
      g[v][u] = {v, u, w};
    }
  }

  int e_delta(Edge e) { return lab[e.u] + lab[e.v] - g[e.u][e.v].w * 2; }

  void update_slack(int u, int x) {
    if (!slack[x] || e_delta(g[u][x]) < e_delta(g[slack[x]][x]))
      slack[x] = u;
  }

  void set_slack(int x) {
    slack[x] = 0;
    for (int u = 1; u <= n; ++u) {
      if (g[u][x].w && st[u] != x && S[st[u]] == 0)
        update_slack(u, x);
    }
  }

  void q_push(int x) {
    if (x <= n)
      q.push(x);
    else
      for (int i = 0; i < flower_cnt[x]; ++i)
        q_push(flower[x][i]);
  }

  void set_st(int x, int b) {
    st[x] = b;
    if (x > n) {
      for (int i = 0; i < flower_cnt[x]; ++i)
        set_st(flower[x][i], b);
    }
  }

  void reverse_flower(int b, int l, int r) {
    while (l < r) {
      swap(flower[b][l], flower[b][r]);
      ++l;
      --r;
    }
  }

  void rotate_flower(int b, int pr) {
    int cnt = flower_cnt[b];
    for (int i = 0; i < cnt; ++i)
      tmp[i] = flower[b][(i + pr) % cnt];
    for (int i = 0; i < cnt; ++i)
      flower[b][i] = tmp[i];
  }

  int get_pr(int b, int xr) {
    int pr = 0;
    while (flower[b][pr] != xr)
      ++pr;
    if (pr & 1) {
      reverse_flower(b, 1, flower_cnt[b] - 1);
      return flower_cnt[b] - pr;
    }
    return pr;
  }

  void set_match(int u, int v) {
    match[u] = g[u][v].v;
    if (u > n) {
      Edge e = g[u][v];
      int xr = flower_from[u][e.u];
      int pr = get_pr(u, xr);
      for (int i = 0; i < pr; ++i)
        set_match(flower[u][i], flower[u][i ^ 1]);
      set_match(xr, v);
      rotate_flower(u, pr);
    }
  }

  void augment(int u, int v) {
    while (true) {
      int xnv = st[match[u]];
      set_match(u, v);
      if (!xnv)
        return;
      set_match(xnv, st[pa[xnv]]);
      u = st[pa[xnv]];
      v = xnv;
    }
  }

  int get_lca(int u, int v) {
    static int tim = 0;
    ++tim;
    while (u || v) {
      if (u) {
        if (vis[u] == tim)
          return u;
        vis[u] = tim;
        u = st[match[u]];
        if (u)
          u = st[pa[u]];
      }
      swap(u, v);
    }
    return 0;
  }

  void add_blossom(int u, int lca, int v) {
    int b = n + 1;
    while (b <= nx && st[b])
      ++b;
    if (b > nx)
      ++nx;
    lab[b] = 0;
    S[b] = 0;
    match[b] = match[lca];
    flower_cnt[b] = 0;
    flower[b][flower_cnt[b]++] = lca;
    for (int x = u, y; x != lca; x = st[pa[y]]) {
      flower[b][flower_cnt[b]++] = x;
      y = st[match[x]];
      flower[b][flower_cnt[b]++] = y;
      q_push(y);
    }
    reverse_flower(b, 1, flower_cnt[b] - 1);
    for (int x = v, y; x != lca; x = st[pa[y]]) {
      flower[b][flower_cnt[b]++] = x;
      y = st[match[x]];
      flower[b][flower_cnt[b]++] = y;
      q_push(y);
    }
    set_st(b, b);
    for (int x = 1; x <= nx; ++x) {
      g[b][x] = {b, x, 0};
      g[x][b] = {x, b, 0};
    }
    for (int x = 1; x <= n; ++x)
      flower_from[b][x] = 0;
    for (int i = 0; i < flower_cnt[b]; ++i) {
      int xs = flower[b][i];
      for (int x = 1; x <= nx; ++x) {
        if (g[xs][x].w &&
            (!g[b][x].w || e_delta(g[xs][x]) < e_delta(g[b][x]))) {
          g[b][x] = g[xs][x];
          g[x][b] = g[x][xs];
        }
      }
      for (int x = 1; x <= n; ++x) {
        if (flower_from[xs][x])
          flower_from[b][x] = xs;
      }
    }
    set_slack(b);
  }

  void expand_blossom(int b) {
    for (int i = 0; i < flower_cnt[b]; ++i)
      set_st(flower[b][i], flower[b][i]);
    int xr = flower_from[b][g[b][pa[b]].u];
    int pr = get_pr(b, xr);
    for (int i = 0; i < pr; i += 2) {
      int xs = flower[b][i];
      int xns = flower[b][i + 1];
      pa[xs] = g[xns][xs].u;
      S[xs] = 1;
      S[xns] = 0;
      slack[xs] = 0;
      set_slack(xns);
      q_push(xns);
    }
    S[xr] = 1;
    pa[xr] = pa[b];
    for (int i = pr + 1; i < flower_cnt[b]; ++i) {
      int xs = flower[b][i];
      S[xs] = -1;
      set_slack(xs);
    }
    st[b] = 0;
  }

  bool on_found_edge(Edge e) {
    int u = st[e.u], v = st[e.v];
    if (S[v] == -1) {
      pa[v] = e.u;
      S[v] = 1;
      int nu = st[match[v]];
      slack[v] = slack[nu] = 0;
      S[nu] = 0;
      q_push(nu);
    } else if (S[v] == 0) {
      int lca = get_lca(u, v);
      if (!lca) {
        augment(u, v);
        augment(v, u);
        return true;
      }
      add_blossom(u, lca, v);
    }
    return false;
  }

  bool matching() {
    for (int i = 1; i <= nx; ++i)
      S[i] = -1, slack[i] = 0;
    while (!q.empty())
      q.pop();
    for (int x = 1; x <= nx; ++x) {
      if (st[x] == x && !match[x]) {
        pa[x] = 0;
        S[x] = 0;
        q_push(x);
      }
    }
    if (q.empty())
      return false;
    while (true) {
      while (!q.empty()) {
        int u = q.front();
        q.pop();
        if (S[st[u]] == 1)
          continue;
        for (int v = 1; v <= n; ++v) {
          if (g[u][v].w && st[u] != st[v]) {
            if (!e_delta(g[u][v])) {
              if (on_found_edge(g[u][v]))
                return true;
            } else
              update_slack(u, st[v]);
          }
        }
      }
      int d = INF;
      for (int u = 1; u <= n; ++u) {
        if (S[st[u]] == 0)
          d = min(d, lab[u]);
      }
      for (int b = n + 1; b <= nx; ++b) {
        if (st[b] == b && S[b] == 1)
          d = min(d, lab[b] / 2);
      }
      for (int x = 1; x <= nx; ++x) {
        if (st[x] == x && slack[x]) {
          if (S[x] == -1)
            d = min(d, e_delta(g[slack[x]][x]));
          else if (S[x] == 0)
            d = min(d, e_delta(g[slack[x]][x]) / 2);
        }
      }
      if (d == INF)
        return false;
      for (int u = 1; u <= n; ++u) {
        if (S[st[u]] == 0) {
          if (lab[u] == d)
            return false;
          lab[u] -= d;
        } else if (S[st[u]] == 1)
          lab[u] += d;
      }
      for (int b = n + 1; b <= nx; ++b) {
        if (st[b] == b) {
          if (S[b] == 0)
            lab[b] += d * 2;
          else if (S[b] == 1)
            lab[b] -= d * 2;
        }
      }
      while (!q.empty())
        q.pop();
      for (int x = 1; x <= nx; ++x) {
        if (st[x] == x && slack[x] && st[slack[x]] != x &&
            !e_delta(g[slack[x]][x])) {
          if (on_found_edge(g[slack[x]][x]))
            return true;
        }
      }
      for (int b = n + 1; b <= nx; ++b) {
        if (st[b] == b && S[b] == 1 && !lab[b])
          expand_blossom(b);
      }
    }
  }

  int solve() {
    nx = n;
    int cnt = 0, mx = 0;
    for (int i = 0; i <= 2 * n + 2; ++i) {
      lab[i] = match[i] = slack[i] = pa[i] = S[i] = vis[i] = 0;
      st[i] = 0;
      flower_cnt[i] = 0;
    }
    for (int i = 0; i <= n; ++i)
      st[i] = i;
    for (int u = 1; u <= n; ++u) {
      for (int v = 1; v <= n; ++v) {
        flower_from[u][v] = (u == v ? u : 0);
        mx = max(mx, g[u][v].w);
      }
    }
    for (int u = 1; u <= n; ++u)
      lab[u] = mx;
    while (matching())
      ++cnt;
    return cnt;
  }
} blossom;

int new_node(int x, int &tot) {
  ori[tot + 1] = x;
  return ++tot;
}

void toggle_edge(int u, int v) {
  if (u > v)
    swap(u, v);
  sx[u][v] ^= 1;
}

void solve() {
  cin >> n >> m;
  memset(idL, -1, sizeof(idL));
  memset(idR, -1, sizeof(idR));

  int tot = 0;
  for (int i = 1; i <= n; ++i) {
    if (i != n)
      idL[i] = new_node(i, tot);
  }
  for (int i = 1; i <= n; ++i) {
    if (i != 1)
      idR[i] = new_node(i, tot);
  }

  blossom.init(tot);

  for (int i = 0; i < m; ++i) {
    int u, v;
    cin >> u >> v;
    if (idL[u] != -1 && idL[v] != -1)
      blossom.add_edge(idL[u], idL[v], BIG - 1);
    if (idR[u] != -1 && idR[v] != -1)
      blossom.add_edge(idR[u], idR[v], BIG - 1);
  }

  init_cnt = 0;
  for (int i = 2; i < n; ++i) {
    blossom.add_edge(idL[i], idR[i], BIG);
    init_u[++init_cnt] = idL[i];
    init_v[init_cnt] = idR[i];
  }

  int cnt = blossom.solve();
  if (cnt != tot / 2) {
    cout << -1 << '\n';
    return;
  }

  for (int i = 1; i <= tot; ++i) {
    adj[i].clear();
    for (int j = 1; j <= tot; ++j)
      sx[i][j] = 0;
  }

  for (int i = 1; i <= tot; ++i) {
    if (blossom.match[i] && i < blossom.match[i])
      toggle_edge(i, blossom.match[i]);
  }
  for (int i = 1; i <= init_cnt; ++i)
    toggle_edge(init_u[i], init_v[i]);

  for (int i = 1; i <= tot; ++i) {
    for (int j = i + 1; j <= tot; ++j) {
      if (sx[i][j]) {
        adj[i].push_back(j);
        adj[j].push_back(i);
      }
    }
  }

  int st = idL[1], ed = idR[n];
  vi aux, path;
  int last = 0, cur = st;
  while (true) {
    aux.push_back(cur);
    if (cur == ed)
      break;
    int nxt = 0;
    for (int v : adj[cur]) {
      if (v != last) {
        nxt = v;
        break;
      }
    }
    if (!nxt) {
      cout << -1 << '\n';
      return;
    }
    last = cur;
    cur = nxt;
    if ((int)aux.size() > tot + 5) {
      cout << -1 << '\n';
      return;
    }
  }

  path.push_back(1);
  for (int i = 1; i < (int)aux.size(); ++i) {
    int u = aux[i - 1], v = aux[i];
    if (ori[u] != ori[v])
      path.push_back(ori[v]);
  }

  if (path.back() != n || ((int)path.size() - 1) % 2) {
    cout << -1 << '\n';
    return;
  }

  cout << (int)path.size() - 1 << '\n';
  for (int i = 0; i < (int)path.size(); ++i) {
    if (i)
      cout << ' ';
    cout << path[i];
  }
  cout << '\n';
}

int main() {
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);

  int t = 1;
  cin >> t;
  while (t--)
    solve();
  return 0;
}
