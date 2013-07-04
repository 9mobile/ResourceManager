//
//  JSONParserTest.cpp
//  Framework-x
//
//  Created by the9 on 13-3-1.
//
//

#include "JSONParserTest.h"
#include "JSONElement.h"

using namespace cocos2d;

void JSONParserTest::runTest()
{
    //(1)导入头文件 JSONElement.h
    
    //(2)从字符串反序列化 JSON对象
    //  (2-1)待反序列化的json字符串
    std::string jsonStr="{\"name\":\"wp\",\"id\":12.8,\"tall\":true,\"phone\":[12,23,14],\"son\":{\"name\":\"wxp\"},\"books\":[{\"name\":\"lenovo\"},{\"name\":\"mac pro\"},{\"name\":\"other\"}]}";
    //  (2-2)调用createWithString静态方法创建JSONObject
    JSONObject* jsonObj=JSONObject::createWithString(jsonStr);
    //  (2-3)根据json结构取数据
    double pid=jsonObj->getNumber("id");
    CCLOG("pid=%lf",pid);
    std::string name=jsonObj->getString("name");
    CCLOG("name=%s",name.c_str());
    bool tall=jsonObj->getBool("tall");
    CCLOG("tall=%d",tall);
    JSONArray* jsonArray=jsonObj->getJSONArray("phone");
    CCLOG("phone size:%d",jsonArray->count());
    int phid=jsonArray->getNumber(1);
    CCLOG("the id of 2nd phone is :%d",phid);
    JSONObject* sonObj=jsonObj->getJSONObject("son");
    CCLOG("son name:%s",sonObj->getString("name"));
    JSONArray* books=jsonObj->getJSONArray("books");
    CCLOG("book size:%d",books->count());
    JSONObject* macObj=books->getJSONObject(1);
    CCLOG("the name of 2nd book is :%s",macObj->getString("name"));
    
    //(3)从字符串反序列化 JSON数组
    //  (3-1)待反序列化的json字符串
    jsonStr="[1,2,3,4,{\"name\":\"the9\"}]";
    //  (3-2)调用createWithString静态方法创建JSONArray
    jsonArray=JSONArray::createWithString(jsonStr);
    //  (3-3)根据json结构取数据
    CCLOG("array size : %d",jsonArray->count());
    CCLOG("first element:%lf",jsonArray->getNumber(0));
    JSONObject* wpObj=jsonArray->getJSONObject(4);
    CCLOG("name : %s",wpObj->getString("name"));
    
    //(4)从文件反序列化
    //  (4-1)待反序列化文件（大武侠战报）
    std::string path=CCFileUtils::sharedFileUtils()->fullPathForFilename("battle.json");
    //  (4-2)调用createWithFile静态方法创建JSONArray
    jsonArray=JSONArray::createWithFile(path);
    //  (4-3)根据json结构取数据
    JSONObject* battleObj=jsonArray->getJSONObject(0);
    JSONObject* reportObj=battleObj->getJSONObject("report");
    JSONObject* roleinfoObj=reportObj->getJSONObject("roleinfo");
    JSONObject* player1Obj=roleinfoObj->getJSONObject("1");
    name=player1Obj->getString("name");
    CCLOG("第一个角色的呢称是:%s",name.c_str());
    JSONObject* round_dataObj=reportObj->getJSONObject("round_data");
    JSONObject* round1Obj=round_dataObj->getJSONObject("1");
    JSONObject* normal_dataObj=round1Obj->getJSONObject("normal_data");
    JSONObject* normal_data1Obj=normal_dataObj->getJSONObject("1");
    JSONArray* mpArray=normal_data1Obj->getJSONArray("mp");
    int mp=mpArray->getNumber(0);
    CCLOG("第一回合角色1的mp变化值为:%d",mp);
    
}
