g_table = g_table or {}  --处理table相关



--fun 删除数组多个元素
--para t 数组
--para idx_ls idx_ls元素内容必须从小到大排序
--[[
ex:
local remove_idxs = {}
for idx,v in ipairs(array) do 
    table.insert(remove_idxs, idx)
end
g_table.remove(array, remove_idxs)
]]
function g_table.remove(array, idx_ls)
    for idx = 1, #idx_ls do
        re_idx = #idx_ls - idx + 1
        table.remove(array, idx_ls[re_idx]) --倒序删除数组索引，才会正确. 
    end
end


function g_table.MultSetInsert(bag, element)
    bag[element] = (bag[element] or 0) + 1
end

function g_table.MultSetRemove(bag, element)
    local count = bag[element]
    bag[element] = (count and count > 1) and count - 1 or nil
end

--使table变成只读
function g_table.ReadOnly(t)
    local proxy = {}
    local mt = { -- create metatable
    __index = t,
    __newindex = function (t, k, v)
    ErrorLog("attempt to update a read-only table")
    end
    }
    setmetatable(proxy, mt)
    return proxy
end

-- 获取table={}元素数量
function g_table.GetTableNum(t)
	if type(t) ~= "table" then return 0 end
	local cnt = 0
	for k,v in pairs(t) do
		cnt = cnt + 1
	end
	return cnt
end

--数组合并数组,比如{1,2} 合并{3,4}={1,2,3,4}
function g_table.ConcatVaule(t, dependTable) 
	for key, var in pairs(dependTable) do
	    table.insert(t,var)
	end
end

--跟踪table读写
--para t table
--para key 被跟踪的key
--para op 跟踪的动作。为 "r" "w" "rw",分别表示读，写，读写
--例子： t = g_table.Track(t, nil, "rw")
function g_table.Track(t, k, op)
    local mt = { -- create metatable
        __index = function (tt, kk)
                    if k == nil or k == kk then
                        print("*access to element " .. tostring(kk))
                    end
                    return t[kk] -- access the original table
                    end,
        __newindex = function (tt, kk, v)
                    if k == nil or k == kk then
                        print("*update of element " .. tostring(kk) .. " to " .. tostring(v))
                    end
                        t[kk] = v -- update original table
                    end,
    }

    local proxy = {}
    setmetatable(proxy, mt)
    return proxy
end
--队列与双端队列, 运行几千年才可能溢出
---------------------------------------------
--[[
使用例子
lp=g_utility.NewDeque()
lp:PushFront(1)
lp:PushFront(2)
lp:PushBack(-1)
lp:PushBack(-2)

x=lp:PopFront()
print(x)
x=lp:PopBack()
print(x)
x=lp:PopFront()
print(x)
x=lp:PopBack()
print(x)

x=lp:PopBack()
print(x)
--输出结果
-- 2
-- -2
-- 1
-- -1
-- lua：... List is empty！
--]]
--para is_equal  比较函数， nil用默认==
function g_table.NewDeque(is_equal)
	local List={first=0, last=-1}
	List.is_equal = is_equal

	--use example:
	--[[
		function for_each(enemy)
			print("for each uin=", enemy.uin)
		end
		obj:ForEach(for_each)	
	--]]
	function List:ForEach(cb)
		for k,v in pairs(self) do
			if type(k) == "number" then
				cb(v)
			end
		end
	end

	--失败返回nil 
	--否则返回值，如果是table返回引用
	function List:Find(value)
		for k,v in pairs(self) do
			if type(k) == "number" then
				if self.is_equal == nil then
					if v == value then
						return v
					end
				else
					if self.is_equal(v, value) then
						return v
					end
				end
			end
		end
		return nil
	end

	function List:IsEmpty(value)
 		if self.first>self.last then 
	   		return true
	   end
	   return false
	end
	
	function List:Size(value)
 		local n_first = 0 - self.first
 		local n_last  = self.last + 1
 		return n_first + n_last
	end

	function List:PushFront(value)
	   self.first=self.first-1
	   self[ self.first ]=value
	end

	function List:PushBack(value)
	   self.last=self.last+1
	   self[ self.last ]=value
	end

	function List:PopFront()
	   local first=self.first
	   if first>self.last then 
	   		return nil
	   end
	   local value =self[first]
	   self[first]=nil
	   self.first=first+1
	   return value
	end

	function List:PopBack(list)
	   local last=self.last
	   if last<self.first then
	   		return nil
	   end
	   local value =self[last]
	   self[last]=nil
	   self.last=last-1
	   return value
	end
	
    return List
end
