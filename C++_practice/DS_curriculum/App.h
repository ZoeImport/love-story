#ifndef _APP_H_
#define _APP_H_

#include "HashTable.h"
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
UserInfo searchByUserName(HashTable &hashTable, std::string &userName);
UserInfo searchByPhoneNumber(HashTable &hashTable, std::string &phoneNumber);
int AppFunction(std::ifstream &ifs, HashTable &hashTableByPhoneNumber,
                HashTable &hashTableByUserName,
                std::vector<UserInfo> &user_info_list_phoneNumber,
                std::vector<UserInfo> &user_info_list_userName);
int RunSystem(std::string filePath);

#endif