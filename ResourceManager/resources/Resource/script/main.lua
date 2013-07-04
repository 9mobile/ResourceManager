--[[
 description:脚本入口点
 author:the9
 date:2013/03/13
 --]]

Game = {}

--启动游戏
function Game:start()
    
    --系统初始化
    self:init()
    
    --加载公共模块
    self:requireCommonModule()
    
    --启动场景
    local developMode = ResourceManager:sharedResourceManager():isDevelopMode()
    local isVersionLatest = ResourceManager:sharedResourceManager():isVersionLatest()
    --如果版本已经更新，则从ModuleUpdateSene开始，否则从VersionUpdateSene开始
    if developMode or isVersionLatest then
        ModuleUpdateScene:enter()
        else
        VersionUpdateScene:enter()
    end
end

--重启游戏
function Game:restart()
    --(1)清理所有已经加载的模块
    self:unrequireAllUserModules()
    --(2)重新执行本文件
    local path = CCFileUtils:sharedFileUtils():fullPathForFilename("script/main.lua")
    dofile(path)
end

--系统初始化
function Game:init()
    self.systemModuleList={}    --系统模块列表，在游戏重启清除所有加载的模块时系统模块被保留
    for k,v in pairs(package.loaded) do
        self.systemModuleList[k]=v
    end
end

--删除所有游戏层加载的模块
function Game:unrequireAllUserModules()
    
    for k,v in pairs(package.loaded) do
        if not self.systemModuleList[k] then
            package.loaded[k] = nil
        end
    end

end

--加载公共模块
function Game:requireCommonModule()
    require ("functions")
    require ("ModuleUpdateScene")
    require ("VersionUpdateScene")
end

Game:start()
