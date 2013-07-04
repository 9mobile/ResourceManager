--[[
 description:实用功能集合
 author:the9
 date:2013/07/03
 --]]

--获取当前场景
function CurrentScene()
    return CCDirector:sharedDirector():getRunningScene()
end

function RunScene(scene)
    if not scene then
        scene = CCScene:create()
    end
    if not CurrentScene() then
        CCDirector:sharedDirector():runWithScene(scene)
        else
        CCDirector:sharedDirector():replaceScene(scene)
    end
end

--构造script handler
function MakeScriptHandler(target,selector)
    return function(...) return selector(target,...) end
end

