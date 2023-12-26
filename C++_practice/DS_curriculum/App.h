#ifndef _APP_H_
#define _APP_H_

#include "HashTable.h"
#include "OpenAddressingHashTable.h"
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
UserInfo searchByUserNameOpen(OpenAddressHashTable &hashTable, std::string &userName);
UserInfo searchByPhoneNumberOpen(OpenAddressHashTable &hashTable, std::string &phoneNumber);
int AppFunction(std::string filePath);
int AppFunctionOpen(std::string filePath);

#endif