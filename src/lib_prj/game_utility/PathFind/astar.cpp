#include "astar.h"
#include "memory.h"
#include <unordered_map>
#include <utility>

using namespace PathFind;
struct Node
{
	enum NodeState
	{
		NS_NONE,
		NS_OPENLIST,
		NS_CLOSELIST,
	};
	Pos m_pos;
	Node *m_father;
	uint32_t m_g;
	uint32_t m_h;
	NodeState m_state;
	
	Node(const Pos &pos)
		: m_pos(pos)
		, m_father(nullptr)
		, m_g(0)
		, m_h(0)
		, m_state(NS_NONE)
	{}

	inline bool operator< (const Node &point) const
	{
		return (m_g + m_h) < (point.m_g + point.m_h);
	}
};

class NodeCmp
{
	bool reverse = false;
public:
	NodeCmp(const bool& revparam = false)
	{
		reverse = revparam;
	}
	using NodePoint = Node *;
	bool operator()(const NodePoint &lhs, const NodePoint &rhs) const
	{
		if (reverse) return (*rhs < *lhs);
		else return (*lhs < *rhs);
	}
};
//节点管理
class NodeMap
{
	Pos m_src;
	int32_t m_radius = DEFAULT_MAX_RADIUS;
	std::unordered_map<uint32_t, Node> m_NodeMap;
public:
	NodeMap(const Pos &src, uint32_t radius)
		:m_src(src)
		,m_radius(radius)
	{}
	~NodeMap()
	{
		m_NodeMap.clear();
	}
	inline Node* Get(const Pos &pos) {
		return Get(pos.x, pos.y);
	};
	Node* Get(uint32_t x, uint32_t y);

private:
	inline uint32_t calIndex(uint32_t x, uint32_t y) const;
	inline bool checkInRangeX(uint32_t x) const;
	inline bool checkInRangeY(uint32_t y) const;
};


Node* NodeMap::Get(uint32_t x, uint32_t y)
{
	if (!checkInRangeX(x))
	{
		return nullptr;
	}
	if (!checkInRangeY(y))
	{
		return nullptr;
	}
	uint32_t idx = calIndex(x, y);

	auto it = m_NodeMap.find(idx);
	if (it != m_NodeMap.end())
	{
		return &(it->second);
	}
	else
	{
		auto pair = m_NodeMap.emplace(std::make_pair(idx, Node(Pos((uint16_t)x, (uint16_t)y))));
		return &(pair.first->second);//链式哈希表，每个数据存储占据一个节点，所以增删操作，不会迭代器失效）
	}
}

bool NodeMap::checkInRangeX(uint32_t x) const
{
	uint32_t maxX = m_src.x + m_radius;
	uint32_t minX = m_src.x > m_radius ? m_src.x - m_radius : 0;

	if (x > maxX || x < minX)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool NodeMap::checkInRangeY(uint32_t y) const
{
	uint32_t maxY = m_src.y + m_radius;
	uint32_t minY = m_src.y > m_radius ? m_src.y - m_radius : 0;

	if (y > maxY || y < minY)
	{
		return false;
	}
	else
	{
		return true;
	}
}

uint32_t NodeMap::calIndex(uint32_t x, uint32_t y) const
{
	uint32_t dx = x - (m_src.x - m_radius);
	uint32_t dy = y - (m_src.y - m_radius);

	return dx + dy * 2 * m_radius;
}



namespace
{
	inline uint32_t calH(const Pos &fromPos, const Pos &toPos)
	{
		return abs(fromPos.x - toPos.x) + abs(fromPos.y - toPos.y);
	}

	inline uint32_t distance(const Pos &a, const Pos &b)
	{
		return abs(a.x - b.x) + abs(a.y - b.y);
	}
	// 偏移计算矩阵(x, y, weight)
	const int16_t offset[8][3] =
	{
		{-1, 1,		14},
		{0 , 1,		10},
		{1 , 1,		14},
		{-1, 0,		10},
		{1 , 0,		10},
		{-1, -1,	14},
		{0 , -1,	10},
		{1 , -1,	14},
	};
}
bool AStar::FindPath(const Pos &src, const Pos &dst, std::vector<Pos> &path, IsBlockFun m_blockFun)
{
	path.clear();
	if (src == dst)
	{
		m_result = UNREACHABLE;
		return false;
	}
	// 检查完全阻塞
	if (m_blockFun(dst, dst))
	{
		//L_INFO("dst is block");
		m_result = UNREACHABLE;
		return false;
	}

	// 距离、步长检查 
	uint32_t dx = abs(src.x - dst.x);
	uint32_t dy = abs(src.y - dst.y);
	if (dx > m_max_radius || dy > m_max_radius)
	{
		//L_INFO("dx > m_max_radius ");
		m_result = TOO_FAR;
		return false;
	}

	// ====================== A* 算法开始 ======================
	NodeMap nodeMap(src, m_max_radius);
	std::priority_queue<Node*, std::vector<Node*>, NodeCmp> openList(NodeCmp(true));

	//把起点加入 open list 。
	Node *startPoint = nodeMap.Get(src);
	startPoint->m_state = Node::NS_OPENLIST;
	openList.push(startPoint);

	uint32_t loopCnt = 0;
	while (loopCnt < MAX_FIND_NODE_NUM)
	{
		//L_INFO("loop %d", loopCnt);
		// 遍历 open list ，查找 F 值最小的节点，把它作为当前要处理的节点。
		if (openList.empty())
		{
			//L_INFO("openList is empty,find fail. loopCnt=%d", loopCnt);
			m_result = UNREACHABLE;
			break;
		}
		Node *curNode = openList.top();
		openList.pop();
		//把这个节点移到 close list 。
		curNode->m_state = Node::NS_CLOSELIST;

		//L_INFO("cur pos %d %d", curNode->m_pos.x, curNode->m_pos.y);
		// 拓展周围8个节点
		for (int i = 0; i < 8; i++)
		{
			uint16_t x = curNode->m_pos.x + offset[i][0];
			uint16_t y = curNode->m_pos.y + offset[i][1];

			if (m_blockFun(curNode->m_pos, Pos(x,y)))
			{
				//L_INFO("continue, is block");
				continue;
			}
			// 检查节点合法性
			Node *pPoint = nodeMap.Get(x, y);
			if (!pPoint)
			{
				//L_INFO("continue,near point fail. %d %d", curNode->m_pos.x + offset[i][0], curNode->m_pos.y + offset[i][1]);
				continue;
			}

			//L_INFO("near point %d %d", pPoint->m_pos.x, pPoint->m_pos.y);
			// 如果它是不可抵达的或者它在 close list 中，忽略它。
			if (pPoint->m_state == Node::NS_CLOSELIST)
			{
				//L_INFO("continue, is close list");
				continue;
			}

			// 如果它已经在 open list 中，检查这条路径 是否更好，用 G 值作参考。如果是这样，把它的父亲设置为当前方格，并重新计算它的 G 和 F 值。
			if (Node::NS_OPENLIST == pPoint->m_state)
			{
				uint32_t g = curNode->m_g + offset[i][2];
				if (pPoint->m_g > g) //h值一样，不用比较
				{
					pPoint->m_g = g;
					pPoint->m_father = curNode;
					//L_INFO("update OpenList %p", pPoint);
				}
				//L_INFO("continue, is openlist");
				continue;
			}

			pPoint->m_father = curNode;
			pPoint->m_g = curNode->m_g + offset[i][2];
			pPoint->m_h = calH(pPoint->m_pos, dst); //获取H值，
			pPoint->m_state = Node::NS_OPENLIST;
			openList.push(pPoint);
			//L_INFO("add openlist %d %d", pPoint->m_pos.x, pPoint->m_pos.y);

			// 是否到达目标点
			if (pPoint->m_pos == dst)
			{
				while (pPoint)
				{
					path.push_back(pPoint->m_pos);
					pPoint = pPoint->m_father;
				}
				m_result = SUCCESS;
				return true;
			}
		}
		loopCnt++;
	}

	if (loopCnt == MAX_FIND_NODE_NUM)
	{
		//L_INFO("findPath:max loop ");
		m_result = TOO_MUCH_NODE;
	}
	return false;
}
