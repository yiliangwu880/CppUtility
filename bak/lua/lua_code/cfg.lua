--配置文件，目的，让所有代码，移植到不同项目只需修改这里

function ErrorLog(...)
    g_utility.ErrorLog(...)
end
function DebugLog(...)
    g_utility.DebugLog(...)
end

g_work_path = "./"  --main.lua 的工作目录