// Solução para o problema POSTERS
// link: https://www.codechef.com/problems/POSTERS

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

mt19937 rng((int)chrono::steady_clock::now().time_since_epoch().count());

// from: https://github.com/brunomaletta/Biblioteca/blob/master/Codigo/Grafos/kuhn.cpp
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

bool f(pair<ii, ii> a, pair<ii, ii> b) {
  /*   bool p1 = b.second.first <= a.first.first;
     bool p2 = b.first.first >= a.second.first;
     bool p3 = b.second.second <= a.first.second;
     bool p4 = b.first.second >= a.second.second;
          cout << "p1: " << p1 << "\n";
         cout << "p2: " << p2 << "\n";
         cout << "p3: " << p3 << "\n";
         cout << "p4: " << p4 << "\n";
         cout << "a: (" << a.first.first << "," << a.first.second << "),";
            cout << "(" << a.second.first << "," << a.second.second << ")\n";

         cout << "b: (" << b.first.first << "," << b.first.second << "),";
            cout << "(" << b.second.first << "," << b.second.second << ")\n";
 */
  if (b.second.first < a.first.first || b.first.first > a.second.first ||
      b.second.second < a.first.second || b.first.second > a.second.second) {
    // cout << "NO\n";
    return false;
  }
  // cout << "YES\n";
  return true;
}

int32_t main() {
  cin.tie(0);
  ios_base::sync_with_stdio(0);
  int t;
  cin >> t;
  while (t--) {
    int n;
    cin >> n;
    vector<pair<ii, ii>> retangulos(n);
    for (int i = 0; i < n; i++) {
      int x0, y0, x1, y1;
      cin >> x0 >> x1 >> y0 >> y1;
      retangulos[i] = make_pair(ii(x0, y0), ii(x1, y1));
    }
    kuhn k(n, n);

    for (int i = 0; i < n; i++) {
      for (int j = 0; j < n; j++) {
        int deltax1 = retangulos[i].first.second - retangulos[i].first.first;
        int deltax2 = retangulos[j].first.second - retangulos[j].first.first;
        if (i != j && f(retangulos[i], retangulos[j]) && deltax1 > deltax2) {
          k.add(i, j);
          //       cout << i << " " << j << "\n";

          //  k.add(i, j);
          // cout << i << " " << j << "\n";
        }
      }
    }
    cout << n - k.matching() << "\n";
    // cout << "\n";
  }
}
