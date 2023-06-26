// Sweep line + Binary lifting
#include <bits/stdc++.h>

#define int long long
using namespace std;
int oo = 1e8;
#define MAX 812345
#define LOGN 20

typedef pair<int, int> ii;
typedef vector<int> vi;

// <x1, y1, x2, y2>
vector<tuple<int, int, int, int>> segmentos;
vector<int> baloes;
int nxt[MAX];
int h[MAX];
vi filhos[MAX];
int tab[MAX][LOGN];

tuple<int, int, int> getreal(pair<int, int> &a) {
  auto &[id, tipo] = a;
  tuple<int, int, int> retval;
  if (tipo == 1) {
    retval = make_tuple(baloes[id], -oo, tipo);
  } else if (tipo == 0) {
    retval = make_tuple(get<0>(segmentos[id]), -get<1>(segmentos[id]), tipo);
  } else {
    retval = make_tuple(get<2>(segmentos[id]), -get<3>(segmentos[id]), tipo);
  }
  return retval;
}

bool cmp(pair<int, int> &a, pair<int, int> &b) {
  int tipoa = a.second, tipob = b.second;
  if ((tipoa != 1 && tipob != 1) || (tipoa == 1 && tipob == 1)) {
    return getreal(a) < getreal(b);
  } else {
    if (tipoa == 1) {
      int x;
      int idb = b.first;
      if (tipob == 0) {
        x = get<0>(segmentos[idb]);
      } else {
        x = get<2>(segmentos[idb]);
      }
      int ida = a.first;
      return (baloes[ida] < x || (baloes[ida] == x && tipob == 2));
    } else {
      return !cmp(b, a);
    }
  }
}

struct point {
  int x, y;
  point(int _x, int _y) : x(_x), y(_y) {}
  point() {}
};

struct vet {
  int x, y;
  vet(int _x, int _y) : x(_x), y(_y) {}
  vet(point a, point b) : x(b.x - a.x), y(b.y - a.y) {}
  vet() {}
  vet operator-(vet oth) {
    vet c;
    c.x = this->x - oth.x;
    c.y = this->y - oth.y;
    return c;
  }
};

// produto vetorial
int cross(vet u, vet v) { return u.x * v.y - u.y * v.x; }

struct classcmp {  // TODO:
  bool operator()(int bah, int eh) {
    auto [a, b, c, d] = segmentos[bah];
    auto [w, x, y, z] = segmentos[eh];

    if (a < w) {
      vet u(point(a, b), point(c, d)), v(point(a, b), point(w, x));
      return cross(u, v) > 0;
    } else {
      vet u(point(w, x), point(y, z)), v(point(w, x), point(a, b));
      return cross(u, v) < 0;
    }
  }
};

void dfs(int u, int _h = 0) {
  h[u] = _h;
  for (auto &v : filhos[u]) {
    dfs(v, _h + 1);
  }
}

void build(int n) {
  for (auto &u : filhos) u.clear();
  for (int i = 0; i < n; i++) {
    tab[i][0] = nxt[i];
    if (nxt[i] != i) filhos[nxt[i]].push_back(i);
  }
  for (int j = 1; j < LOGN; j++) {
    for (int i = 0; i < n; i++) {
      tab[i][j] = tab[tab[i][j - 1]][j - 1];
    }
  }
  for (int i = 0; i < n; i++) {
    if (nxt[i] == i) {
      dfs(i);
    }
  }
}

int goup(int u, int x) {
  for (int j = 0; j < LOGN; j++) {
    if ((1 << j) & x) {
      u = tab[u][j];
    }
  }
  return u;
}

bool eh_balao(int u) { return u >= segmentos.size(); }

ii getans(int u, int v) {  // v pode ser o coringa
  if (v == baloes.size() + segmentos.size()) {
    if (eh_balao(u)) {
      return make_pair(baloes[u - segmentos.size()], -1);
    } else {
      int id = u;
      int y1, y2, x1, x2;
      x1 = get<0>(segmentos[id]);
      y1 = get<1>(segmentos[id]);
      x2 = get<2>(segmentos[id]);
      y2 = get<3>(segmentos[id]);

      assert(y1 != y2);
      if (y1 < y2) {
        return make_pair(x2, -1);
      } else {
        return make_pair(x1, -1);
      }
    }
  } else {
    int x = -1;
    if (eh_balao(u)) {
      x = baloes[u - segmentos.size()];
    } else {
      // TODO: codigo repetido
      int id = u;
      int y1, y2, x1, x2;
      x1 = get<0>(segmentos[id]);
      y1 = get<1>(segmentos[id]);
      x2 = get<2>(segmentos[id]);
      y2 = get<3>(segmentos[id]);

      assert(y1 != y2);
      if (y1 < y2) {
        x = x2;
      } else {
        x = x1;
      }
    }
    int id = v;
    assert(get<1>(segmentos[id]) == get<3>(segmentos[id]));
    return make_pair(x, get<1>(segmentos[id]));
  }
}

int32_t main() {
  int n, c;

  while (cin >> n >> c) {
    // <id, tipo>
    vector<pair<int, int>> ev;
    for (int i = 0; i < n; i++) {  // Segmentos
      int x1, y1, x2, y2;
      cin >> x1 >> y1 >> x2 >> y2;

      if (x1 > x2) swap(x1, x2), swap(y1, y2);

      segmentos.emplace_back(x1, y1, x2, y2);
      ev.emplace_back(segmentos.size() - 1, 0);
      ev.emplace_back(segmentos.size() - 1, 2);
    }

    for (int i = 0; i < c; i++) {  // Baloes
      int x;
      cin >> x;
      baloes.push_back(x);
      ev.emplace_back(baloes.size() - 1, 1);
    }
    sort(ev.begin(), ev.end(), cmp);

    int coringa = segmentos.size() + baloes.size();
    nxt[coringa] = coringa;
    set<int, classcmp> s;
    for (auto &[id, tipo] : ev) {
      if (tipo == 0) {  // Entrada de segmento
        int y1, y2;
        y1 = get<1>(segmentos[id]);
        y2 = get<3>(segmentos[id]);

        if (y1 == y2) {  // Estacionaria
          nxt[id] = id;
        } else if (y1 < y2) {  // Rampa pra cima
        } else {               // Rampa pra baixo*
          auto it = s.lower_bound(id);
          if (it == s.end()) {
            nxt[id] = coringa;
          } else {
            nxt[id] = *it;
          }
        }
        s.insert(id);
      } else if (tipo == 1) {  // Query Balao
        if (s.empty()) {
          nxt[id + segmentos.size()] = coringa;
        } else {
          nxt[id + segmentos.size()] = *(s.begin());
        }

      } else {  // Saida de segmento
        int y1, y2;
        y1 = get<1>(segmentos[id]);
        y2 = get<3>(segmentos[id]);

        s.erase(id);
        if (y1 == y2) {        // Estacionaria
        } else if (y1 < y2) {  // Rampa pra cima*
          auto it = s.lower_bound(id);
          if (it == s.end()) {
            nxt[id] = coringa;
          } else {
            nxt[id] = *it;
          }
        } else {  // Rampa pra baixo
        }
      }
    }
    build(segmentos.size() + baloes.size() + 1);
    for (int i = 0; i < baloes.size(); i++) {
      int id = segmentos.size() + i;
      ii ans = getans(goup(id, h[id] - 1), goup(id, h[id]));
      if (ans.second == -1) {
        cout << ans.first << "\n";
      } else {
        cout << ans.first << " " << ans.second << "\n";
      }
    }

    baloes.clear();
    segmentos.clear();
  }
}

/*
// Quero processar eventos de entrada primeiro
// Baloes em segundo
// Eventos de saida por último


//         /
// Se for / Preciso processar na saida
// porém se for \   Preciso processar na entrada
//               \
*/
