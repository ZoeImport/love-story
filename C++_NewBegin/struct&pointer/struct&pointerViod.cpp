#include<iostream>
#include<string>
#include"struct.h"
using namespace std;
//struct&pointerVoid.h中函数定义
//此处使用地址传递，保证形参的修改得以对实参的值进行修改
void swap_byme(int *num_1,int *num_2){
  int temp;
  temp=*num_1;
  *num_1=*num_2;
  *num_2=temp;
}
/*
  如果不使用地址传递，使用值传递，就无法对实参进行修改，在对函数调用时就相当于将传入的值创建了一个新的副本
  对于以上的这种情况为了减少空间的浪费，就可以使用地址传递，但是对于地址传递来说，对传入的数据就有被修改的风险，此时就需要用到const关键字
  例如如下定义的函数showInfo()
*/

void showInfo(const Person *p){
  cout<<"Name:"<<p->name<<endl;
  cout<<"Age:"<<p->age<<endl;
  cout<<"Gender:"<<p->gender<<endl;
}

/*
  对于值传递来说，简单的sum函数为例
*/
int numSum(int num_1,int num_2){
  return num_1+num_2;
}