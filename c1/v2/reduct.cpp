#include <cstdio>

#define MAX_V 10000000


struct node {
    int v;
    int w;
    node* next;
};

node* mempool = new node[MAX_V*2];
node* Gg[MAX_V] = {NULL};
node** G = Gg;
int Ggc[MAX_V] = {0};
int* Gc = Ggc;

int memsize = 0;

void add_el(node** el, int v) {
    node* e = &(mempool[memsize++]);
    e->v = v;
    e->w = 1;
    e->next = *el;
    *el = e;
}

bool checkGraph(int v) {
    //sprawdzam, czy graf posiada dwa wieszchołki st 1, jeden st 3, jedes st 5
    int i1[2];
    int i3;
    int i5;
    int s1 = 0;
    int s3 = 0;
    int s5 = 0;
    for(int i = 0; i < v; ++i) {
        switch(Gc[i]) {
        case 1:
            if(s1 < 2)
                i1[s1++] = i;
            else
                return false;
            break;
        case 2:
            break;
        case 3:
            if(s3 == 0) {
                s3++;
                i3 = i;
            } else
                return false;
            break;
        case 5:
            if(s5 == 0) {
                s5++;
                i5 = i;
            } else
                return false;
            break;
        default:
            return false;
        }
    }
    if(s1 != 2 && s3 != 1 && s5 != 1)
        return false;

    //redukuję graf - usuwam wierzchołki stopnia 2
    for(int i = 0; i < v; ++i) {
        if(Gc[i] == 2) {
            if(G[i]->v == G[i]->next->v)
                return false;
            node* tmp = G[G[i]->v];
            while(tmp != NULL) {
                if(tmp->v == i) {
                    tmp->v = G[i]->next->v;
                    tmp->w += G[i]->next->w;
                    break;
                }
                tmp = tmp->next;
            }
            tmp = G[G[i]->next->v];
            while(tmp != NULL) {
                if(tmp->v == i) {
                    tmp->v = G[i]->v;
                    tmp->w += G[i]->w;
                    break;
                }
                tmp = tmp->next;
            }
        }
    }

    //sprawdzam, czy graf nie ma pętli własnych
    for(int i = 0; i < v; ++i) {
        node* tmp = G[i];
        while(tmp != NULL) {
            if(tmp->v == i) // graf zawiera petle wlasna
                return false;
            tmp = tmp->next;
        }
    }

    //sprawdzam, czy z wierzcholkow stopnia 1 mozna dojsc do wierzcholka stopnia 5 nie przechodzac przez wierzcholek st. 3
    for(int i = 0; i < 2; i++) {
        if(G[i1[i]]->v != i5) return false;
    }
    if(G[i1[0]]->w != G[i1[1]]->w)//sprawdzam, czy czułki są równej długości
        return false;
    //sprawdzam, czy z wierzcholka st.3 mozna dojsc do wierzcholka st.5 tż dwie drogi są równej długości, a trzeci krótsza o co najmniej 1
    int z3do5[3];
    node* tmp3 = G[i3];
    for(int i = 0; i < 3; i++) {
        z3do5[i] = tmp3->w;
        if(tmp3->v != i5) return false;
        tmp3 = tmp3->next;
    }
    if(z3do5[0] == z3do5[1] && z3do5[0] > z3do5[2] && z3do5[2] > G[i1[0]]->w)
        return true;
    if(z3do5[0] == z3do5[2] && z3do5[0] > z3do5[1] && z3do5[1] > G[i1[0]]->w)
        return true;
    if(z3do5[2] == z3do5[1] && z3do5[1] > z3do5[0] && z3do5[0] > G[i1[0]]->w)
        return true;

    return false;
}

bool createGraph() {
    int v;
    int e;

    fscanf(stdin, "%d %d", &v, &e);
    /*for(int i = 0; i < v; ++i) {
        G[i] = NULL;
        Gc[i] = 0;
    }*/
    bool ret = true;
    for(int i = 0; i < e; ++i) {
        int x, y;
        fscanf(stdin, "%d %d", &x, &y);
        ret &= (x!=y);
        if(ret) {
            add_el(&G[x], y); //Dodawanie krawedzi miedzy wierzcholkiem x i y
            add_el(&G[y], x); //Dodawanie krawedzi miedzy wierzcholkiem x i y
            Gc[x]++;
            Gc[y]++;
        }
    }
    if(!ret) return false;
    ret = checkGraph(v);
    G = (G+v);
    Gc = (Gc+v);
    return ret;

}

int main() {
    int numGraph;
    fscanf(stdin, "%d", &numGraph);
    while(numGraph-- > 0) {
        if(createGraph()) {
            fprintf(stdout, "tak\n");
        } else {
            fprintf(stdout, "nie\n");
        }
    }
    //delete[] G;
    //delete[] Gc;
    delete[] mempool;
    return 0;
}
