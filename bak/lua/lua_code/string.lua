
g_str = g_str or {}


--fun ·Ö¸î×Ö·û´®
--para str Ô´×Ö·û´®
--para set ·Ö¸ô·û×Ö·û´®
--return ½á¹û£º¸ñÊ½{str1, str2,..}
function g_str.split(str, sep)
	local sep, fields = sep or "\t", {}
	
	local pattern = string.format("([^%s]+)", sep)
	
	string.gsub(str, pattern, function(c) fields[#fields+1] = c end)

	return fields
end
