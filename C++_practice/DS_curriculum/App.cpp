#include "App.h"
#include "HashTable.h"
#include "UserInfo.h"
#include <fstream>
#include <iostream>
#include <string>

int menuMsgOut() {
  std::cout << "======================" << std::endl;
  std::cout << "<欢迎使用电话号码查询系统>" << std::endl;
  std::cout << "======================" << std::endl;
  std::cout << " 输入对应按键进行操作：" << std::endl;
  std::cout << " 1:输入电话号码进行查询" << std::endl;
  std::cout << " 2:输入用户名称进行查询" << std::endl;
  std::cout << " 0:退出电话号码查询系统" << std::endl;
  std::cout << "======================" << std::endl;
  return 0;
}

UserInfo searchByPhoneNumber(HashTable &hashTable, std::string &phoneNumber) {
  auto index = strToInt(phoneNumber);
  return hashTable[index];
}

UserInfo searchByUserName(HashTable &hashTable, std::string &userName) {
  auto index = strToInt(userName);
  return hashTable[index];
}

int AppFunction(std::string filePath, HashTable &hashTableByPhoneNumber,
                HashTable &hashTableByUserName,
                std::vector<UserInfo> &user_info_list_phoneNumber,std::vector<UserInfo> &user_info_list_userName) {
  std::ifstream ifs_phoneNumber(filePath);
  std::ifstream ifs_userName(filePath);
  std::string inputInfo = "";
  int menuChoose = 0;
  initHashTableByUserName(ifs_phoneNumber, hashTableByUserName, user_info_list_userName);
  initHashTableByPhoneNumber(ifs_userName, hashTableByPhoneNumber, user_info_list_phoneNumber);
  ifs_phoneNumber.close();
  ifs_userName.close();
  menuMsgOut();
  while (true) {
    std::cin >> menuChoose;
    switch (menuChoose) {
    case 0:
      std::cout << "正在退出电话号码查询系统..." << std::endl;
      break;
    case 1:
      std::cin >> inputInfo;
      std::cout << "电话号码:" << inputInfo << " | 查询信息： "
                << searchByPhoneNumber(hashTableByPhoneNumber, inputInfo);
      break;
    case 2:
      std::cin >> inputInfo;
      std::cout << "用户名称:" << inputInfo << " | 查询信息： "
                << searchByUserName(hashTableByUserName, inputInfo);
      break;
    }
    if (!menuChoose)
      break;
    std::cout << "============================" << std::endl;
  }
  return 0;
}

int RunSystem(std::string filePath){
   HashTable hashTableByPhoneNumber;
  HashTable hashTableByUserName;
  std::vector<UserInfo> user_info_list_phoneNumber;
  std::vector<UserInfo> user_info_list_userName;
  AppFunction(filePath, hashTableByPhoneNumber,hashTableByUserName,user_info_list_phoneNumber,user_info_list_userName);
  return 0;
}