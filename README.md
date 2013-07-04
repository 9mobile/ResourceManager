ResourceManager
===============
ResourceManager是基于cocos2d-x实现的一个资源更新系统。实现了
 1）增量更新，每次版本更新玩家只需要下载修改部分，而不需要下载整个游戏
 2）按需下载，某些资源用到时才下载，大大减少玩家首次下载游戏体积。
 3）基于lua，业务逻辑也可以更新。

包含：
1、docs，包括使用说明文档
2、project.ios，iOS平台的demo工程
3、resources，demo里面涉及的资源
4、tool，包括一个java工具，对比不同版本资源文件，生成增量更新包以及增量更新配置
