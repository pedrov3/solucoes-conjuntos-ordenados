// Solução para o problema Birthday
// Link: https://codeforces.com/contest/590/problem/E

#include <bits/stdc++.h>
#define all(x) x.begin(), x.end()
#define to_i(ch) (ch - 'a');
#define MAXN 750

bool fecho[MAXN][MAXN];

using namespace std;

typedef long double ld;
typedef long long ll;
typedef pair<int, int> ii;
typedef tuple<int, int, int> iii;
typedef vector<int> vi;

mt19937 rng((int)chrono::steady_clock::now().time_since_epoch().count());

struct kuhn {
  int n, m;
  vector<vector<short int>> g;
  vector<bool> vis;
  vector<short int> ma, mb;

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
    for (auto &i : g) shuffle(i.begin(), i.end(), rng);
    while (aum) {
      for (int j = 0; j < m; j++) vis[n + j] = 0;
      aum = 0;
      for (int i = 0; i < n; i++)
        if (ma[i] == -1 and dfs(i)) ret++, aum = 1;
    }
    return ret;
  }
};

pair<set<short>, set<short>> recover(kuhn &K) {
  K.matching();
  int n = K.n, m = K.m;
  for (int i = 0; i < n + m; i++) K.vis[i] = 0;
  for (int i = 0; i < n; i++)
    if (K.ma[i] == -1) K.dfs(i);
  set<short> ca, cb;
  for (int i = 0; i < n; i++)
    if (!K.vis[i]) ca.emplace(i);
  for (int i = 0; i < m; i++)
    if (K.vis[n + i]) cb.emplace(i);
  return {ca, cb};
}

struct node {
#define K 2
  int nxt[K];
  short term = -1;
  int link = -1, exi = -1;
  node() { memset(nxt, -1, sizeof(nxt)); }
};

vector<node> aca;  // aho-corasick automaton

int get_link(int at, int c) {
  while (at != -1 and aca[at].nxt[c] == -1) {
    at = aca[at].link;
  }
  return at == -1 ? 0 : aca[at].nxt[c];
}

void build() {
  queue<int> q;
  q.push(0);
  aca[0].exi = aca[0].link = -1;
  while (not q.empty()) {
    int i = q.front();
    q.pop();
    for (int c = 0; c < 2; c++) {
      int j = aca[i].nxt[c];
      if (j == -1) continue;
      int u = i;
      int v = get_link(aca[u].link, c);
      aca[j].link = v;
      aca[j].exi = aca[v].term != -1 ? v : aca[v].exi;
      q.push(j);
    }
  }
  for (int i = 0; i < aca.size(); i++) {
    if (aca[i].link == -1) aca[i].link = 0;
    if (aca[i].exi == -1) aca[i].exi = 0;
  }
}

void process(string &word, int dx) {
  int n = word.size(), u = 0;
  for (int i = 0; i < n; i++) {
    int c = to_i(word[i]);
    u = aca[u].nxt[c];
    // se tem alguma substring pelo caminho
    if (aca[u].term != -1 && aca[u].term != dx) {
      int _u = aca[u].term;
      int _v = dx;
      fecho[_u][_v] = 1;
    }
    if (aca[u].exi != 0 && aca[aca[u].exi].term != dx) {
      int _u = aca[aca[u].exi].term;
      int _v = dx;
      fecho[_u][_v] = 1;
    }
  }
}
void ins(string &word, int dx) {
  int n = word.size(), u = 0;
  for (int i = 0; i < n; i++) {
    int c = to_i(word[i]);
    if (aca[u].nxt[c] == -1) {
      aca[u].nxt[c] = aca.size();
      aca.emplace_back();
    }
    u = aca[u].nxt[c];
  }
  aca[u].term = dx;
}

int32_t main() {
  cin.tie(0);
  ios_base::sync_with_stdio(0);
  aca.emplace_back();

  int n;
  cin >> n;
  vector<string> lembrancas(n);
  for (int i = 0; i < n; i++) {
    cin >> lembrancas[i];
    ins(lembrancas[i], i);
  }

  build();
  for (int i = 0; i < n; i++) process(lembrancas[i], i);

  for (int k = 0; k < n; k++) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        if (fecho[i][k] && fecho[k][j]) {
          fecho[i][j] = 1;
        }
      }
    }
  }

  // Teorema de Dilworth
  kuhn k(n, n);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (fecho[i][j]) {
        k.add(i, j);
      }
    }
  }

  cout << n - k.matching() << "\n";
  auto [sa, sb] = recover(k);
  for (int i = 0; i < n; i++) {
    if (!sa.count(i) && !sb.count(i)) {
      cout << i + 1 << " ";
    }
  }
  cout << "\n";
}
