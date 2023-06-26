#include <bits/stdc++.h>
#define int long long
#define all(x) x.begin(), x.end()
using namespace std;

typedef long double ld;
typedef long long ll;
typedef pair<int, int> ii;
typedef vector<int> vi;
const ll oo = 1987654321987654321;

#define MAX 112
vector<vi> adj, adj_rev;
set<int> adj_scc[MAX];
int vis[MAX], raizes[MAX];
vi order, nodos_raiz;
int components[MAX], sz_comp = 0;

int sz[MAX];

int g[MAX][MAX], g1[MAX][MAX], g2[MAX][MAX], g3[MAX][MAX];
int m1[MAX][MAX], m2[MAX][MAX], m3[MAX][MAX];

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
  memset(vis, 0, sizeof(vis));
  order.clear();
  for (int u = 0; u < n; u++) {
    dfs1(u, adj);
  }

  memset(vis, 0, sizeof(vis));
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

// construir matriz de incidência
void incidencia(int mat[][MAX], int retval[][MAX], int n) {
  int lin = 0;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (mat[i][j]) {
        retval[lin][i] = 1;
        retval[lin][j] = 1;
        lin++;
        // guardar as labels
      }
    }
  }
}

int32_t main() {
  cin.tie(0);
  ios_base::sync_with_stdio(0);

  int n, m;
  cin >> n >> m;
  vector<vi> adj(n);
  for (int i = 0; i < m; i++) {
    int u, v;
    cin >> u >> v;
    --u, --v;
    adj[u].emplace_back(v);
    g[u][v] = 1;
  }
  // encontrar g1 SCC
  scc(adj);

  // adj_scc
  map<int, int> id;
  for (auto &u : nodos_raiz) {
    if (!id.count(u)) id[u] = id.size();
    for (auto &v : adj_scc[u]) {
      if (!id.count(v)) id[v] = id.size();
      g1[id[u]][id[v]] = 1;
    }
    if (sz[u] > 1 || (sz[u] == 1 && g2[u][u])) {
      g1[id[u]][id[u]] = 1;
    }
  }

  // encontrar g2 só copiar m[i][i] = 0
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      g2[i][j] = g1[i][j];
    }
    g2[i][i] = 0;
  }

  incidencia(g1, m1, n);

  // Fecho transitivo - usando floyd-warshall
  int g2T[MAX][MAX];
  int dist[MAX][MAX];
  for (auto &x : dist)
    for (auto &y : x) y = oo;

  for (int i = 0; i < n; i++) {
    dist[i][i] = 0;
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
  incidencia(g2T, m2, n);

  // Multiplicação
  for (int i = 0; i < m; i++) {
    for (int j = 0; j < m; j++) {
      m3[i][j] = 0;
      for (int k = 0; k < n; k++) {
        m3[i][j] |= m1[i][k] ^ m2[k][j];
      }
    }
  }

  // Transformar matriz de incidencia em um grafo
  for (int e = 0; e < m; e++) {
    int u = -1, v = -1;
    for (int i = 0; i < n; i++) {
      if (m3[e][i]) {
        if (u == -1)
          u = i;
        else
          v = i;
      }
    }
    assert(u != -1 && v != -1);
    g3[u][v] = 1;
  }

  vector<vi> ans(n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      int x = g1[i][j] - g3[i][j];
      if (x) {
        ans[i].emplace_back(j);
      }
    }
  }

  for (int u = 0; u < n; u++) {
    cout << u + 1 << ": ";
    bool first = true;
    if (ans[u].empty()) {
      cout << "(vazia)\n";
    }
    for (auto &v : ans[u]) {
      if (!first) cout << ",";
      cout << v + 1;
      first = false;
    }
    cout << "\n";
  }
}
