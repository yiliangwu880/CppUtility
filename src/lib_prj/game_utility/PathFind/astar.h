/*
author:yiliangwu880
	you can get more refer from https ://github.com/yiliangwu880/CppUtility.git
*/
#pragma once
#include <time.h>
#include <sys/time.h>
#include <functional>
#include <queue>

namespace PathFind
{

	static const uint32_t DEFAULT_MAX_RADIUS = 40;//@brief 默认寻路半径
	static const uint32_t MAX_FIND_NODE_NUM = DEFAULT_MAX_RADIUS * 2 * DEFAULT_MAX_RADIUS * 2;//最大查找节点

	struct Pos
	{
		uint16_t x;
		uint16_t y;

	public:
		Pos(uint16_t ix, uint16_t iy)
			:x(ix)
			, y(iy)
		{};
		bool operator==(const Pos& pos) const {
			return x == pos.x && y == pos.y;
		};
	};
	//return true表示可以从 beforePos 走到 pos，
	using IsBlockFun = std::function<bool(const Pos &beforePos, const Pos &pos)>;

	class AStar
	{
	public:
		enum FindResult
		{
			None,
			SUCCESS,
			UNREACHABLE,
			TOO_FAR,
			TOO_MUCH_NODE,
		};

	private:
		uint32_t m_max_radius = DEFAULT_MAX_RADIUS;
		FindResult m_result = None;

	public:
		static AStar &Ins()
		{
			static AStar ins;
			return ins;
		}
		bool FindPath(const Pos &fromPos, const Pos &toPos, std::vector<Pos> &m_path, IsBlockFun m_blockFun);
		void SetMaxRadius(uint32_t max_radius) { m_max_radius = max_radius; };
		FindResult LastResult() { return m_result; }
	private:
		AStar() {};
	};

}

