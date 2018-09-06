--[[实用功能
--]]

require(g_work_path.."table")


local ms = {}
g_table.MultSetInsert(ms, 11)
assert(ms[11] == 1)
g_table.MultSetInsert(ms, 11)
assert(ms[11] == 2)
g_table.MultSetRemove(ms, 11)
assert(ms[11] == 1)
g_table.MultSetRemove(ms, 11)
assert(ms[11] == nil)

local concat1 = {1}
local concat2 = {3,4}
g_table.ConcatVaule(concat1, concat2)
assert(concat1[1]==1)
assert(concat1[2]==3)
assert(concat1[3]==4)



--local t = {a=1,2}
--t.c = t
--g_utility.PrintTable(t)
