#include <cstdio>

#define INT_MAX 2147483647

#define QSIZE 1000000
#define ESIZE 1000000

unsigned long long int minD = INT_MAX;

struct node {
    int v;
    node* next;
};

node** F;

unsigned int* p;

node* mempool;
int memcount;

int FindSet(unsigned int x) {
    if(x != p[x])
        return FindSet(p[x]);
    return x;
}

void Union(unsigned int x, unsigned int y) {
    int nx = FindSet(x);
    int ny = FindSet(y);
    if(nx == ny)
        return;
    else if(nx<ny)
        p[nx] = ny;
    else
        p[ny] = nx;

    node* n = &(mempool[memcount++]);
    n->next = F[x];
    n->v = y;
    F[x] = n;
    n = &(mempool[memcount++]);
    n->next = F[y];
    n->v = x;
    F[y] = n;
}

class Queue {
    public:
    Queue() {
        size = 0;
        tail = 0;
        head = 0;
    }

    void enqueue(unsigned int el) {
        q[tail++] = el;
        size++;

        if(tail == QSIZE)
            tail = 0;
    }

    unsigned int dequeue() {
       unsigned int x = q[head++];
        size--;

        if (head == QSIZE)
            head = 0;

        return x;
    }

    bool empty() {
        return !size;
    }

    private:
    unsigned int q[QSIZE];
    unsigned int head;
    unsigned int tail;
    unsigned int size;
};

struct Edge {
    unsigned int to;
    Edge *next;

    Edge() {
        to = 0;
        next = NULL;
    }
};

Edge eMemPool[ESIZE];
unsigned int eMemCount = 0;

class Graph {
private:
    unsigned int size;
public:
    Edge **nodes;
    Graph(unsigned int nodesNumber) {
        this->size = nodesNumber;
        this->nodes = new Edge*[size];

        for (unsigned int i = 0; i < nodesNumber; i++)
            this->nodes[i] = NULL;
    }

    ~Graph() {
        delete[] this->nodes;
    }

    void addEdge(unsigned int fromNode, unsigned int toNode) {
        //fprintf(stdout, "%u %u\n", fromNode, toNode);
        Edge *e = &eMemPool[eMemCount++];
        e->to = toNode;
        e->next = nodes[fromNode];
        nodes[fromNode] = e;

        e = &eMemPool[eMemCount++];

        e->to = fromNode;
        e->next = nodes[toNode];
        nodes[toNode] = e;
    }

    unsigned int getSize() {
        return size;
    }
};

void performTest(Graph *g) {
    unsigned int *d = new unsigned int[g->getSize()];

    unsigned int *color = new unsigned int[g->getSize()];
    mempool = new node[2*g->getSize()];
    memcount = 0;

    Queue f;
    unsigned int M;
    fscanf(stdin, "%u", &M);
    p = new unsigned int[g->getSize()];
    for(int i = 0; i < g->getSize(); i++) {
        p[i] = i;
    }
    F = new node*[g->getSize()];
    for (unsigned int i = 0; i < g->getSize(); i++) {
        color[i] = INT_MAX;
        d[i] = 0;
    }

    while(M--) {
        unsigned int v;
        fscanf(stdin, "%u", &v);
        f.enqueue(v);
        color[v] = v;
    }


    while (!f.empty()) {
        unsigned int u = f.dequeue();
        Edge *tmp = g->nodes[u];
        while (tmp != NULL) {
            //fprintf(stdout, "from:%u tmp->to=%u color:%u\n", u, tmp->to, color[tmp->to]);
            if (color[tmp->to] == INT_MAX) {
                d[tmp->to] = d[u] + 1;
                color[tmp->to] = FindSet(color[u]);
                f.enqueue(tmp->to);
            } else {
                //fprintf(stdout, "kolory %u %u\n", FindSet(color[u]), FindSet(color[tmp->to]));
                if(FindSet(color[u]) != FindSet(color[tmp->to])) {
                    //FUZJA!
                    fprintf(stdout, "%u ", (d[u]+1));
                    //UNION
                    Union(color[tmp->to], color[u]);
                }
            }
            tmp = tmp->next;
        }
    }
    fprintf(stdout, "\n");
    delete[] d;
}

unsigned int *color;

int main () {

    unsigned int C, Vn;
    unsigned long long En;

    fscanf(stdin, "%u", &C);

    while (C--) {
        eMemCount = 0;
        fscanf(stdin, "%u %llu", &Vn, &En);

        Graph *g = new Graph(Vn);

        while (En--) {
            unsigned int fromNode, toNode;
            fscanf(stdin, "%u %u", &fromNode, &toNode);
            g->addEdge(fromNode, toNode);
        }
        //fprintf(stdout, )
        performTest(g);
        delete g;
        //return 0;

    }

    return 0;
}
