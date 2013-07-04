//
//  JSONElement.h
//  Framework-x
//
//  Created by the9 on 13-2-27.
//
//

#ifndef __Framework_x__JSONElement__
#define __Framework_x__JSONElement__

#include <iostream>
#include "cocos2d.h"

/**
 ******************JSONObject******************
 * 是json数据结构“对象”的封装
 */

class JSONArray;

class JSONObject : public cocos2d::CCObject{
    
    cocos2d::CCDictionary* dataHolder;
    
public:
    
    JSONObject();
    
    ~JSONObject();
    
    bool init();
    
    //创建一个空的JSONObject
    CREATE_FUNC(JSONObject);
    
    //根据指定json字符串创建一个JSONObject
    static JSONObject* createWithString(const std::string& jsonStr);
    
    //根据指定json文件创建一个JSONObject
    static JSONObject* createWithFile(const std::string& jsonFilePath);
    
    //检查JSONObject中是否包含指定的键
    bool has(const std::string& key);
    
    //返回JSONObject中"键值对"的数量
    int count();
    
    //往JSONObject中添加一个bool值
    void putBool(const std::string& key,bool value);
    
    //返回JSONObject中指定键的bool值
    bool getBool(const std::string& key);
    
    //往JSONObject中添加一个double值
    void putNumber(const std::string& key,double value);
    
    //返回JSONObject中指定键的double值
    double getNumber(const std::string& key);
    
    //往JSONObject中添加一个string值
    void putString(const std::string& key,const std::string& value);
    
    //返回JSONObject中指定键的string值
    const char* getString(const std::string& key);
    
    //往JSONObject中添加一个子JSONObject
    void putJSONObject(const std::string& key,JSONObject* value);
    
    //返回JSONObject中指定键的子JSONObject
    JSONObject* getJSONObject(const std::string& key);
    
    //往JSONObject中添加一个JSONArray
    void putJSONArray(const std::string& key,JSONArray* value);
    
    //返回JSONObject中指定键的JSONArray
    JSONArray* getJSONArray(const std::string& key);
    
    //往JSONObject中添加一个对象，对象类型必需是以下五种之一:JSONObject JSONArray JSONBool JSONNumber JSONString
    void put(const std::string& key,CCObject* value);
    
    //返回JSONObject中指定键的对象，返回的对象类型为以下五种之一:JSONObject JSONArray JSONBool JSONNumber JSONString
    CCObject* get(const std::string& key);
    
    //返回JSONObject中所有的键
    cocos2d::CCArray* allKeys();
    
    //删除JSONObject中指定的"键值对"
    void remove(const std::string& key);
    
    //删除JSONObject中所有的"键值对"
    void removeAll();
    
    //返回JSONObject的json字符串形式
    const char* toString();
    
    //返回JSONObject的json字符串的美化形式
    const char* toPrettyString();
    
    //返回JSONObject的lua字符串形式
    const char* toLuaString();
    
};


/**
 ******************JSONArray******************
 * 是json数据结构“数组”的封装
 */
class JSONArray : public cocos2d::CCObject {
    
    cocos2d::CCArray* dataHolder;
    
public:
    
    JSONArray();
    
    ~JSONArray();
    
    bool init();
    
    //创建一个空的JSONArray
    CREATE_FUNC(JSONArray);
    
    //根据指定json字符串创建一个JSONArray
    static JSONArray* createWithString(const std::string& jsonStr);
    
    //根据指定json文件创建一个JSONArray
    static JSONArray* createWithFile(const std::string& jsonFilePath);
    
    //返回JSONArray中对象的数量
    int count();
    
    //往JSONArray中添加一个bool值
    void addBool(bool value);
    
    //往JSONArray中插入一个bool值
    void insertBool(bool value,unsigned int index);
    
    //返回JSONArray中指定位置对象的bool值
    bool getBool(int index);
    
    //往JSONArray中添加一个double值
    void addNumber(double value);
    
    //往JSONArray中插入一个double值
    void insertNumber(double value,unsigned int index);
    
    //返回JSONArray中指定位置对象的double值
    double getNumber(int index);
    
    //往JSONArray中添加一个string值
    void addString(const std::string& value);
    
    //往JSONArray中插入一个string值
    void insertString(const std::string& value,unsigned int index);
    
    //返回JSONArray中指定位置对象的string值
    const char* getString(int index);
    
    //往JSONArray中添加一个JSONObject
    void addJSONObject(JSONObject* value);
    
    //往JSONArray中插入一个JSONObject
    void insertJSONObject(JSONObject* value,unsigned int index);
    
    //返回JSONArray中指定位置的JSONObject
    JSONObject* getJSONObject(int index);
    
    //往JSONArray中添加一个子JSONArray
    void addJSONArray(JSONArray* value);
    
    //往JSONArray中插入一个子JSONArray
    void insertJSONArray(JSONArray* value,unsigned int index);
    
    //返回JSONArray中指定位置的子JSONArray
    JSONArray* getJSONArray(int index);
    
    //往JSONArray中添加一个对象，对象类型必需是以下五种之一:JSONObject JSONArray JSONBool JSONNumber JSONString
    void add(CCObject* value);
    
    //往JSONArray中插入一个对象，对象类型必需是以下五种之一:JSONObject JSONArray JSONBool JSONNumber JSONString
    void insert(CCObject* value,unsigned int index);
    
    //返回JSONArray中指定位置的对象，返回的对象类型为以下五种之一:JSONObject JSONArray JSONBool JSONNumber JSONString
    CCObject* get(int index);
    
    //删除JSONArray中最后一个对象
    void removeLast();
    
    //删除JSONArray中指定位置的对象
    void remove(unsigned int index);
    
    //删除JSONArray中所有对象
    void removeAll();
    
    //返回JSONArray的json字符串形式
    const char* toString();
    
    //返回JSONArray的json字符串的美化形式
    const char* toPrettyString();
    
    //返回JSONArray的lua字符串形式
    const char* toLuaString();
};

/**
 ******************JSONElement******************
 * 可放入JSONObject JSONArray中简单类型(bool number string)的包装器的基类，提供三种简单类型之间互相转换的接口
 */

class JSONElement : public cocos2d::CCObject {
    
public:
    
    virtual ~JSONElement();
    
    //返回bool值
    virtual bool boolValue()=0;
    
    //返回double值
    virtual double numberValue()=0;
    
    //返回string值
    virtual const char* stringValue()=0;
    
};

/**
 ******************JSONBool******************
 * 简单类型 bool 的包装器
 */
class JSONBool :public JSONElement {
    
    bool value;
    
public:
    
    JSONBool(bool value);
    
    //创建具有指定bool值的JSONBool
    static JSONBool* create(bool value);
    
    //返回bool值
    bool boolValue();
    
    //返回double值 1或者0
    double numberValue();
    
    //返回string值，为“true”或“false”
    const char* stringValue();
    
};

/**
 ******************JSONNumber******************
 * 简单类型 number 的包装器
 */

class JSONNumber : public JSONElement {
    
    double value;
    
public:
    
    JSONNumber(double value);
    
    //创建具有指定number值的JSONNumber
    static JSONNumber* create(double value);
    
    //返回bool值
    bool boolValue();
    
    //返回double值
    double numberValue();
    
    //返回string值
    const char* stringValue();
    
};

/**
 ******************JSONString******************
 * 简单类型 string 的包装器
 */

class JSONString : public JSONElement{
    
    cocos2d::CCString* value;
    
public:
    
    JSONString(const std::string& value);
    
    ~JSONString();
    
    //创建具有指定string值的JSONString
    static JSONString* create(const std::string& value);
    
    //返回bool值
    bool boolValue();
    
    //返回double值
    double numberValue();
    
    //返回string值
    const char* stringValue();
    
};

#endif /* defined(__Framework_x__JSONElement__) */
