
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "AVLtree.h"

typedef node Tree;
//printing tree in ascii

typedef struct asciinode_ascii_struct asciinode_ascii;

struct asciinode_ascii_struct
{
  asciinode_ascii * left, * right;

  //length of the edge from this node_ascii to its children
  int edge_length; 
    
  int height;      

  int lablen;

  //-1=I am left, 0=I am root, 1=right   
  int parent_dir;   
                         
  char label[32];  
};


#define MAX_HEIGHT 1000
int lprofile[MAX_HEIGHT];
int rprofile[MAX_HEIGHT];
#define INFINITY (1<<20)

//adjust gap between left and right node_asciis
int gap = 3;  

//used for printing next node_ascii in the same level, 
//this is the x coordinate of the next char printed
int print_next;    

int MIN (int X, int Y)  
{
  return ((X) < (Y)) ? (X) : (Y);
}

int MAX (int X, int Y)  
{
  return ((X) > (Y)) ? (X) : (Y);
}

asciinode_ascii * build_ascii_tree_recursive(Tree * t) 
{
  asciinode_ascii * node_ascii;
  
  if (t == NULL) return NULL;

  node_ascii = malloc(sizeof(asciinode_ascii));
  node_ascii->left = build_ascii_tree_recursive(t->left);
  node_ascii->right = build_ascii_tree_recursive(t->right);
  
  if (node_ascii->left != NULL) 
  {
    node_ascii->left->parent_dir = -1;
  }

  if (node_ascii->right != NULL) 
  {
    node_ascii->right->parent_dir = 1;
  }

  sprintf(node_ascii->label, "(%d)%s", t->data->en[0], t->data->en);
  node_ascii->lablen = strlen(node_ascii->label);

  return node_ascii;
}


//Copy the tree into the ascii node_ascii structre
asciinode_ascii * build_ascii_tree(Tree * t) 
{
  asciinode_ascii *node_ascii;
  if (t == NULL) return NULL;
  node_ascii = build_ascii_tree_recursive(t);
  node_ascii->parent_dir = 0;
  return node_ascii;
}

//Free all the node_asciis of the given tree
void free_ascii_tree(asciinode_ascii *node_ascii) 
{
  if (node_ascii == NULL) return;
  free_ascii_tree(node_ascii->left);
  free_ascii_tree(node_ascii->right);
  free(node_ascii);
}

//The following function fills in the lprofile array for the given tree.
//It assumes that the center of the label of the root of this tree
//is located at a position (x,y).  It assumes that the edge_length
//fields have been computed for this tree.
void compute_lprofile(asciinode_ascii *node_ascii, int x, int y) 
{
  int i, isleft;
  if (node_ascii == NULL) return;
  isleft = (node_ascii->parent_dir == -1);
  lprofile[y] = MIN(lprofile[y], x-((node_ascii->lablen-isleft)/2));
  if (node_ascii->left != NULL) 
  {
	  for (i=1; i <= node_ascii->edge_length && y+i < MAX_HEIGHT; i++) 
    {
	    lprofile[y+i] = MIN(lprofile[y+i], x-i);
    }
  }
  compute_lprofile(node_ascii->left, x-node_ascii->edge_length-1, y+node_ascii->edge_length+1);
  compute_lprofile(node_ascii->right, x+node_ascii->edge_length+1, y+node_ascii->edge_length+1);
}

void compute_rprofile(asciinode_ascii *node_ascii, int x, int y) 
{
  int i, notleft;
  if (node_ascii == NULL) return;
  notleft = (node_ascii->parent_dir != -1);
  rprofile[y] = MAX(rprofile[y], x+((node_ascii->lablen-notleft)/2));
  if (node_ascii->right != NULL) 
  {
	  for (i=1; i <= node_ascii->edge_length && y+i < MAX_HEIGHT; i++) 
    {
	    rprofile[y+i] = MAX(rprofile[y+i], x+i);
    }
  }
  compute_rprofile(node_ascii->left, x-node_ascii->edge_length-1, y+node_ascii->edge_length+1);
  compute_rprofile(node_ascii->right, x+node_ascii->edge_length+1, y+node_ascii->edge_length+1);
}

//This function fills in the edge_length and 
//height fields of the specified tree
void compute_edge_lengths(asciinode_ascii *node_ascii) 
{
  int h, hmin, i, delta;
  if (node_ascii == NULL) return;
  compute_edge_lengths(node_ascii->left);
  compute_edge_lengths(node_ascii->right);

  /* first fill in the edge_length of node_ascii */
  if (node_ascii->right == NULL && node_ascii->left == NULL) 
  {
	  node_ascii->edge_length = 0;
  } 
  else 
  {
    if (node_ascii->left != NULL) 
    {
	    for (i=0; i<node_ascii->left->height && i < MAX_HEIGHT; i++) 
      {
		    rprofile[i] = -INFINITY;
	    }
	    compute_rprofile(node_ascii->left, 0, 0);
	    hmin = node_ascii->left->height;
    } 
    else 
    {
	    hmin = 0;
    }
	  if (node_ascii->right != NULL) 
    {
	    for (i=0; i<node_ascii->right->height && i < MAX_HEIGHT; i++) 
      {
		    lprofile[i] = INFINITY;
	    }
	    compute_lprofile(node_ascii->right, 0, 0);
	    hmin = MIN(node_ascii->right->height, hmin);
    } 
    else 
    {
	    hmin = 0;
    }
	  delta = 4;
	  for (i=0; i<hmin; i++) 
    {
	    delta = MAX(delta, gap + 1 + rprofile[i] - lprofile[i]);
    }
	  
    //If the node_ascii has two children of height 1, then we allow the
    //two leaves to be within 1, instead of 2 
	  if (((node_ascii->left != NULL && node_ascii->left->height == 1) ||
	      (node_ascii->right != NULL && node_ascii->right->height == 1))&&delta>4) 
    {
      delta--;
    }
	    
    node_ascii->edge_length = ((delta+1)/2) - 1;
  }

  //now fill in the height of node_ascii
  h = 1;
  if (node_ascii->left != NULL) 
  {
	  h = MAX(node_ascii->left->height + node_ascii->edge_length + 1, h);
  }
  if (node_ascii->right != NULL) 
  {
	  h = MAX(node_ascii->right->height + node_ascii->edge_length + 1, h);
  }
  node_ascii->height = h;
}

//This function prints the given level of the given tree, assuming
//that the node_ascii has the given x cordinate.
void print_level(asciinode_ascii *node_ascii, int x, int level) 
{
  int i, isleft;
  if (node_ascii == NULL) return;
  isleft = (node_ascii->parent_dir == -1);
  if (level == 0) 
  {
	  for (i=0; i<(x-print_next-((node_ascii->lablen-isleft)/2)); i++) 
    {
	    printf(" ");
    }
	  print_next += i;
	  printf("%s", node_ascii->label);
	  print_next += node_ascii->lablen;
  } 
  else if (node_ascii->edge_length >= level) 
  {
	  if (node_ascii->left != NULL) 
    {
	    for (i=0; i<(x-print_next-(level)); i++) 
      {
		    printf(" ");
	    }
	    print_next += i;
	    printf("/");
	    print_next++;
    }
	  if (node_ascii->right != NULL) 
    {
	    for (i=0; i<(x-print_next+(level)); i++) 
      {
		    printf(" ");
	    }
	    print_next += i;
	    printf("\\");
	    print_next++;
    }
  } 
  else 
  {
	  print_level(node_ascii->left, 
                x-node_ascii->edge_length-1, 
                level-node_ascii->edge_length-1);
	  print_level(node_ascii->right, 
                x+node_ascii->edge_length+1, 
                level-node_ascii->edge_length-1);
  }
}

//prints ascii tree for given Tree structure
void print_ascii_tree(Tree * t) 
{
  asciinode_ascii *proot;
  int xmin, i;
  if (t == NULL) return;
  proot = build_ascii_tree(t);
  compute_edge_lengths(proot);
  for (i=0; i<proot->height && i < MAX_HEIGHT; i++) 
  {
	  lprofile[i] = INFINITY;
  }
  compute_lprofile(proot, 0, 0);
  xmin = 0;
  for (i = 0; i < proot->height && i < MAX_HEIGHT; i++) 
  {
	  xmin = MIN(xmin, lprofile[i]);
  }
  for (i = 0; i < proot->height; i++) 
  {
	  print_next = 0;
	  print_level(proot, -xmin, i);
	  printf("\n");
  }
  if (proot->height >= MAX_HEIGHT) 
  {
	  printf("(This tree is taller than %d, and may be drawn incorrectly.)\n", MAX_HEIGHT);
  }
  free_ascii_tree(proot); 
}

/*
 * Eg:
struct Tree 
{
  Tree * left, * right;
  int element;
};
  Tree * root = NULL;
  ...
  print_ascii_tree(root);
*/

