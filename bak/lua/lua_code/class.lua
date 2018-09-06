
--[[
实现类，对象代码
例子：
local D1 = g_class.InheritClass(BaseC)
function D1:Construct(...)
    ...
end
 local obj = g_class.New(D1)    
]]

g_class = g_class or {}

--实现构造函数，先调用基类的，再调用派生类的
local function Construct(my_class, obj, ...)
    if my_class.base then
        Construct(my_class.base, obj, ...)
    end

    if my_class.Construct then
        my_class.Construct(obj, ...)
    end 
end

--用类创建对象
--如果类定义了Construct函数，会用...调用
function g_class.New(my_class, ...)
	local obj={}
    local mt = {__index = function(t, k)
			        local value = my_class[k]
			        obj[k] = value  --估计提升效率用，不用每次都触发metamethod
			        return value
		        end}
    setmetatable(obj, mt)
    Construct(my_class, obj, ...)
	return obj
end

--继承基类
--return 返回派生类
--[[例如：
 my_class = g_class.InheritClass(base_class)
 function my_class:f1()
 ...
 end
]]
function g_class.InheritClass(base_class)
    local new_class = {}
    if nil == base_class then
        return new_class
    end

    new_class.base = base_class
    local mt = {__index = function(t, k)
			        local value = base_class[k]
			        new_class[k] = value  --估计提升效率用，不用每次都触发metamethod
			        return value
		        end}
    setmetatable(new_class, mt)
    return new_class
end


