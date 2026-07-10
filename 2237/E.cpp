#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;
void solve() {
  int i, n;
  cin >> n;
  vi a(n), b(n), perm(n, 1);
  vector<bool> done(n, false);
  for (i = 0; i < n; i++) {
    cin >> a[i];
    a[i]--;
  }
  for (i = 0; i < n; i++) {
    cin >> b[i];
    if (b[i] != -1) {
      b[i]--;
    }
  }
  for (i = 0; i < n; i++) {
    if (a[i] == i) {
      if (b[i] == -1) {
        b[i] = a[i];
      } else if (b[i] != a[i]) {
        cout << "NO\n";
        return;
      }
    }
  }
  {
    stack<int> st;
    vector<bool> done1(n, false);
    for (i = 0; i < n; i++) {
      if (done1[a[i]])
        continue;
      done1[a[i]] = true;
      int t = a[a[i]];
      int count = 1;
      while (t != a[i]) {
        count++;
        done1[t] = true;
        st.push(t);
        t = a[t];
      }
      perm[a[i]] = count;
      while (!st.empty()) {
        perm[st.top()] = count;
        st.pop();
      }
    }
  }
  for (i = 0; i < n; i++) {
    if (b[i] != -1 && done[b[i]])
      continue;
    if (b[i] != -1 && perm[b[i]] == perm[a[i]]) {
      done[b[i]] = true;
      int t1 = a[i];
      int t2 = a[b[i]];
      while (t1 != i) {
        if (b[t1] == -1) {
          b[t1] = t2;
        } else if (b[t1] != t2) {
          cout << "NO\n";
          return;
        }
        t2 = a[t2];
        t1 = a[t1];
        done[b[t1]] = true;
      }
    } else if (b[i] != -1) {
      cout << "NO\n";
      return;
    }
  }
  vector<queue<int>> root(n, queue<int>());
  vector<bool> done1(n, false);
  for (i = 0; i < n; i++) {
    if (done1[i])
      continue;
    else {
      if (done[i]) {
        done1[i] = done[i];
        continue;
      } else {
        root[perm[i]].push(i);
        done1[i] = true;
        int t = a[i];
        while (t != i) {
          done1[t] = true;
          t = a[t];
        }
      }
    }
  }
  for (i = 0; i < n; i++) {
    if (b[i] != -1 && !done[b[i]]) {
      if (root[perm[b[i]]].empty()) {
        cout << "NO\n";
        return;
      }
      done[b[i]] = true;
      b[i] = root[perm[b[i]]].front();
      root[perm[b[i]]].pop();
      int t1 = a[a[i]];
      int t2 = b[i];
      while (t1 != a[i]) {
        b[t2] = a[t2];
        done[b[t2]] = true;
        t1 = a[t1];
        t2 = a[t2];
      }
    }
  }
  cout << "YES\n";
  for (i = 0; i < n - 1; i++) {
    cout << b[i] + 1 << ' ';
  }
  cout << b[n - 1] + 1 << '\n';
}

int main() {
  ios::sync_with_stdio(false);
  cin.tie(0);

  int t;
  cin >> t;
  while (t--)
    solve();

  return 0;
}
