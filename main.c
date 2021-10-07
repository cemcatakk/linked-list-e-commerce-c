#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct product{
  int id;
  char name[20];
  char category[20];
  int price;
  struct product *next;
}Product;

typedef struct basket{
int id;
int amount;
Product *productlist;
struct basket* next;
}Basket;

typedef struct customer{
  int id;
  char name[20];
  char surname[20];
struct customer*next;
Basket* basketlist;
}Customer;

Product* allocProduct(int id,char name[20],char category[20],int price){
  Product* newProduct = (Product*)malloc(sizeof(Product));
  newProduct->id=id;
  strcpy(newProduct->name,name);
  strcpy(newProduct->category,category);
  newProduct->price=price;
  newProduct->next=NULL;
  return newProduct;
}

Basket* allocBasket(int id,int amount){
  Basket* newBasket = (Basket*)malloc(sizeof(Basket));
  newBasket->id=id;
  newBasket->amount=amount;
  newBasket->next=NULL;
  newBasket->productlist=NULL;
  return newBasket;
}

Customer* allocCustomer(int id,char name[20],char surname[20]){
  Customer* newCustomer = (Customer*) malloc(sizeof(Customer));
  newCustomer->id=id;
  strcpy(newCustomer->name,name);
  strcpy(newCustomer->surname,surname);
  newCustomer->basketlist=NULL;
  newCustomer->next=NULL;
  return newCustomer;
}
Customer* addCustomerToEnd(Customer* head_ref, Customer* new_node)
{
    Customer *last = head_ref;  
    new_node->next = NULL;
    if (head_ref == NULL)
    {
       head_ref = new_node;
       return head_ref;
    }  
    while (last->next != NULL){
      last = last->next;
    }
    last->next = new_node;
    return head_ref;    
}
Basket* addBasketToEnd(Basket* head_ref, Basket* new_node)
{
    Basket *last = head_ref;  
    new_node->next = NULL;
    if (head_ref == NULL)
    {
       head_ref = new_node;
       return head_ref;
    }  
    while (last->next != NULL){
      last = last->next;
    }
    last->next = new_node;
    return head_ref;    
}
Product* addProductToEnd(Product* head_ref, Product* new_node)
{
    Product *last = head_ref;  
    new_node->next = NULL;
    if (head_ref == NULL)
    {
       head_ref = new_node;
       return head_ref;
    }  
    while (last->next != NULL){
      last = last->next;
    }
    last->next = new_node;
    return head_ref;    
}
Product* addProduct(Product* head_ref,Product* new_node){
    new_node->next = head_ref;
    head_ref   = new_node;
  return head_ref;
}

Customer* findCustomer(Customer* head,int id){
  Customer* temp = head;
  while(temp!=NULL){
    if(id==temp->id){
      return temp;
    }
    temp=temp->next;
  }
  return NULL;
}

void addBasketToCustomer(Customer* customer,Basket* basket){
  customer->basketlist=addBasketToEnd(customer->basketlist,basket);
}
Basket* customerHasBasket(int basketID,Customer* cust){
  Basket* temp = cust->basketlist;
  while(temp!=NULL){
    if(temp->id==basketID){
      return temp;
    }
    temp=temp->next;
  }
  return NULL;
}
Basket* addProductToBasket(Basket* basket,Product* product){
  basket->productlist = addProduct(basket->productlist, product);
  return basket;
}
Customer* readBaskets(Customer* head,char fname[30]){
  Basket* temp =NULL;
  FILE* f = fopen(fname,"r");
  int cid,productList,amount;
  while(!feof(f)){
    fscanf(f,"%d\t%d\t%d\n",&cid,&productList,&amount);
    Customer* cust = findCustomer(head,cid);
    Basket* newBasket = customerHasBasket(productList, cust);
    if(newBasket==NULL){
      newBasket = allocBasket(cid, amount);
    }
    addBasketToCustomer(cust,newBasket);
  }
  fclose(f);
  return head;
}

Product* readProducts(char fname[30]){
  Product* list=NULL;
  FILE*f = fopen(fname,"r");
  int id,price;
  char name[15];
  char category[15];
  while(!feof(f)){
    fscanf(f,"%d\t%s\t%s\%d\n",&id,name,category,&price);
    Product* newP = allocProduct(id, name,category, price);
    list = addProductToEnd(list, newP);
  }
  return list;
}
Customer* readCustomers(char fname[30]){
  Customer* head = NULL;
  FILE* f = fopen(fname,"r");
  int id;
  char name[20],surname[20];
  Customer* newCustomer;
  while(!feof(f)){
    fscanf(f,"%d\t%s\t%s\n",&id,name,surname);
    newCustomer =allocCustomer(id, name, surname);
    head = addCustomerToEnd(head,newCustomer);
  }
  fclose(f);
  return head;
}

Customer* addProductsToCustomers(Customer* chead,Product* phead){
  Customer* ctemp = chead;
  Basket* btemp = NULL;
  Product* ptemp = phead;
  while(ctemp!=NULL){
    btemp = ctemp->basketlist;
    while(btemp!=NULL){
      int code = btemp->amount;
      ptemp = phead;
      while(ptemp!=NULL){
        if(ptemp->id==code){
          addProductToBasket(btemp,ptemp);
          break;
        }
        ptemp=ptemp->next;
      }
      btemp=btemp->next;
    }
    ctemp=ctemp->next;
  }
  return chead;
}

void printCustomers(Customer* head){
  Customer* temp = head;
  Basket* btemp;
  while(temp!=NULL){
    printf("%d %s %s\n",temp->id,temp->name,temp->surname);
    btemp = temp->basketlist;
    while(btemp!=NULL){
      printf("Basket id: %d\n",btemp->id);
      Product* ptemp = btemp->productlist;
      while(ptemp!=NULL){
        printf("%d %s %s %d\n",ptemp->id,ptemp->name,ptemp->category,ptemp->price);
        ptemp = ptemp->next;
      }
      btemp=btemp->next;
    }
    temp=temp->next;
  }
}
void printProducts(Product* head){
  Product* temp = head;
  while(temp!=NULL){
    printf("%d %s %s %d\n",temp->id,temp->name,temp->category,temp->price);
    temp=temp->next;
  }
}
int main(void) {
  Product* productHead = readProducts("product.txt");
  Customer* customerHead = readCustomers("customer.txt");
  customerHead = readBaskets(customerHead, "basket.txt");
  customerHead=addProductsToCustomers(customerHead, productHead);
  printCustomers(customerHead);


  return 0;
}
