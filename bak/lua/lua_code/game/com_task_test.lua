require(g_work_path.."game/com_task")

g_task_com = g_task_com or {} 
local this = g_task_com

local EQUAL=0
local GREATER=1
local LESS=2

--所有任务类型配置

--local target_type_infos=
--{
--    --[[ 
--        最后一个参数para 表达 task_state.num 

--    --]]
--    [target_type.KILL_MONSTER]={EQUAL, GREATER},             --杀id怪，>=para2
--    [target_type.LV]={GREATER},                    --等级 >=para1
--    [target_type.GET_QUALITY_ITEM]={EQUAL, GREATER, GREATER},    --获取id物品，>=品质，>=para2
--    [target_type.GET_ITEM]={EQUAL, GREATER},             --获取id物品，>=para2
--}



local function Assert(cond, add_info)
	assert(cond, add_info)
end

function this.test_lv_cb_finish(task_state)
    this.test_lv_cb(task_state, task_state, true)
end
function this.test_lv_cb_update(task_state)
    this.test_lv_cb(task_state, task_state, false)
end

function this.test_lv_cb(task_state, cb_para, is_finish)
    if 1 ~= cb_para.p then
        ErrorLog("error  cb_para", cb_para) --回调参数不是注册的对象
    end
    cb_para.cb_cnt = cb_para.cb_cnt or 0
    cb_para.cb_cnt = cb_para.cb_cnt + 1
    if 1 == cb_para.cb_cnt then
        Assert(task_state.num == 0, "task_state.num == 0")
        Assert(false == is_finish, "false == is_finish")
    elseif 2 == cb_para.cb_cnt then
        Assert(task_state.num == 1, "task_state.num == 1")
        Assert(true == is_finish, "true == is_finish")
    end
end


function this.test_get_item_cb_finish(task_state)
    this.test_get_item_cb(task_state, task_state, true)
end
function this.test_get_item_cb_update(task_state)
    this.test_get_item_cb(task_state, task_state, false)
end
function this.test_get_item_cb()
    local m_member = 11
    return
    function(task_state, cb_para, is_finish)
        cb_para.cb_cnt = cb_para.cb_cnt or 0
        cb_para.cb_cnt = cb_para.cb_cnt + 1
        Assert(11 == m_member, "11 == m_member")
        if 1 == cb_para.cb_cnt then
            Assert(task_state.num == 1, "task_state.num == 0")
            Assert(false == is_finish, "false == is_finish")
        elseif 2 == cb_para.cb_cnt then
            Assert(task_state.num == 2, "task_state.num == 1")
            Assert(true == is_finish, "true == is_finish")
        end
    end
end

function this.test_get_quality_item_cb_finish(task_state)
    this.test_get_quality_item_cb(task_state, task_state, true)
end
function this.test_get_quality_item_cb_update(task_state)
    this.test_get_quality_item_cb(task_state, task_state, false)
end
function this.test_get_quality_item_cb(task_state, cb_para, is_finish)
    cb_para.cb_cnt = cb_para.cb_cnt or 0
    cb_para.cb_cnt = cb_para.cb_cnt + 1
    if 1 == cb_para.cb_cnt then
        Assert(task_state.num == 1, "task_state.num == 0")
        Assert(false == is_finish, "false == is_finish")
    elseif 2 == cb_para.cb_cnt then
        Assert(task_state.num == 2, "task_state.num == 1")
        Assert(true == is_finish, "true == is_finish")
    end
end

function this.test()
	print("TestComTask")
    local task_state 
    task_state = 
    {
        num = 0,
        cfg={
           target_type  = g_target_type.LV,  --值为 target_type
           para1=1, --需要比较的值, 不需要的nil (策划配置来的)
        },
        finish_cb = this.test_lv_cb_finish, 
        update_cb = this.test_lv_cb_update,
        p = 1,
    }
    local obj = g_class.New(g_task_com)
    Assert(0 == obj:GetRegTaskNum(), "num error")
    obj:RegTask(task_state)
    Assert(1 == obj:GetRegTaskNum(), "num error")

    --Update
    obj:Update(g_target_type.LV, {0})
    obj:Update(g_target_type.LV, {1})
    Assert(0 == obj:GetRegTaskNum(), "num error")
	
    task_state = 
    {
        num = 0,
        cfg={
           target_type  = g_target_type.GET_ITEM,  --值为 target_type
           para1=1001, para2 = 2
        },
        finish_cb = this.test_get_item_cb_finish, 
        update_cb = this.test_get_item_cb_update,
    }
    obj:RegTask(task_state)
    Assert(1 == obj:GetRegTaskNum(), "reg task num error")
    obj:Update(g_target_type.GET_ITEM, {1002, 1})
    Assert(1 == obj:GetRegTaskNum(), "reg task num error")
    obj:Update(g_target_type.GET_ITEM, {1001, 1})
    Assert(1 == obj:GetRegTaskNum(), "reg task num error")
    obj:Update(g_target_type.GET_ITEM, {1001, 1})
    Assert(0 == obj:GetRegTaskNum(), "reg task num error")

	
    task_state = 
    {
        num = 0,
        cfg={
           target_type  = g_target_type.GET_ITEM,  --值为 target_type
           para1=1001, para2 = 2
        },
        finish_cb = this.test_get_item_cb_finish, 
        update_cb = this.test_get_item_cb_update,
    }
    obj:RegTask(task_state)
    Assert(1 == obj:GetRegTaskNum(), "reg task num error")
    obj:Update(g_target_type.GET_ITEM, {1002, 1})
    Assert(1 == obj:GetRegTaskNum(), "reg task num error")
    obj:Update(g_target_type.GET_ITEM, {1001, 1})

    --中途加新任务
    task_state = 
    {
        num = 0,
        cfg={
           target_type  = g_target_type.GET_QUALITY_ITEM,  --值为 target_type
           para1=1001, para2 = 2, para3=2 --品质》2，数量>=2
        },
        finish_cb = this.test_get_quality_item_cb_finish, 
        update_cb = this.test_get_quality_item_cb_update,
    }
    obj:RegTask(task_state)
    Assert(2 == obj:GetRegTaskNum(), "reg task num error")
    obj:Update(g_target_type.GET_ITEM, {1001, 1})
    obj:Update(g_target_type.GET_QUALITY_ITEM, {1001, 2, 1})
    Assert(1 == obj:GetRegTaskNum(), "reg task num error")
    obj:Update(g_target_type.GET_QUALITY_ITEM, {1001, 2, 1})
    Assert(0 == obj:GetRegTaskNum(), "reg task num error")

    
    local unreg_task_state = 
    {
        num = 0,
        cfg={
           target_type  = g_target_type.GET_QUALITY_ITEM,  --值为 target_type
           para1=1001, para2 = 2, para3=2 --品质》2，数量>=2
        },
        finish_cb = this.test_get_quality_item_cb_finish, 
        update_cb = this.test_get_quality_item_cb_update,
    }
    assert(false == obj:UnRegTask(unreg_task_state))
    obj:RegTask(unreg_task_state)
    local old_num = obj:GetRegTaskNum()
    assert(true == obj:UnRegTask(unreg_task_state))
    assert(old_num-1 == obj:GetRegTaskNum())
	
end

g_task_com.test()