#include <cstdio>
#include <algorithm>
#include <cstring>

using namespace std;

const int maxn = 100005;

struct Node* null;

struct Node{
    Node *ch[2], *fa;
    int size;
    bool rev;
    Node(){
        ch[0] = ch[1] = fa = null;
        size = 1;
        rev = false;
    }
    inline void clear(){
        ch[0] = ch[1] = fa = null;
        size = 1;
        rev = false;
    }
    inline void push_up(){
        if(this == null)
            return;
        size = ch[0]->size + ch[1]->size + 1;
    }
    inline void setc(Node *p, int d){
        ch[d] = p;
        p->fa = this;
    }
    inline bool d(){
        return fa->ch[1]==this;
    }
    inline void update_rev(){
        if(this == null)
            return;
        swap(ch[0], ch[1]);
        rev ^= true;
    }
    inline void push_down(){
        if(this == null)
            return;
        if(rev){
            ch[0]->update_rev();
            ch[1]->update_rev();
            rev = false;
        }
    }
};

Node pool[maxn];
Node *root, *tail, *node[maxn];

inline void rotate(Node *x){
    //printf("---BEF rotate(%d)---\n", x-null);
    //for(Node *s = pool; s != tail; s++)
    //      printf("%d---%d\n", s-pool, s->size);
    Node *f = x->fa;
    Node *ff = f->fa;
//    ff->push_down();
    f->push_down();
    x->push_down();
    int c = x->d(), cc = f->d();
    x->setc(f, !c);
    f->setc(x->ch[!c], c);
    if(ff->ch[cc]==f)
        ff->setc(x, cc);
    else
        x->fa = ff;
    f->push_up();
//    printf("---END rotate(%d)---\n", x-null);
        //for(Node *s = pool; s != tail; s++)
          //      printf("%d---%d\n", s-pool, s->size);
}

void splay(Node* &root, Node* x, Node* goal){
    while(x->fa != goal){
        x->fa->fa->push_down();
        x->fa->push_down();
        x->push_down();
        if(x->fa->fa == goal)
            rotate(x);
        else{
            if(x->d() == x->fa->d())
                rotate(x->fa);
            else
                rotate(x);
            rotate(x);
        }
    }
    x->push_up();
    if(goal == null)
        root = x;
}

Node *get_kth(Node *r, int k){
    Node *x = r;
    x->push_down();
    while(x->ch[0]->size + 1 != k){
        if(x->ch[0]->size+1 > k)
            x = x->ch[0];
        else{
            k -= x->ch[0]->size+1;
            x = x->ch[1];
        }
        x->push_down();
    }
    return x;
}

void build(Node* &x, int l, int r, Node *fa){
    if(l>r)
        return;
    x = tail++;
    x->clear();
    x->fa = fa;
    int mid = (l+r)>>1;
    node[mid] = x;
    build(x->ch[0], l, mid-1, x);
    build(x->ch[1], mid+1, r, x);
    x->push_up();
}

void init(int n){
    tail = pool;
    null = tail++;
    null->clear();
    null->size = 0;
    root = tail++;
    root->clear();
    Node *p = tail++;
    p->clear();
    root->setc(p, 1);
    build(p->ch[0], 1, n, p);
    p->push_up();
    root->push_up();
    //printf("%d---\n", root->size);
}
int a[maxn], b[maxn];

bool cmp(int i, int j){
    return a[i]<a[j] || a[i]==a[j]&&i<j;
}

int main(){
    int n, ans;
    while(scanf("%d", &n)==1 && n){
        for(int i = 1; i <= n; i++){
            scanf("%d", &a[i]);
            b[i] = i;
        }
        sort(b+1, b+n+1, cmp);
        init(n);
        for(int i = 1; i <= n; i++){
            //for(Node *s = pool; s != tail; s++)
              //  printf("%d---%d\n", s-pool, s->size);
        //printf("----\n");
            splay(root, node[b[i]], null);
        //printf("----\n");
            ans = root->ch[0]->size;
            //for(Node *s = pool; s != tail; s++)
//                printf("%d---%d\n", s-pool, s->size);
            printf("%d%c", ans, ((i==n)?'\n':' '));
            splay(root, get_kth(root, i), null);
            splay(root, get_kth(root, ans+2), root);
            root->ch[1]->ch[0]->update_rev();
        }
    }
    return 0;
}
