#include <bits/stdc++.h>

using namespace std;

#define x first
#define y second
#define mp make_pair
#define pb push_back
#define sz(a) (int)(a.size())
#define all(a) a.begin(), a.end()
#define R(a) ((a)%mod)

typedef vector<int> VI;
typedef vector<VI> VVI;
typedef long long ll;
typedef vector<ll> VL;
typedef vector<VL> VVL;
typedef vector<VVL> VVVL;
typedef vector<bool> VB;
typedef vector<string> VS;
typedef pair<int, int> PI;
typedef vector<PI> VPI;
typedef vector<VPI> VVPI;
typedef vector<double> VD;
typedef vector<VD> VVD;

struct TarjanSCC {
    int n;
    int index, totalComponents;
    VVI adj, components;
    VI idx, componentOf, st, low;
    VB inStack;
    
    TarjanSCC (int n): n(n), adj(n) {}

    void addEdge (int a, int b) {
        adj[a].push_back(b);
    }

    void DFS (int v) {
        idx[v] = index;
        low[v] = index;
        index += 1;
        st.push_back(v);
        inStack[v] = true;

        for (auto w: adj[v]) {
            if (idx[w] == -1) {
                DFS(w);
                low[v] = min(low[v], low[w]);
            } else if (inStack[w]) {
                low[v] = min(low[v], low[w]);                
            }
        }

        if (low[v] == idx[v]) {
            int w;
            components.push_back(VI());
            do {
                w = st.back();
                st.pop_back();
                inStack[w] = false;
                componentOf[w] = totalComponents;
                components[totalComponents].push_back(w);
            } while (w != v);
            totalComponents++;
        }
    }

    void buildSCC () {
        index = 0, totalComponents = 0;
        idx = VI(n,-1), low = VI(n), componentOf = VI(n), inStack = VB(n, false);
        st.clear();
        
        for (int i = 0; i < n; i++) if (idx[i] == -1) {
            DFS(i);
        }
    }

    /*
        Finds an asignment for a 2-SAT problem and stores in sol
        neg[i] is the inverse of i
    */
    void find2SATSolution (VI &sol, VI &neg) {
        sol = VI(n, -1);
        for (auto comp: components) {
            for (auto j: comp) if (sol[j] == -1) {
                sol[j] = 1;
                sol[neg[j]] = 0;
            }
        }
    }
};

int main(int argc, char const *argv[]) {
    ios::sync_with_stdio(false);
    
    TarjanSCC* T;
    int n, m;
    while (cin >> n >> m && n != 0) {
        T = new TarjanSCC(n);
        for (int i = 0; i < m; ++i) {
            int a, b;
            cin >> a >> b;
            T->addEdge(a - 1, b - 1);
        }
        T->buildSCC();
        if (T->totalComponents != n) {
            cout << "IMPOSSIBLE" << endl;
        } else {
            reverse(all(T->components));
            for (auto c: T->components) {
                cout << c.front() + 1 << endl;
            }
        }
    }

    return 0;
}