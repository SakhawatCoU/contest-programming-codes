struct TopologicalSort {
    int n;
    VVI adj;
    VI sorted;

    TopologicalSort (int n): n(n), adj(n) {}

    void addEdge (int a, int b) {
        adj[a].push_back(b);
    }

    void dfs (int u, VB &marked) {
        if (marked[u]) {
            return;
        }
        for (auto v: adj[u]) {
            dfs(v, marked);
        }
        marked[u] = true;
        sorted.push_back(u);
    }

    void sort () {
        VB marked(n, false);
        sorted.clear();

        for (int i = 0; i < n; ++i) {
            dfs(i, marked);
        }
    }
};


/*
    Finds SCC with Tarjan's Algorithm.
    In addition the SCC's will be in topological order.
*/

struct StronglyConnectedComponents {
    int n;
    int totalComponents;
    VVI adj, components;
    VI idx, componentOf, st, low;
    VB inStack;
    
    StronglyConnectedComponents (int n): n(n), adj(n) {}

    void addEdge (int a, int b) {
        adj[a].push_back(b);
    }

    int DFS (int v, int index) {
        idx[v] = index;
        low[v] = index;
        index += 1;
        st.push_back(v);
        inStack[v] = true;

        for (auto w: adj[v]) {
            if (idx[w] == -1) {
                index = DFS(w, index);
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
        return index;
    }

    void buildSCC () {
        totalComponents = 0;
        idx = VI(n,-1), low = VI(n), componentOf = VI(n), inStack = VB(n, false);
        st.clear();
        
        for (int i = 0; i < n; i++) if (idx[i] == -1) {
            DFS(i, 0);
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

template <class T> struct Kruskal {
    int n;
    vector <pair <T, PI>> edges;
    DisjointSet* D;

    Kruskal (int n): n(n) {}

    void addEdge (int s, int t, T d) {
        edges.push_back(make_pair(d, make_pair(s, t)));
    }

    T MST (vector <pair <T, PI>>* mst = NULL) {
        T ret = 0;
        D = new DisjointSet(n);
        sort(all(edges));
        for (auto e: edges) if (D->Union(e.y.x, e.y.y)) {
            ret += e.x;
            if (mst) {
                mst->push_back(e);
            }
        }
        return ret;
    }
};

template <class T> struct Dijkstra {
    int n;
    bool directed;
    vector <vector <pair <T, int>>> adj;

    Dijkstra (int n, bool directed = false): n(n), adj(n), directed(directed) {}

    void addEdge (int a, int b, T d) {
        adj[a].push_back(make_pair(d, b));
        if (!directed) {
            adj[b].push_back(make_pair(d, a));
        }
    }

    void buildTree (int s, vector<T> &dist) {
        dist = vector <T>(n, inf);
        priority_queue <pair <T, vector<pair <T, int>>>, vector<pair <T, int>>, greater<pair <T, int>>> q;

        dist[s] = 0;
        q.push(make_pair(dist[s], s));
        do {
            auto u = q.top();
            q.pop();
            for (auto e: adj[u.y]) if(u.x + e.x < dist[e.y]) {
                dist[e.y] = u.x + e.x;
                q.push(make_pair(dist[e.y], e.y));
            }
        } while (!q.empty());
    }
};

struct BiconnectedComponents {
    int n;
    int totalComponents;
    VVPI adj;
    VPI edges;
    VI idx, low;
    VI cutVertices, bridges;
    VI st;
    VVI components;

    BiconnectedComponents (int n): n(n), adj(n) {}

    void addEdge (int a, int b) {
        int i = sz(edges);
        adj[a].push_back(make_pair(b, i));
        adj[b].push_back(make_pair(a, i));
        edges.push_back(make_pair(a, b));
    }

    int DFS (PI v, int index) {
        idx[v.x] = index;
        low[v.x] = index;
        index += 1;

        int children = 0;
        bool ap = false;
        for (auto w: adj[v.x]) if (w.y != v.y) {
            if (idx[w.x] == -1) {
                st.push_back(w.y);
                index = DFS(w, index);
                low[v.x] = min(low[v.x], low[w.x]);
                if (low[w.x] > idx[v.x]) {
                    bridges.push_back(w.y);
                }
                children++;
                if (low[w.x] >= idx[v.x]) {
                    if (v.y != -1 || children >= 2) {
                        ap = true;
                    }
                    components.push_back(VI());
                    totalComponents++;
                    int u;
                    do {
                        u = st.back();
                        st.pop_back();
                        components.back().push_back(u);
                    } while (u != w.y);
                }
            } else if (idx[w.x] < idx[v.x]) {
                st.push_back(w.y);
                low[v.x] = min(low[v.x], idx[w.x]);
            }
        }
        if (ap) {
            cutVertices.push_back(v.x);
        }
        return index;
    }

    void buildBCC () {
        idx = VI(n, -1), low = VI(n);
        cutVertices.clear();
        bridges.clear();
        st.clear();
        components.clear();
        totalComponents++;

        for (int i = 0; i < n; i++) if (idx[i] == -1) {
            DFS(make_pair(i, -1), 0);
        }
    }
};

struct HopcroftKarp { // Carefull, -1 is not used as 'not matched'
    int n, m;
    VVI adj;
    VI right, left;
    VI dist;

    HopcroftKarp (int n, int m): n(n), m(m), adj(n) {}

    void addEdge (int l, int r) {
        adj[l].push_back(r);
    }
    
    bool bfs () {
        queue <int> q;
        dist = VI(n + 1, -1);
        for (int l = 0; l < n; ++l) if (right[l] == m) {
            dist[l] = 0;
            q.push(l);
        }

        while (!q.empty()) {
            int l = q.front();
            q.pop();
            if (dist[n] == -1 || dist[l] < dist[n]) {
                for (auto r: adj[l]) if (dist[left[r]] == -1) {
                    dist[left[r]] = dist[l] + 1;
                    q.push(left[r]);
                }
            }
        }
        return dist[n] != -1;
    }

    bool dfs (int l) {
        if (l != n) {
            for (auto r: adj[l]) if (dist[left[r]] == dist[l] + 1 && dfs(left[r])) {
                left[r] = l;
                right[l] = r;
                return true;
            }
            dist[l] = -1;
            return false;
        }
        return true;
    }

    int match () {
        right = VI(n, m);
        left = VI(m, n);
        int ret = 0;
        while (bfs()) {
            for (int l = 0; l < n; ++l) if (right[l] == m && dfs(l)) {
                ret++;
            }
        }
        return ret;
    }

    void minimumVertexCover (VB &leftCover, VB &rightCover) { // {side}Cover[i] = true iff i of {side} in the the vertex cover (not in maximum independent set)
        leftCover = VB(n, true), rightCover = VB(m, false);
        queue <int> q;
        dist = VI(n + 1, -1);
        for (int l = 0; l < n; ++l) if (right[l] == m) {
            dist[l] = 0;
            q.push(l);
        }

        while (!q.empty()) {
            int l = q.front();
            q.pop();
            leftCover[l] = false;
            if (dist[n] == -1 || dist[l] < dist[n]) {
                for (auto r: adj[l]) if (dist[left[r]] == -1) {
                    dist[left[r]] = dist[l] + 1;
                    rightCover[r] = true;
                    q.push(left[r]);
                }
            }
        }
    }
};

void stableMatching (int n, VVI &maleRank, VVI &femaleRank, VI &wife) {
    // a male m prefers w to w' if maleRank[m][w] < maleRank[m][w']
    // returns male-optimal matching

    VI freeMen;
    VVPI fq(n);
    VI husband(n, -1);
    for (int m = 0; m < n; ++m) {
        for (int w = 0; w < n; ++w) {
            fq[m].push_back(make_pair(maleRank[m][w], w));
        }
        sort(all(fq[m]), greater<PI>());
        freeMen.push_back(m);
    }
    while (!freeMen.empty()) {
        int m = freeMen.back(), w = fq[m].back().y;
        fq[m].pop_back();
        if (husband[w] == -1) {
            husband[w] = m;
            freeMen.pop_back();
        } else if (femaleRank[w][m] < femaleRank[w][husband[w]]) {
            freeMen.pop_back();
            freeMen.push_back(husband[w]);
            husband[w] = m;
        }
    }
    wife = VI(n);
    for (int w = 0; w < n; ++w) {
        wife[husband[w]] = w;
    }
}
