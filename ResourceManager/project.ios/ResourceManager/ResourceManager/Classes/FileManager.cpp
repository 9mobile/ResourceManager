//
//  FileManager.cpp
//  Framework-x
//
//  Created by the9 on 13-4-23.
//
//

//#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
//#endif

#include "support/zip_support/unzip.h"

#include "FileManager.h"

using namespace std;
using namespace cocos2d;

#define BUFFER_SIZE    8192
#define MAX_FILENAME   512

const char* FileManager::splicePath(const std::string &parentPath, const std::string &childPath)
{
    std::string tempPath1 = parentPath;
    std::string tempPath2 = childPath;
    //去掉path1末尾的'/'
    if (tempPath1.size()>0 && tempPath1.at(tempPath1.size()-1)=='/') {
        tempPath1.erase(tempPath1.size()-1);
    }
    //去掉path2开头的'/'
    if (tempPath2.size()>0 && tempPath2.at(0)=='/') {
        tempPath2.erase(0);
    }
    //拼接
    std::string path=tempPath1;
    path.append("/");
    path.append(tempPath2);
    return CCString::create(path)->getCString();
}

bool FileManager::fileExists(const std::string& filePath)
{
    return (0 == access(filePath.c_str(), F_OK));
}

bool FileManager::renameFile(const std::string& oldFilePath, const std::string& newFilePath)
{
    return 0 == rename(oldFilePath.c_str(), newFilePath.c_str());
}

bool FileManager::removeFile(const std::string& filePath)
{
    return 0 == remove(filePath.c_str());
}

bool FileManager::createDirectory(const string& directoryPath)
{
    vector<string> directoryPathArray; // 需要创建目录的路径数组
    
    string fullPath = directoryPath.c_str();
    
    //找到不存在目录，放入数组
    string directorySubPath;
    do {
        int pos = fullPath.find_last_of('/');
        directorySubPath = fullPath.substr(0, pos);
        if(0 != access(directorySubPath.c_str(), F_OK)) // 目录不存在
        {
            directoryPathArray.insert(directoryPathArray.begin(), directorySubPath);
            fullPath = directorySubPath.c_str();
        } else
        {
            break;
        }
    } while (true);
    
    // 从父到子逐层创建目录
    bool result = true;
    vector<string>::iterator it = directoryPathArray.begin();
    while (it != directoryPathArray.end()) {
        if(! doCreateDirectory(*it) )
        {
            result = false;
            break;
        }
        it++;
    }
    return result;
}

bool FileManager::doCreateDirectory(const string& directoryPath)
{
    //#if (CC_TARGET_PLATFORM != CC_PLATFORM_WIN32)
    mode_t processMask = umask(0);
    int ret = mkdir(directoryPath.c_str(), S_IRWXU | S_IRWXG | S_IRWXO);
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


bool FileManager::uncompressZipFile(const string &zipFileName, const string &directory)
{
    // Open the zip file
    unzFile zipfile = unzOpen(zipFileName.c_str());
    if (! zipfile)
    {
        CCLOG("can not open downloaded zip file %s", zipFileName.c_str());
        return false;
    }
    
    // Get info about the zip file
    unz_global_info global_info;
    if (unzGetGlobalInfo(zipfile, &global_info) != UNZ_OK)
    {
        CCLOG("can not read file global info of %s", zipFileName.c_str());
        unzClose(zipfile);
    }
    
    // Buffer to hold data read from the zip file
    char readBuffer[BUFFER_SIZE];
    
    CCLOG("start uncompressing %s, to:%s", zipFileName.c_str(), directory.c_str());
    
    // Loop to extract all files.
    uLong i;
    for (i = 0; i < global_info.number_entry; ++i)
    {
        // Get info about current file.
        unz_file_info fileInfo;
        char fileName[MAX_FILENAME];
        if (unzGetCurrentFileInfo(zipfile,
                                  &fileInfo,
                                  fileName,
                                  MAX_FILENAME,
                                  NULL,
                                  0,
                                  NULL,
                                  0) != UNZ_OK)
        {
            CCLOG("can not read file info");
            unzClose(zipfile);
            return false;
        }
        
        // 检查是否带'/'结尾
        string saveToDirectory = directory.c_str();
        if (saveToDirectory[saveToDirectory.size() - 1] != '/') {
            saveToDirectory.append("/");
        }
        string fullPath = saveToDirectory + fileName;
        
        // Check if this entry is a directory or a file.
        const size_t filenameLength = strlen(fileName);
        if (fileName[filenameLength-1] == '/')
        {
            // Entry is a direcotry, so create it.
            // If the directory exists, it will failed scilently.
            if (!createDirectory(fullPath.c_str()))
            {
                CCLOG("can not create directory %s", fullPath.c_str());
                unzClose(zipfile);
                return false;
            }
        }
        else
        {
            // Entry is a file, so extract it.
            
            // Open current file.
            if (unzOpenCurrentFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not open file %s", fileName);
                unzClose(zipfile);
                return false;
            }
            
            // Create a file to store current file.
            FILE *out = fopen(fullPath.c_str(), "wb");
            if (! out)
            {
                CCLOG("can not open destination file %s", fullPath.c_str());
                unzCloseCurrentFile(zipfile);
                unzClose(zipfile);
                return false;
            }
            
            // Write current file content to destinate file.
            int error = UNZ_OK;
            do
            {
                error = unzReadCurrentFile(zipfile, readBuffer, BUFFER_SIZE);
                if (error < 0)
                {
                    CCLOG("can not read zip file %s, error code is %d", fileName, error);
                    unzCloseCurrentFile(zipfile);
                    unzClose(zipfile);
                    return false;
                }
                
                if (error > 0)
                {
                    fwrite(readBuffer, error, 1, out);
                }
            } while(error > 0);
            
            fclose(out);
        }
        
        unzCloseCurrentFile(zipfile);
        
        // Goto next entry listed in the zip file.
        if ((i+1) < global_info.number_entry)
        {
            if (unzGoToNextFile(zipfile) != UNZ_OK)
            {
                CCLOG("can not read next file");
                unzClose(zipfile);
                return false;
            }
        }
    }
    
    CCLOG("end uncompressing");
    
    return true;
}

bool FileManager::writeStringToFile(const std::string& content, const std::string& filePath, bool isAppend)
{    
    const char *mode = isAppend ? "ab" : "wb";
    FILE *fp = fopen(filePath.c_str(), mode); //
    if (! fp)
    {
        CCLOG("创建文件错误，errno=%d", errno);
        return false;
    }

    size_t written = fwrite(content.c_str(), content.length(), 1, fp);
    fclose(fp);
    return written == content.length();
}
