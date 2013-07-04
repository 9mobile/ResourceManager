//
//  JSONParser.cpp
//  Framework-x
//
//  Created by the9 on 13-3-1.
//
//

#include "JSONParser.h"

using namespace cocos2d;

CCObject* JSONParser::parseString(const std::string &jsonStr)
{
    JSONNODE* node=json_parse(jsonStr.c_str());
    CCObject* obj=JSONParser::parseJSONNode(node);
    json_delete(node);
    return obj;
}

CCObject* JSONParser::parseFile(const std::string &jsonFile)
{
    std::string stringBuffer;
    stringBuffer.clear();
    unsigned long fileDataSize = 0;
    unsigned char* fileData=CCFileUtils::sharedFileUtils()->getFileData(jsonFile.c_str(), "rb", &fileDataSize);
    if (fileData!=NULL) {
        for (int i=0; i<fileDataSize; i++) {
            stringBuffer.push_back(fileData[i]);
        }
        delete [] fileData;
    }
    return JSONParser::parseString(stringBuffer);
}

CCObject* JSONParser::parseJSONNode(JSONNODE *jsonNode)
{
    CCAssert(jsonNode!=NULL, "Invalid JSON Node");
    if (json_type(jsonNode)==JSON_NODE) {
        return JSONParser::parseToJSONObject(jsonNode);
    }else if (json_type(jsonNode)==JSON_ARRAY){
        return JSONParser::parseToJSONArray(jsonNode);
    }
    return NULL;
}

JSONObject* JSONParser::parseToJSONObject(JSONNODE* jsonNode)
{
    JSONObject* jsonObject=JSONObject::create();
    JSONNODE_ITERATOR i = json_begin(jsonNode);
    while (i != json_end(jsonNode)){
        if (*i == NULL){
            printf("Invalid JSON Node\n");
            return NULL;
        }
        char nodeType=json_type(*i);
        json_char *nodeName = json_name(*i);
        switch (nodeType) {
            case JSON_NULL:
            {
                //TODO
            }
                break;
            case JSON_BOOL:
            {
                jsonObject->putBool(nodeName, json_as_bool(*i));
            }
                break;
            case JSON_NUMBER:
            {
                jsonObject->putNumber(nodeName, json_as_float(*i));
            }
                break;
            case JSON_STRING:
            {
                json_char* nodeValue=json_as_string(*i);
                jsonObject->putString(nodeName, nodeValue);
                json_free(nodeValue);
            }
                break;
            case JSON_NODE:
            {
                jsonObject->putJSONObject(nodeName, JSONParser::parseToJSONObject(*i));
            }
                break;
            case JSON_ARRAY:
            {
                jsonObject->putJSONArray(nodeName, JSONParser::parseToJSONArray(*i));
            }
                break;
                
            default:
                break;
        }
        json_free(nodeName);
        ++i;
    }
    return jsonObject;
}

JSONArray* JSONParser::parseToJSONArray(JSONNODE* jsonNode)
{
    JSONArray* jsonArray=JSONArray::create();
    JSONNODE_ITERATOR i = json_begin(jsonNode);
    while (i != json_end(jsonNode)){
        if (*i == NULL){
            printf("Invalid JSON Node\n");
            return NULL;
        }
        char nodeType=json_type(*i);
        switch (nodeType) {
            case JSON_NULL:
            {
                //TODO
            }
                break;
            case JSON_BOOL:
            {
                jsonArray->addBool(json_as_bool(*i));
            }
                break;
            case JSON_NUMBER:
            {
                jsonArray->addNumber(json_as_float(*i));
            }
                break;
            case JSON_STRING:
            {
                json_char* value=json_as_string(*i);
                jsonArray->addString(value);
                json_free(value);
            }
                break;
            case JSON_NODE:
            {
                jsonArray->addJSONObject(JSONParser::parseToJSONObject(*i));
            }
                break;
            case JSON_ARRAY:
            {
                jsonArray->addJSONArray(JSONParser::parseToJSONArray(*i));
            }
                break;
                
            default:
                break;
        }
        ++i;
    }
    return jsonArray;
}