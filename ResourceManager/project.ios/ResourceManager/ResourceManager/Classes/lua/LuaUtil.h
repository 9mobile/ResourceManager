//
//  LuaUtil.h
//  Framework-x
//
//  Created by the9 on 13-3-13.
//
//

#ifndef __Framework_x__LuaUtil__
#define __Framework_x__LuaUtil__

#include <iostream>
#include "cocos2d.h"

class LuaUtil {
    
public:
    
    //注册lua脚本引擎
    static void registerLuaEngine();
    
    //设置lua包路径
    static void setPackagePath(std::vector<std::string> paths);
    
    //执行脚本文件
    static void executeScriptFile(const std::string& filePath);
    
    //运行全局的function
    static void executeGlobalFunction(const std::string& functionName,cocos2d::CCObject* param,const char* paramType);
    
    //运行table的function
    static void* executeTableFunction(const std::string& tableName,const std::string& functionName,cocos2d::CCArray* params,cocos2d::CCArray* paramTypes,bool hasReturnValue);
    
    //设置table的字段
    static void setTableFieid(const std::string& tableName,const std::string& key,cocos2d::CCObject* value,const std::string& valueType);
    
    //运行peertable的function
    static void* executePeertableFunction(cocos2d::CCObject* userdata,const std::string& functionName,cocos2d::CCArray* params,cocos2d::CCArray* paramTypes,bool hasReturnValue);
    
private:
    
    //获取包路径
    static const char* getPackagePath();
    
    //设置包路径
    static void setPackagePath(const std::string& path);
    
    //判断一个userdata是否有peertable
    static bool hasPeertable(void* userdata);
    
};

#endif /* defined(__Framework_x__LuaUtil__) */
