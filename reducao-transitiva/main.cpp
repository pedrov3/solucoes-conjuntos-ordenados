#include <bits/stdc++.h>

#include "util.hpp"
#define all(x) x.begin(), x.end()
using namespace std;

// TODO: [ ] Opcao de mostrar os vértices nas classes de equivalência /
// componente fortemente conexa

// TODO: [ ] Simplificar o código
typedef long double ld;
typedef long long ll;
typedef pair<int, int> ii;
typedef vector<int> vi;
const ll oo = 1e9 + 7;

#define MAX 112

vector<vi> adj, adj_rev;
set<int> adj_scc[MAX];

vi vis(MAX), raizes(MAX);
vi order, nodos_raiz;
vi components(MAX);
int sz_comp = 0;

vi sz(MAX);

void dfs1(int u, vector<vi> &adj) {
  if (vis[u]) return;
  vis[u] = 1;
  for (auto &v : adj[u]) {
    dfs1(v, adj);
  }
  order.push_back(u);
}

void dfs2(int u, vector<vi> &adj_rev) {
  if (vis[u]) return;
  vis[u] = 1;
  components[sz_comp++] = u;
  for (auto &v : adj_rev[u]) {
    dfs2(v, adj_rev);
  }
}

void scc(vector<vi> &adj) {
  int n = adj.size();
  adj_rev.resize(n);
  for (auto &u : adj_rev) u.clear();
  for (int u = 0; u < n; u++) {
    for (auto &v : adj[u]) {
      adj_rev[v].push_back(u);
    }
  }
  for (auto &x : vis) x = 0;
  order.clear();
  for (int u = 0; u < n; u++) {
    dfs1(u, adj);
  }

  for (auto &x : vis) x = 0;
  reverse(order.begin(), order.end());
  nodos_raiz.clear();
  for (auto &u : order) {
    if (!vis[u]) {
      sz_comp = 0;
      dfs2(u, adj_rev);
      int raiz = components[0];
      sz[raiz] = sz_comp;
      for (int i = 0; i < sz_comp; i++) {
        int v = components[i];
        raizes[v] = raiz;
      }
      nodos_raiz.push_back(raiz);
    }
  }
  for (auto &u : adj_scc) {  // TODO
    u.clear();
  }
  for (int u = 0; u < n; u++) {
    for (auto &v : adj[u]) {
      int _u = raizes[u], _v = raizes[v];
      if (_u != _v) {
        adj_scc[_u].insert(_v);
      }
    }
  }
}

vector<vi> reducaoTransitiva(vector<vi> &adj) {
  int n = adj.size();

  vector<vi> g(MAX, vi(MAX)), g1(MAX, vi(MAX)), g2(MAX, vi(MAX)),
      g3(MAX, vi(MAX));

  vector<vi> m1(MAX, vi(MAX)), m2(MAX, vi(MAX)), m3(MAX, vi(MAX));

  for (int u = 0; u < n; u++) {
    for (auto &v : adj[u]) {
      g[u][v] = 1;
    }
  }

  // encontrar g1 SCC
  scc(adj);

  // adj_scc
  map<int, int> id;
  for (auto &u : nodos_raiz) {
    for (auto &v : adj_scc[u]) {
      g1[u][v] = 1;
    }
    if (sz[u] > 1 || (sz[u] == 1 && g2[u][u])) {
      g1[u][u] = 1;
    }
  }
  // encontrar g2 só copiar m[i][i] = 0
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      g2[i][j] = g1[i][j];
    }
    g2[i][i] = 0;
  }

  //TODO: Fecho transitivo - usando floyd-warshall
  vector<vector<int>> g2T(MAX, vector<int>(MAX, 0));
  vector<vector<int>> dist(MAX, vector<int>(MAX, 0));
  for (auto &x : dist)
    for (auto &y : x) y = oo;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (g2[i][j]) dist[i][j] = 1;
    }
  }

  for (int k = 0; k < n; k++)
    for (int i = 0; i < n; i++)
      for (int j = 0; j < n; j++)
        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (dist[i][j] != oo) {
        g2T[i][j] = 1;
      }
    }
  }

  // Multiplicação
  m3 = strassen(g1, g2T);

  /*
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      m3[i][j] = 0;
      for (int k = 0; k < n; k++) {
        m3[i][j] |= g1[i][k] && g2T[k][j];
      }
    }
  }*/

  vector<vi> ans(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int x = g1[i][j] - m3[i][j];
      if (x == 1) {
        ans[i].emplace_back(j);
      }
    }
  }
  return ans;
}

int main() {
  int n, m;
  cin >> n >> m;
  adj.resize(n);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    adj[u].emplace_back(v);
  }

  vector<vi> gt = reducaoTransitiva(adj);
  for (int i = 0; i < gt.size(); i++) {
    cout << i + 1 << ": ";

    if (gt[i].empty()) {
      cout << "(vazia)\n";
      continue;
    }
    for (auto &v : gt[i]) {
      cout << v + 1 << " ";
    }
    cout << "\n";
  }
}
