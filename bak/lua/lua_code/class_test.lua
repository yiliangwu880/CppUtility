
require(g_work_path.."class")
require(g_work_path.."utility")
--[[记录函数运行状态,
格式:{
class_name=order(运行顺序，1开始), ..., 
class_name_cnt = 运行次数, ...
cur_c_order = 0, --目前构造函数次序
Construct_cb1 = nil, --构造回调函数定义
Construct_cb2 = nil, --构造回调函数定义
}

]]
local c_state = {} --Construct运行状态
local f1_state = {} --f1运行状态
local f2_state = {} 

local function Clear()
    c_state = {cur_c_order = 0,}
    f1_state = {} 
    f2_state = {} 
end

local BaseC = {}

function BaseC:Construct(...)
    c_state.cur_c_order = c_state.cur_c_order + 1
    c_state.BaseC = c_state.cur_c_order
    if c_state.Construct_cb1 then
        c_state.Construct_cb1(self, ...)
    end
end
function BaseC:f1()
    f1_state.BaseC_cnt = f1_state.BaseC_cnt or 0
    f1_state.BaseC_cnt = f1_state.BaseC_cnt + 1
end
function BaseC:f2()
    f2_state.BaseC_cnt = f2_state.BaseC_cnt or 0
    f2_state.BaseC_cnt = f2_state.BaseC_cnt + 1
end

-------------------------------------------------------
local D1 = g_class.InheritClass(BaseC)
function D1:Construct(...)
    c_state.cur_c_order = c_state.cur_c_order + 1
    c_state.D1 = c_state.cur_c_order
    if c_state.Construct_cb2 then
        c_state.Construct_cb2(self, ...)
    end
end
function D1:f1()
    f1_state.D1_cnt = f1_state.D1_cnt or 0
    f1_state.D1_cnt = f1_state.D1_cnt + 1
end

-------------------------------------------------------
local function TestNewObj()
    DebugLog("TestNewObj")
    Clear()
    local obj = g_class.New(BaseC)
    obj.f1()

    assert(c_state.BaseC == 1)
    assert(f1_state.BaseC_cnt == 1)
    obj:f1()
    assert(f1_state.BaseC_cnt == 2)
    obj.f1 = function()end
    assert(f1_state.BaseC_cnt == 2)
end



local function TestDeriveClass()
    DebugLog("TestDeriveClass")
    Clear()
    local cb_obj1 = nil
    local cb_obj2 = nil
    local function base_c_cb(self, ...)
        local p1,p2,p3 = ...
        assert(p1 == 1)
        assert(p2 == "2")
        assert(p3 == 3)
        cb_obj1 = self
    end

    local function derive_c_cb(self, ...)
        local p1,p2,p3 = ...
        assert(p1 == 1)
        assert(p2 == "2")
        assert(p3 == 3)
        cb_obj2 = self
    end
    c_state.Construct_cb1=base_c_cb
    c_state.Construct_cb2=derive_c_cb
    local obj = g_class.New(D1, 1,"2",3)
    assert(c_state.BaseC == 1) --构造顺序
    assert(c_state.D1 == 2)
    obj:f2() --调用基类的
    assert(f2_state.BaseC_cnt == 1)
    obj:f1()  --调用派生类的
    assert(f1_state.BaseC_cnt == nil)
    assert(f1_state.D1_cnt == 1)
    assert(obj == cb_obj1)
    assert(obj == cb_obj2)


end

TestNewObj()
TestDeriveClass()