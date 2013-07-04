--[[
 description:模块更新场景
 author:the9
 date:2013/07/03
 --]]

ModuleUpdateScene={}

ModuleUpdateScene.TAG_LABEL_TIPS = 100

ModuleUpdateScene.layer = nil

function ModuleUpdateScene:enter()
    
    print("ModuleUpdateScene:enter")
    
    self.layer=CCLayer:create()
    
    local size = CCDirector:sharedDirector():getWinSize();
    --(1)背景
    local spriteBg=CCSprite:create("bg/Background_FullScreen_login.png");
    spriteBg:setPosition(size.width/2, size.height/2);
    self.layer:addChild(spriteBg);
    
    --(2)标题
    local labelTitle = CCLabelTTF:create("模块更新", "Helvetica", 30);
    labelTitle:setPosition( size.width / 2, size.height/2 + 30 );
    self.layer:addChild(labelTitle, 1);
    
    --(3)模块copy001按钮
    local menuItemCopy001 = CCMenuItemFont:create("copy001")
    menuItemCopy001:setFontSizeObj(18)
    menuItemCopy001:registerScriptTapHandler(MakeScriptHandler(self,self.menuCallbackCopy001))
    menuItemCopy001:setPosition(-40, -20)
    
    --(4)模块map001按钮
    local menuItemMap001 = CCMenuItemFont:create("map001")
    menuItemMap001:setFontSizeObj(18)
    menuItemMap001:registerScriptTapHandler(MakeScriptHandler(self,self.menuCallbackMap001))
    menuItemMap001:setPosition(40, -20)
    
    local menu = CCMenu:create()
    menu:addChild(menuItemCopy001)
    menu:addChild(menuItemMap001)
    self.layer:addChild(menu)
    
    
    --(5)tips
    local labelTips = CCLabelTTF:create("", "Helvetica", 18);
    labelTips:setPosition( size.width / 2, 80 )
    labelTips:setTag(ModuleUpdateScene.TAG_LABEL_TIPS)
    self.layer:addChild(labelTips)
    
    local scene = CCScene:create()
    scene:addChild(self.layer)
    RunScene(scene)
    
    local resourceManager=ResourceManager:sharedResourceManager()
    resourceManager:setRootURL("http://localhost:8080/ResourceManagerDemo")
    
    --以下两行为demo中写法，实际项目中可以自己封装lua继承C++
    tolua.setpeer(self.layer,self)
    resourceManager:setScriptDelegate(self.layer)
    
end

function ModuleUpdateScene:menuCallbackCopy001()
    local moduleName = "copy001"
    self:showTips("开始检查模块:"..moduleName)
    local resourceManager=ResourceManager:sharedResourceManager()
    local fileNum = resourceManager:checkModule(moduleName)
    self:showTips("模块["..moduleName.."]需要更新的文件数量为："..fileNum)
    if fileNum > 0 then
        resourceManager:updateModule(moduleName)
    end
end

function ModuleUpdateScene:menuCallbackMap001()
    local moduleName = "map001"
    self:showTips("开始检查模块:"..moduleName)
    local resourceManager=ResourceManager:sharedResourceManager()
    local fileNum = resourceManager:checkModule(moduleName)
    self:showTips("模块["..moduleName.."]需要更新的文件数量为："..fileNum)
    if fileNum > 0 then
        resourceManager:updateModule(moduleName)
    end
end

function ModuleUpdateScene:onVersionCheckCompleted(latest)
    --此场景中该function不会回调
end

function ModuleUpdateScene:onVersionUpdated()
    --此场景中该function不会回调
end

function ModuleUpdateScene:onModuleUpdated(moduleName)
    self:showTips("模块["..moduleName.."]已更新")
end

function ModuleUpdateScene:onSingleFileUpdated(filePath)
    self:showTips("文件["..filePath.."]已更新")
end

function ModuleUpdateScene:onErrorOccured(message)
    self:showTips(message)
end

function ModuleUpdateScene:showTips(tips)
    print(tips)
    local labelTips = self.layer:getChildByTag(ModuleUpdateScene.TAG_LABEL_TIPS)
    labelTips = tolua.cast(labelTips,"CCLabelTTF")
    labelTips:setString(tips)
end
