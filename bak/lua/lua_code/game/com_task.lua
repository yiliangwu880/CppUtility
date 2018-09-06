--[[
任务功能
新任务类型需要做:
    target_type_infos里面加target_tyep类型
    策划配置按照 target_type_infos 要求填值
    加任务事件调用： g_task_com:RegTask
    任务更新触发调用：[g_task_com:Update(target_type, para_ls)]

使用例子：
    )注册一个任务
    
    local obj = g_class.New(g_task_com)
    function my_finish(task_state)
        //可以做存档操作，比如存task_id, num, is_finish
    end
    function my_update(task_state)
         //可以做存档操作，比如存task_id, num
    end
   local task_state = 
    {
        num = 0, 
        cfg={
           target_type  = target_type.LV,  --值为 target_type
           para1=1, --需要比较的值, 不需要的nil (策划配置来的)
        },
        finish_cb = my_finish, 
        update_cb = my_update,
    }

    obj.RegTask(task_state)
    ...
    
    obj:Update(target_type.GET_ITEM, {1002, 1})
--]]

require(g_work_path.."class")

g_task_com = g_task_com or {} 

local ErrorLog = print --根据不同项目修改日志函数
local EQUAL=0
local GREATER=1
local LESS=2

--和策划配置一致,任务类型
g_target_type=  
{
    KILL_MONSTER=1,
    LV=2,
    GET_QUALITY_ITEM = 3,
    GET_ITEM = 4,
}
--配置所有任务类型
local target_type_infos=
{
    --[[ 
        target_type_infos[xx]的成员，除最后一个和特殊表示，都是表达 Update参数和任务配置参数的关系。全部Update参数符合配置参数才会匹配该配置任务。
        最后一个参数 表达 task_state.num(进度)  和 配置参数para的关系, 符合条件表示完成。
        target_type_info.is_last_para_absolute = true表示从Update传入的最后一个参数是绝对值，否则表示累加值.
    --]]
    [g_target_type.KILL_MONSTER]={EQUAL, GREATER},             --杀id怪，>=para2
    [g_target_type.LV]={GREATER, is_last_para_absolute = true},                    --等级 >=para1
    [g_target_type.GET_QUALITY_ITEM]={EQUAL, GREATER, GREATER},    --获取id物品，>=品质，>=para2
    [g_target_type.GET_ITEM]={EQUAL, GREATER},             --获取id物品，>=para2
}

--target_cfg类型, 策划配置结构模板,代码没使用
local target_cfg=
{
   target_type=xx,  --值为 target_type
   para1=xx,para2=xx,para3=xx , --需要比较的值, 不需要的nil (策划配置来的)
}

--task_state类型
local task_state=
{
    num,  --这个进度，唯一状态变量
    cfg,  --target_cfg
    finish_cb, --完成任务回调,  格式，finish_cb(task_state)
    update_cb, --进度变化回调,  格式，update_cb(task_state)
}

function g_task_com:Construct()
--    所有注册的任务，格式： 
--    {
--        [g_target_type.xx]={task_state1,task_state2,..},
--        ..
--    }
   self.m_targets={} 
end



--注册任务，任务变化的时候会回调. 任务完成自动取消注册
--完成任务回调 task_state.finish_cb(task_state)
--进度变化回调 task_state.update_cb(task_state)
--para task_state  类型为 task_state. 任务事件会修改状态
function g_task_com:RegTask(task_state)
    --check illegal data
    if type(task_state.num) ~= "number" then
        ErrorLog("RegTask fail,illegal task_state")
        return
    end
    if type(task_state.finish_cb) ~= "function" then
        ErrorLog("RegTask fail,illegal task_state")
        return
    end
    if type(task_state.update_cb) ~= "function" then
        ErrorLog("RegTask fail,illegal task_state")
        return
    end
    if type(task_state.cfg.target_type) ~= "number" then
        ErrorLog("RegTask fail,illegal task_state")
        return
    end
    if type(task_state.cfg.para1) ~= "number" then
        ErrorLog("RegTask fail,illegal task_state")
        return
    end

    local target_type = task_state.cfg.target_type
    self.m_targets[target_type] = self.m_targets[target_type] or {}
    table.insert(self.m_targets[target_type], task_state)
end

--成功删除 返回true,找不到返回false
function g_task_com:UnRegTask(task_state)
    local target_type = task_state.cfg.target_type
    local task_state_ls = self.m_targets[target_type]
    if nil == task_state_ls then
        return false
    end
    for _, v in ipairs(task_state_ls) do
        if v == task_state then
            table.remove(task_state_ls, _)
            return true
        end
    end
    return false
end

--判断非最后一个参数决定的任务条件是否符合
--para target_type_info 为 target_type_infos的一个元素
--para para 格式为 {para1, para2, para3}
--return true表示符合条件
function g_task_com:IsMatchCondition(target_type_info, task_cfg, para)
    local para_num = #target_type_info
    for idx=1,para_num - 1 do
        local idx_str = tostring(idx)
        if target_type_info[idx] == EQUAL then
            if  para[idx] ~= task_cfg["para"..idx_str] then
                return false
            end        
        end
        if target_type_info[idx] == GREATER then
            if  para[idx] < task_cfg["para"..idx_str] then
                return false
            end        
        end
        if target_type_info[idx] == LESS then
            if  para[idx] > task_cfg["para"..idx_str] then
                return false
            end        
        end
    end --for idx=1,para_num - 1 do

    return true
end

function g_task_com:GetRegTaskNum()    
    local  num = 0
    for _, target_ls in pairs(self.m_targets) do
        num = num + #target_ls
    end
    return num
end

--事件
--para para_ls 格式位 {para1, para2, para3}, 每个参数的意义根据 target_type在target_type_infos里面表达的意义来
function g_task_com:Update(target_type, para_ls)
    local target_ls = self.m_targets[target_type] 
    if nil == target_ls then
        return
    end
    local target_type_info = target_type_infos[target_type]
    if nil == target_type_info then
        ErrorLog("find target_type_info fail, target_type =", target_type)
        return
    end
    --is_last_para_absolute true表示最后一个参数表示绝对值，false 表示累加值. default 表示累加
    is_last_para_absolute = target_type_info.is_last_para_absolute

    local remove_task_state_ids = {}  --完成等删除的任务
    --task_state 和 target_type_info 比较，不符合条件退出,符合改变进度
    for task_state_idx,task_state in ipairs(target_ls) do 
        local task_cfg = task_state.cfg
        local para_num = #target_type_info

        --判断是否符合条件
        if false == self:IsMatchCondition(target_type_info, task_cfg, para_ls) then
            break
        end

        --符合条件，判断最后一个参数
        local is_finish = false
        if true == is_last_para_absolute then
            task_state.num = para_ls[para_num]
        else
            task_state.num = task_state.num + para_ls[para_num]
        end
        local cfg_para = task_cfg["para"..tostring(para_num)] 
        if target_type_info[para_num] == GREATER then
            if  task_state.num >= cfg_para then
                is_finish = true
            end       
        elseif target_type_info[para_num] == LESS then
            if  task_state.num <= cfg_para then
                is_finish = true
            end    
        elseif target_type_info[para_num] == EQUAL then
            if  task_state.num == cfg_para then
                is_finish = true
            end        
        end

        if true == is_finish then
            task_state.finish_cb(task_state)
            table.insert(remove_task_state_ids, task_state_idx)
        else
            task_state.update_cb(task_state)
        end
    end --for i,task_state in ipairs(target_ls) do 

    --remove_task_state_ids元素内容从小到大排序,然后倒序删除数组索引，才会正确. 
    for idx = 1, #remove_task_state_ids do
        local re_idx = #remove_task_state_ids - idx + 1
        table.remove(target_ls, remove_task_state_ids[re_idx])
    end
end



