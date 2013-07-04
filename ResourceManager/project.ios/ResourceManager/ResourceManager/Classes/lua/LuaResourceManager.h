//
//  LuaFramework.h
//  Framework-x
//
//  Created by the9 on 13-3-21.
//
//

#ifndef Framework_x_LuaFramework_h
#define Framework_x_LuaFramework_h

extern "C" {
#include "tolua++.h"
#include "tolua_fix.h"
}

#include <map>
#include <string>
#include "tolua_fix.h"
#include "CCLuaValue.h"
#include "cocos2d.h"
#include "ResourceManager.h"

using namespace cocos2d;

TOLUA_API int tolua_ResourceManager_open(lua_State* tolua_S);

#endif
