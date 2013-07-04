//
//  JSONFormatter.cpp
//  Framework-x
//
//  Created by the9 on 13-3-1.
//
//

#include "JSONFormatter.h"

using namespace cocos2d;

const char* JSONFormatter::format(CCObject *object, bool pretty)
{
    JSONNODE* jsonNode=JSONFormatter::formatToJSONNode(object);
    json_char *jsonChar = pretty?json_write_formatted(jsonNode):json_write(jsonNode);
    CCString* ccstr=CCString::create(jsonChar);
    json_free(jsonChar);
    json_delete(jsonNode);
    return ccstr->getCString();
}

JSONNODE* JSONFormatter::formatToJSONNode(CCObject* object)
{
    CCAssert(object, "Invalid Object");
    JSONObject* jsonObject=NULL;
    JSONArray* jsonArray=NULL;
    if ((jsonObject=dynamic_cast<JSONObject*>(object))!=NULL) {
        return JSONFormatter::formatJSONObjectToJSONNode(jsonObject);
    }else if ((jsonArray=dynamic_cast<JSONArray*>(object))!=NULL){
        return JSONFormatter::formatJSONArrayToJSONNode(jsonArray);
    }
    CCAssert(false, "Invalid Object Type");
    return NULL;
}

JSONNODE* JSONFormatter::formatJSONObjectToJSONNode(JSONObject* jsonObject)
{
    CCAssert(jsonObject, "Invalid JSONObject");
    JSONNODE *jsonNode = json_new(JSON_NODE);
    int count=jsonObject->count();
    CCArray* allKeys=jsonObject->allKeys();
    for (int i=0;i<count;i++) {
        CCString* key=(CCString*)allKeys->objectAtIndex(i);
        CCObject* obj=jsonObject->get(key->getCString());
        JSONBool* jsonBool=NULL;
        JSONNumber* jsonNumber=NULL;
        JSONString* jsonString=NULL;
        JSONObject* jsonObj=NULL;
        JSONArray* jsonArray=NULL;
        if ((jsonBool=dynamic_cast<JSONBool*>(obj))!=NULL) {
            json_push_back(jsonNode, json_new_b(key->getCString(), jsonBool->boolValue()));
        }else if ((jsonNumber=dynamic_cast<JSONNumber*>(obj))!=NULL){
            json_push_back(jsonNode, json_new_f(key->getCString(), jsonNumber->numberValue()));
        }else if ((jsonString=dynamic_cast<JSONString*>(obj))!=NULL){
            json_push_back(jsonNode, json_new_a(key->getCString(), jsonString->stringValue()));
        }else if ((jsonObj=dynamic_cast<JSONObject*>(obj))!=NULL){
            JSONNODE* nodeObj=JSONFormatter::formatJSONObjectToJSONNode(jsonObj);
            json_set_name(nodeObj, key->getCString());
            json_push_back(jsonNode, nodeObj);
        }else if ((jsonArray=dynamic_cast<JSONArray*>(obj))!=NULL){
            JSONNODE* nodeArray=JSONFormatter::formatJSONArrayToJSONNode(jsonArray);
            json_set_name(nodeArray, key->getCString());
            json_push_back(jsonNode, nodeArray);
        }
    }
    return jsonNode;
}

JSONNODE* JSONFormatter::formatJSONArrayToJSONNode(JSONArray* jsonArray)
{
    CCAssert(jsonArray, "Invalid JSONArray");
    JSONNODE *jsonNode = json_new(JSON_ARRAY);
    int count=jsonArray->count();
    for (int i=0;i<count;i++) {
        CCObject* obj=jsonArray->get(i);
        JSONBool* jsonBool=NULL;
        JSONNumber* jsonNumber=NULL;
        JSONString* jsonString=NULL;
        JSONObject* jsonObj=NULL;
        JSONArray* jsonArray=NULL;
        if ((jsonBool=dynamic_cast<JSONBool*>(obj))!=NULL) {
            json_push_back(jsonNode, json_new_b(NULL, jsonBool->boolValue()));
        }else if ((jsonNumber=dynamic_cast<JSONNumber*>(obj))!=NULL){
            json_push_back(jsonNode, json_new_f(NULL, jsonNumber->numberValue()));
        }else if ((jsonString=dynamic_cast<JSONString*>(obj))!=NULL){
            json_push_back(jsonNode, json_new_a(NULL, jsonString->stringValue()));
        }else if ((jsonObj=dynamic_cast<JSONObject*>(obj))!=NULL){
            json_push_back(jsonNode, JSONFormatter::formatJSONObjectToJSONNode(jsonObj));
        }else if ((jsonArray=dynamic_cast<JSONArray*>(obj))!=NULL){
            json_push_back(jsonNode, JSONFormatter::formatJSONArrayToJSONNode(jsonArray));
        }
    }
    return jsonNode;
}