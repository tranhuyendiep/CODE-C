#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include "AVLtree.h"
#include "printTree.h"
#include "dictionary.h"

int main()
{
	node *root=NULL;
	int n,i,op;
    word *ptr = dict;
	
    while(ptr->en)
        root=insert(root, ptr++);

    print_ascii_tree(root);
/*
	do
	{
		printf("\n1.Create:");
		printf("\n2.Insert:");
		printf("\n3.Delete:");
		printf("\n4.Print:");
		printf("\n5.Find");
		printf("\n6.Quit:");
		printf("\n\nEnter Your Choice:");
		scanf("%d",&op);
		
		switch(op)
		{
			case 1: printf("\nEnter no. of elements:");
					scanf("%d",&n);
					printf("\nEnter tree data:");
					root=NULL;
					for(i=0;i<n;i++)
					{
						scanf("%d",&x);
						root=insert(root,x);
					}
					break;
				
			case 2: printf("\nEnter a data:");
					scanf("%d",&x);
					root=insert(root,x);
					break;
					
			case 3: printf("\nEnter a data:");
					scanf("%d",&x);
					root=Delete(root,x);
					break;
			
			case 4: printf("\nPreorder sequence:\n");
					preorder(root);
//					printf("\n\nInorder sequence:\n");
//					inorder(root);
					printf("\n");
					break;
			case 5: printf("\nEnter a data you want find:");
					scanf("%d",&x);
					 	if (find(root,x))
							printf("FOUND");
						else
							printf("NOT FOUND");
					break;		
		}
	}while(op!=6);
*/
	return 0;
}
 
node * insert(node *T, word *x)
{
	if(T==NULL)
	{
		T=(node*)malloc(sizeof(node));
		T->data=x;
		T->left=NULL;
		T->right=NULL;
	}
	else if(0 < strcmp(x->en, T->data->en))		// insert in right subtree
	{
		T->right=insert(T->right,x);
		if(BF(T)==-2)
			if(0 < strcmp(x->en, T->right->data->en))
				T=RR(T);
			else
				T=RL(T);
	}
	else if(0 > strcmp(x->en, T->data->en))
	{
		T->left=insert(T->left,x);
		if(BF(T)==2)
			if(0 > strcmp(x->en, T->left->data->en))
				T=LL(T);
			else
				T=LR(T);
	}
	
	T->ht=height(T);
		
	return(T);
}
 
node * Delete(node *T,word x)
{
	node *p;
	
	if(T==NULL)
	{
		return NULL;
	}
	else
        if(0 < strcmp(x.en, T->data->en))
		{
			T->right=Delete(T->right,x);
			if(BF(T)==2)
				if(BF(T->left)>=0)
					T=LL(T);
				else
					T=LR(T);
		}
		else
            if(0 > strcmp(x.en, T->data->en))
			{
				T->left=Delete(T->left,x);
				if(BF(T)==-2)	//Rebalance during windup
					if(BF(T->right)<=0)
						T=RR(T);
					else
						T=RL(T);
			}
			else
			{
				//data to be deleted is found
				if(T->right!=NULL)
				{	//delete its inorder succesor
					p=T->right;
					
					while(p->left!= NULL)
						p=p->left;
					
					T->data=p->data;
					T->right=Delete(T->right,*(p->data));
					
					if(BF(T)==2)//Rebalance during windup
						if(BF(T->left)>=0)
							T=LL(T);
						else
							T=LR(T);\
				}
				else
					return(T->left);
			}
	T->ht=height(T);
	return(T);
}
bool find(node *T, word x)
{
	if(T==NULL) return 0;
	if(0 == strcmp(T->data->en, x.en))return 1;
	bool res1 ;
	bool res2;
	if (0 < strcmp(x.en, T->data->en)){
	res1 = find(T->left, x);
	// node found, no need to look further
	if(res1==1) return true;}
	/* node is not found in left,
	so recur on right subtree */
	else 
	res2 = find(T->right, x);

	return res2;
	 
}
 
int height(node *T)
{
	int lh,rh;
	if(T==NULL)
		return(0);
	
	if(T->left==NULL)
		lh=0;
	else
		lh=1+T->left->ht;
		
	if(T->right==NULL)
		rh=0;
	else
		rh=1+T->right->ht;
	
	if(lh>rh)
		return(lh);
	
	return(rh);
}
 
node * rotateright(node *x)
{
	node *y;
	y=x->left;
	x->left=y->right;
	y->right=x;
	x->ht=height(x);
	y->ht=height(y);
	return(y);
}
 
node * rotateleft(node *x)
{
	node *y;
	y=x->right;
	x->right=y->left;
	y->left=x;
	x->ht=height(x);
	y->ht=height(y);
	
	return(y);
}
 
node * RR(node *T)
{
	T=rotateleft(T);
	return(T);
}
 
node * LL(node *T)
{
	T=rotateright(T);
	return(T);
}
 
node * LR(node *T)
{
	T->left=rotateleft(T->left);
	T=rotateright(T);
	
	return(T);
}
 
node * RL(node *T)
{
	T->right=rotateright(T->right);
	T=rotateleft(T);
	return(T);
}
 
int BF(node *T)
{
	int lh,rh;
	if(T==NULL)
		return(0);
 
	if(T->left==NULL)
		lh=0;
	else
		lh=1+T->left->ht;
 
	if(T->right==NULL)
		rh=0;
	else
		rh=1+T->right->ht;
 
	return(lh-rh);
}
 
void preorder(node *T)
{
	if(T!=NULL)
	{
		printf("%s(Bf=%d)",T->data->en,BF(T));
		preorder(T->left);
		preorder(T->right);
	}
}
 
void inorder(node *T)
{
	if(T!=NULL)
	{
		inorder(T->left);
		printf("%s(Bf=%d)",T->data->en,BF(T));
		inorder(T->right);
	}
}

 
