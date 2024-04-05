猪的笔记

# 编译

C++是高级程序语言，编译器负责把高级程序语言编译为汇编语言，最后再把汇编语言转换为机器语言



```c++
#include<iostream>
struct Node{
    int data;
    Node* head;
};
struct Num {
    int data; 
};
void initNode(Node*list){
    list->head=nullptr;
}

bool isEmpty(Node* list){
    if(list->head==nullptr)
        return true;
    return false;
}

void insertNode(Node*list,int data){
    if(isEmpty(list)){
        Node* newNode=new Node;
        newNode->data=data;
        list->head=newNode;
    }
    // else 
}



int main(){
    Num no1;
    no1.data=20;
    std::cout<<no1.data<<std::endl;
    Node *list;
    insertNode(list,no1.data);
    std::cout<<list->head->data<<std::endl;
return 0;
}
```

小猪佩奇


cd 
g++
ls
./

//今天猪的笔记重新开张！2024.3.27 俺的羊驼驼舌头痛痛，但还是帮俺重装了电脑！感动！哭哭
乱码：
先切换成utf，然后全选保存，剪切，切换到gbk，粘贴，保存，重新编译

按F1快捷截图
ctrl t 挂起

2024.4.5
数据结构：
线性结构只有一个首结点和一个尾结点，其他结点只有一个直接前驱和一个直接后继

线性表：（线性表是具有相同特性的数据元素的一个有限序列）：有穷性，一致性（有相同的数据类型），序列性
线性表的顺序存储结构称为：顺序表，借助数组类型实现

顺序表：
#define MaxSize;
typedef struct{
  ElemType data[MaxSize];
  int length;
}SqList；
typedef ElemType;

建立顺序表：
void CreaList(SqList*& L,ElemType a[],int n){
  int k=0,i=0;
  L=(SqList*)malloc(sizeof(SqList));
  while(i<n){
    L->data[k]=a[i];
    k++;i++;
  }
  L->length=k;
}

初始化线性表：InitList(&L)
void InitList(SqList*&L)
{
  L=(SqList*)malloc(sizeof(SqList));
  L->length=0;
}

销毁线性表：DestroyList(&L)
void DestroyLst(SqList*&L)
{
  free(L);
}

判断线性表是否为空表：ListEmpty(L)
void ListEmpty(SqList*L){
  return(L->length==0);
}

求线性表的长度：ListLength(L)
void ListLength(SqList*L){
  return(L->length);
}

输出线性表：DispList(L)
void DispList(SqList *L){
  for(int i=0;i<L->length;i++){
    printf("%d",L->data[i]);
    printf("\n");
  }
}

按序号求线性表中的元素：GetElem(L,i,&e)
bool GetElem(SqList *L,int i,ElemType &e)
{
  if(i<1||i>L->length)
  return false;
  e=L->data[i-1];
  retun ture;
}

按元素值查找：LocateElem(L,e)
int LocateElem(SqList *L,ElemType e)
{
  int i=0;
  while(i<L->length&& L->data[i]!=e)
  i++;
  if(i>=L->length)
  return 0;
  else return i+1;
}

插入数据元素：ListInsert(&L,i,e)
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

  删除数据元素：ListDelete:(&L,i,&e)
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