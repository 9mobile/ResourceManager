/*
** Lua binding: ResourceManager
** Generated automatically by tolua++-1.0.92 on Wed Jul  3 14:23:36 2013.
*/

/****************************************************************************
 Copyright (c) 2011 cocos2d-x.org

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/


#include "LuaResourceManager.h"

/* Exported function */
TOLUA_API int  tolua_ResourceManager_open (lua_State* tolua_S);

/* function to register type */
static void tolua_reg_types (lua_State* tolua_S)
{
 tolua_usertype(tolua_S,"ResourceManager");
 tolua_usertype(tolua_S,"ResourceUpdateDelegate");
 tolua_usertype(tolua_S,"CCObject");
}

/* method: onVersionCheckCompleted of class  ResourceUpdateDelegate */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceUpdateDelegate_onVersionCheckCompleted00
static int tolua_ResourceManager_ResourceUpdateDelegate_onVersionCheckCompleted00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceUpdateDelegate",0,&tolua_err) ||
     !tolua_isboolean(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceUpdateDelegate* self = (ResourceUpdateDelegate*)  tolua_tousertype(tolua_S,1,0);
  bool latest = ((bool)  tolua_toboolean(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onVersionCheckCompleted'", NULL);
#endif
  {
   self->onVersionCheckCompleted(latest);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onVersionCheckCompleted'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onVersionUpdated of class  ResourceUpdateDelegate */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceUpdateDelegate_onVersionUpdated00
static int tolua_ResourceManager_ResourceUpdateDelegate_onVersionUpdated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceUpdateDelegate",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceUpdateDelegate* self = (ResourceUpdateDelegate*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onVersionUpdated'", NULL);
#endif
  {
   self->onVersionUpdated();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onVersionUpdated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onModuleUpdated of class  ResourceUpdateDelegate */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceUpdateDelegate_onModuleUpdated00
static int tolua_ResourceManager_ResourceUpdateDelegate_onModuleUpdated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceUpdateDelegate",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceUpdateDelegate* self = (ResourceUpdateDelegate*)  tolua_tousertype(tolua_S,1,0);
  const std::string moduleName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onModuleUpdated'", NULL);
#endif
  {
   self->onModuleUpdated(moduleName);
   tolua_pushcppstring(tolua_S,(const char*)moduleName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onModuleUpdated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onSingleFileUpdated of class  ResourceUpdateDelegate */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceUpdateDelegate_onSingleFileUpdated00
static int tolua_ResourceManager_ResourceUpdateDelegate_onSingleFileUpdated00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceUpdateDelegate",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceUpdateDelegate* self = (ResourceUpdateDelegate*)  tolua_tousertype(tolua_S,1,0);
  const std::string filePath = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onSingleFileUpdated'", NULL);
#endif
  {
   self->onSingleFileUpdated(filePath);
   tolua_pushcppstring(tolua_S,(const char*)filePath);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onSingleFileUpdated'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: onErrorOccured of class  ResourceUpdateDelegate */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceUpdateDelegate_onErrorOccured00
static int tolua_ResourceManager_ResourceUpdateDelegate_onErrorOccured00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceUpdateDelegate",0,&tolua_err) ||
     !tolua_isstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceUpdateDelegate* self = (ResourceUpdateDelegate*)  tolua_tousertype(tolua_S,1,0);
  const char* message = ((const char*)  tolua_tostring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'onErrorOccured'", NULL);
#endif
  {
   self->onErrorOccured(message);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'onErrorOccured'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: sharedResourceManager of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_sharedResourceManager00
static int tolua_ResourceManager_ResourceManager_sharedResourceManager00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertable(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  {
   ResourceManager* tolua_ret = (ResourceManager*)  ResourceManager::sharedResourceManager();
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"ResourceManager");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'sharedResourceManager'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setDelegate of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_setDelegate00
static int tolua_ResourceManager_ResourceManager_setDelegate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"ResourceUpdateDelegate",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  ResourceUpdateDelegate* delegate = ((ResourceUpdateDelegate*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setDelegate'", NULL);
#endif
  {
   self->setDelegate(delegate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setDelegate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getDelegate of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_getDelegate00
static int tolua_ResourceManager_ResourceManager_getDelegate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getDelegate'", NULL);
#endif
  {
   ResourceUpdateDelegate* tolua_ret = (ResourceUpdateDelegate*)  self->getDelegate();
    tolua_pushusertype(tolua_S,(void*)tolua_ret,"ResourceUpdateDelegate");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getDelegate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setScriptDelegate of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_setScriptDelegate00
static int tolua_ResourceManager_ResourceManager_setScriptDelegate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isusertype(tolua_S,2,"CCObject",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  CCObject* scriptDelegate = ((CCObject*)  tolua_tousertype(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setScriptDelegate'", NULL);
#endif
  {
   self->setScriptDelegate(scriptDelegate);
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setScriptDelegate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getScriptDelegate of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_getScriptDelegate00
static int tolua_ResourceManager_ResourceManager_getScriptDelegate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getScriptDelegate'", NULL);
#endif
  {
   CCObject* tolua_ret = (CCObject*)  self->getScriptDelegate();
    int nID = (tolua_ret) ? (int)tolua_ret->m_uID : -1;
    int* pLuaID = (tolua_ret) ? &tolua_ret->m_nLuaID : NULL;
    toluafix_pushusertype_ccobject(tolua_S, nID, pLuaID, (void*)tolua_ret,"CCObject");
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getScriptDelegate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: setRootURL of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_setRootURL00
static int tolua_ResourceManager_ResourceManager_setRootURL00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string rootUrl = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'setRootURL'", NULL);
#endif
  {
   self->setRootURL(rootUrl);
   tolua_pushcppstring(tolua_S,(const char*)rootUrl);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'setRootURL'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getRootURL of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_getRootURL00
static int tolua_ResourceManager_ResourceManager_getRootURL00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getRootURL'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->getRootURL();
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getRootURL'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isVersionLatest of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_isVersionLatest00
static int tolua_ResourceManager_ResourceManager_isVersionLatest00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isVersionLatest'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isVersionLatest();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isVersionLatest'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: getVersion of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_getVersion00
static int tolua_ResourceManager_ResourceManager_getVersion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'getVersion'", NULL);
#endif
  {
   float tolua_ret = (float)  self->getVersion();
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'getVersion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: isDevelopMode of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_isDevelopMode00
static int tolua_ResourceManager_ResourceManager_isDevelopMode00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'isDevelopMode'", NULL);
#endif
  {
   bool tolua_ret = (bool)  self->isDevelopMode();
   tolua_pushboolean(tolua_S,(bool)tolua_ret);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'isDevelopMode'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: checkVersion of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_checkVersion00
static int tolua_ResourceManager_ResourceManager_checkVersion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'checkVersion'", NULL);
#endif
  {
   self->checkVersion();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'checkVersion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateVersion of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_updateVersion00
static int tolua_ResourceManager_ResourceManager_updateVersion00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateVersion'", NULL);
#endif
  {
   self->updateVersion();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateVersion'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: checkModule of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_checkModule00
static int tolua_ResourceManager_ResourceManager_checkModule00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string moduleName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'checkModule'", NULL);
#endif
  {
   int tolua_ret = (int)  self->checkModule(moduleName);
   tolua_pushnumber(tolua_S,(lua_Number)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)moduleName);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'checkModule'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: updateModule of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_updateModule00
static int tolua_ResourceManager_ResourceManager_updateModule00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string moduleName = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'updateModule'", NULL);
#endif
  {
   self->updateModule(moduleName);
   tolua_pushcppstring(tolua_S,(const char*)moduleName);
  }
 }
 return 1;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'updateModule'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: stopUpdate of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_stopUpdate00
static int tolua_ResourceManager_ResourceManager_stopUpdate00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,2,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'stopUpdate'", NULL);
#endif
  {
   self->stopUpdate();
  }
 }
 return 0;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'stopUpdate'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* method: localizedString of class  ResourceManager */
#ifndef TOLUA_DISABLE_tolua_ResourceManager_ResourceManager_localizedString00
static int tolua_ResourceManager_ResourceManager_localizedString00(lua_State* tolua_S)
{
#ifndef TOLUA_RELEASE
 tolua_Error tolua_err;
 if (
     !tolua_isusertype(tolua_S,1,"ResourceManager",0,&tolua_err) ||
     !tolua_iscppstring(tolua_S,2,0,&tolua_err) ||
     !tolua_isnoobj(tolua_S,3,&tolua_err)
 )
  goto tolua_lerror;
 else
#endif
 {
  ResourceManager* self = (ResourceManager*)  tolua_tousertype(tolua_S,1,0);
  const std::string str = ((const std::string)  tolua_tocppstring(tolua_S,2,0));
#ifndef TOLUA_RELEASE
  if (!self) tolua_error(tolua_S,"invalid 'self' in function 'localizedString'", NULL);
#endif
  {
   const char* tolua_ret = (const char*)  self->localizedString(str);
   tolua_pushstring(tolua_S,(const char*)tolua_ret);
   tolua_pushcppstring(tolua_S,(const char*)str);
  }
 }
 return 2;
#ifndef TOLUA_RELEASE
 tolua_lerror:
 tolua_error(tolua_S,"#ferror in function 'localizedString'.",&tolua_err);
 return 0;
#endif
}
#endif //#ifndef TOLUA_DISABLE

/* Open function */
TOLUA_API int tolua_ResourceManager_open (lua_State* tolua_S)
{
 tolua_open(tolua_S);
 tolua_reg_types(tolua_S);
 tolua_module(tolua_S,NULL,0);
 tolua_beginmodule(tolua_S,NULL);
  tolua_cclass(tolua_S,"ResourceUpdateDelegate","ResourceUpdateDelegate","CCObject",NULL);
  tolua_beginmodule(tolua_S,"ResourceUpdateDelegate");
   tolua_function(tolua_S,"onVersionCheckCompleted",tolua_ResourceManager_ResourceUpdateDelegate_onVersionCheckCompleted00);
   tolua_function(tolua_S,"onVersionUpdated",tolua_ResourceManager_ResourceUpdateDelegate_onVersionUpdated00);
   tolua_function(tolua_S,"onModuleUpdated",tolua_ResourceManager_ResourceUpdateDelegate_onModuleUpdated00);
   tolua_function(tolua_S,"onSingleFileUpdated",tolua_ResourceManager_ResourceUpdateDelegate_onSingleFileUpdated00);
   tolua_function(tolua_S,"onErrorOccured",tolua_ResourceManager_ResourceUpdateDelegate_onErrorOccured00);
  tolua_endmodule(tolua_S);
  tolua_cclass(tolua_S,"ResourceManager","ResourceManager","CCObject",NULL);
  tolua_beginmodule(tolua_S,"ResourceManager");
   tolua_function(tolua_S,"sharedResourceManager",tolua_ResourceManager_ResourceManager_sharedResourceManager00);
   tolua_function(tolua_S,"setDelegate",tolua_ResourceManager_ResourceManager_setDelegate00);
   tolua_function(tolua_S,"getDelegate",tolua_ResourceManager_ResourceManager_getDelegate00);
   tolua_function(tolua_S,"setScriptDelegate",tolua_ResourceManager_ResourceManager_setScriptDelegate00);
   tolua_function(tolua_S,"getScriptDelegate",tolua_ResourceManager_ResourceManager_getScriptDelegate00);
   tolua_function(tolua_S,"setRootURL",tolua_ResourceManager_ResourceManager_setRootURL00);
   tolua_function(tolua_S,"getRootURL",tolua_ResourceManager_ResourceManager_getRootURL00);
   tolua_function(tolua_S,"isVersionLatest",tolua_ResourceManager_ResourceManager_isVersionLatest00);
   tolua_function(tolua_S,"getVersion",tolua_ResourceManager_ResourceManager_getVersion00);
   tolua_function(tolua_S,"isDevelopMode",tolua_ResourceManager_ResourceManager_isDevelopMode00);
   tolua_function(tolua_S,"checkVersion",tolua_ResourceManager_ResourceManager_checkVersion00);
   tolua_function(tolua_S,"updateVersion",tolua_ResourceManager_ResourceManager_updateVersion00);
   tolua_function(tolua_S,"checkModule",tolua_ResourceManager_ResourceManager_checkModule00);
   tolua_function(tolua_S,"updateModule",tolua_ResourceManager_ResourceManager_updateModule00);
   tolua_function(tolua_S,"stopUpdate",tolua_ResourceManager_ResourceManager_stopUpdate00);
   tolua_function(tolua_S,"localizedString",tolua_ResourceManager_ResourceManager_localizedString00);
  tolua_endmodule(tolua_S);
 tolua_endmodule(tolua_S);
 return 1;
}


#if defined(LUA_VERSION_NUM) && LUA_VERSION_NUM >= 501
 TOLUA_API int luaopen_ResourceManager (lua_State* tolua_S) {
 return tolua_ResourceManager_open(tolua_S);
};
#endif

