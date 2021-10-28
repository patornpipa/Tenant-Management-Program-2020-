#include<stdio.h> 
#include<stdlib.h> 
#include<string.h>


#define SIZE 53

int max(int a, int b);
char roomtype[10];
struct infomember *hashdata[SIZE];

struct node 
{
	struct infomember *member;
    struct node *left, *right; 
    int height; 
}; 

struct infomember
{
	char fname[30], lname[30], phone[10];
	int nameroom, roomtype, pricerent, powerprice, waterprice; 
	int waterunit, powerunit, totalpower, totalwater, totalprice;	
};

int height(struct node *n) 
{ 
    if (n == NULL) 
        return 0; 
    return n->height; 
} 

int max(int a, int b) 
{ 
    return (a > b)? a : b; 
} 

struct node* newnode(struct infomember *key)
{ 
    struct node* node = (struct node*) malloc(sizeof(struct node));
    node->member = key; 
    node->left   = NULL; 
    node->right  = NULL; 
    node->height = 1;  // new node is initially added at leaf 
    return(node); 
} 

struct node *rightrotate(struct node *y) 
{ 
    struct node *x = y->left; 
    struct node *t2 = x->right; 
    
    x->right = y; 
    y->left = t2; 
    
    x->height = max(height(x->left), height(x->right))+1; 
    y->height = max(height(y->left), height(y->right))+1; 
    
    return x; 
} 


struct node *leftrotate(struct node *x) 
{ 
    struct node *y = x->right; 
    struct node *t2 = y->left; 
    
    y->left = x; 
    x->right = t2; 

    x->height = max(height(x->left), height(x->right))+1; 
    y->height = max(height(y->left), height(y->right))+1; 

    return y; 
} 
  

int getbalance(struct node *n) 
{ 
    if (n == NULL) 
        return 0; 
    return height(n->left) - height(n->right); 
} 
  
struct node* insert(struct node* node, struct infomember *key) 
{ 
    if (node == NULL) 
        return(newnode(key)); 
  
    if (key->nameroom < node->member->nameroom) 
        node->left  = insert(node->left, key); 
    else if (key->nameroom > node->member->nameroom) 
        node->right = insert(node->right, key); 
    else // Equal keys not allowed 
    {
    	printf("\tDuplicate nameroom!!!!!!\n");
    	return node;
	}
         
  
    node->height = 1 + max(height(node->left), 
                           height(node->right)); 
  

    int balance = getbalance(node); 
  
    // Left Left Case 
    if (balance > 1 && key->nameroom < node->left->member->nameroom) 
        return rightrotate(node); 
  
    // Right Right Case 
    if (balance < -1 && key->nameroom > node->right->member->nameroom) 
        return leftrotate(node); 
  
    // Left Right Case 
    if (balance > 1 && key->nameroom > node->left->member->nameroom) 
    { 
        node->left =  leftrotate(node->left); 
        return rightrotate(node); 
    } 
  
    // Right Left Case 
    if (balance < -1 && key->nameroom < node->right->member->nameroom) 
    { 
        node->right = rightrotate(node->right); 
        return leftrotate(node); 
    } 
  
    /* return the (unchanged) node pointer */
    return node; 
} 

struct node * minvaluenode(struct node* node) 
{ 
    struct node* current = node; 
  
    /* loop down to find the leftmost leaf */
    while (current->left != NULL) 
        current = current->left; 
  
    return current; 
} 

struct node* deletenode(struct node* root, int roomname, int *check) 
{ 
    if (root == NULL) 
        return root;
  
    // If the key to be deleted is smaller than the 
    // root's key, then it lies in left subtree 
    if ( roomname < root->member->nameroom)
        root->left = deletenode(root->left, roomname, &*check); 
  
    // If the key to be deleted is greater than the 
    // root's key, then it lies in right subtree 
    else if( roomname > root->member->nameroom ) 
        root->right = deletenode(root->right, roomname, &*check); 
  
    // if key is same as root's key, then This is 
    // the node to be deleted 
    else
    { 
        // node with only one child or no child 
        if( (root->left == NULL) || (root->right == NULL) ) 
        { 
            struct node *temp = root->left ? root->left : root->right; 
  
            // No child case 
            if (temp == NULL) 
            { 
                temp = root; 
                root = NULL; 
            } 
            else // One child case 
             *root = *temp; // Copy the contents of 
                            // the non-empty child 
            free(temp); 
        } 
        else
        { 
            // node with two children: Get the inorder 
            // successor (smallest in the right subtree) 
            struct node* temp = minvaluenode(root->right); 
  
            // Copy the inorder successor's data to this node 
            root->member = temp->member; 
  
            // Delete the inorder successor 
            root->right = deletenode(root->right, temp->member->nameroom, &*check); 
        }
		*check = 1; 
    } 
  
    // If the tree had only one node then return 
    if (root == NULL) 
      return root; 
  
    // STEP 2: UPDATE HEIGHT OF THE CURRENT NODE 
    root->height = 1 + max(height(root->left), 
                           height(root->right)); 
  
    // STEP 3: GET THE BALANCE FACTOR OF THIS NODE (to 
    // check whether this node became unbalanced) 
    int balance = getbalance(root); 
  
    // If this node becomes unbalanced, then there are 4 cases 
  
    // Left Left Case 
    if (balance > 1 && getbalance(root->left) >= 0) 
        return rightrotate(root); 
  
    // Left Right Case 
    if (balance > 1 && getbalance(root->left) < 0) 
    { 
        root->left =  leftrotate(root->left); 
        return rightrotate(root); 
    } 
  
    // Right Right Case 
    if (balance < -1 && getbalance(root->right) <= 0) 
        return leftrotate(root); 
  
    // Right Left Case 
    if (balance < -1 && getbalance(root->right) > 0) 
    { 
        root->right = rightrotate(root->right); 
        return leftrotate(root); 
    } 
  
    return root; 
} 

struct node* update(struct node* root, int target, int *check)
{
	if (root == NULL) 
        return root;
    if ( target < root->member->nameroom)
        root->left = update(root->left,target,&*check); 
    else if( target > root->member->nameroom ) 
        root->right = update(root->right,target,&*check); 
	else if(target == root->member->nameroom)
	{
		printf("\tUpdate info Tenant member\n");
		printf("\t=======================================================\n");
		printf("\tNameRoom Tenant : %d\n",root->member->nameroom);
		printf("\tEnter Unit of Power : ");
		scanf("%d",&root->member->powerunit);
		printf("\tEnter Unit of water : ");
		scanf("%d",&root->member->waterunit);
		printf("\t=======================================================\n");
		//calculate
		root->member->totalpower = root->member->powerprice * root->member->powerunit;
		root->member->totalwater = root->member->waterprice * root->member->waterunit;
		root->member->totalprice = root->member->totalpower + root->member->totalwater + root->member->pricerent;
		*check = 1;
	}
	return root;
}
struct node* printinv(struct node* root, int target, int *check)
{

	if (root == NULL) 
        return root;
    if ( target < root->member->nameroom)
        root->left = printinv(root->left,target,&*check); 
    else if( target > root->member->nameroom ) 
        root->right = printinv(root->right,target,&*check); 
	else if(target == root->member->nameroom)
	{
		printf("\tPrint an invoice Tenant member\n");
		printf("\t=======================================================\n");
		printf("\tNameRoom Tenant : %d\n",root->member->nameroom);
		printf("\tTenant name : %s %s\n",root->member->fname,root->member->lname);
		printf("\tTenant phone : %s \n",root->member->phone);
		if(root->member->roomtype == 1)
			strcpy(roomtype, "Normal");
		else
			strcpy(roomtype, "Suite");
		printf("\tRoom type : %s\n",roomtype);
		printf("\t-------------------------------------------------------\n");
		printf("\tRoom Cost = %d\n",root->member->pricerent);
		printf("\t Power cost (Powerprice x Powerunit) :%d x %d = %d \n",root->member->powerprice,root->member->powerunit,
		root->member->totalpower);
		printf("\t Water cost (Waterprice x Waterunit) :%d x %d = %d \n",root->member->waterprice,root->member->waterunit,
		root->member->totalwater);
		printf("\t-------------------------------------------------------\n");
		printf("\tTotal = %d\n",root->member->totalprice);
		*check = 1;
	}
	return root;
}

void inorder(struct node *root) 
{ 
    if(root != NULL) 
    { 
        inorder(root->left); 
        printf("%d\n", root->member->nameroom); 
        inorder(root->right); 
    } 
} 




int printmenu()
{
	int select;
	printf("\t=======================================================\n");
	printf("\tTenant Management Program\n"
			"\t\t1.Add Tenant member\n"
			"\t\t2.delete Tenant member\n"			
			"\t\t3.Update power - water unit Tenant member\n"
			"\t\t4.Print an invoice Tenant member\n"
			"\t\t5.Serach Info Tenant member\n"
			"\t\t0.Exit Program\n"
			"\t=======================================================\n"
			"\tselect: ");

	scanf("%d",&select);
	return select;
}

/*struct infomember* readtxt()
{
	FILE *info;
	info = fopen("data.txt","r");
	while(!feof(info))
	{
		struct infomember* node = (struct infomember*) malloc(sizeof(struct infomember));
		fscanf(info,"%d%s%s%s%d",&node->nameroom, &node->fname, &node->lname, &node->phone, &node->roomtype);
		
		
		root = insert(node, node->nameroom);
	}
	
}*/

struct infomember* addtenant()
{	
	struct infomember* node = (struct infomember*) malloc(sizeof(struct infomember));
	printf("\t Add Tenant member\n");
	printf("\t=======================================================\n");
	printf("\tNameRoom Tenant (1-50): ");
	scanf("%d",&node->nameroom);
	printf("\t=======================================================\n");
	if(node->nameroom >= 1 && node->nameroom <= 50)
	{
		printf("\tFirst name Tenant : ");
		scanf("%s",&node->fname);
		printf("\tLast name Tenant : ");
		scanf("%s",&node->lname);
		printf("\tPhone number Tenant : ");
		scanf("%s",&node->phone);
		printf("\tselect Room type [1.normal or 2.Suite] : ");
		scanf("%d",&node->roomtype);
		
		node->totalpower = 0;
		node->totalwater = 0;
		node->totalprice = 0;
		node->waterunit = 0;
		node->powerunit = 0;
		
		if(node->roomtype == 1)
		{
			node->pricerent = 2500;
			node->powerprice = 4;
			node->waterprice = 3;
		}
		else if(node->roomtype == 2)
		{
			node->pricerent = 3500;
			node->powerprice = 5;
			node->waterprice = 4;
		}
		else
		{
			printf("\ttry again, please enter roomtype 1 or 2\n");
			addtenant();	
		}
		printf("\t=======================================================\n");
		printf("\tAdd tenant complete!!!\n");
		return(node);
	}
	else
	{
		printf("\ttry again, please enter nameroom 1-50\n");
		printf("\t=======================================================\n");
		addtenant();
	}	
}
//----------------------------------------------------- Hashing---------------------------------------------------

void intitial()
{
	int i;
	for(i = 0; i < SIZE; i++)
	{
		hashdata[i] = (struct infomember*)malloc(sizeof(struct infomember));
		hashdata[i]->nameroom = -1;
	}
		
}

void inserthash(struct infomember* newdata)
{
	int key = newdata->nameroom % SIZE;
	if(hashdata[key]->nameroom == -1)
		hashdata[key] = newdata;
	else
	{
		++key;
		inserthash(newdata);
	}		
}



void readtxt()
{
	char del[100];
	FILE *info;
	info = fopen("data.txt","r");
	struct infomember* newdata;

	while(!feof(info))
	{
		newdata = (struct infomember*)malloc(sizeof(struct infomember));
		fscanf(info,"%d%s%s%s",&newdata->nameroom, &newdata->fname, 
		&newdata->lname, &newdata->phone);
		fscanf(info,"%d%d%d%d",&newdata->roomtype, &newdata->pricerent, 
		&newdata->powerprice, &newdata->waterprice);
		fscanf(info,"%d%d%d%d%d",&newdata->waterunit, &newdata->powerunit, 
		&newdata->totalpower, &newdata->totalwater, &newdata->totalprice);
		inserthash(newdata);				
	}
	fclose(info);
}

void printtxt(struct node *root,FILE *data)
{
	if(root != NULL)
	{
		printtxt(root->left,data);
		fprintf(data,"%d %s %s %s %d %d %d %d %d %d %d %d %d\n",root->member->nameroom, 
		root->member->fname, root->member->lname, root->member->phone, root->member->roomtype, 
		root->member->pricerent, root->member->powerprice, root->member->waterprice, 
		root->member->waterunit, root->member->powerunit, root->member->totalpower, 
		root->member->totalwater, root->member->totalprice);
		printtxt(root->right,data);
	}		
}

int serachhash(int target)
{
	int key = target % SIZE;
	while(hashdata[key]->nameroom != -1)
	{
		if(hashdata[key]->nameroom == target)
			return key;
		else
			key++;
	}
	return -1;
	
}

void printhash(int key)
{
	printf("\tNameRoom Tenant : %d\n",hashdata[key]->nameroom);
	printf("\tTenant name : %s %s\n",hashdata[key]->fname,hashdata[key]->lname);
	printf("\tTenant phone : %s \n",hashdata[key]->phone);
	if(hashdata[key]->roomtype == 1)
		strcpy(roomtype, "Normal");
	else if(hashdata[key]->roomtype == 2)
		strcpy(roomtype, "Suite");
	printf("\tRoom type : %s\n",roomtype);
	printf("\tRoom price : %d bath\n",hashdata[key]->pricerent);
	printf("\tWater price : %d bath, Power price : %d bath\n",hashdata[key]->waterprice, 
	hashdata[key]->powerprice);
	printf("\tWater unit : %d, Power unit : %d\n",hashdata[key]->waterunit, 
	hashdata[key]->powerunit);
	printf("\tWater price net : %d, Power price net : %d\n",hashdata[key]->totalwater, 
	hashdata[key]->totalpower);
	printf("\tTotal : %d\n",hashdata[key]->totalprice);					
}

void searchroom()
{
	
	int nameroom, key = -1;
	printf("\tEnter nameroom: ");
	scanf("%d",&nameroom);
	printf("\t=======================================================\n");
	key = serachhash(nameroom);
	if(key != -1)
		printhash(key);
	else
		printf("\tRoom empty\n");
	
}



/*void printhash()
{
	int i;
	for(i=0; i<SIZE; i++)
	{
		if(hashdata[i]->nameroom != -1)
		{
			printf("\tNameRoom Tenant : %d\n",hashdata[i]->nameroom);
			printf("\tTenant name : %s %s\n",hashdata[i]->fname,hashdata[i]->lname);
			printf("\tTenant phone : %s \n",hashdata[i]->phone);
			if(hashdata[i]->roomtype == 1)
				strcpy(roomtype, "Normal");
			else
				strcpy(roomtype, "Suite");
			printf("\tRoom type : %s\n",roomtype);
		}
		
	}
	
}*/
//----------------------------------------------------- Main ----------------------------------------------------------
int main() 
{
	FILE *data;
	int check = 0;
	
	int select=-1;
  struct node *root = NULL;
  while(select != 0)
  {
	  select = printmenu();
	 
	  if(select == 1)
	  {
	  		printf("\t=======================================================\n");
	  		data = fopen("data.txt","w");
	  		root = insert(root, addtenant());
			printtxt(root,data);
			fclose(data);					
	  }
	  else if(select == 2)
	  {
	  		printf("\t=======================================================\n");
	  		printf("\tDelete Tenant member \n");
	  		printf("\t=======================================================\n");
	  		data = fopen("data.txt","w");
			int nameroom;
			printf("\tNameRoom Tenant : ");
			scanf("%d",&nameroom);
			root = deletenode(root, nameroom, &check);
			printf("\t=======================================================\n");
			if(check == 0)
	  			printf("\tTarget not found !!!! \n");
	  		else
	  			printf("\tDelete complete !!!! \n");
			check = 0;
			printtxt(root,data);
			fclose(data);	
	  }
	  
	  else if(select == 3) //update
	  {
	  		printf("\t=======================================================\n");
	  		printf("\tUpdate power - water unit Tenant member\n");
	  		printf("\t=======================================================\n");
	  		data = fopen("data.txt","w");
	  		int target;
	  		printf("\tEnter room number : ");
			scanf("%d",&target);
			printf("\t=======================================================\n");
			root = update(root,target,&check);
			if(check == 0)
	  			printf("\tTarget not found !!!! \n");
	  		else
	  			printf("\tUpdate complete !!!! \n");
			check = 0;
			printtxt(root,data);
			fclose(data);			
	  }
	  else if(select == 4) //printinvoice
	  {
	  		printf("\t=======================================================\n");
	  		printf("\tPrint an invoice Tenant member\n");
	  		printf("\t=======================================================\n");
	  		int target;
	  		printf("\tEnter room number : ");
			scanf("%d",&target);
			printf("\t=======================================================\n");
	  		root = printinv(root,target,&check);
	  		if(check == 0)
	  			printf("\tTarget not found !!!! \n");
			check = 0;
	  }
	  else if(select == 5) //serach
	  {
	  		printf("\t=======================================================\n");
	  		printf("\tSerach Info Tenant member\n");
	  		printf("\t=======================================================\n");
	  		intitial();
	  		readtxt();
	  		searchroom();	
	  }
	}
	printf("\t=======================================================\n");
  	printf("\tGood bye, See you agin......\n");
  	
    return 0; 
} 

