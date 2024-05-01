//顺序表：
#define MaxSize;
typedef struct{
  ElemType data[MaxSize];
  int length;
}SqList；
typedef ElemType;

//建立顺序表：
void CreaList(SqList*& L,ElemType a[],int n){
  int k=0,i=0;
  L=(SqList*)malloc(sizeof(SqList));
  while(i<n){
    L->data[k]=a[i];
    k++;i++;
  }
  L->length=k;
}

//初始化线性表：InitList(&L)
void InitList(SqList*&L)
{
  L=(SqList*)malloc(sizeof(SqList));
  L->length=0;
}

//销毁线性表：DestroyList(&L)
void DestroyLst(SqList*&L)
{
  free(L);
}

//判断线性表是否为空表：ListEmpty(L)
void ListEmpty(SqList*L){
  return(L->length==0);
}

//求线性表的长度：ListLength(L)
void ListLength(SqList*L){
  return(L->length);
}

//输出线性表：DispList(L)
void DispList(SqList *L){
  for(int i=0;i<L->length;i++){
    printf("%d",L->data[i]);
    printf("\n");
  }
}

//按序号求线性表中的元素：GetElem(L,i,&e)
bool GetElem(SqList *L,int i,ElemType &e)
{
  if(i<1||i>L->length)
  return false;
  e=L->data[i-1];
  retun ture;
}

//按元素值查找：LocateElem(L,e)
int LocateElem(SqList *L,ElemType e)
{
  int i=0;
  while(i<L->length&& L->data[i]!=e)
  i++;
  if(i>=L->length)
  return 0;
  else return i+1;
}

//插入数据元素：ListInsert(&L,i,e)
bool ListInsert(SqList *&L,int i;ElemType e)
{
  int j;
  if(i<1||i>L->length+1||L->length==MaxSize)
  return false;
  i--;
  for(j=L->length;j>i;j--)
  L->data[i]=L->data[j-1];
  L->data[i]=e;
  L->length++;
  return true;
  }

  //删除数据元素：ListDelete:(&L,i,&e)
  bool ListDelete(SqList*&L,int i,ElemType &e){
    int j;
    if(i<1||i>L->length)
    return false;
    i--;
    e=L->data[i];
    for(j=i;j<length-1;j++)
    L->data[j]=L->data[j+1];
    L->length--;
    return true;
  }



//初始化线性表：InitList(&L)
void InitList(LinkNode *&L){
L=(LinkNode*)malloc(sizeof(LinkNode));
L->next=NULL;
}

//销毁线性表：DestroyList(&L)
void DestroyList(LinkNode*&L){
  LinkNode*pre=L,*p=L->next;
  while(p!=NULL)
  {
    free(pre);
    pre=p;
    p=p->next;
  }
  free(pre);
}

//判断线性表是否为空表：ListEmpty(L)
bool ListEmpty(LinkNode *L)
{
  return(L->next==NULL);
}

//求线性表的长度：ListLength(L)
int ListLength(LinkNode*L)
{
  int n=0;
  LinkNode*p=L;
  while(p->next!=NULL)
  {
    n++;
    p=p->next;
  }
  return(n);
}

//输出线性表：DispList(L)
void DispList(LinkNode*L)
{
  LinkNode *p=L->next;
  while(p!=NULL)
  {
    printf("%d",p->data);
    p=p->next;
  }
  printf("\n");
}

//按序号求线性表中元素：GEtElem(L,i,&e)
bool GetElem(LinkNode *L,int i,ElemType &e)
{
  int j=0;
  LinkNode*p=L;
  if(i<=0)return false;
  while(j<i&&p!=NULL)
  {
    j++;
    p=p->next;
  }
  if(p==NULL)
  return false;
else
{ 
  e=p->data;
  return true;
}
}

//按元素值查找：LocateElem(L,e)
int LocateElem(LinkNode *l,ElemType e)
{
  int i=1;
  LinkNode*p=L->next;
  while(p!=NULL&&p->data!=e)
  {
    p=p->next;
    i++;
  }
  if(p==NULL)
  return(0);
else return(i);
}

//插入数据元素：ListInsert(&L,i,e)
bool ListInsert(LinkNode*&L,int i,ElemType e)
{
  int j=0;
  LinkNode*p=L,*s;
  if(i<=0)return false;
  while(j<i-1&&p!=NULL)
  {
    j++;
    p=p->next;
  }
  if(p==NULL)
  return false;
else
{
  s=(LinkNode*)malloc(sizeof(LinkNode));
  s->data=e;
  s->next=p->next;
  p->next=s;
  return true;
}

}

//删除数据元素：ListDelete(&L,i,&e)
bool ListDelete(LinkNode*&L,int i,,ElemType &e)
{
  int j=0;
  LinkNode*p=L,*q;
  if(i<=0)return false;
  while(j<i-1&&p!=NULL)
  {
    j++;
    p=p->next;
  }
  if(p==NULL)
  return false;
else
{
  q=p->next;
  if(q==NULL)
  return false;
e=q->data;
p->next=q->next;
free(q);
return true;
}
}