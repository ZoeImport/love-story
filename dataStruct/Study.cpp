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