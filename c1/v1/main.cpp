#include <cstdio>

//#define MAX_E 10000000
#define MAX_V 10000000


struct node {
    int v;
    node* next;
};

node* mempool = new node[MAX_V*2];
node** G = new node*[MAX_V];
long* Gc = new long[MAX_V];

int memsize = 0;

bool add_el(node** el, int v) {
    //dodaję nowy wierzchołek na końcu, żeby sprawdzić, czy krawedz się nie duplikuje.
    if(*el == NULL) {
        *el = &(mempool[memsize++]);
        (*el)->v = v;
        (*el)->next = NULL;
    } else {
        node* tmp = *el;
        if(tmp == NULL || tmp->v == v)
            return false;
        while(tmp->next != NULL) {
            if(tmp->v == v)
                return false;
            tmp = tmp->next;
        }
        tmp->next = &(mempool[memsize++]);
        tmp->next->next = NULL;
        tmp->next->v = v;
    }
    return true;
}

bool checkGraph(node** G, long v, long* Gc) {
    //sprawdzam, czy graf posiada dwa wieszchołki st 1, jeden st 3, jedes st 5
    int i1[2];
    int i3;
    int i5;
    int s1 = 0;
    int s2 = 0;
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
            s2++;
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
    int z1do5[2];
    for(int i = 0; i < 2; i++) {
        z1do5[i] = 0;
        int w = i1[i];
        int prevW = w;
        while(w != i5) {
            node* tmp = G[w];
            while(tmp != NULL) {
                if(tmp->v == prevW) {
                    tmp = tmp->next;
                } else {
                    prevW = w;
                    w = tmp->v;
                    break;
                }
            }
            z1do5[i]++;
            if(w == i3)
                return false;
        }
    }
    if(z1do5[0] != z1do5[1])
        return false;
    //sprawdzam, czy z wierzcholka st.3 mozna dojsc do wierzcholka st.5 tż dwie drogi są równej długości, a trzeci krótsza o co najmniej 1
    int z3do5[3];
    node* tmp3 = G[i3];
    for(int i = 0; i < 3; i++) {
        z3do5[i] = 1;
        int w = tmp3->v;
        int prevW = i3;
        while(w != i5) {
            z3do5[i]++;
            node* tmp = G[w];
            while(tmp != NULL) {
                if(tmp->v == prevW) {
                    tmp = tmp->next;
                } else {
                    prevW = w;
                    w = tmp->v;
                    break;
                }
            }
            if(w == i3)
                return false;
        }
        tmp3 = tmp3->next;
    }
    if(z3do5[0]+z3do5[1]+z3do5[2]+z1do5[0]+z1do5[1]-5 < s2)
        return false;
    if(z3do5[0] == z3do5[1] && z3do5[0] > z3do5[2] && z3do5[2] > z1do5[0])
        return true;
    if(z3do5[0] == z3do5[2] && z3do5[0] > z3do5[1] && z3do5[1] > z1do5[0])
        return true;
    if(z3do5[2] == z3do5[1] && z3do5[1] > z3do5[0] && z3do5[0] > z1do5[0])
        return true;

    return false;

}

bool createGraph(FILE* pFile) {
    int v;
    int e;

    fscanf(pFile, "%d %d", &v, &e);
    for(int i = 0; i < v; ++i) {
        G[i] = NULL;
        Gc[i] = 0;
    }
    bool ret = true;
    for(int i = 0; i < e; ++i) {
        int x, y;
        fscanf(pFile, "%d %d", &x, &y);
        ret &= add_el(&G[x], y); //Dodawanie krawedzi miedzy wierzcholkiem x i y
        ret &= add_el(&G[y], x); //Dodawanie krawedzi miedzy wierzcholkiem x i y
        Gc[x]++;
        Gc[y]++;
    }
    if(!ret) return false;
    return checkGraph(G, v, Gc);

}

int main() {
    //pula pamieci
    int numGraph;
    FILE* pFile = fopen ("test.txt","r");
    fscanf(pFile, "%d\n", &numGraph);
//    fscanf(stdin, "%d\n", &numGraph);
    while(numGraph-- > 0) {
        if(createGraph(pFile)) {
  //      if(createGraph(stdin)) {
            fprintf(stdout, "tak\n");
        } else {
            fprintf(stdout, "nie\n");
        }
    }
    return 0;
}
/**/
