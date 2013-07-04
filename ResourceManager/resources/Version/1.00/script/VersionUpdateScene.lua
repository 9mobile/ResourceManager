--[[
 description:版本更新场景
 author:the9
 date:2013/07/03
 --]]

VersionUpdateScene={}

VersionUpdateScene.TAG_LABEL_TIPS = 100

VersionUpdateScene.layer = nil



function VersionUpdateScene:enter()
    print("VersionUpdateScene:enter")
    
    self.layer=CCLayer:create()
    
    local size = CCDirector:sharedDirector():getWinSize();
    --(1)背景
    local spriteBg=CCSprite:create("bg/Background_FullScreen_login.png");
    spriteBg:setPosition(size.width/2, size.height/2)
    self.layer:addChild(spriteBg);
    
    --(2)标题
    local labelTitle = CCLabelTTF:create("更新客户端版本", "Helvetica", 30);
    labelTitle:setPosition( size.width / 2, size.height/2 );
    self.layer:addChild(labelTitle);
    
    --(3)tips
    local labelTips = CCLabelTTF:create("", "Helvetica", 18);
    labelTips:setPosition( size.width / 2, 80 )
    labelTips:setTag(VersionUpdateScene.TAG_LABEL_TIPS)
    self.layer:addChild(labelTips)
    
    local scene = CCScene:create()
    scene:addChild(self.layer)
    RunScene(scene)
    
    local resourceManager=ResourceManager:sharedResourceManager()
    resourceManager:setRootURL("http://localhost:8080/ResourceManagerDemo")
    
    --以下两行为demo中写法，实际项目中可以自己封装lua继承C++
    tolua.setpeer(self.layer,self)
    resourceManager:setScriptDelegate(self.layer)   
    
    self:showTips("开始检查版本")
    resourceManager:checkVersion()
    
end

function VersionUpdateScene:onVersionCheckCompleted(latest)
    if latest then
        self:showTips("版本已经最新，不需要更新")
        ModuleUpdateScene:enter()
        else
        self:showTips("版本过时，开始更新")
        ResourceManager:sharedResourceManager():updateVersion()
    end
end

function VersionUpdateScene:onVersionUpdated()
    --版本更新完毕
    self:showTips("版本更新完毕")
    Game:restart()
end

function VersionUpdateScene:onModuleUpdated(moduleName)
    --此场景中该function不会回调
end

function VersionUpdateScene:onSingleFileUpdated(filePath)
    --此场景中该function不会回调
end

function VersionUpdateScene:onErrorOccured(message)
    self:showTips(message)
end

function VersionUpdateScene:showTips(tips)
    print(tips)
    local labelTips = self.layer:getChildByTag(VersionUpdateScene.TAG_LABEL_TIPS)
    labelTips = tolua.cast(labelTips,"CCLabelTTF")
    labelTips:setString(tips)
end


