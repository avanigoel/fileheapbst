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
     printf("Value is %d",value);
     printf("Index is %d",index);
     fseek(fp,8*index,SEEK_SET);
     fprintf(fp,"%7d",value);
}

int getValueAt(FILE *fp, int index)
{
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
                printf("\n%d Data found!!\n",data);
                return 1;
            }
            
        }
        
        if(i == size+2)
        {
            printf("\n%d Data not found!!\n",data);
            return 0;
        }
        
    }
}


void heapify_bottom(FILE* fp)
{
    int size,parent,current,t;
    fscanf(fp,"%7d",&size);
    printf("\nSize is %d\n",size );
    while(size/2 >= 1)
    {
        
        //fseek(fp,(size)*8,SEEK_SET);
        //fscanf(fp,"%7d",&current);
        current = getValueAt(fp,size);
        printf("\nCurrent is %d\n",current);
        //fseek(fp,(size/2)*8,SEEK_SET);
        //fscanf(fp,"%7d",&parent);
        parent = getValueAt(fp,size/2);
        printf("\nParent is %d\n",parent);
        
        if(current < parent)
        {
            /*t = current;
             current = parent;
             parent = t;*/
            
            //printf("size is %d\n",size);
            fseek(fp,(size)*8,SEEK_SET);
            fprintf(fp,"%7d",parent);
            //setValueAt(fp, size, parent);
            //printf("%d\n",current);
            //setValueAt(fp, size/2, current);

        fseek(fp,(size/2)*8,SEEK_SET);
            fprintf(fp,"%7d",current);


        printf("size/2 is %d",size/2);

        }
        
        /*printf("\nAfter swapping\n");
         printf("\nCurrent is %d\n",current);
         printf("\nParent is %d\n",parent);
         
         printf("sizev is %d\n",size);
         fseek(fp,(size-1)*8+7,SEEK_SET);
         fprintf(fp,"%8d",current);
         printf("%d\n",current);
         
         fseek(fp,7+((size/2)-1)*8,SEEK_SET);
         fprintf(fp,"%8d",parent);*/
        
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

        //parent = getValueAt(fp,i);

        fseek(fp,8+((2*i)+1)*8,SEEK_SET);
        fscanf(fp,"%7d",&left);
        //left = getValueAt(fp,(2*i)+1);

        printf("parent is %d\n",parent);
        printf("left is %d\n",left);
        

        if(2*i+2 < size)
        {
            fseek(fp,8+((2*i)+2)*8,SEEK_SET);
            fscanf(fp,"%7d",&right);
            
            //right = getValueAt(fp,(2*i)+2);
            
            printf("right is %d\n",right);


            if(left < parent && left < right)
            {
                printf("left smallest %d",left);
                fseek(fp,8+(i)*8,SEEK_SET);

                fprintf(fp,"%7d",left);
                fseek(fp,8+((2*i)+1)*8,SEEK_SET);
                fprintf(fp,"%7d",parent);
            }
            else if(right < parent && right < left)
            {
                printf("right smallest %d",right);
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
                printf("left smallest %d",left);
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
    //fseek(fp,0,SEEK_SET);
    //fscanf(fp,"%7d",&size);
    size = getSize(fp);
    if(size==0)
    {
        printf("\nEmpty file!!\n");
        return -1;
    }
    //fseek(fp,8,SEEK_SET);
    //fscanf(fp,"%7d",&no);
    no = getValueAt(fp, 1);

    printf("removing %d\n",no);

    //fseek(fp,8*size,SEEK_SET);
    /*fseek(fp,-7,SEEK_END);*/
    //fscanf(fp,"%7d",&last);
    last = getValueAt(fp,size);
    removeLast(fp);
    
   /* fseek(fp,7,SEEK_SET);
    fprintf(fp,"%8d",last);*/
    
    setValueAt(fp, 1, last);
    /*fseek(fp,0,SEEK_SET);
    fprintf(fp,"%7d",size-1);*/
    
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
    struct node* new;
    new = (struct node*)malloc(sizeof(struct node));
    new->lc=NULL;
    new->rc=NULL;
    new->min=-1;
    new->max=-1;
    new->fp=hinit();

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
            printf("Data not present\n");
            break;
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
                printf("Data not present\n");
                break;
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
    //Lmax = cur->lc->max;
    printf("cyvs\n");
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
        //fseek(cur->fp,1,SEEK_CUR);
        if(size < L)
        {
            printf("fiikn");
            fseek(cur->fp,0,SEEK_SET);
            hinsert(cur->fp,data);
            printf("ffgfg");
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
                printf("ddss");
                no=hdelmin(cur->fp);
                printf("ttrt");
                fseek(f2,0,SEEK_SET);
                hinsert(f2,no);
                printf("sdd\n");
                if(no > max)
                {
                    max = no;
                }
            }

            if(data >= max)
            {
                fseek(cur->fp,0,SEEK_SET);
                hinsert(cur->fp,data);
                /*popped=pop();
                fseek(cur->fp,0,SEEK_SET);
                popped->min = hfindmin(cur->fp);
                fseek(cur->fp,0,SEEK_SET);
                popped->max = hfindmax(cur->fp);*/
        
            }
            else if(data < max)
            {
                fseek(f2,0,SEEK_SET);
                hinsert(f2,data);
               /* popped=pop();
                fseek(f2,0,SEEK_SET);
                popped->min = hfindmin(f2);
                fseek(f2,0,SEEK_SET);
                popped->max = hfindmax(f2);*/
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
            //update(root,data);
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
        /*printf("%d\t",hfindmin(root->fp));
        printf("%d\t",hfindmax(root->fp));*/
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
        //printf("%d\t",root->min);
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
void statistics(struct node* root)
{
    int c=0,h,l=0;
    //h = height(root);
    //printf("Height is %d\n",h);
    if(root)
    {
        if(root->lc == NULL && root->rc == NULL)
        {
            l++;
            c++;
        }
        else
        {
            c++;
        }
        statistics( root->lc);
        statistics( root->rc);

    }

    
    printf("Nodes is %d\n",c);
    printf("Leaves is %d\n",l);

}

void main()
{
    /*FILE* f1;
    FILE* f2;
    f1=hinit();
    f2=hinit();
    hinsert(f1,90);
    hinsert(f1,78);
    hinsert(f1,45);
    hinsert(f1,23);
    //heapify_bottom(f1);
    hinsert(f2,80);
    hinsert(f2,70);
    hinsert(f2,67);
    hinsert(f2,57);
    hinsert(f2,43);
    hsearch(f1,78);
    hsearch(f2,80);
    hsearch(f2,70);
    hsearch(f2,57);
    printf("\nmin1 is %d",hfindmin(f1));
    printf("\nmin2 is %d",hfindmin(f2));
    printf("\nDeleted1 is %d",hdelmin(f1));
    printf("\nDeleted2 is %d",hdelmin(f2));
    printf("\nMax1 is %d",hfindmax(f1));
    printf("\nMax1 is %d",hfindmax(f2));*/

    struct node* root;
    int nins,num,i,n1,n2;
    root = dbinit();
    printf("Root is %u\n",root);
    printf("\nEnter the no. of instructions\n");
    
    scanf("%d",&nins);
    //nins = 14;
    //int data[14] = {11, 19, 21, 36, 43, 41, 45, 47, 60, 64, 71, 89, 75, 37};
    printf("Insert keys:\n");
    //int data[200];
    for(i=0;i < nins;i++)
    {
        scanf("%d",&num);
        dbinsert(root,num);
        //dbinsert(root,i);
    }
   

    printf("\nInorder listing of min and max values of leaves\n");
    inorder(root);

    printf("\n Inorder listing of min and max values read from files\n");
    listing(root);

    printf("\nSorted listing of min values at all nodes\n");
    preorder(root);

    printf("\nSorted listing of max values at all nodes\n");
    postorder(root);

    printf("\nStatistics of the BST\n");
    printf("The no of leaves are %d\n",leaves(root));
    printf("The no of nodes are %d\n",nodes(root));
    printf("The height is %d\n",height(root));

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
