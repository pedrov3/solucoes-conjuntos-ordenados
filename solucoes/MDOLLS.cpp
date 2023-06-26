// Solução para o problema MDOLLS - Nested Dolls
// link: https://www.spoj.com/problems/MDOLLS/

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

#define MAX 20004  // TODO: problematico

vector<ll> bit(MAX, 0LL);
void add(int i, ll delta) {
  for (; i < MAX; i += i & (-i)) bit[i] = max(bit[i], delta);
}
ll get(int i) {
  if (i <= 0) return 0LL;
  ll ans = 0LL;
  for (; i > 0; i -= i & (-i)) ans = max(ans, bit[i]);
  return ans;
}

int lis(vi &vet) {
  int n = vet.size();
  vector<ii> tmp(n);
  for (int i = 0; i < n; i++) {
    tmp[i].first = vet[i];
    tmp[i].second = i + 1;  //-(i + 1);
  }
  sort(all(tmp));
  int ans = 0;
  fill(all(bit), 0);
  for (int i = 0; i < n; i++) {
    int pos = tmp[i].second;  //-tmp[i].second;
    int now = get(pos - 1);
    ans = max(ans, now + 1);
    add(pos, now + 1);
  }
  return ans;
}

bool cmp(ii a, ii b) {
  return a.first < b.first || (a.first == b.first && a.second > b.second);
}

int32_t main() {
  cin.tie(0);
  ios_base::sync_with_stdio(0);

  int t;
  cin >> t;
  while (t--) {
    int m;
    cin >> m;
    vector<ii> dolls(m);
    for (auto &[width, height] : dolls) {
      cin >> width >> height;
    }
    sort(all(dolls), cmp);
    vi vet;
    for (auto &[ignore, x] : dolls) {
      vet.emplace_back(x);
    }
    reverse(all(vet));
    cout << lis(vet) << "\n";
  }
}
