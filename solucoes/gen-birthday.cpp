// Gerador de casos de testes para o problema Birthday

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

#define MAXN 750
#define MAXC 13333
int32_t main(int32_t argc, char *argv[]) {
  cin.tie(0);
  ios_base::sync_with_stdio(0);
  if (argc < 2) {
    cout << "usage: " << argv[0] << " <seed>\n";
    exit(1);
  }
  rnd = mt19937(atoi(argv[1]));

  set<string> s;
  for (int i = 0; i < MAXN; i++) {
    int n = gen(2, MAXC);
    string str = "";
    for (int j = 0; j < n - 1; j++) {
      str += 'a';
    }
    str += char(gen('a', 'b'));
    s.insert(str);
  }
  cout << s.size() << "\n";
  for (auto &str : s) {
    cout << str << "\n";
  }
}
