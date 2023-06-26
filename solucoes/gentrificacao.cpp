// Solucao para o problema Gentrificacao usando o Teorema de Dilworth
// Link:
// https://www.facebook.com/codingcompetitions/hacker-cup/2015/round-3/problems/C

#include <bits/stdc++.h>
#define int long long

using namespace std;

mt19937 rng((int)chrono::steady_clock::now().time_since_epoch().count());

struct kuhn {
  int n, m;
  vector<vector<int>> g;
  vector<int> vis, ma, mb;

  kuhn(int n_, int m_) : n(n_), m(m_), g(n), vis(n + m), ma(n, -1), mb(m, -1) {}

  void add(int a, int b) { g[a].push_back(b); }

  bool dfs(int i) {
    vis[i] = 1;
    for (int j : g[i])
      if (!vis[n + j]) {
        vis[n + j] = 1;
        if (mb[j] == -1 or dfs(mb[j])) {
          ma[i] = j, mb[j] = i;
          return true;
        }
      }
    return false;
  }
  int matching() {
    int ret = 0, aum = 1;
    for (auto& i : g) shuffle(i.begin(), i.end(), rng);
    while (aum) {
      for (int j = 0; j < m; j++) vis[n + j] = 0;
      aum = 0;
      for (int i = 0; i < n; i++)
        if (ma[i] == -1 and dfs(i)) ret++, aum = 1;
    }
    return ret;
  }
};

#define MAX 512
int fecho[MAX][MAX];

int32_t main() {
  cin.tie(0);
  ios_base::sync_with_stdio(0);
  int t;
  cin >> t;
  for (int nc = 1; nc <= t; nc++) {
    for (auto& x : fecho)
      for (auto& y : x) y = 0;

    int n, m;
    cin >> n >> m;
    for (int i = 0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      fecho[u][v] = 1;
    }

    for (int k = 0; k < n; k++) {
      for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
          if (fecho[i][k] && fecho[k][j]) {
            fecho[i][j] = 1;
          }
        }
      }
    }
    kuhn k(n, n);
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (fecho[i][j] && !fecho[j][i]) {
          k.add(i, j);
        }
      }
    }
    cout << "Case #" << nc << ": " << n - k.matching() << "\n";
  }
}
