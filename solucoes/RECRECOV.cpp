// Solução para o problema RECRECOV
// link: https://www.codechef.com/problems/RECRECOV

#include <bits/stdc++.h>

#include <ext/pb_ds/assoc_container.hpp>
#include <ext/pb_ds/detail/standard_policies.hpp>
#include <ext/pb_ds/tree_policy.hpp>

#define int long long
#define all(x) x.begin(), x.end()

using namespace __gnu_pbds;
using namespace std;

template <class T>
using ordered_set =
    tree<T, null_type, less<T>, rb_tree_tag, tree_order_statistics_node_update>;
typedef long double ld;
typedef long long ll;
typedef pair<int, int> ii;
typedef tuple<int, int, int> iii;
typedef vector<int> vi;
const ll oo = 1987654321987654321;

template <class It>
void db(It b, It e) {
  for (auto it = b; it != e; it++) cout << *it << ' ';
  cout << endl;
}

// Kuhn
//
// Computa matching maximo em grafo bipartido
// 'n' e 'm' sao quantos vertices tem em cada particao
// chamar add(i, j) para add aresta entre o cara i
// da particao A, e o cara j da particao B
// (entao i < n, j < m)
// Para recuperar o matching, basta olhar 'ma' e 'mb'
// 'recover' recupera o min vertex cover como um par de
// {caras da particao A, caras da particao B}
//
// O(|V| * |E|)
// Na pratica, parece rodar tao rapido quanto o Dinic

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

pair<vector<int>, vector<int>> recover(kuhn& K) {
  K.matching();
  int n = K.n, m = K.m;
  for (int i = 0; i < n + m; i++) K.vis[i] = 0;
  for (int i = 0; i < n; i++)
    if (K.ma[i] == -1) K.dfs(i);
  vector<int> ca, cb;
  for (int i = 0; i < n; i++)
    if (!K.vis[i]) ca.push_back(i);
  for (int i = 0; i < m; i++)
    if (K.vis[n + i]) cb.push_back(i);
  return {ca, cb};
}

int32_t main() {
  cin.tie(0);
  ios_base::sync_with_stdio(0);
  int t;
  cin >> t;
  while (t--) {
    int n, m;
    cin >> n >> m;
    kuhn k(n, n);
    for (int i = 0; i < m; i++) {
      int u, v;
      cin >> u >> v;
      --u, --v;
      k.add(u, v);
    }
    cout << n - k.matching() << "\n";
  }
}
