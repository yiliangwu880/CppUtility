--[[
模板代码
--]]



--从table中间开始迭代例子
tMembers={1,2,3}
for k,uin1 in next, tMembers  do
    for _,uin2 in next, tMembers, k  do --迭代相同table,注意过程别修改table内容
        --这里会获取组合 21,31, 32
    end
end