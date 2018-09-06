--[[实用功能
--]]

require(g_work_path.."utility")





--local t = {a=1,2}
--t.c = t
--g_utility.PrintTable(t)

local function TestTrack1()
    local t = {a=3}
	local t2 = {a=33}
    t = g_utility.Track(t, nil, nil, "my_table")
    t.a = 4
    local ib = t.b
    t2 = g_utility.Track(t2, nil, nil, "my_table")
    t2.a = 44
    local ib = t2.b
	
	t = g_utility.StopTrack(t)
	ib = t.a
	assert(t.a == 4)
	
	t2 = g_utility.StopTrack(t2)
	ib = t2.a
	assert(t2.a == 44)
	
end

local function TestTrack2()
    local t = {a=3, b = 32}
    t = g_utility.Track(t, nil, "r", "my_table")
    t.a = 4
    local ib = t.b

	
	t = g_utility.StopTrack(t)
	ib = t.a
	assert(t.a == 4)
	
end
local function TestTrack3()
    local t = {a=3, b = 32}
    t = g_utility.Track(t, nil, "w", "my_table")
    t.a = 4
    local ib = t.b

	
	t = g_utility.StopTrack(t)
	ib = t.a
	assert(t.a == 4)
	
end
--TestTrack1()  --正确的情况：出4个跟踪日志
--if false then
if true then
  print("ignore utility test")

else
 print("below case. track log is ok")
 TestTrack2()  --正确的情况：出1个跟踪日志
 print("below case. track log is ok")
 TestTrack3()  --正确的情况：出1个跟踪日志
end

