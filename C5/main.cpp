#include <cstdio>

#define INT_MAX 2147483647

#define QSIZE 1000000
#define ESIZE 16000000

unsigned long long int minD = INT_MAX;

template <class T>
class Queue {
    public:
    Queue() {
        size = 0;
        tail = 0;
        head = 0;
    }

    void enqueue(T el) {
        q[tail++] = el;
        size++;

        if(tail == QSIZE)
            tail = 0;
    }

    T dequeue() {
       T x = q[head++];
        size--;

        if (head == QSIZE)
            head = 0;

        return x;
    }

    bool empty() {
        return !size;
    }

    private:
    T q[QSIZE];
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

void performTest(Graph *g, unsigned int fromNode, unsigned int toNode) {
    bool *checked = new bool[g->getSize()];
    unsigned int *d = new unsigned int[g->getSize()];

    for (unsigned int i = 0; i < g->getSize(); i++) {
        checked[i] = false;
        d[i] = 0;
    }

    checked[fromNode] = true;

    Queue <unsigned int> f;
    f.enqueue(fromNode);

    while (!f.empty()) {
        unsigned int u = f.dequeue();

        //retDomy++;

        Edge *tmp = g->nodes[u];
        while (tmp != NULL) {
            if (!checked[tmp->to]) {
                d[tmp->to] = d[u] + 1;
                if(tmp->to == toNode) {
                    unsigned int ret = d[tmp->to];
                    if(ret<minD){
                        minD = ret;
                    }

                    delete[] checked;
                    delete[] d;
                    return;
                } else {
                    checked[tmp->to] = true;
                    f.enqueue(tmp->to);
                }
            }
            tmp = tmp->next;
        }
    }
    delete[] checked;
    delete[] d;
}

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

        unsigned int D;
        fscanf(stdin, "%u", &D);
        unsigned int* zapytania = new unsigned int[D];
        for(unsigned int i = 0; i < D; i++) {
            fscanf(stdin, "%u", &(zapytania[i]));
        }
        for(unsigned int fromIndex = 0; fromIndex < D; fromIndex++) {
            for(unsigned int toIndex = fromIndex+1; toIndex < D; toIndex++) {
                performTest(g, zapytania[fromIndex], zapytania[toIndex]);
            }
        }

        fprintf(stdout, "%llu\n", minD);
        minD = INT_MAX;
        delete g;
    }

    return 0;
}
