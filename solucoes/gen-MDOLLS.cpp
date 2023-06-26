// Gerador de caso de testes para o problema MDOLLS - Nested Dolls

#include <bits/stdc++.h>
#define int long long
#define all(x) x.begin(), x.end()
using namespace std;

typedef long long ll;
typedef pair<int, int> ii;
typedef vector<int> vi;
const ll oo = 1987654321987654321;

mt19937 rnd;
int gen(int a, int b) {
  uniform_int_distribution<int> dist(a, b);
  return dist(rnd);
}

#define MAXT 2
#define MAXN 4
#define MAXW 10
#define MAXH 10

int32_t main(int32_t argc, char *argv[]) {
  cin.tie(0);
  ios_base::sync_with_stdio(0);
  if (argc < 2) {
    cout << "usage: " << argv[0] << " <seed>\n";
    exit(1);
  }
  rnd = mt19937(atoi(argv[1]));

  int t = MAXT;
  cout << t << "\n";
  while (t--) {
    int n = gen(1, MAXN);
    cout << n << "\n";
    for (int i = 0; i < n; i++) {
      int w, h;
      w = gen(1, MAXW);
      h = gen(1, MAXH);
      cout << w << " " << h << "\n";
    }
  }
}
