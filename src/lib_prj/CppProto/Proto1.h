
#pragma once
#include <string>
#include <vector>
#include <unordered_map>

#pragma pack(push)
#pragma pack(1)
namespace proto {

	//先定义参数信息，以后再具体实现, 域的顺序 和 遗漏，都会初始化的时候检查出来
#define PROTO_ALL_PROTO1 \
	DB_CLASS_NAME(Ride)\
	DB_FIELD(ids)\
	DB_FIELD(s)\
	DB_FIELD(m)\
	DB_CLASS_END\
\
	DB_CLASS_NAME(insert_cs)\
	DB_FIELD(id)\
	DB_FIELD(a)\
	DB_FIELD(vecInt)\
	DB_FIELD(ride)\
	DB_FIELD(vecData)\
	DB_FIELD(mapData)\
	DB_CLASS_END\
\
	DB_CLASS_NAME(insert_sc)\
	DB_FIELD(id)\
	DB_FIELD(ret)\
	DB_FIELD(a)\
	DB_CLASS_END\
\
	DB_CLASS_NAME(Data)\
	DB_FIELD(id)\
	DB_FIELD(ids)\
	DB_CLASS_END\



	struct Data
	{
		uint8_t id;
		std::vector<uint32_t> ids;
	};
	struct Ride 
	{
		std::vector<uint32_t> ids;
		std::string s;
		std::unordered_map<uint32_t, uint32_t> m;
	};

	//约定前 uint16_t 必须为 消息号id.
	struct insert_cs
	{
		const uint16_t id = 1;
		uint32_t a;
		std::vector<uint32_t> vecInt;
		Ride ride; 
		std::vector<Data> vecData;
		std::unordered_map<uint32_t, Data> mapData;
	};
	struct insert_sc
	{
		const uint16_t id = 2;
		bool ret;
		uint32_t a;
	};

}



#pragma pack(pop)
