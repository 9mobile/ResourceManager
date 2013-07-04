//
//  ResourceManager.h
//  Framework-x
//
//  Created by the9 on 13-4-18.
//
//

#ifndef __Framework_x__ResourceManager__
#define __Framework_x__ResourceManager__

#include <iostream>
#include "cocos2d.h"
#include "HttpDownloader.h"

class ResourceUpdateDelegate :public cocos2d::CCObject{
    
public:
    
    virtual ~ResourceUpdateDelegate(){}
    
    /**
     * 资源版本检查回调
     * @param latest [true]客户端版本已经最新,不需要更新;[false]客户端版本已过时,需要更新
     */
    virtual void onVersionCheckCompleted(bool latest){};
    
    /**
     * 版本更新完毕
     */
    virtual void onVersionUpdated(){};
    
    /**
     * 模块更新完毕
     */
    virtual void onModuleUpdated(const std::string& moduleName){};
    
    /**
     * 单个文件更新的回调
     */
    virtual void onSingleFileUpdated(const std::string& filePath){};
    
    /**
     * 资源检查、更新过程中出错的回调
     * @param 错误消息
     */
    virtual void onErrorOccured(const char* message){};
    
};

#define LocalizedString(str)    ResourceManager::sharedResourceManager()->localizedString(str)

/**
 ******************ResourceManager******************
 * 资源管理器
 */
class ResourceManager :public cocos2d::CCObject{
    
    
public:
    
    static ResourceManager* sharedResourceManager();
    
    /**
     * 设置、获取资源更新事件委托
     */
    CC_SYNTHESIZE_RETAIN(ResourceUpdateDelegate*, delegate, Delegate);
    
#if ScriptType == 1
    
    /**
     * 设置、获取资源更新事件委托（脚本）
     */
    CC_SYNTHESIZE(cocos2d::CCObject*, scriptDelegate, ScriptDelegate);
    
#endif
    
    /**
     * 设置资源根地址
     */
    void setRootURL(const std::string& rootUrl);
    
    /**
     * 获取资源根地址
     */
    const char* getRootURL();
    
    /**
     * 版本是否更新
     */
    bool isVersionLatest();
    
    /**
     * 获取版本
     */
    CC_SYNTHESIZE_READONLY(float, localVersion, Version);
    
    /**
     * 是否是开发模式
     */
    bool isDevelopMode();
    
    /**
     * 更新SearchPath设置
     */
    void configSearchPath();
    
    /**
     * 检查版本号
     * 异步接口，检查完毕后会回调delegate的versionCheckCompleted接口
     */
    void checkVersion();
    
    /*
     * 更新版本
     */
    void updateVersion();
    
    /**
     * 检查模块资源版本
     * @return 需要更新的文件数量，如果是0表示模块不需要更新
     */
    int checkModule(const std::string& moduleName);
    
    /**
     * 更新模块资源
     */
    void updateModule(const std::string& moduleName);
    
    /**
     * 停止更新
     */
    void stopUpdate();
    
    /**
     * 本地化字符串
     */
    const char* localizedString(const std::string& str);
    
    /**
     *  下载器下载结果回调
     */
    void onDownloaderResult(HttpDownloader* downloader,DownloadResult result);
    
    
    ~ResourceManager();
    
private:
    
    enum DownloadFileType {
        kDownloadFileTypeVersionFile,               //version说明文件下载
        kDownloadFileTypeUpdatePackage,             //全局update包下载
        kDownloadFileTypeUpdateListPackage,         //更新列表包下载
        kDownloadFileTypeModule                     //模块资源下载
    };
    
    /**
     * 注：与资源服务器生成的updateList保持一致，不可随意改动
     */
    enum FileOperateType{
        kFileOperateTypeCreate=0,                   //创建
        kFileOperateTypeUpdate=1,                   //更新
        kFileOperateTypeDelete=2                    //删除
    };
    
    std::string rootUrl;                            //资源根地址
    
    HttpDownloader* downloader;                     //下载器
    
    DownloadFileType downloadFileType;              //操作类型
    
    float localResourceVersion;                     //本地资源版本
    
    float serverResourceVersion;                    //服务端资源版本
    
    bool versionLatest;                             //版本是否最新的标志
    
    cocos2d::CCDictionary*  downloadCache;          //待更新模块资源缓存 k-模块名  v-CCArray
    
    std::string currentModuleName;                  //当前正在更新的模块名
    
    ResourceManager();
    
    //下载版本描述文件
    void downloadVersionFile();
    
    //下载更新列表
    void downloadUpdateList(float versionOld,float versionNew);
    
    //下载模块文件
    void downloadModuleFile();
    
    //比较新下载的version说明文件内的版本号与本地保存的版本号
    void compareVersion();
    
    //从下载文件更新版本
    void updateVersionFormFile();
    
    //单个模块文件下载完成后的处理
    void processAfterSingleModleFileDownload();
    
    //合并UpdateList
    void mergeUpdateList(const std::string& oldUpdateList,const std::string& newUpdateList,const std::string& savePath);
    
    //保存版本号
    void saveVersion(float version);
    
    //获取文件的绝对路径(本地资源目录下)
    const char* fullLocalResourcePath(const std::string& filePath);
    
    //报告错误，对delegate和scriptDelegate进行回调
    void reportError(const std::string& errorMsg);
    
};

#endif /* defined(__Framework_x__ResourceManager__) */
