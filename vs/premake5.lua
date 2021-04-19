--
-- premake5 file to build RecastDemo
-- http://premake.github.io/
--

local action = _ACTION or ""
local outdir = action

workspace  "CppUtility"
	configurations { 
		"Debug",
		--"Release"
	}
	location (outdir)
    defines { "DEBUG_TEST_CPU_TIME" }

project "CppUtility"
	language "C++"
	kind "ConsoleApp"


	includedirs { 
		"../external/svr_util/include/",
	}
    files	{
		"../src/**.cpp",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../src/**.cc",
		"../src/**.h",
		"../src/**.hpp",
		"../src/**.cpps",  --显示用，不参加cmake编译
		"../src/**.txt",
		"../src/**.md",
        "../external/svr_util/include/**.h",
	}


project "SocketTest"
	language "C++"
	kind "ConsoleApp"


	includedirs { 
		--"../dep/hiredis",
	}
	files	{ 
		"../src/SocketTest/**.cpp",  --**递归所有子目录，指定目录可用 "cc/*.cpp" 或者  "cc/**.cpp"
		"../src/SocketTest/**.cc",
		"../src/SocketTest/**.h",
		"../src/SocketTest/**.hpp",
		"../src/SocketTest/**.cpps",  --显示用，不参加cmake编译
	}