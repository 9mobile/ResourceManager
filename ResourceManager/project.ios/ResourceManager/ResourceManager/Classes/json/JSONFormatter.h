//
//  JSONFormatter.h
//  Framework-x
//
//  Created by the9 on 13-3-1.
//
//

#ifndef __Framework_x__JSONFormatter__
#define __Framework_x__JSONFormatter__

#include <iostream>
#include "JSONElement.h"
#include "libjson.h"

/**
 ******************JSONFormatter******************
 * json 格式化器
 */

class JSONFormatter {
    
private:
    
    static JSONNODE* formatToJSONNode(cocos2d::CCObject* object);
    
    static JSONNODE* formatJSONObjectToJSONNode(JSONObject* jsonObject);
    
    static JSONNODE* formatJSONArrayToJSONNode(JSONArray* jsonArray);
    
public:
    
    //将指定对象格式化为json字符串，对象的类型必需为JSONObject JSONArray二者之一
    static const char* format(cocos2d::CCObject* object,bool pretty);
    
};

#endif /* defined(__Framework_x__JSONFormatter__) */
