#include "ClothesOrder.h"
#include "List.h"
#include <cstddef>
#include <iostream>
#include <ostream>
#include <string>
#include <variant>

int ClothesOrder::orderCount=0;

int ClothesOrder::getId() { return this->id; }

std::string ClothesOrder::getTimeDate() { return this->timeDate; }

int ClothesOrder::getPrice() { return this->price; }

std::string ClothesOrder::getTradeName() { return this->tradeName; }

std::string ClothesOrder::getBuyer() { return this->buyer; }

void ClothesOrder::setId(int id) { this->id = id; }

void ClothesOrder::setTimeDate(std::string timeDate) {
  this->timeDate = timeDate;
}

void ClothesOrder::setPrice(int price) { this->price = price; }

void ClothesOrder::setTradeName(std::string tradeName) {
  this->tradeName = tradeName;
}

void ClothesOrder::setBuyer(std::string buyer) { this->buyer = buyer; }

std::ostream &operator<<(std::ostream &cout, ClothesOrder &order) {
  cout << "订单信息|  id: " << order.getId()
       << " tradeName: " << order.getTradeName()
       << " buyer: " << order.getBuyer()
       << " timeDate: " << order.getTimeDate()
       << " price: " << order.getPrice() << std::endl;
  return cout;
}

bool ClothesOrder::operator==(const ClothesOrder& order){
    return this->id==order.id;
}

 bool ClothesOrder::operator!=(const ClothesOrder& order){
  return !(*this==order);
 }

int insertOrder(LinkedList<ClothesOrder>* &list){
    std::string tradeName;
    std::string buyer;
    std::string timeDate;
    int price;
    std::cout<<"请输入商品名："<<std::endl;
    std::cin>>tradeName;
    std::cout<<"请输入顾客名："<<std::endl;
    std::cin>>buyer;
    std::cout<<"请输入时间(格式0000-00-00):"<<std::endl;
    std::cin>>timeDate;
    std::cout<<"请输入价格："<<std::endl;
    std::cin>>price;
    list->append(ClothesOrder(++ClothesOrder::orderCount,tradeName,buyer,timeDate,price));
    return 0;
}

bool ModifyOrder(LinkedList<ClothesOrder>*&list){
    std::string tradeName;
    std::string buyer;
    std::string timeDate;
    int price;
    int id;
    std::cout<<"请输入要修改的订单id"<<std::endl;
    std::cin>>id;
    auto &temp=list->find(ClothesOrder(id))->data;
    if (temp==ClothesOrder(0)) {
      std::cout<<"没有找到相关订单的信息"<<std::endl;
      return false;
    }else {
      std::cout<<"查询到以下订单:"<<std::endl<<temp<<std::endl;
    }
    std::cout<<"请输入修改后商品名："<<std::endl;
    std::cin>>tradeName;
    std::cout<<"请输入修改后顾客名："<<std::endl;
    std::cin>>buyer;
    std::cout<<"请输入修改后时间(格式0000-00-00):"<<std::endl;
    std::cin>>timeDate;
    std::cout<<"请输入修改后价格："<<std::endl;
    std::cin>>price;
    temp=ClothesOrder(id,tradeName,buyer,timeDate,price);
    return true;
}

bool deleteOrder(LinkedList<ClothesOrder>*&list){
  int id;
    std::cout<<"请输入要删除的订单id"<<std::endl;
    std::cin>>id;
    auto &temp=list->find(ClothesOrder(id))->data;
    if (temp==ClothesOrder(0)) {
      std::cout<<"没有找到相关订单的信息"<<std::endl;
      return false;
    }else {
      std::cout<<"查询到以下订单:"<<std::endl<<temp<<std::endl;
      std::cout<<"是否删除订单(y/n)"<<std::endl;
      std::string condition="n";
      std::cin>>condition;
      if (condition=="y"||condition=="Y") {
        list->remove(ClothesOrder(id));
        std::cout<<"订单删除成功"<<std::endl;
        return true;
      }else {
        std::cout<<"订单删除失败"<<std::endl;
        return false;
      }
    }
  return false;
}

ClothesOrder searchOrder(LinkedList<ClothesOrder>*&list){
  int id;
    std::cout<<"请输入查找的订单id"<<std::endl;
    std::cin>>id;
    auto &temp=list->find(ClothesOrder(id))->data;
    if (temp==ClothesOrder(0)) {
      std::cout<<"没有找到相关订单的信息"<<std::endl;
      return ClothesOrder();
    }else {
      std::cout<<"查询到以下订单:"<<std::endl<<temp<<std::endl;
      return temp;
    }
}
