//
//  FileManager.h
//  Framework-x
//
//  Created by the9 on 13-4-23.
//
//

#ifndef __Framework_x__FileManager__
#define __Framework_x__FileManager__

#include <iostream>
#include "cocos2d.h"

class FileManager {
    
public:
    
    //拼接两个路径
    static const char* splicePath(const std::string& parentPath,const std::string& childPath);
    
    //测试此路径名表示的文件/文件夹是否存在
    static bool fileExists(const std::string& filePath);
    
    /*
     * 文件重命名。
     * 可以实现移动到其它目录，但是如果要移动的目录不存在，则会失败;
     * 如果新文件名对应文件已存在，则覆盖。
     *  
     */
    static bool renameFile(const std::string& oldFilePath, const std::string& newFilePath);
    
    //保存数据加文件
    static bool writeStringToFile(const std::string& content,const std::string& filePath,bool isAppend = false);
    
    //删除文件
    static bool removeFile(const std::string& filePath);
    
    /* 创建目录。
     * 如果目录存在，则返回成功。
     * 如果该目录任意一级父目录不存在，则创建这些父目录
     */
    static bool createDirectory(const std::string& directoryPath);
    
    //解压zip文件到指定目录
    static bool uncompressZipFile(const std::string& zipFile,const std::string& directory);
    
private:
    static bool doCreateDirectory(const std::string& directoryPath);    
};

#endif /* defined(__Framework_x__FileManager__) */
