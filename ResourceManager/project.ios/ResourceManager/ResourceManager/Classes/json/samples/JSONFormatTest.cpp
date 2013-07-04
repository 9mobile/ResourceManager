//
//  JSONFormatTest.cpp
//  Framework-x
//
//  Created by the9 on 13-3-1.
//
//

#include "JSONFormatTest.h"
#include "JSONElement.h"

using namespace cocos2d;

void JSONFormatTest::runTest()
{
    //(1)导入头文件 JSONElement.h
    
    //(2)序列化简单JSON对象
    //  (2-1)创建JSONObject
    JSONObject* jsonObj=JSONObject::create();
    //  (2-2)存入数据
    jsonObj->putBool("isHappy", true);
    jsonObj->putNumber("num", 12);
    jsonObj->putString("word", "hello");
    //  (2-3)调用toString()方法
    CCLOG("%s",jsonObj->toString());
    
    //(3)序列化简单JSON数组
    //  (3-1)创建JSONArray
    JSONArray* jsonArray=JSONArray::create();
    //  (3-2)存入数据
    jsonArray->addBool(false);
    jsonArray->addNumber(21);
    jsonArray->addString("bye");
    //  (3-3)调用toString()方法
    CCLOG("%s",jsonArray->toString());
    
    //(4)JSONObject与JSONArray互相嵌套
    JSONObject* sonObj=JSONObject::create();
    sonObj->putNumber("age", 2.4);
    jsonObj->putJSONObject("son", sonObj);
    JSONArray* books=JSONArray::create();
    for (int i=0; i<3; i++) {
        JSONObject* bookObj=JSONObject::create();
        CCString* nameStr=CCString::createWithFormat("name_%d",i);
        bookObj->putString("name", nameStr->getCString());
        bookObj->putNumber("bid", 100+i);
        books->addJSONObject(bookObj);
    }
    jsonObj->putJSONArray("books", books);
    CCLOG("%s",jsonObj->toString());
    
}
