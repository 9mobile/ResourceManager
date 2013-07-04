//
//  HttpDownloader.cpp
//
//  Created by the9 on 13-3-11.
//
//

#include <curl/curl.h>
#include <curl/easy.h>
#include <stdio.h>

#include <pthread.h>
#include <semaphore.h>

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
//#endif

#include "HttpDownloader.h"

using namespace cocos2d;
using namespace std;

// 临时文件后缀，下载完成后会通过rename方式去掉
#define FileTempSuffix ".tmp"

// 读取curl错误信息
static char s_errorBuffer[CURL_ERROR_SIZE];

// 网络有数据时回调，直接回写到file
static size_t downLoadFile(void *ptr, size_t size, size_t nmemb, void *filePointer)
{
    FILE *fp = (FILE*)filePointer;
    size_t written = fwrite(ptr, size, nmemb, fp);
    return written;
}

// 下载进度回调
static int progressFunc(void *ptr, double totalToDownload, double nowDownloaded, double totalToUpLoad, double nowUpLoaded)
{

    float *pProgress = (float *)ptr;
    *pProgress = nowDownloaded/totalToDownload;
    
    *pProgress = *pProgress < 0 ? 0 : *pProgress;
    *pProgress = *pProgress > 1 ? 1 : *pProgress;
//    CCLOG("downloading... %d%%", (int)(*pProgress*100));
    
    return 0;
}

static HttpDownloader *s_downloader = NULL;

// 线程
static pthread_t        s_networkThread;
static sem_t *          s_pSem = NULL;
static bool need_quit = false;
#define CC_ASYNC_HTTPREQUEST_SEMAPHORE "download_sem"

// Worker thread
void* downloadThread(void *data)
{
    while (true)
    {
        // Wait for http request tasks from main thread
        int semWaitRet = sem_wait(s_pSem);
        if (semWaitRet < 0) {
            CCLog("HttpRequest async thread semaphore error: %s\n", strerror(errno));
            break;
        }
        
        if (need_quit)
        {
            break;
        }
        
        // 下载和保存文件
        CCLog("即将下载文件.......");
        s_downloader->doDownload();
    }
    
    if (s_pSem != NULL) {
        sem_unlink(CC_ASYNC_HTTPREQUEST_SEMAPHORE);
        sem_close(s_pSem);
        
        s_pSem = NULL;
    }
    
    CCLog("下载线程退出！");
    return NULL;
}



HttpDownloader::HttpDownloader()
{
    isDownloading = false;
    _pTarget = NULL;
    _pDownloadCallback = NULL;
    _pFile = NULL;
}

HttpDownloader::~HttpDownloader()
{
    if (_pTarget) {
        _pTarget->release();
    }
    
    CCDirector::sharedDirector()->getScheduler()->unscheduleSelector(schedule_selector(HttpDownloader::callBackSelector), this);
    need_quit = true;
    
    // 通知线程退出
    sem_post(s_pSem);
}

void HttpDownloader::setUrl(const char* url)
{
    _url = url;
    
    // 重置
    isDownloading = false;
    _totalSize = 0;
    _progress = 0;
}

const char* HttpDownloader::getUrl()
{
    return _url.getCString();
}

void HttpDownloader::setFullPath(const char* fullPath)
{
    _fullPath = fullPath;
}

void HttpDownloader::setDownloadCallback(CCObject* pTarget,
                                         SEL_Download pDownloadCallback)
{
    _pTarget = pTarget;
    _pDownloadCallback = pDownloadCallback;
    
    if (_pTarget)
    {
        _pTarget->retain();
    }
}

// 获取下载进度
float HttpDownloader::getProgress()
{
    return _progress;
}

const char* HttpDownloader::getErrorMessage()
{
    return _errorMsg.getCString();
}


bool HttpDownloader::download()
{
    if (s_downloader == NULL) {
        s_downloader = this;
        
        s_pSem = sem_open(CC_ASYNC_HTTPREQUEST_SEMAPHORE, O_CREAT, 0644, 0);
        if (s_pSem == SEM_FAILED) {
            CCLog("download Semaphore failed");
            s_pSem = NULL;
            return false;
        }
        
        pthread_create(&s_networkThread, NULL, downloadThread, NULL);
        pthread_detach(s_networkThread);
    }
    
    if (isDownloading) { // 不支持同时下载多个文件
        CCLog("正在下载文件。不支持多个同时下载！\n");
        return false;
    }
    
    isDownloading = true;
    
    // Notify thread start to work
    sem_post(s_pSem);
    
    return true;
}

void HttpDownloader::doDownload()
{
    // 创建要保存的文件
    if(!createFile())
    {
        doCallback(kDownloadError);
        return;
    }
    
    if (_url.length() == 0) {
        _errorMsg = "要下载url为空";
        CCLOG("url not set");
        // 回调
        doCallback(kDownloadError);
        return;
    }
    
    CURL *curl = curl_easy_init();
    if (! curl)
    {
        CCLOG("can not init curl");
        _errorMsg = "初始化curl出错";
        doCallback(kDownloadError);
        return ;
    }
    
    CURLcode res;
    curl_easy_setopt(curl, CURLOPT_URL, _url.getCString());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, downLoadFile);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, _pFile);
    // 设置超时时间为无限
    curl_easy_setopt(curl, CURLOPT_TIMEOUT, 0);
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, s_errorBuffer);
    
    // 下载进度
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, false);
    curl_easy_setopt(curl, CURLOPT_PROGRESSFUNCTION, progressFunc);
    curl_easy_setopt(curl, CURLOPT_PROGRESSDATA, &_progress);
    
    res = curl_easy_perform(curl);
    
    int responseCode;
    CURLcode code = curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &responseCode);
    if (code != CURLE_OK || responseCode != 200)
    {
        code = CURLE_HTTP_RETURNED_ERROR;
    }
    
    curl_easy_cleanup(curl);
    
    fclose(_pFile);
    _pFile = NULL;
    
    
    if (res != 0 || code != CURLE_OK || responseCode != 200)
    {
        // 删除temp文件
        string tempFile = _fullPath.getCString();
        tempFile.append(FileTempSuffix);
        remove(tempFile.c_str());
        
        _errorMsg.initWithFormat("下载失败，responseCode=%d  %s", responseCode, s_errorBuffer);
        CCLOG(_errorMsg.getCString());
        doCallback(kDownloadError);
        
        return ;
    }
    
    // 重命名文件
    if(! renameFile() )
    {
        doCallback(kDownloadError);
        return;
    }
    
    CCLOG("succeed downloading package %s, to %s", _url.getCString(), _fullPath.getCString());
    // 回调
    _errorMsg = "下载成功";
    doCallback(kDownloadSucceed);
}

/*
 * 创建文件夹
 */
bool HttpDownloader::createDirectory(const char *path)
{
    CCLOG("create directory:%s", path);
    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(path, S_IRWXU | S_IRWXG | S_IRWXO);
    umask(processMask);
    if (ret != 0 && (errno != EEXIST))
    {
        return false;
    }
    
    return true;
    //#else
    //    BOOL ret = CreateDirectoryA(path, NULL);
    //	if (!ret && ERROR_ALREADY_EXISTS != GetLastError())
    //	{
    //		return false;
    //	}
    //    return true;
    //#endif
}

/*
 * 确保要保存文件所在文件夹存在，如果不存在，则递归创建
 */
bool HttpDownloader::confirmDirectory()
{
    vector<string> directoryPathArray; // 需要创建目录的路径数组
    
    string fullPath = _fullPath.getCString();
    
    //找到不存在目录，放入数组
    string directoryPath;
    do {
        int pos = fullPath.find_last_of('/');
        directoryPath = fullPath.substr(0, pos);
        if(0 != access(directoryPath.c_str(), F_OK)) // 目录不存在
        {
            directoryPathArray.insert(directoryPathArray.begin(), directoryPath);
            fullPath = directoryPath.c_str();
        } else
        {
            break;
        }
    } while (true);
    
    // 从父到子逐层创建目录
    bool result = true;
    vector<string>::iterator it = directoryPathArray.begin();
    while (it != directoryPathArray.end()) {
        if(! createDirectory((*it).c_str()) )
        {
            result = false;
            break;
        }
        it++;
    }
    return result;
}

bool HttpDownloader::createFile()
{
    if (_fullPath.length() == 0)
    {
        _errorMsg = "没有设置要保存的文件路径";
        return false;
    }
    
    string tempStdFullPath = _fullPath.getCString();
    
    if(tempStdFullPath[tempStdFullPath.size() - 1] == '/') // 需要指定文件而不是路径
    {
        _errorMsg = "需要指定文件而不是目录";
        return false;
    }
    
    //创建目录
    if(!confirmDirectory())
    {
        _errorMsg.initWithFormat("创建目录错误, errno=%d", errno);
        doCallback(kDownloadError);
        return false;
    }
    
    tempStdFullPath.append(FileTempSuffix); // 先写入一个temp文件，然后重命名，确保下载事物操作
    _pFile = fopen(tempStdFullPath.c_str(), "wb"); //
    if (! _pFile)
    {
        CCLOG("errno=%d", errno);
        _errorMsg.initWithFormat("创建文件错误, errno=%d", errno);
        doCallback(kDownloadError);
        return false;
    }
    
    return true;
}

bool HttpDownloader::renameFile()
{
    CCString tmpFullPath;
    tmpFullPath.initWithFormat("%s%s", _fullPath.getCString(), FileTempSuffix);
    if (0 != rename(tmpFullPath.getCString(), _fullPath.getCString())) {
        _errorMsg.initWithFormat("重命名文件失败。errno=%d", errno);
        return false;
    }
    return true;
}

void HttpDownloader::doCallback(DownloadResult result)
{
    isDownloading = false;
    
    _result = result;
    
    CCDirector::sharedDirector()->getScheduler()->scheduleSelector(schedule_selector(HttpDownloader::callBackSelector), this, 0, false, 0, 0);
}

void HttpDownloader::callBackSelector(float deta)
{
    if (_pTarget && _pDownloadCallback)
    {
        (_pTarget->*_pDownloadCallback)(this, _result);
    }
}

