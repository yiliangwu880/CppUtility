
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#pragma pack(push)
#pragma pack(1)
namespace proto {
	struct test2_sc
	{
		const uint16_t id = 101;
		bool ret;
	};


	//先定义参数信息，以后再具体实现, 域的顺序 和 遗漏，都会初始化的时候检查出来
#define PROTO_ALL_PROTO2 \
	DB_CLASS_NAME(test2_sc)\
	DB_FIELD(id)\
	DB_FIELD(ret)\
	DB_CLASS_END\
}
#pragma pack(pop)
