--新增测试模块，在str_all_test_file 加字符串就可以了

require "cfg"
local str_all_lib_file=
{
}

local str_all_test_file=
{
	g_work_path.."game/com_task_test",
	g_work_path.."utility_test",
	g_work_path.."class_test",
	g_work_path.."table_test",
}

for _, var in ipairs(str_all_lib_file) do
    require(var)
end
for _, var in ipairs(str_all_test_file) do
    require(var)
end

g_utility.CheckIllegalGlobalName()

print("run end")
