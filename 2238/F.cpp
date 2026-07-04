#include <bits/stdc++.h>
using namespace std;

using vi = vector<int>;
const long long MOD = 1000000007LL;

int max_fac = 4e05;
vector<long long> fact(max_fac + 1, 1);

long long power(long long base, long long exp) {
  long long ret = 1;
  while (exp > 0) {
    if (exp & 1) {
      ret = (ret * base) % MOD;
    }
    base = (base * base) % MOD;
    exp >>= 1;
  }
  return ret;
}

long long inverse(long long n) {
  if (n == 1)
    return 1;
  return power(n, MOD - 2);
}

void solve() {
  int i, t, n, k;
  cin >> n >> k;
  k++;
  t = min(n, 19);
  int m = 0;
  for (i = 0; i < t - 1; i++) {
    if (k & (1 << i))
      m += 1;
  }
  int last = k >> (t - 1);
  m += last;
  long long c_m = (((fact[2 * m - 2] * inverse(m)) % MOD) *
                   inverse((fact[m - 1] * fact[m - 1]) % MOD)) %
                  MOD;
  long long ret = (c_m * ((fact[m] * inverse(fact[last])) % MOD)) % MOD;
  cout << ret << '\n';
}

void prec() {
  for (int i = 2; i <= max_fac; i++) {
    fact[i] = (fact[i - 1] * i) % MOD;
  }
}

int main() {
  prec();
  ios::sync_with_stdio(false);
  cin.tie(0);
  int t;
  cin >> t;
  while (t--)
    solve();

  return 0;
}
