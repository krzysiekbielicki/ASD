#include <cstdio>

int Vn;

FILE* file;

struct node {
    int v;
    int w;
    node* next;
};

node** F;

int* p;

bool* visited;

int domow;
int elektrowni;
int kabla;

node* mempool;
int memcount;

int FindSet(int x) {
    if(x != p[x])
        return FindSet(p[x]);
    return x;
}

void traverse(int v, int r) {
    visited[v] = true;

    node* n = F[v];
    while (n != NULL) {
        if (r > 0 && !visited[n->v]) {
            domow++;
            kabla += n->w;
            traverse(n->v, r-1);

        }
        n = n->next;
    }

}

void Union(int x, int y, int w) {
    int nx = FindSet(x);
    int ny = FindSet(y);
    if(nx == ny)
        return;
    else if(nx<ny)
        p[nx] = ny;
    else
        p[ny] = nx;

    kabla += w;
    elektrowni--;
    node* n = &(mempool[memcount++]);
    n->next = F[x];
    n->v = y;
    n->w = w;
    F[x] = n;
    n = &(mempool[memcount++]);
    n->next = F[y];
    n->v = x;
    n->w = w;
    F[y] = n;
}

void buildCity() {
    int En;
    fscanf(stdin, "%d %d", &Vn, &En);
    mempool = new node[2*En];
    memcount = 0;
    F = new node*[Vn];
    for(int i = 0; i < Vn; i++)
        F[i] = NULL;
    p = new int[Vn];
    elektrowni = Vn;
    kabla = 0;
    for(int i = 0; i < Vn; i++) {
        p[i] = i;
    }
    int u, v, w;
    while(En-- > 0) {
        fscanf(stdin, "%d %d %d", &u, &v, &w);
        Union(u, v, w);
    }
    fprintf(stdout, "%d %d\n", elektrowni, kabla);
    int Q, r;
    fscanf(stdin, "%d", &Q);
    visited = new bool[Vn];
    while(Q-- > 0) {
        for(int i = 0; i < Vn; i++)
            visited[i] = false;
        domow = 1;
        kabla = 0;
        fscanf(stdin, "%d %d", &v, &r);
        traverse(v, r);
        fprintf(stdout, "%d %d\n", domow, kabla);
    }
    delete[] visited;
    delete[] mempool;
    delete[] F;
    delete[] p;

}

int main() {
    int C;
    fscanf(stdin, "%d", &C);
    while(C-- > 0) {
        buildCity();
    }
    return 0;
}
