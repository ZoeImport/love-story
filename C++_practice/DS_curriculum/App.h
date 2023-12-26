#ifndef _APP_H_
#define _APP_H_

#include "HashTable.h"
#include "OpenAddressHashTable.h"
#include "UserInfo.h"
#include <iostream>
#include <string>

/*
    appHeaderFile
    包括:
        按名字查询
        按电话号码查询
*/

int menuMsgOut();
template <typename tableType>
UserInfo searchByPhoneNumber(tableType &hashTable, std::string &phoneNumber);
template <typename tableType>
UserInfo searchByPhoneNumber(tableType &hashTable, std::string &phoneNumber) {
  auto index = strToInt(phoneNumber);
  return hashTable[index];
}
template <typename tableType>
UserInfo searchByUserName(tableType &hashTable, std::string &userName);
template <typename tableType>
UserInfo searchByUserName(tableType &hashTable, std::string &userName) {
  auto index = strToInt(userName);
  return hashTable[index];
}
// UserInfo searchByUserNameOpen(OpenAddressHashTable &hashTable, std::string
// &userName); UserInfo searchByPhoneNumberOpen(OpenAddressHashTable &hashTable,
// std::string &phoneNumber);

template <typename tableType>
tableType &initHashTableByPhoneNumber(std::ifstream &ifs, tableType &hashTable,
                                      std::vector<UserInfo> &user_info_list);
template <typename tableType>
tableType &
initHashTableByPhoneNumber(std::ifstream &ifs, tableType &hashTable,
                           std::vector<UserInfo> &user_info_list_phoneNumber) {
  readUserInfo(ifs, user_info_list_phoneNumber);
  for (auto atom : user_info_list_phoneNumber) {
    hashTable[strToInt(atom.getPhoneNumber())] = atom;
  }
  return hashTable;
}
template <typename tableType>
tableType &initHashTableByUserName(std::ifstream &ifs, tableType &hashTable,
                                   std::vector<UserInfo> &user_info_list);
template <typename tableType>
tableType &
initHashTableByUserName(std::ifstream &ifs, tableType &hashTable,
                        std::vector<UserInfo> &user_info_list_userName) {
  readUserInfo(ifs, user_info_list_userName);
  for (auto atom : user_info_list_userName) {
    hashTable[strToInt(atom.getUserName())] = atom;
  }
  return hashTable;
}

// int AppFunction(std::string filePath);
// int AppFunctionOpen(std::string filePath);

template <typename tableType> int APP(std::string filePath);
template <typename tableType> int APP(std::string filePath) {
  tableType hashTableByPhoneNumber;
  tableType hashTableByUserName;
  std::vector<UserInfo> user_info_list_phoneNumber;
  std::vector<UserInfo> user_info_list_userName;
  std::ifstream ifs_phoneNumber(filePath);
  std::ifstream ifs_userName(filePath);
  std::string inputInfo = "";
  int menuChoose = 0;
  initHashTableByPhoneNumber<tableType>(ifs_phoneNumber, hashTableByPhoneNumber,
                                        user_info_list_userName);
  std::cout << "======================" << std::endl;
  initHashTableByUserName<tableType>(ifs_userName, hashTableByUserName,
                                     user_info_list_phoneNumber);
  ifs_phoneNumber.close();
  ifs_userName.close();
  menuMsgOut();
  do {
    std::cin >> menuChoose;
    switch (menuChoose) {
    case 0:
      std::cout << "正在退出电话号码查询系统..." << std::endl;
      break;
    case 1:
      std::cin >> inputInfo;
      std::cout << "电话号码:" << inputInfo << " | 查询信息： "
                << searchByPhoneNumber<tableType>(hashTableByPhoneNumber,
                                                  inputInfo);
      break;
    case 2:
      std::cin >> inputInfo;
      std::cout << "用户名称:" << inputInfo << " | 查询信息： "
                << searchByUserName<tableType>(hashTableByUserName, inputInfo);
      break;
    default:
      std::cout << "错误参数请重新输入..." << std::endl;
    }
    // if (!menuChoose)
    //   break;
    std::cout << "============================" << std::endl;
  } while (menuChoose);
  return 0;
}

#endif