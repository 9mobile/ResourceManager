//
//  LuaUtil.cpp
//  Framework-x
//
//  Created by the9 on 13-3-13.
//
//

extern "C" {
#include "tolua++.h"
#include "tolua_fix.h"
}

#include "LuaUtil.h"
#include "cocos2d.h"
#include "CCLuaEngine.h"
#include "LuaResourceManager.h"

using namespace cocos2d;

static std::string defaultPackagePath;

void LuaUtil::registerLuaEngine()
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
    tolua_ResourceManager_open(pEngine->getLuaStack()->getLuaState());
}

void LuaUtil::setPackagePath(std::vector<std::string> paths)
{
    if (defaultPackagePath.size()==0) {
        defaultPackagePath = LuaUtil::getPackagePath();
    }
    std::string path=defaultPackagePath;
    for (std::vector<std::string>::const_iterator iter = paths.begin(); iter != paths.end(); ++iter)
    {
        path.append(";");
        path.append(*iter);
        path.append("/?.lua");
    }
    LuaUtil::setPackagePath(path);
}

void LuaUtil::executeScriptFile(const std::string &filePath)
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    CCString* pstrFileContent = CCString::createWithContentsOfFile(filePath.c_str());
    if (pstrFileContent)
    {
        pEngine->executeString(pstrFileContent->getCString());
    }
#else
    std::string path = CCFileUtils::sharedFileUtils()->fullPathForFilename(filePath.c_str());
    pEngine->executeScriptFile(path.c_str());
#endif
}

void LuaUtil::executeGlobalFunction(const std::string &functionName, cocos2d::CCObject *param, const char *paramType)
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    lua_State* m_state=pEngine->getLuaStack()->getLuaState();
    lua_getglobal(m_state, functionName.c_str());       /* query function by name, stack: function */
    if (!lua_isfunction(m_state, -1))
    {
        CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName.c_str());
        lua_pop(m_state, 1);
        return;
    }
    if (param!=NULL) {
        pEngine->getLuaStack()->pushCCObject(param, paramType==NULL?"CCObject":paramType);
    }
    int error = lua_pcall(m_state, param!=NULL?1:0, 0, 0);             /* call function, stack: ret */
    // lua_gc(m_state, LUA_GCCOLLECT, 0);
    
    if (error)
    {
        CCLOG("[LUA ERROR] %s", lua_tostring(m_state, - 1));
        lua_pop(m_state, 1); // clean error message
        return ;
    }
}

//运行table的function
void* LuaUtil::executeTableFunction(const std::string& tableName,const std::string& functionName,cocos2d::CCArray* params,cocos2d::CCArray* paramTypes,bool hasReturnValue)
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    lua_State* m_state=pEngine->getLuaStack()->getLuaState();
    
    //    printf("executeTableFunction [%s].[%s] start lua top:%d\n",tableName.c_str(),functionName.c_str(),lua_gettop(m_state));
    
    lua_getglobal(m_state, tableName.c_str());
    
    //检查栈顶是不是table
    if (!lua_istable(m_state, -1)) {
        CCLOG("[LUA ERROR] name '%s' does not represent a Lua Table", tableName.c_str());
        lua_pop(m_state, 1);
        return NULL;
    }
    
    //functionName入栈
    lua_pushstring(m_state, functionName.c_str());
    
    //从table中取function
    lua_gettable(m_state, -2);
    
    //检查栈顶是不是function
    if (!lua_isfunction(m_state, -1)) {
        CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName.c_str());
        lua_pop(m_state, 1);
        return NULL;
    }
    
    //table作为第一个参数入栈，保证lua使用table:function()调用方法时第一个参数self为table
    lua_pushvalue(m_state,-2);
    
    //参数入栈
    if (params!=NULL) {
        for (int i = 0; i < params->count(); i++) {
            CCObject* param = params->objectAtIndex(i);
            const char* paramType = ((CCString*)paramTypes->objectAtIndex(i))->getCString();
            //判断是否是基本类型
            if (strcmp(paramType, "CCBool") == 0) {
                
                bool boolValue = ((CCBool*)param)->getValue();
                lua_pushboolean(m_state, boolValue);
                
            }else if (strcmp(paramType, "CCInteger") == 0){
                
                int intValue = ((CCInteger*)param)->getValue();
                lua_pushnumber(m_state, intValue);
                
            }else if (strcmp(paramType, "CCFloat") == 0){
                
                float floatValue = ((CCFloat*)param)->getValue();
                lua_pushnumber(m_state, floatValue);
                
            }else if (strcmp(paramType, "CCDouble") == 0){
                
                double doubleValue = ((CCDouble*)param)->getValue();
                lua_pushnumber(m_state, doubleValue);
                
            }else if (strcmp(paramType, "CCString") == 0){
                
                const char* strValue = ((CCString*)param)->getCString();
                lua_pushstring(m_state, strValue);
                
            }else{
                
                pEngine->getLuaStack()->pushCCObject(param,paramType);
                
            }
        }
    }
    
    //调用function
    int error = lua_pcall(m_state, params!=NULL?params->count()+1:1, 0, 0);
    
    if (error)
    {
        CCLOG("[LUA ERROR] %s", lua_tostring(m_state, - 1));
        lua_pop(m_state, 1); // clean error message
        return NULL;
    }
    
    void* result = NULL;
    
    if (hasReturnValue) {
        
        if (lua_type(m_state, -1) == LUA_TBOOLEAN) {
            
            bool value = lua_toboolean(m_state, -1);
            result = CCBool::create(value);
            
        }else if (lua_type(m_state, -1) == LUA_TNUMBER) {
            
            double value = lua_tonumber(m_state, -1);
            result = CCDouble::create(value);
            
        }else if (lua_type(m_state, -1) == LUA_TSTRING) {
            
            const char* value = lua_tostring(m_state, -1);
            result = CCString::create(value);
            
        }else{
            
            result = tolua_tousertype(m_state, -1, NULL);
            
        }
        lua_pop(m_state, 1);
    }
    
    //弹出table
    lua_pop(m_state, 1);
    
    //    printf("executeTableFunction [%s].[%s] end lua top:%d\n",tableName.c_str(),functionName.c_str(),lua_gettop(m_state));
    
    return result;
}

void LuaUtil::setTableFieid(const std::string &tableName, const std::string &key, cocos2d::CCObject *value, const std::string &valueType)
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    lua_State* m_state=pEngine->getLuaStack()->getLuaState();
    //    printf("setTableFieid [%s].[%s] start lua top:%d\n",tableName.c_str(),key.c_str(),lua_gettop(m_state));
    //tableName入栈
    lua_getglobal(m_state, tableName.c_str());
    //检查栈顶是不是table
    if (!lua_istable(m_state, -1)) {
        CCLOG("[LUA ERROR] name '%s' does not represent a Lua Table", tableName.c_str());
        lua_pop(m_state, 1);
        return;
    }
    //key入栈
    lua_pushstring(m_state, key.c_str());
    pEngine->getLuaStack()->pushCCObject(value, valueType.c_str());
    lua_settable(m_state, -3);
    lua_pop(m_state, 1);
    //    printf("setTableFieid [%s].[%s] end lua top:%d\n",tableName.c_str(),key.c_str(),lua_gettop(m_state));
}

void* LuaUtil::executePeertableFunction(CCObject* userdata, const std::string &functionName, cocos2d::CCArray *params, cocos2d::CCArray *paramTypes, bool hasReturnValue)
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    lua_State* m_state=pEngine->getLuaStack()->getLuaState();
    
    if (!LuaUtil::hasPeertable(userdata)) {  //如果userdata没有peertable则直接返回
        return NULL;
    }
    //        printf("executePeertableFunction [%s] start lua top:%d\n",functionName.c_str(),lua_gettop(m_state));
    
    /* stack: userdata */
    tolua_pushusertype(m_state, userdata, "CCObject");
    
    //functionName入栈
    lua_pushstring(m_state, functionName.c_str());
    
    //从userdata中取function
    lua_gettable(m_state, -2);
    
    //检查栈顶是不是function
    if (!lua_isfunction(m_state, -1)) {
        CCLOG("[LUA ERROR] name '%s' does not represent a Lua function", functionName.c_str());
        lua_pop(m_state, 1);
        return NULL;
    }
    
    //userdata作为第一个参数入栈，保证lua使用userdata:function()调用方法时第一个参数self为userdata
    lua_pushvalue(m_state,-2);
    
    //参数入栈
    if (params!=NULL) {
        for (int i = 0; i < params->count(); i++) {
            CCObject* param = params->objectAtIndex(i);
            const char* paramType = ((CCString*)paramTypes->objectAtIndex(i))->getCString();
            //判断是否是基本类型
            if (strcmp(paramType, "CCBool") == 0) {
                
                bool boolValue = ((CCBool*)param)->getValue();
                lua_pushboolean(m_state, boolValue);
                
            }else if (strcmp(paramType, "CCInteger") == 0){
                
                int intValue = ((CCInteger*)param)->getValue();
                lua_pushnumber(m_state, intValue);
                
            }else if (strcmp(paramType, "CCFloat") == 0){
                
                float floatValue = ((CCFloat*)param)->getValue();
                lua_pushnumber(m_state, floatValue);
                
            }else if (strcmp(paramType, "CCDouble") == 0){
                
                double doubleValue = ((CCDouble*)param)->getValue();
                lua_pushnumber(m_state, doubleValue);
                
            }else if (strcmp(paramType, "CCString") == 0){
                
                const char* strValue = ((CCString*)param)->getCString();
                lua_pushstring(m_state, strValue);
                
            }else{
                
                pEngine->getLuaStack()->pushCCObject(param,paramType);
                
            }
        }
    }
    
    //调用function
    int error = lua_pcall(m_state, params!=NULL?params->count()+1:1, hasReturnValue?1:0, 0);
    
    if (error)
    {
        CCLOG("[LUA ERROR] %s", lua_tostring(m_state, - 1));
        lua_pop(m_state, 1); // clean error message
        return NULL;
    }
    
    void* result = NULL;
    if (hasReturnValue) {
        
        if (lua_type(m_state, -1) == LUA_TBOOLEAN) {
            
            bool value = lua_toboolean(m_state, -1);
            result = CCBool::create(value);
            
        }else if (lua_type(m_state, -1) == LUA_TNUMBER) {
            
            double value = lua_tonumber(m_state, -1);
            result = CCDouble::create(value);
            
        }else if (lua_type(m_state, -1) == LUA_TSTRING) {
            
            const char* value = lua_tostring(m_state, -1);
            result = CCString::create(value);
            
        }else{
            
            result = tolua_tousertype(m_state, -1, NULL);
            
        }
        lua_pop(m_state, 1);
    }
    
    //弹出userdata
    lua_pop(m_state, 1);
    
    //        printf("executePeertableFunction [%s] end lua top:%d\n",functionName.c_str(),lua_gettop(m_state));
    
    return result;
}

//=========================以下为私有API=========================

const char* LuaUtil::getPackagePath()
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    lua_State* m_state=pEngine->getLuaStack()->getLuaState();
    lua_getglobal(m_state, "package");                                  /* L: package */
    lua_getfield(m_state, -1, "path");                /* get package.path, L: package path */
    std::string path =  lua_tostring(m_state, -1);
    lua_pop(m_state, 1);
    return CCString::create(path)->getCString();
}

void LuaUtil::setPackagePath(const std::string &path)
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    lua_State* m_state=pEngine->getLuaStack()->getLuaState();
    lua_getglobal(m_state, "package");                          /* L: package */
    lua_pushstring(m_state, path.c_str());                      /* L: package newpath */
    lua_setfield(m_state, -2, "path");                          /* package.path = newpath, L: package */
    lua_pop(m_state, 1);                                        /* L: - */
}

bool LuaUtil::hasPeertable(void* userdata)
{
    CCLuaEngine* pEngine = CCLuaEngine::defaultEngine();
    lua_State* m_state=pEngine->getLuaStack()->getLuaState();
    
    //    printf("hasPeertable start lua top:%d\n",lua_gettop(m_state));
    
    bool flag = false;
    
    /* stack: userdata */
    tolua_pushusertype(m_state, userdata, "CCObject");
    
    lua_getfenv(m_state, -1);
    if (lua_rawequal(m_state, -1, TOLUA_NOPEER)) {
        flag = false;
    }else{
        flag = true;
    }
    lua_pop(m_state, 2);
    
    //    printf("hasPeertable end lua top:%d\n",lua_gettop(m_state));
    
    return flag;
}
