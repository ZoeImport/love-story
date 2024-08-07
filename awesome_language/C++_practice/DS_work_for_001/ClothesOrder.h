#ifndef _ORDER_H_
#define _ORDER_H_

#include "List.h"
#include <iostream>
#include <ostream>
#include <string>

// ��������

class ClothesOrder
{
public:
  ClothesOrder() : id(0){};
  ClothesOrder(int id, std::string tradeName = "", std::string buyer = "",
               std::string timeDate = "", int price = 0)
      : id(id), tradeName(tradeName), buyer(buyer), timeDate(timeDate),
        price(price){};
  void setId(int id);
  void setPrice(int price);
  void setTimeDate(std::string timeDate);
  void setTradeName(std::string tradeName);
  void setBuyer(std::string buyer);
  int getId();
  std::string getTimeDate();
  std::string getTradeName();
  int getPrice();
  std::string getBuyer();
  static int orderCount;
  friend std::ostream &operator<<(std::ostream &cout, ClothesOrder &order);
  bool operator==(const ClothesOrder &order);
  bool operator!=(const ClothesOrder &order);

private:
  int id;
  std::string tradeName;
  std::string buyer;
  std::string timeDate;
  int price;
};

// ����<<�����
std::ostream &operator<<(std::ostream &cout, ClothesOrder &order);
int insertOrder(LinkedList<ClothesOrder> *&list);
bool ModifyOrder(LinkedList<ClothesOrder> *&list);
bool deleteOrder(LinkedList<ClothesOrder> *&list);
ClothesOrder searchOrder(LinkedList<ClothesOrder> *&list);

#endif