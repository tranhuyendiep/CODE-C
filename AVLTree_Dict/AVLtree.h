#ifndef XXX
#define XXX


typedef struct word
{
    char *en;
    char *vn;
}word;

typedef struct node
{
	word *data;
	struct node *left,*right;
	int ht;
}node;
 
node *insert(node *,word *);
node *Delete(node *,word);
bool find(node *, word);
void preorder(node *);
void inorder(node *);
int height( node *);
node *rotateright(node *);
node *rotateleft(node *);
node *RR(node *);
node *LL(node *);
node *LR(node *);
node *RL(node *);
int BF(node *);

#endif
