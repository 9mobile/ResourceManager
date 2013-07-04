//
//  ResourceManager.cpp
//  Framework-x
//
//  Created by the9 on 13-4-18.
//
//

#include "ResourceManager.h"
#include "FileManager.h"
#include "JSONElement.h"
#include "LuaUtil.h"

#define RMVersionFile                     "version.txt"                 //版本描述文件
#define RMUpdateZipFile                   "update.zip"                  //增量更新包
#define RMUpdateListZipFile               "updatelist.zip"              //模块更新列表包
#define RMModuleDirectory                 "module"                      //模块资源目录
#define RMModuleConfigDirectory           "module-config"               //模块配置目录
#define RMModuleConfigSuffix              ".txt"                        //模块资源配置文件后缀
#define RMServerUpdateDirectory           "Update"                      //增量更新包在资源服务器上的目录
#define RMServerResourceDirectory         "Resource"                    //最新资源包在资源服务器上的目录
#define RMLocalResourceDirectory          "Resource"                    //最新资源包在本地的目录
#define RMVersionPathFormat               "%0.2f/%0.2f"                 //更新包的版本目录名的格式
#define RMUpdateListFile                  "UpdateList.txt"              //更新列表文件
#define RMOldUpdateListFile               "UpdateList-old.txt"          //旧的更新列表文件（临时）

#define RMVersionKey                      "version"                     //本地的版本号在UserDefault中的键名，

#define BundleResourceVersion             1                             //如果程序包内无资源（微端模式），此处填0
#define RMDevelopMode                     false                          //开发模式标志

USING_NS_CC;

ResourceManager::ResourceManager()
{
    //实例化downloader
    downloader = new HttpDownloader();
    downloader->setDownloadCallback(this, (SEL_Download)&ResourceManager::onDownloaderResult);
    CC_SAFE_RETAIN(downloader);
    //创建模块待下载文件缓存
    downloadCache = CCDictionary::create();
    CC_SAFE_RETAIN(downloadCache);
    //初始化本地资源版本
    localVersion = CCUserDefault::sharedUserDefault()->getFloatForKey(RMVersionKey);
}

ResourceManager::~ResourceManager()
{
    CC_SAFE_RELEASE_NULL(downloader);
    CC_SAFE_RELEASE_NULL(downloadCache);
}

ResourceManager* ResourceManager::sharedResourceManager()
{
    static ResourceManager instance;
    return &instance;
}

void ResourceManager::setRootURL(const std::string& rootUrl)
{
    this->rootUrl = rootUrl;
    if (this->rootUrl.at(this->rootUrl.size()-1) != '/') {
        this->rootUrl.append("/");
    }
}

const char* ResourceManager::getRootURL()
{
    return this->rootUrl.c_str();
}

bool ResourceManager::isVersionLatest()
{
    return versionLatest;
}

bool ResourceManager::isDevelopMode()
{
    return RMDevelopMode;
}

void ResourceManager::configSearchPath()
{
    //(1) 清除所有SearchPath设置
    std::vector<std::string> emptySearchPaths;
    CCFileUtils::sharedFileUtils()->setSearchPaths(emptySearchPaths);
    //(3) 设置CCFileUtils的SearchPath和lua的PackagePath
    std::vector<std::string> searchPaths;
    std::vector<std::string> packagePaths;
    std::string scriptPath = "script";
    
    //WritablePath
    if (!this->isDevelopMode()) {
        searchPaths.push_back(this->fullLocalResourcePath(""));
        packagePaths.push_back(this->fullLocalResourcePath(scriptPath));
    }
    
    //Bundle
    searchPaths.push_back("");
    packagePaths.push_back(CCFileUtils::sharedFileUtils()->fullPathForFilename(scriptPath.c_str()));
    
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);
    LuaUtil::setPackagePath(packagePaths);
}

void ResourceManager::checkVersion()
{
    if (this->isDevelopMode()) {
        return;
    }
    //检查Bundle内资源版本，如果比Local内资源版本高，则先做一次更新
    if (localVersion < BundleResourceVersion) {
        //(1) TODO 删除Local下所有与module无关的文件
        //(2) 下载 UpdateList
        this->downloadUpdateList(localVersion, BundleResourceVersion);
    }else{
        //下载 版本描述文件
        this->downloadVersionFile();
    }
}

void ResourceManager::updateVersion()
{
    if (this->isDevelopMode()) {
        return;
    }
    if (localVersion < serverResourceVersion) {
        //(1) 下载地址
        std::string downloadPath = FileManager::splicePath(this->rootUrl, RMServerUpdateDirectory);
        CCString* versionPath = CCString::createWithFormat(RMVersionPathFormat,serverResourceVersion,localVersion);
        downloadPath = FileManager::splicePath(downloadPath, versionPath->getCString());
        downloadPath = FileManager::splicePath(downloadPath, RMUpdateZipFile);
        downloader->setUrl(downloadPath.c_str());
        //(2) 保存路径
        std::string savePath = this->fullLocalResourcePath(RMUpdateZipFile);
        downloader->setFullPath(savePath.c_str());
        //(3) 下载
        this->downloadFileType = kDownloadFileTypeUpdatePackage;
        downloader->download();
    }else{
        std::string errorMsg = LocalizedString("Resources are already up to date");
        this->reportError(errorMsg);
    }
}

int ResourceManager::checkModule(const std::string &moduleName)
{
    if (this->isDevelopMode()) {
        return 0;
    }
    //(1)检查模块是否存在
    std::string moduleConfigPath;
    moduleConfigPath.append(moduleName);
    moduleConfigPath.append(RMModuleConfigSuffix);
    moduleConfigPath = FileManager::splicePath(RMModuleConfigDirectory, moduleConfigPath);
    moduleConfigPath = FileManager::splicePath(RMModuleDirectory, moduleConfigPath);
    moduleConfigPath = CCFileUtils::sharedFileUtils()->fullPathForFilename(moduleConfigPath.c_str());
    if (!FileManager::fileExists(moduleConfigPath)) {
        std::string errorMsg = LocalizedString("module not exist");
        this->reportError(errorMsg);
        return -1;
    }
    //(2)取模块下载列表
    CCArray* downloadList = (CCArray*)downloadCache->objectForKey(moduleName);
    if (downloadList == NULL) {
        //缓存中没有当前模块的下载列表，则搜索比对生成列表并添加到缓存
        downloadList=CCArray::create();
        JSONObject* moduleObj = JSONObject::createWithFile(moduleConfigPath);
        JSONObject* updateListObj = JSONObject::createWithFile(CCFileUtils::sharedFileUtils()->fullPathForFilename(RMUpdateListFile));
        //将模块配置中存在，updateList中也存在的记录添加到列表
        CCArray* keyArray = moduleObj->allKeys();
        if (keyArray) {
            for (int i = 0; i < keyArray->count(); i++) {
                CCString* path = (CCString*)keyArray->objectAtIndex(i);
                if (updateListObj->has(path->getCString())) {
                    downloadList->addObject(path);
                }
            }
        }
        downloadCache->setObject(downloadList, moduleName);
    }
    return downloadList->count();
}

void ResourceManager::updateModule(const std::string &moduleName)
{
    if (this->isDevelopMode()) {
        return;
    }
    int fileNum = this->checkModule(moduleName);
    if (fileNum  ==  -1) {
        //表示检查版本时出错，错误回调已经在 checkModule 中进行
        return;
    }
    if (fileNum  ==  0) {
        std::string errorMsg = LocalizedString("Module is already up to date");
        this->reportError(errorMsg);
        return;
    }
    currentModuleName = moduleName;
    this->downloadModuleFile();
}

void ResourceManager::stopUpdate()
{
    
}

const char* ResourceManager::localizedString(const std::string &str)
{
    //TODO 待实现
    return str.c_str();
}

void ResourceManager::downloadVersionFile()
{
    //从服务器下载版本描述文件
    //(1) 下载地址
    std::string downloadPath = FileManager::splicePath(this->rootUrl, RMVersionFile);
    downloader->setUrl(downloadPath.c_str());
    //(2) 保存地址
    std::string savePath = this->fullLocalResourcePath(RMVersionFile);
    downloader->setFullPath(savePath.c_str());
    //(3) 下载
    this->downloadFileType = kDownloadFileTypeVersionFile;
    downloader->download();
}

void ResourceManager::downloadUpdateList(float versionOld, float versionNew)
{
    //从服务器上下载更新列表
    //(1) 下载地址
    std::string downloadPath = FileManager::splicePath(this->rootUrl, RMServerUpdateDirectory);
    CCString* versionPath = CCString::createWithFormat(RMVersionPathFormat,versionNew,versionOld);
    downloadPath = FileManager::splicePath(downloadPath, versionPath->getCString());
    downloadPath = FileManager::splicePath(downloadPath, RMUpdateListZipFile);
    downloader->setUrl(downloadPath.c_str());
    //(2) 保存地址
    std::string savePath = this->fullLocalResourcePath(RMUpdateListZipFile);
    downloader->setFullPath(savePath.c_str());
    //(3) 下载
    this->downloadFileType = kDownloadFileTypeUpdateListPackage;
    downloader->download();
}

void ResourceManager::downloadModuleFile()
{
    CCArray* downloadList=(CCArray*)downloadCache->objectForKey(currentModuleName);
    if (downloadList == NULL||downloadList->count() == 0) {
        return;
    }
    //下载第一个文件
    CCString* filePath=(CCString*)downloadList->objectAtIndex(0);
    //(1) 下载地址
    std::string downloadPath=FileManager::splicePath(this->rootUrl, RMServerResourceDirectory);
    downloadPath=FileManager::splicePath(downloadPath, filePath->getCString());
    downloader->setUrl(downloadPath.c_str());
    //(2) 保存地址
    std::string savePath=this->fullLocalResourcePath(filePath->getCString());
    downloader->setFullPath(savePath.c_str());
    //(3) 下载
    this->downloadFileType = kDownloadFileTypeModule;
    downloader->download();
}

void ResourceManager::onDownloaderResult(HttpDownloader *downloader, DownloadResult result)
{
    if (result == kDownloadSucceed) {
        //文件下载成功
        if (this->downloadFileType == kDownloadFileTypeVersionFile) {
            //版本描述文件
            this->compareVersion();
        }else if (this->downloadFileType == kDownloadFileTypeUpdatePackage){
            //增量更新包
            this->downloadUpdateList(localVersion, serverResourceVersion);
            //TODO 下载更新列表
        }else if (this->downloadFileType == kDownloadFileTypeUpdateListPackage){
            //更新列表
            this->updateVersionFormFile();
        }else{
            //模块文件
            this->processAfterSingleModleFileDownload();
        }
    }else if (result == kDownloadError){
        //文件下载出错
        if (this->delegate) {
            CCLOG("error:%s",downloader->getErrorMessage());
            this->reportError(downloader->getErrorMessage());
        }
    }else{
        //文件下载进度，不关心
    }
}

void ResourceManager::compareVersion()
{
    //(1)取版本描述文件内的版本号
    CCString* versionStr = CCString::createWithContentsOfFile(RMVersionFile);
    serverResourceVersion = versionStr->floatValue();
    //(3)比对
    versionLatest = (localVersion >= serverResourceVersion);
    if (this->delegate) {
        this->delegate->onVersionCheckCompleted(false);
    }
    
#if ScriptType ==1
    if (this->scriptDelegate) {
        CCArray* params = CCArray::createWithObject(CCBool::create(versionLatest));
        CCArray* paramTypes = CCArray::createWithObject(CCString::create("CCBool"));
        LuaUtil::executePeertableFunction(this->scriptDelegate, "onVersionCheckCompleted", params, paramTypes, false);
    }
#endif
}

void ResourceManager::updateVersionFormFile()
{
    //(1)解压update.zip
    const std::string zipFilePath  =  this->fullLocalResourcePath(RMUpdateZipFile);
    if (FileManager::fileExists(zipFilePath)) {
        bool status = FileManager::uncompressZipFile(zipFilePath, this->fullLocalResourcePath(""));
        if (!status) {
            std::string errorMsg = LocalizedString("uncompress zip file error");
            this->reportError(errorMsg);
            //TODO 解压出错的情况下是否需要还原第一步的操作有待商榷，在强制更新的前提下，如果更新失败，下次登录时，逻辑仍然会走到这里，则不需要还原。
            return;
        }else{
            FileManager::removeFile(zipFilePath.c_str());
        }
    }
    //(2)如果存在updateList则改名为updateList-old
    std::string updateListOld = this->fullLocalResourcePath(RMOldUpdateListFile);
    std::string updateList = this->fullLocalResourcePath(RMUpdateListFile);
    if (FileManager::fileExists(updateList)) {
        FileManager::renameFile(updateList, updateListOld);
    }
    //(3)解压updatelist.zip
    const std::string updateListZipFile  =  this->fullLocalResourcePath(RMUpdateListZipFile);
    bool status = FileManager::uncompressZipFile(updateListZipFile, this->fullLocalResourcePath(""));
    if (!status) {
        std::string errorMsg = LocalizedString("uncompress zip file error");
        this->reportError(errorMsg);
        return;
    }else{
        FileManager::removeFile(updateListZipFile.c_str());
    }
    //(4)updateList-old与updateList合并，冲突时以updateList为准
    if (FileManager::fileExists(updateListOld)) {
        this->mergeUpdateList(updateListOld, updateList,updateList);
        FileManager::removeFile(updateListOld.c_str());
    }
    //(5)更新完成，进行后续操作
    if (localVersion < BundleResourceVersion) {
        //从Bundle进行的更新（记录资源版本,下载版本描述文件)
        this->saveVersion(BundleResourceVersion);
        this->downloadVersionFile();
    }else{
        //从Server进行的更新（记录资源版本,更新SearchPath)
        this->saveVersion(serverResourceVersion);
        versionLatest=true;
        
        this->configSearchPath();
        if (this->delegate) {
            this->delegate->onVersionUpdated();
        }
#if ScriptType ==1
        if (this->scriptDelegate) {
            LuaUtil::executePeertableFunction(this->scriptDelegate, "onVersionUpdated", NULL, NULL, false);
        }
#endif
    }
}

void ResourceManager::processAfterSingleModleFileDownload()
{
    CCArray* downloadList = (CCArray*)downloadCache->objectForKey(currentModuleName);
    CCString* filePath = (CCString*)downloadList->objectAtIndex(0);
    //回调
    if (this->delegate) {
        this->delegate->onSingleFileUpdated(filePath->getCString());
    }
#if ScriptType ==1
    if (this->scriptDelegate) {
        CCArray* params = CCArray::createWithObject(filePath);
        CCArray* paramTypes = CCArray::createWithObject(CCString::create("CCString"));
        LuaUtil::executePeertableFunction(this->scriptDelegate, "onSingleFileUpdated", params, paramTypes, false);
    }
#endif
    //从updateList中删除
    std::string updateListFile = this->fullLocalResourcePath(RMUpdateListFile);
    JSONObject* updateListObj=JSONObject::createWithFile(updateListFile);
    updateListObj->remove(filePath->getCString());
    //回写updateList
    FileManager::writeStringToFile(updateListObj->toPrettyString(), updateListFile);
    //从缓存列表中删除
    downloadList->removeObjectAtIndex(0);
    //检查缓存列表
    if (downloadList->count() == 0) {
        if (this->delegate) {
            this->delegate->onModuleUpdated(this->currentModuleName);
        }
#if ScriptType ==1
        if (this->scriptDelegate) {
            CCArray* params = CCArray::createWithObject(CCString::create(this->currentModuleName));
            CCArray* paramTypes = CCArray::createWithObject(CCString::create("CCString"));
            LuaUtil::executePeertableFunction(this->scriptDelegate, "onModuleUpdated", params, paramTypes, false);
        }
#endif
    }else{
        this->downloadModuleFile();
    }
}

void ResourceManager::mergeUpdateList(const std::string &oldUpdateList, const std::string &newUpdateList, const std::string &savePath)
{
    JSONObject* fileJsonObjNew = JSONObject::createWithFile(newUpdateList);
    if (FileManager::fileExists(oldUpdateList)) {
        JSONObject* fileJsonObjOld = JSONObject::createWithFile(oldUpdateList);
        //将旧配置中存在，新配置中不存在的项目添加到新配置
        CCArray* keyArray=fileJsonObjOld->allKeys();
        if (keyArray) {
            for (int i = 0; i < keyArray->count(); i++) {
                std::string filePath=((CCString*)keyArray->objectAtIndex(i))->getCString();
                if (!fileJsonObjNew->has(filePath)) {
                    fileJsonObjNew->putString(filePath, fileJsonObjOld->getString(filePath));
                }
            }
        }
    }
    //保存新配置
    FileManager::writeStringToFile(fileJsonObjNew->toPrettyString(), savePath);
}

void ResourceManager::saveVersion(float version)
{
    localVersion = version;
    CCUserDefault::sharedUserDefault()->setFloatForKey(RMVersionKey, localVersion);
    CCUserDefault::sharedUserDefault()->flush();
}

const char* ResourceManager::fullLocalResourcePath(const std::string& filePath)
{
    std::string localResourcePath = FileManager::splicePath(CCFileUtils::sharedFileUtils()->getWritablePath(), RMLocalResourceDirectory);
    return FileManager::splicePath(localResourcePath, filePath);
}

void ResourceManager::reportError(const std::string &errorMsg)
{
    if (this->delegate) {
        this->delegate->onErrorOccured(errorMsg.c_str());
    }
#if ScriptType ==1
    if (this->scriptDelegate) {
        CCArray* params = CCArray::createWithObject(CCString::create(errorMsg));
        CCArray* paramTypes = CCArray::createWithObject(CCString::create("CCString"));
        LuaUtil::executePeertableFunction(this->scriptDelegate, "onErrorOccured", params, paramTypes, false);
    }
#endif
}
