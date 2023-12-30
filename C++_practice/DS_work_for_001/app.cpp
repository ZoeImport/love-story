#include "ClothesOrder.h"
#include "List.h"
#include "Sort.h"
#include "Stack.h"
#include <iostream>
#include <ostream>
#include <utility>
#include <vector>

void menuMsg() {
  std::cout << "======================" << std::endl;
  std::cout << "<欢迎使用服装订单管理系统> " << std::endl;
  std::cout << "======================" << std::endl;
  std::cout << "输入相应按键进行功能使用   " << std::endl;
  std::cout << "1:查看现有服装订单列表    " << std::endl;
  std::cout << "2:增加新订单            " << std::endl;
  std::cout << "3:修改指定订单          " << std::endl;
  std::cout << "4:删除指定订单          " << std::endl;
  std::cout << "5:查找指定订单          " << std::endl;
  std::cout << "6:价格升序查看订单列表    " << std::endl;
  std::cout << "7:价格降序查看订单列表    " << std::endl;
  std::cout << "0:退出服装订单系统       " << std::endl;
  std::cout << "======================" << std::endl;
}

int initOrderList(LinkedList<ClothesOrder> *&list) {
  ClothesOrder order1(1, "Nike", "Lisa", "2022-3-3", 1000);
  ClothesOrder order2(2, "adidas", "Jack", "2021-2-1", 777);
  ClothesOrder order3(3, "CONVERSE", "Tom", "2021-1-3", 469);
  ClothesOrder order4(4, "Anta", "Lucy", "2022-4-3", 339);
  ClothesOrder order5(5, "LiNing", "Lucas", "2022-4-3", 500);
  ClothesOrder order6(6, "puma", "Bob", "2022-4-3", 590);
  list->append(order1);
  list->append(order2);
  list->append(order3);
  list->append(order4);
  list->append(order5);
  list->append(order6);
  ClothesOrder::orderCount = 6;
  return 0;
}

int showListAsc(LinkedList<ClothesOrder> *&list) {
  std::vector<ClothesOrder> order_arr;
  std::vector<std::pair<int, int>> price_arr;
  for (auto &temp : list->forEach(order_arr)) {
    price_arr.push_back(std::make_pair(temp.getId(), temp.getPrice()));
    std::cout << temp << std::endl;
  }
  quickSort(price_arr, 0, price_arr.size() - 1);
  for (auto num : price_arr) {
    std::cout << list->find(ClothesOrder(num.first))->data;
  }

  return 0;
}

int showListDesc(LinkedList<ClothesOrder> *&list) {
  std::vector<ClothesOrder> order_arr;
  std::vector<std::pair<int, int>> price_arr;
  for (auto &temp : list->forEach(order_arr)) {
    price_arr.push_back(std::make_pair(temp.getId(), temp.getPrice()));
    std::cout << temp << std::endl;
  }
  bubbleSort(price_arr);
  for (auto num : price_arr) {
    std::cout << list->find(ClothesOrder(num.first))->data;
  }
  return 0;
}

int appFunction(LinkedList<ClothesOrder> *&list) {
  initOrderList(list);
  menuMsg();
  int menuChoose;
  do {
    std::cin >> menuChoose;
    switch (menuChoose) {
    case 0:
      std::cout << "正在退出服装管理系统..." << std::endl;
      break;
    case 1:
      list->print();
      break;
    case 2:
      insertOrder(list);
      break;
    case 3:
      ModifyOrder(list);
      break;
    case 4:
      deleteOrder(list);
      break;
    case 5:
      searchOrder(list);
      break;
    case 6:
      showListAsc(list);
      break;
    case 7:
      showListDesc(list);
      break;
    default:
      std::cout << "错误参数请重新输入..." << std::endl;
    }
    std::cout << "============================" << std::endl;
  } while (menuChoose);
  return 0;
}
