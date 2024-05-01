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

