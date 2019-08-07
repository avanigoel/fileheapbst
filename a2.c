#include<stdio.h>
#include<stdlib.h>
#include<string.h>

char* itoa(int num, char* str, int base);

struct node* S[2000];
int top = -1;

struct node
{
    struct node* lc;
    int min;
    int max;
    struct node* rc;
    FILE* fp;
    char name[50];
};

int L = 32;
int n = 0;
FILE* hinit()
{
    FILE* fp;
    char buff[100];
    itoa(n,buff,10);
    strcat(buff,".txt");
    n++;
    
    fp = fopen(buff,"w+");
    fprintf(fp,"%7d",0);
    fseek(fp,0,SEEK_SET);
    
    return fp;
}

//Get size of heap 
int getSize(FILE *fp)
{
    int size;
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%7d",&size);
    return size;
}

//update size of heap
void updateSize(FILE *fp, int size)
{
     fseek(fp,0,SEEK_SET);
     fprintf(fp,"%7d",size);
}

//remove last element (replace with space and then update the size)
void removeLast(FILE *fp)
{
    int size;
    size = getSize(fp);
    fseek(fp,8*(size),SEEK_SET);
    fprintf(fp,"%7s","");
    updateSize(fp,size-1);
}



void appendToFile(FILE *fp, int val)
{
    int size = getSize(fp);
    fseek(fp,8*(size)+7,SEEK_SET);
    fprintf(fp,"%8d",val);
    updateSize(fp,size+1);
}

void setValueAt(FILE *fp, int index, int value)
{
    //first element means index 1
     
     fseek(fp,8*index,SEEK_SET);
     fprintf(fp,"%7d",value);
}

int getValueAt(FILE *fp, int index)
{
    //first element means index 1
    int val;
    fseek(fp,8*(index),SEEK_SET);
    fscanf(fp,"%7d",&val);
    return val;
}

int hsearch(FILE* fp,int data)
{
    int size,i,no;
    fscanf(fp,"%7d",&size);
    if(size == 0)
    {
        printf("\nFile Empty!!\n");
        return 0;
    }
    else
    {
        
        for(i = 1;i <= size+1; i++)
        {
            no = getValueAt(fp, i);
            if(data == no)
            {
                printf("\nPRESENT\n",data);
                return 1;
            }
            
        }
        
        if(i == size+2)
        {
            return 0;
        }
        
    }
}


void heapify_bottom(FILE* fp)
{
    int size,parent,current,t;
    fscanf(fp,"%7d",&size);
   
    while(size/2 >= 1)
    {
        current = getValueAt(fp,size);
                
        parent = getValueAt(fp,size/2);
                
        if(current < parent)
        {
            
            fseek(fp,(size)*8,SEEK_SET);
            fprintf(fp,"%7d",parent);
            
            fseek(fp,(size/2)*8,SEEK_SET);
            fprintf(fp,"%7d",current);
            
        }
        
               
        size = size/2;
        
    }
        
}


void hinsert(FILE* fp,int data)
{
    
    appendToFile(fp, data);

    fseek(fp,0,SEEK_SET);
    heapify_bottom(fp);
            
}


int hfindmin(FILE* fp)
{
    int size,min;
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%7d",&size);
    if(size == 0)
    {
        return -1;
    }
    else
    {
        fseek(fp,8,SEEK_SET);
        fscanf(fp,"%7d",&min);
        return min;
    }
}

void heapify_root(FILE* fp)
{
    int size,parent,left,right,i=0;
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%7d",&size);
    while(2*i+1 < size)
    {
        fseek(fp,8+(i)*8,SEEK_SET);
        fscanf(fp,"%7d",&parent);

        fseek(fp,8+((2*i)+1)*8,SEEK_SET);
        fscanf(fp,"%7d",&left);
                   

        if(2*i+2 < size)
        {
            fseek(fp,8+((2*i)+2)*8,SEEK_SET);
            fscanf(fp,"%7d",&right);
                                   
            
            if(left < parent && left < right)
            {
                
                fseek(fp,8+(i)*8,SEEK_SET);

                fprintf(fp,"%7d",left);
                fseek(fp,8+((2*i)+1)*8,SEEK_SET);
                fprintf(fp,"%7d",parent);
            }
            else if(right < parent && right < left)
            {
                
                fseek(fp,8+(i)*8,SEEK_SET);
                fprintf(fp,"%7d",right);
                fseek(fp,8+((2*i)+2)*8,SEEK_SET);
                fprintf(fp,"%7d",parent);
            }
        }
        else
        {
            if(left < parent)
            {
                
                fseek(fp,8+(i)*8,SEEK_SET);
                fprintf(fp,"%7d",left);
                fseek(fp,8+((2*i)+1)*8,SEEK_SET);
                fprintf(fp,"%7d",parent);
            }
        }
                        
        i++;
    }
    

}

int hdelmin(FILE* fp)
{
    int no,size,last;
    
    size = getSize(fp);
    if(size==0)
    {
        printf("\nEmpty file!!\n");
        return -1;
    }
    
    no = getValueAt(fp, 1);

    
    last = getValueAt(fp,size);
    removeLast(fp);
    
    setValueAt(fp, 1, last);
        
    fseek(fp,0,SEEK_SET);
    heapify_root(fp);
    return no;
}

int hfindmax(FILE* fp)
{
    int size,max=0,i,no;
    fseek(fp,0,SEEK_SET);
    fscanf(fp,"%7d",&size);
    if(size==0)
    {
        printf("\nEmpty file!!\n");
        return -1;
    }
    else
    {
        for(i=1;i <= size;i++)
        {
            fseek(fp,7+(i-1)*8,SEEK_SET);
            fscanf(fp,"%7d",&no);
            if(no > max)
            {
                max = no;
            }
            
        }
        return max;
    }
}

struct node* dbinit()
{
    char buff[100];
    itoa(n,buff,10);
    strcat(buff,".txt");
    struct node* new;
    new = (struct node*)malloc(sizeof(struct node));
    new->lc=NULL;
    new->rc=NULL;
    new->min=-1;
    new->max=-1;
    new->fp=hinit();
    
    *(new->name) = *buff;
    
    return new;
}

void dbsearch(struct node* root,int data)
{
    struct node* cur = root;
    int Lmin,Lmax,Rmin,Rmax;
    while(cur->lc!=NULL && cur->rc!=NULL)
    {
        if(data < cur->min || data > cur->max)
        {
            printf("ABSENT\n");
            return;
        }
        else
        {
            Lmin = cur->lc->min;
            Rmin = cur->rc->min;
            Lmax = cur->lc->max;
            Rmax = cur->rc->max;
            
            if(data >= Lmin && data <= Lmax)
            {
                cur = cur->lc;
                
            }
            else if(data >= Rmin && data <= Rmax)
            {
                cur = cur->rc;
            }
            else if(data > Lmax && data < Rmin)
            {
                printf("ABSENT\n");
                return;
            }
        }
        
    }
    fseek(cur->fp,0,SEEK_SET);
    hsearch(cur->fp,data);
    
}
void push(struct node* x)
{
    if(top>2000)
    {
        printf("Stack Overflow!!");
        
    }
    else
    {
        S[++top]=x;
        //printf("Element pushed on the stack successfully...")
    }
}


struct node* pop()
{
    struct node* x;
    if(top==-1)
    {
        printf("Stack Underflow!!");
        
    }
    else
    {
        x=S[top--];
        //printf("Element popp on the stack successfully...")
    }
    return x;
}
void dbinsert(struct node* root,int data)
{
    
    FILE* f2;
    FILE* fp;
    struct node* cur = root;
    struct node* popped;
    struct node* left;
    struct node* right;
    int Lmax,size,i,no;
    int max=0;
    
   
    while(cur->lc!=NULL && cur->rc!=NULL)
    {
        Lmax = cur->lc->max;
        if(data <= Lmax)
        {
            push(cur);
            cur = cur->lc;
        }
        else
        {
            push(cur);
            cur = cur->rc;
        }
    }
    
    if(hsearch(cur->fp,data)==1)
    {
        printf("\nCan't insert,data already present!!\n");
    }
    else
    {
        fseek(cur->fp,0,SEEK_SET);
        fscanf(cur->fp,"%7d",&size);
        
        if(size < L)
        {
            
            fseek(cur->fp,0,SEEK_SET);
            hinsert(cur->fp,data);
           
            fseek(cur->fp,0,SEEK_SET);
            cur->min = hfindmin(cur->fp);
            cur->max = hfindmax(cur->fp);
        }
        else
        {
            f2 = hinit();
            left=(struct node*)malloc(sizeof(struct node));
            right=(struct node*)malloc(sizeof(struct node));
            cur->lc=left;
            cur->rc=right;
            fseek(cur->fp,0,SEEK_SET);
            right->fp=cur->fp;
            left->fp=f2;
            for(i=0;i<L/2;i++)
            {
               
                fseek(cur->fp,0,SEEK_SET);
                
                no=hdelmin(cur->fp);
                
                fseek(f2,0,SEEK_SET);
                hinsert(f2,no);
                
                if(no > max)
                {
                    max = no;
                }
            }

            if(data >= max)
            {
                fseek(cur->fp,0,SEEK_SET);
                hinsert(cur->fp,data);
                
        
            }
            else if(data < max)
            {
                fseek(f2,0,SEEK_SET);
                hinsert(f2,data);
               
            }

            left->min = hfindmin(f2);
            left->max = hfindmax(f2);
            right->min = hfindmin(cur->fp);
            right->max = hfindmax(cur->fp);

            cur->fp=NULL;

            cur->min = cur->lc->min;
            cur->max = cur->rc->max;

            while(top!=-1)
            {
                popped=pop();
                popped->min = popped->lc->min;
                popped->max = popped->rc->max;

            }
            
        }
    }
    
}

void inorder(struct node* root)
{
    if(root!=NULL)
    {
        inorder(root->lc);
        if(root->lc==NULL && root->rc==NULL)
        {

            printf("%d\t",root->min);
            printf("%d\t",root->max);
        }
            inorder(root->rc);
    }
}

void listing(struct node* root)
{
    if(root!=NULL)
    {
        if(root->lc!=NULL)
        {
            listing(root->lc);
        }
        
        if(root->lc==NULL && root->rc==NULL)
        {
            printf("%d\t",hfindmin(root->fp));
            printf("%d\t",hfindmax(root->fp));
        }
       
        if(root->lc!=NULL)
        {
            listing(root->rc);
        }
        

        
    }
}


void preorder(struct node* root)
{
    if(root!=NULL)
    {
       
        preorder(root->lc);
        printf("%d\t",root->min);
        preorder(root->rc);
    }
}

void postorder(struct node* root)
{
    if(root!=NULL)
    {
        postorder(root->lc);
        postorder(root->rc);
        printf("%d\t",root->max);
    }
}

int max(int a,int b)
{
    if(a>b)
    {
        return a;
    }
    else
    {
        return b;
    }
}

int height(struct node* root)
{
    if(root->lc==NULL && root->rc==NULL)
    {
        return 0;
    }
    while(root)
    {
        return 1+max(height(root->lc),height(root->rc));
    }
}

int leaves(struct node* root)
{
    if(root)
    {
        if(root->lc == NULL && root->rc == NULL)
        {
            return 1+leaves(root->lc)+leaves(root->rc);
            
        }

        else return leaves(root->lc)+leaves(root->rc);
    }
}

int nodes(struct node* root)
{
    if(root)
    {
        
            return 1+nodes(root->lc)+nodes(root->rc);
                 

     }
}

void print_BST(struct node* root)
{
    char buff[100];
    itoa(n,buff,10);
    strcat(buff,".txt");
    if(root)
    {
        printf("Range = [%d %d], ",root->min,root->max);
        printf("File = ");
        if(root->fp == NULL)
        {
            printf("None\n");
        }
        else
        {
            printf("%s\n",root->name);
        }

        print_BST(root->lc);
        print_BST(root->rc);
    }
}


int main()
{
    
    struct node* root;
    int nins,num,i,n1,n2;
    root = dbinit();
    printf("Root is %u\n",root);
    printf("\nEnter the no. of instructions\n");
    
    scanf("%d",&nins);
    
    printf("Insert keys:\n");
    
    for(i=0;i < nins;i++)
    {
        scanf("%d",&num);
        dbinsert(root,num);
        
    }
   

    printf("\n+++Inorder listing of min and max values of leaves\n");
    inorder(root);

    printf("\n+++Inorder listing of min and max values read from files\n");
    listing(root);

    printf("\n+++Sorted listing of min values at all nodes\n");
    preorder(root);

    printf("\n+++Sorted listing of max values at all nodes\n");
    postorder(root);

    printf("\n+++Statistics of the BST\n");

    printf("Number of nodes  = %d\n",nodes(root));
    printf("Number of leaves = %d\n",leaves(root));
    printf("Height           = %d\n",height(root));

    printf("\n+++ The BST\n");
    print_BST(root);

    printf("\nInput key: ");
    scanf("%d",&n1);

    printf("\nInput key: ");
    scanf("%d",&n2);

    dbsearch(root,n1);
    dbsearch(root,n2);

}



char* itoa(int num, char* str, int base)
{
    sprintf( str, "%d", num );
    return str;
}
