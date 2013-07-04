//
//  HttpDownloader.h
//
//  Created by the9 on 13-3-11.
//
//

#ifndef __HttpDownloader__
#define __HttpDownloader__

#include "cocos2d.h"

enum DownloadResult
{
    kDownloadSucceed,
    kDownloadError,
    kDownloadProgress
};

class HttpDownloader;
typedef void (cocos2d::CCObject::*SEL_Download)(HttpDownloader*, DownloadResult result);

class HttpDownloader : public cocos2d::CCObject
{
public:
    HttpDownloader();
    virtual ~HttpDownloader();
    
    // 要下载文件的url
    void setUrl(const char* url);
    
    const char* getUrl();
    
    // 文件要保存的路径
    void setFullPath(const char* fullPath);
    
    const char* getFullPath();
    
    // 回调
    void setDownloadCallback(cocos2d::CCObject* pTarget,
                             SEL_Download pDownloadCallback);
    //进行下载
    bool download();
    
    // 下载进度
    float getProgress();
    
    // 错误信息
    const char* getErrorMessage();
    
    // 往主线程回调函数
    void callBackSelector(float deta);
    
protected:
    // 友元
    friend void* downloadThread(void *data);
    
    // 以temp文件名，创建要保存的文件。如果文件夹不存在，则创建
    bool createFile();
    // 将下载完成的temp文件重命名为正确文件名
    bool renameFile();
    // 创建目录
    bool createDirectory(const char *path);
    // 确保要保存文件所在目录存在
    bool confirmDirectory();
    
    // 进行文件下载
    void doDownload();
    
    void doCallback(DownloadResult result);
    
protected:
    cocos2d::CCString           _fullPath;
    cocos2d::CCString           _url;
    
    cocos2d::CCObject           *_pTarget;               /// callback target of pSelector function
    SEL_Download                _pDownloadCallback;        /// 下载回调
    
    size_t _totalSize;
    float _progress;
    cocos2d::CCString _errorMsg;
    DownloadResult _result;
    
    FILE *_pFile;       // 要保存文件指针
    
    bool isDownloading;
};

#endif /* defined(__HttpDownloader__) */
