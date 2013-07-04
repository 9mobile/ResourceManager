//
//  JSONElement.cpp
//  Framework-x
//
//  Created by the9 on 13-2-27.
//
//

#include "JSONElement.h"
#include "JSONParser.h"
#include "JSONFormatter.h"

using namespace cocos2d;

/**
 ******************JSONObject******************
 */

JSONObject::JSONObject()
{
    dataHolder=CCDictionary::create();
    CC_SAFE_RETAIN(dataHolder);
}

JSONObject::~JSONObject()
{
    CC_SAFE_RELEASE_NULL(dataHolder);
}

bool JSONObject::init()
{
    return true;
}

JSONObject* JSONObject::createWithString(const std::string &jsonStr)
{
    return dynamic_cast<JSONObject*>(JSONParser::parseString(jsonStr));
}

JSONObject* JSONObject::createWithFile(const std::string &jsonFilePath)
{
    return dynamic_cast<JSONObject*>(JSONParser::parseFile(jsonFilePath));
}

bool JSONObject::has(const std::string& key)
{
    return dataHolder->objectForKey(key)!=NULL;
}

int JSONObject::count()
{
    return dataHolder->count();
}

void JSONObject::putBool(const std::string& key,bool value)
{
    dataHolder->setObject(JSONBool::create(value), key);
}

bool JSONObject::getBool(const std::string& key)
{
    CCObject* value=dataHolder->objectForKey(key);
    CCAssert(value, "");
    JSONElement* jsonBool=dynamic_cast<JSONElement*>(value);
    CCAssert(jsonBool, "");
    return jsonBool->boolValue();
}

void JSONObject::putNumber(const std::string& key,double value)
{
    dataHolder->setObject(JSONNumber::create(value), key);
}

double JSONObject::getNumber(const std::string& key)
{
    CCObject* value=dataHolder->objectForKey(key);
    CCAssert(value, "");
    JSONElement* jsonNumber=dynamic_cast<JSONElement*>(value);
    CCAssert(jsonNumber, "");
    return jsonNumber->numberValue();
}

void JSONObject::putString(const std::string& key,const std::string& value)
{
    dataHolder->setObject(JSONString::create(value.c_str()), key);
}

const char* JSONObject::getString(const std::string& key)
{
    CCObject* value=dataHolder->objectForKey(key);
    CCAssert(value, "");
    JSONElement* jsonString=dynamic_cast<JSONElement*>(value);
    CCAssert(jsonString, "");
    return jsonString->stringValue();
}

void JSONObject::putJSONObject(const std::string& key,JSONObject* value)
{
    dataHolder->setObject(value, key);
}

JSONObject* JSONObject::getJSONObject(const std::string& key)
{
    CCObject* value=dataHolder->objectForKey(key);
    CCAssert(value, "");
    JSONObject* jsonObject=dynamic_cast<JSONObject*>(value);
    CCAssert(jsonObject, "");
    return jsonObject;
}

void JSONObject::putJSONArray(const std::string& key,JSONArray* value)
{
    dataHolder->setObject(value, key);
}

JSONArray* JSONObject::getJSONArray(const std::string& key)
{
    CCObject* value=dataHolder->objectForKey(key);
    CCAssert(value, "");
    JSONArray* jsonArray=dynamic_cast<JSONArray*>(value);
    CCAssert(jsonArray, "");
    return jsonArray;
}

void JSONObject::put(const std::string& key,CCObject* value)
{
    dataHolder->setObject(value, key);
}

CCObject* JSONObject::get(const std::string& key)
{
    return dataHolder->objectForKey(key);
}

CCArray* JSONObject::allKeys()
{
    return dataHolder->allKeys();
}

void JSONObject::remove(const std::string &key)
{
    dataHolder->removeObjectForKey(key);
}

void JSONObject::removeAll()
{
    dataHolder->removeAllObjects();
}

const char* JSONObject::toString()
{
    return JSONFormatter::format(this,false);
}

const char* JSONObject::toPrettyString()
{
    return JSONFormatter::format(this,true);
}

const char* JSONObject::toLuaString()
{
    //头
    std::string luaStr="{\n";
    int num=this->count();
    CCArray* keyArray=this->allKeys();
    for (int i=0; i<num; i++) {
        //键
        const char* key=((CCString*)keyArray->objectAtIndex(i))->getCString();
        luaStr.append("[\"");
        luaStr.append(key);
        luaStr.append("\"]");
        luaStr.append("=");
        //值
        CCObject* obj=this->get(key);
        JSONBool* jsonBool=NULL;
        JSONNumber* jsonNumber=NULL;
        JSONString* jsonString=NULL;
        JSONObject* jsonObj=NULL;
        JSONArray* jsonArray=NULL;
        if ((jsonBool=dynamic_cast<JSONBool*>(obj))!=NULL) {
            luaStr.append(jsonBool->stringValue());
        }else if ((jsonNumber=dynamic_cast<JSONNumber*>(obj))!=NULL){
            luaStr.append(jsonNumber->stringValue());
        }else if ((jsonString=dynamic_cast<JSONString*>(obj))!=NULL){
            luaStr.append("[==[");
            luaStr.append(jsonString->stringValue());
            luaStr.append("]==]");
        }else if ((jsonObj=dynamic_cast<JSONObject*>(obj))!=NULL){
            std::string value=jsonObj->toLuaString();
            luaStr.append(value);
        }else if ((jsonArray=dynamic_cast<JSONArray*>(obj))!=NULL){
            std::string value=jsonArray->toLuaString();
            luaStr.append(value);
        }
        if (i<num-1) {
            luaStr.append(",");
            luaStr.append("\n");
        }
    }
    //尾
    luaStr.append("\n}");
    CCString* ccstr=CCString::create(luaStr);
    return ccstr->getCString();
}

/**
 ******************JSONArray******************
 */

JSONArray::JSONArray()
{
    dataHolder=CCArray::create();
    CC_SAFE_RETAIN(dataHolder);
}

JSONArray::~JSONArray()
{
    CC_SAFE_RELEASE_NULL(dataHolder);
}

bool JSONArray::init()
{
    return true;
}

JSONArray* JSONArray::createWithString(const std::string &jsonStr)
{
    return dynamic_cast<JSONArray*>(JSONParser::parseString(jsonStr));
}

JSONArray* JSONArray::createWithFile(const std::string &jsonFilePath)
{
    return dynamic_cast<JSONArray*>(JSONParser::parseFile(jsonFilePath));
}

int JSONArray::count()
{
    return dataHolder->count();
}

void JSONArray::addBool(bool value)
{
    JSONBool* jsonBool=JSONBool::create(value);
    dataHolder->addObject(jsonBool);
}

void JSONArray::insertBool(bool value, unsigned int index)
{
    JSONBool* jsonBool=JSONBool::create(value);
    dataHolder->insertObject(jsonBool, index);
}

bool JSONArray::getBool(int index)
{
    CCObject* value=dataHolder->objectAtIndex(index);
    CCAssert(value, "");
    JSONElement* jsonBool=dynamic_cast<JSONElement*>(value);
    CCAssert(jsonBool, "");
    return jsonBool->boolValue();
}

void JSONArray::addNumber(double value)
{
    JSONNumber* jsonNumber=JSONNumber::create(value);
    dataHolder->addObject(jsonNumber);
}

void JSONArray::insertNumber(double value, unsigned int index)
{
    JSONNumber* jsonNumber=JSONNumber::create(value);
    dataHolder->insertObject(jsonNumber, index);
}

double JSONArray::getNumber(int index)
{
    CCObject* value=dataHolder->objectAtIndex(index);
    CCAssert(value, "");
    JSONElement* jsonNumber=dynamic_cast<JSONElement*>(value);
    CCAssert(jsonNumber, "");
    return jsonNumber->numberValue();
}

void JSONArray::addString(const std::string& value)
{
    JSONString* jsonString=JSONString::create(value.c_str());
    dataHolder->addObject(jsonString);
}

void JSONArray::insertString(const std::string &value, unsigned int index)
{
    JSONString* jsonString=JSONString::create(value.c_str());
    dataHolder->insertObject(jsonString, index);
}

const char* JSONArray::getString(int index)
{
    CCObject* value=dataHolder->objectAtIndex(index);
    CCAssert(value, "");
    JSONElement* jsonString=dynamic_cast<JSONElement*>(value);
    CCAssert(jsonString, "");
    return jsonString->stringValue();
}

void JSONArray::addJSONObject(JSONObject* value)
{
    dataHolder->addObject(value);
}

void JSONArray::insertJSONObject(JSONObject *value, unsigned int index)
{
    dataHolder->insertObject(value, index);
}

JSONObject* JSONArray::getJSONObject(int index)
{
    CCObject* value=dataHolder->objectAtIndex(index);
    CCAssert(value, "");
    JSONObject* jsonObject=dynamic_cast<JSONObject*>(value);
    CCAssert(jsonObject, "");
    return jsonObject;
}

void JSONArray::addJSONArray(JSONArray* value)
{
    dataHolder->addObject(value);
}

void JSONArray::insertJSONArray(JSONArray *value, unsigned int index)
{
    dataHolder->insertObject(value, index);
}

JSONArray* JSONArray::getJSONArray(int index)
{
    CCObject* value=dataHolder->objectAtIndex(index);
    CCAssert(value, "");
    JSONArray* jsonArray=dynamic_cast<JSONArray*>(value);
    CCAssert(jsonArray, "");
    return jsonArray;
}

void JSONArray::add(CCObject* value)
{
    dataHolder->addObject(value);
}

void JSONArray::insert(cocos2d::CCObject *value, unsigned int index)
{
    dataHolder->insertObject(value, index);
}

CCObject* JSONArray::get(int index)
{
    return dataHolder->objectAtIndex(index);
}

void JSONArray::removeLast()
{
    dataHolder->removeLastObject();
}

void JSONArray::remove(unsigned int index)
{
    dataHolder->removeObjectAtIndex(index);
}

void JSONArray::removeAll()
{
    dataHolder->removeAllObjects();
}

const char* JSONArray::toString()
{
    return JSONFormatter::format(this,false);
}

const char* JSONArray::toPrettyString()
{
    return JSONFormatter::format(this,true);
}

const char* JSONArray::toLuaString()
{
    //头
    std::string luaStr="{\n";
    int num=this->count();
    for (int i=0; i<num; i++) {
        //值
        CCObject* obj=this->get(i);
        JSONBool* jsonBool=NULL;
        JSONNumber* jsonNumber=NULL;
        JSONString* jsonString=NULL;
        JSONObject* jsonObj=NULL;
        JSONArray* jsonArray=NULL;
        if ((jsonBool=dynamic_cast<JSONBool*>(obj))!=NULL) {
            luaStr.append(jsonBool->stringValue());
        }else if ((jsonNumber=dynamic_cast<JSONNumber*>(obj))!=NULL){
            luaStr.append(jsonNumber->stringValue());
        }else if ((jsonString=dynamic_cast<JSONString*>(obj))!=NULL){
            luaStr.append("[==[");
            luaStr.append(jsonString->stringValue());
            luaStr.append("]==]");
        }else if ((jsonObj=dynamic_cast<JSONObject*>(obj))!=NULL){
            std::string value=jsonObj->toLuaString();
            luaStr.append(value);
        }else if ((jsonArray=dynamic_cast<JSONArray*>(obj))!=NULL){
            std::string value=jsonArray->toLuaString();
            luaStr.append(value);
        }
        if (i<num-1) {
            luaStr.append(",");
            luaStr.append("\n");
        }
    }
    //尾
    luaStr.append("\n}");
    CCString* ccstr=CCString::create(luaStr);
    return ccstr->getCString();
}

/**
 ******************JSONElement******************
 */
JSONElement::~JSONElement()
{
    
}

/**
 ******************JSONBool******************
 */

JSONBool::JSONBool(bool value)
{
    this->value=value;
}

JSONBool* JSONBool::create(bool value)
{
    JSONBool* jsonBool=new JSONBool(value);
    jsonBool->autorelease();
    return jsonBool;
}


bool JSONBool::boolValue()
{
    return value;
}

double JSONBool::numberValue()
{
    return value;
}

const char* JSONBool::stringValue()
{
    return value?"true":"false";
}


/**
 ******************JSONNumber******************
 */

JSONNumber::JSONNumber(double value)
{
    this->value=value;
}

JSONNumber* JSONNumber::create(double value)
{
    JSONNumber* jsonNumber=new JSONNumber(value);
    jsonNumber->autorelease();
    return jsonNumber;
}

bool JSONNumber::boolValue()
{
    return value;
}

double JSONNumber::numberValue()
{
    return value;
}

const char* JSONNumber::stringValue()
{
    CCString* stringValue=CCString::createWithFormat("%lf",value);
    //TODO 去掉小数后面的0，如果最后一个是小数点也去掉。可能有更好的办法，待研究
    std::string str=stringValue->getCString();
    while (str.find_first_of('.')!=-1&&(str.at(str.size()-1)=='.'||str.at(str.size()-1)=='0')) {
        str.erase(str.begin()+str.size()-1);
    }
    stringValue=CCString::create(str);
    return stringValue->getCString();
}

/**
 ******************JSONString******************
 */

JSONString::JSONString(const std::string& value)
{
    this->value=CCString::create(value);
    CC_SAFE_RETAIN(this->value);
}

JSONString::~JSONString()
{
    CC_SAFE_RELEASE_NULL(value);
}

JSONString* JSONString::create(const std::string &value)
{
    JSONString* jsonString=new JSONString(value);
    jsonString->autorelease();
    return jsonString;
}

bool JSONString::boolValue()
{
    return value->getCString();
}

double JSONString::numberValue()
{
    double doubleValue;
    sscanf(value->getCString(), "%lf",&doubleValue);
    return doubleValue;
}

const char* JSONString::stringValue()
{
    return value->getCString();
}

