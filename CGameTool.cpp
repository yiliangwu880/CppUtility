#include "CGameTool.h"
#include "CRedBlackFight.h"

using namespace NPlatformService;
using namespace com_protocol;

#define DEBUG_LOG   DebugInfoLog

namespace NGameService
{
	CGameTool::CGameTool() : m_pHandler(NULL), m_pCfg(NULL)
	{

	}

	CGameTool::~CGameTool()
	{

	}

	void CGameTool::init(CRedBlackFightHandler* pHandler, const NRedBlackFightConfigData::RedBlackFightConfig* pCfg)
	{
		m_pHandler = pHandler;
		m_pCfg = pCfg;
	}

	// 测试配牌配置，仅做为内部配牌测试使用
	bool CGameTool::testConfigGameCards(SRedBlackRoom& roomData, com_protocol::RedBlackCardInfo& redCard, com_protocol::RedBlackCardInfo& blackCard)
	{
		// 测试配牌配置，仅做为内部配牌测试使用
		const NRedBlackFightConfigData::ProfitControlCfg * pRoomControlConfig = m_pHandler->getRoomControlCfg(roomData);
		if (pRoomControlConfig == NULL)
		{
			OptErrorLog("testConfigGameCards error invalid room config, roomId = %u", roomData.roomId);
			return false;
		}

		if (pRoomControlConfig->control_mode != 1) return false;

		const NConfigCardTest::ConfigCardTest& serviceCfgCardsTest = m_pHandler->getSrvOpt().getConfigCardTestCfg(true);

		unsigned int idx = 0;
		unsigned int flag = false;
		unsigned int level = roomData.getLevel();
		RedBlackCards redBlackCards[RedBlackBothSides];  // 红黑双方的牌
		string usename = "";

		map<unsigned int, NConfigCardTest::ServiceCardCfg>::const_iterator it = serviceCfgCardsTest.service_card_cfg_map.find(m_pHandler->getSrvType());
		if (it == serviceCfgCardsTest.service_card_cfg_map.end()) return false;

		const NConfigCardTest::ServiceCardCfg &cfgCardsTest = it->second;
		if (cfgCardsTest.switch_value != 1) return false;

		NumberStr numStr;
		snprintf(numStr, sizeof(numStr) - 1, "%u", roomData.roomId);
		string roomIdStr = numStr;
		snprintf(numStr, sizeof(numStr) - 1, "%u", roomData.srvId);
		roomIdStr.replace(roomIdStr.find_first_of(numStr), strlen(numStr), "");
		uint32_t id = atoi(roomIdStr.c_str());

		for (idx = 0; idx < cfgCardsTest.player_card_cfg_array.size(); ++idx)
		{
			const NConfigCardTest::PlayerCardCfg& cfgCards = cfgCardsTest.player_card_cfg_array[idx];

			if (cfgCards.level != 0 && cfgCards.level != level) continue;
			if (cfgCards.room_id != 0 && cfgCards.room_id != id) continue;
			if (cfgCards.client_type != 0 && cfgCards.client_type != m_pCfg->common_cfg.client_type && m_pCfg->common_cfg.client_type != 0) continue;

			if (cfgCards.switch_value != 1
				|| roomData.getPlayerInfo(cfgCards.name.c_str()) == NULL
				|| cfgCards.area_card_array.size() < RedBlackBothSides) continue;

			for (unsigned int sidesIdx = 0; sidesIdx < RedBlackBothSides; ++sidesIdx)
			{
				const vector<unsigned int>& gameCards = cfgCards.area_card_array[sidesIdx].Cards;
				if (gameCards.size() < RedBlackCardCount)
				{
					redCard.clear_cards();
					blackCard.clear_cards();
					return false;
				}

				// 红黑双方
				RedBlackCards& cards = redBlackCards[sidesIdx];
				const bool isRed = (sidesIdx == 0);
				unsigned char cardValue = 0;
				for (unsigned int idx = 0; idx < RedBlackCardCount; ++idx)
				{
					cardValue = gameCards[idx];
					cards.push_back(cardValue);

					if (isRed) redCard.add_cards(cardValue);
					else blackCard.add_cards(cardValue);
				}
			}

			usename = cfgCards.name;
			flag = true;
			break;
		}

		if (!flag) return false;

		randomCard(roomData, redCard, blackCard, redBlackCards);
		OptWarnLog("test config player cards, username = %s", usename.c_str());

		return true;
	}

	int CGameTool::doCalculateCard(const unsigned int roomId, SRedBlackRoom& roomData, com_protocol::RedBlackCardInfo& redCard, com_protocol::RedBlackCardInfo& blackCard)
	{
		// 测试配牌配置，仅做为内部配牌测试使用
		if (testConfigGameCards(roomData, redCard, blackCard)) return roomData.winNameType;

		SRedBlackRoom tmpRoomData(m_pCfg);
		com_protocol::RedBlackCardInfo tmpRedCard;
		com_protocol::RedBlackCardInfo tmpBlackCard;

		SRedBlackRoom firstRoomData(m_pCfg);
		com_protocol::RedBlackCardInfo firstRedCard;
		com_protocol::RedBlackCardInfo firstBlackCard;


		double areaRate[com_protocol::ERedBlackArea::ERedBlackLucky + 1] = { 0.00 };
		double preRewardValue = -99999999999.99;
		double preKRewardValue = 99999999999.99;
		bool isOK = false;
		unsigned int idx = 0;


		const NRedBlackFightConfigData::ProfitControlCfg * pRoomControlConfig = m_pHandler->getRoomControlCfg(roomData);
		if (pRoomControlConfig == NULL)
		{
			OptErrorLog("doCalculateCard error invalid room config, roomId = %u", roomId);
			return 0;
		}

		bool bKillScore = isKillScore(roomId, roomData, pRoomControlConfig);

		for (idx = 0; idx < pRoomControlConfig->loop_cnt; ++idx)
		{
			tmpRedCard.clear_cards();
			tmpBlackCard.clear_cards();
			tmpRoomData.reset(0);
			randomCalculateCard(tmpRoomData, tmpRedCard, tmpBlackCard);  // 随机牌
			tmpRoomData.getAreaRate(areaRate);

			tmpRoomData.players  = roomData.players;
			tmpRoomData.getAreaRate(tmpRoomData.areaRate);
			double betValue = roomData.getPlayersBetValue();
			double returnValue = tmpRoomData.getPlayersReturnValue();
			double rewardValue = betValue - returnValue;  
			if (bKillScore)
			{
				//x ≤ 实时杀分率 ，则杀分：不断循环模拟发牌且计算本局系统盈亏X(n)，直到循环到X(n)≥0的结果做为开奖结果
				isOK =(rewardValue >= ZeroLine); 
			} 
			else
			{
				//x ＞实时杀分率 ，则不杀分：不断循环模拟发牌且计算本局系统盈亏X(n)，直到循环到X(n)≤0的结果做为开奖结果
				isOK = (rewardValue <= ZeroLine);
			}

			if (pRoomControlConfig->control_mode == 0) //随机模式就不做控制 一次发好牌
			{
				isOK = true;
			}
			else if (fabs(betValue) < ZeroLine) //没有真人玩家不做控制
			{
				isOK = true;
			}

			DEBUG_LOG("doCalculateCard roomId=%u isOK = %u , rewardValue = %.2f , betValue = %.2f , returnValue = %.2f, idx=%d", roomId, isOK, rewardValue, betValue, returnValue, idx);
			if (isOK)
			{
				roomData.winNameType = tmpRoomData.winNameType;
				roomData.cardType = tmpRoomData.cardType;
				roomData.isDoubleNine = tmpRoomData.isDoubleNine;
				roomData.isSameColourA = tmpRoomData.isSameColourA;

				redCard = tmpRedCard;
				blackCard = tmpBlackCard;
				isOK = true;  //
				break;
			}

			if (bKillScore)  //杀分
			{
				if (rewardValue > preRewardValue)  //保存最符合的分
				{
					firstRoomData.winNameType = tmpRoomData.winNameType;
					firstRoomData.cardType = tmpRoomData.cardType;
					firstRoomData.isDoubleNine = tmpRoomData.isDoubleNine;
					firstRoomData.isSameColourA = tmpRoomData.isSameColourA;

					firstRedCard = tmpRedCard;
					firstBlackCard = tmpBlackCard;
					preRewardValue = rewardValue;
				}
			}
			else
			{
				if (rewardValue < preKRewardValue)  //保存最符合的分
				{
					firstRoomData.winNameType = tmpRoomData.winNameType;
					firstRoomData.cardType = tmpRoomData.cardType;
					firstRoomData.isDoubleNine = tmpRoomData.isDoubleNine;
					firstRoomData.isSameColourA = tmpRoomData.isSameColourA;

					firstRedCard = tmpRedCard;
					firstBlackCard = tmpBlackCard;
					preKRewardValue = rewardValue;
				}
			}
			//循环最大数，还不符合期望盈亏，打印错误日志
			if(idx == (pRoomControlConfig->loop_cnt -1))//last loop
			{ 
				if (bKillScore)  //杀分
				{
					if (preRewardValue < 0 )  //
					{
						DebugErrorLog("bKillScore=%d preRewardValue=%f", bKillScore, preRewardValue);
					}
				}
				else
				{
					if (preKRewardValue > 0)  //
					{
						DebugErrorLog("bKillScore=%d preKRewardValue=%f", bKillScore, preKRewardValue);
					}
				}
			}
		}

		if (!isOK)  //如果不是就按照首次开牌结果
		{
			roomData.winNameType = firstRoomData.winNameType;
			roomData.cardType = firstRoomData.cardType;
			roomData.isDoubleNine = firstRoomData.isDoubleNine;
			roomData.isSameColourA = firstRoomData.isSameColourA;

			redCard = firstRedCard;
			blackCard = firstBlackCard;
		}

		DEBUG_LOG("doCalculateCard roomId=%u loop_cnt = %u , roomData.sumReward = %.2f , roomData.sumBet = %.2f", roomId, idx, roomData.sumReward, roomData.sumBet);
		return roomData.winNameType;

	}

	// 随机&计算双方的牌
	int CGameTool::randomCalculateCard(SRedBlackRoom& roomData, com_protocol::RedBlackCardInfo& redCard, com_protocol::RedBlackCardInfo& blackCard)
	{
		unsigned char pokerCard[BasePokerCardCount] = { 0 };
		memcpy(pokerCard, AllPokerCard, BasePokerCardCount * sizeof(unsigned char));

		unsigned int pokerCardCount = BasePokerCardCount - 1;
		unsigned int randomIdx = 0;
		RedBlackCards redBlackCards[RedBlackBothSides];  // 红黑双方的牌
		for (unsigned int sidesIdx = 0; sidesIdx < RedBlackBothSides; ++sidesIdx)
		{
			// 红黑双方
			RedBlackCards& cards = redBlackCards[sidesIdx];
			const bool isRed = (sidesIdx == 0);
			unsigned char cardValue = 0;
			for (unsigned int idx = 0; idx < RedBlackCardCount; ++idx)
			{
				// 随机牌
				randomIdx = CRandomNumber::getUIntNumber(0, pokerCardCount);
				cardValue = pokerCard[randomIdx];
				cards.push_back(cardValue);

				if (isRed) redCard.add_cards(cardValue);
				else blackCard.add_cards(cardValue);

				pokerCard[randomIdx] = pokerCard[pokerCardCount--];
			}
		}

		return randomCard(roomData, redCard, blackCard, redBlackCards);

	}

	int CGameTool::randomCard(SRedBlackRoom& roomData, com_protocol::RedBlackCardInfo& redCard, com_protocol::RedBlackCardInfo& blackCard, RedBlackCards redBlackCards[RedBlackBothSides])
	{
		unsigned int redCardType = 0;
		unsigned int blackCardType = 0;
		RedBlackCards* winCards = NULL;
		bool isRedWin = m_redBlackCardType.doCompareCard(redBlackCards[0], redBlackCards[1], redCardType, blackCardType);
		if (isRedWin)
		{
			roomData.winNameType = com_protocol::EGameBothNameType::ERedName;
			roomData.cardType = redCardType;
			winCards = &redBlackCards[0];
		}
		else
		{
			roomData.winNameType = com_protocol::EGameBothNameType::EBlackName;
			roomData.cardType = blackCardType;
			winCards = &redBlackCards[1];
		}

		// 如果是对子，判断是否是对9以上牌型
		if (roomData.cardType == com_protocol::ERedBlackCardType::ERBCoupleCard)
		{
			std::sort(winCards->begin(), winCards->end(), CRedBlackCardType::descendSort);

			const unsigned int coupleValue = (*winCards)[1] % PokerCardBaseValue;  // 取中间的一张牌才是对子
			if (coupleValue == 1 || coupleValue > m_pCfg->process_control_cfg.luck_double_num)
			{
				roomData.isDoubleNine = true;
			}
		}

		if (roomData.cardType == com_protocol::ERedBlackCardType::ERBSameColourCard)
		{
			std::sort(winCards->begin(), winCards->end(), CRedBlackCardType::descendSort);

			const unsigned int coupleValue = (*winCards)[0] % PokerCardBaseValue;  // 取中间的一张牌才是对子
			if (coupleValue == 1)
			{
				roomData.isSameColourA = true;
			}
		}

		redCard.set_type(redCardType);
		blackCard.set_type(blackCardType);

		return roomData.winNameType;
	}

	bool CGameTool::isKillScore(const unsigned int roomId, SRedBlackRoom& roomData, const NRedBlackFightConfigData::ProfitControlCfg *pRoomControlConfig)
	{
		double darkReward = 0.00;
		if (pRoomControlConfig == NULL)
		{
			return true; //异常就杀分
		}

		if (roomData.sumBet > ZeroLine)
		{
			darkReward = roomData.sumReward / (roomData.sumBet + pRoomControlConfig->init_stock)*(1 - pRoomControlConfig->return_rate);	
		}
		else
		{
			darkReward = pRoomControlConfig->return_rate;
		}
		//杀分率
		double killScore = 1 + (darkReward - 1) / (2 * (1 - pRoomControlConfig->tax_secret));
		DEBUG_LOG("cal killScore=%f darkReward=%f tax_secret=%f", killScore, darkReward, pRoomControlConfig->tax_secret);
		if (killScore < pRoomControlConfig->kill_point_rate_min)
		{
			killScore = pRoomControlConfig->kill_point_rate_min;
			DEBUG_LOG("isKillScore set min");
		}
		else if (killScore > pRoomControlConfig->kill_point_rate_max)
		{
			killScore = pRoomControlConfig->kill_point_rate_max;
			DEBUG_LOG("isKillScore set max");
		}

		unsigned int killRand = CRandomNumber::getUIntNumber(1, 100);
		bool bKillScore = (killRand <= (unsigned int)(killScore * 100));

		DEBUG_LOG("isKillScore roomId=%u killRand=%d killScore*100=%f bKillScore=%d", roomId, killRand, killScore*100, bKillScore);
		return bKillScore;
	}

	double CGameTool::getBetByOption(unsigned int chipSeq)
	{
		return chipSeq >= m_pCfg->bet_cfg.bet_value.size() ? 0 : m_pCfg->bet_cfg.bet_value[chipSeq];
	}


	double CGameTool::getAeraBetLimit(const SRedBlackRoom& roomData, const unsigned int areaType)
	{
		double areaLimit = 0.00;
		const NRedBlackFightConfigData::ProfitControlCfg * pRoomControlConfig = m_pHandler->getRoomControlCfg((SRedBlackRoom&)roomData);
		if (pRoomControlConfig == NULL)
		{
			OptErrorLog("getAeraBetLimit error invalid room config");
			return RedBlackNotFoundRoomCfgError;
		}

		if (areaType == com_protocol::ERedBlackArea::ERedBlackLucky)
		{
			double areaLimitTmp = 0.00;
			for (auto it = m_pCfg->odds_cfg.lucky_rate.begin(); it !=  m_pCfg->odds_cfg.lucky_rate.end(); it++)
			{
				if (areaLimitTmp < it->second)
				{
					areaLimitTmp = it->second;
				}
			}

			if (areaLimitTmp > ZeroLine)
			{
				areaLimit = pRoomControlConfig->reward_limit / areaLimitTmp;
			}
			else
			{
				areaLimit = pRoomControlConfig->reward_limit;
			}

		}
		else if (areaType == com_protocol::ERedBlackArea::ERedBlackRed)
		{
			if (m_pCfg->odds_cfg.red_rate > ZeroLine)
			{
				areaLimit = pRoomControlConfig->reward_limit / m_pCfg->odds_cfg.red_rate;
			}
			else
			{
				areaLimit = pRoomControlConfig->reward_limit;
			}
		}
		else
		{
			if (m_pCfg->odds_cfg.black_rate > ZeroLine)
			{
				areaLimit = pRoomControlConfig->reward_limit / m_pCfg->odds_cfg.black_rate;
			}
			else
			{
				areaLimit = pRoomControlConfig->reward_limit;
			}
		}

		return areaLimit;
	}

	int CGameTool::checkBetChipInfo(const SRedBlackRoom& roomData, SRedBlackPlayerData* playerData, float playerGold, const AreaToBetInfoMap& areaBetInfo, unsigned int rate, unsigned int optType)
	{
		const bool needCheckPlayerBetInfo = (playerData != NULL);
		float playerAllBetValue = 0.00;
		int ret = 0;

		const NRedBlackFightConfigData::ProfitControlCfg * pRoomControlConfig = m_pHandler->getRoomControlCfg((SRedBlackRoom&)roomData);
		if (pRoomControlConfig == NULL)
		{
			OptErrorLog("checkBetChipInfo error invalid room config");
			return RedBlackNotFoundRoomCfgError;
		}

		for (AreaToBetInfoMap::const_iterator plAbIt = areaBetInfo.begin(); plAbIt != areaBetInfo.end(); ++plAbIt)
		{
			// 筹码信息验证
			const unsigned int areaId = plAbIt->first;
			const float betValue = (plAbIt->second.betValue * rate);

			if (needCheckPlayerBetInfo)
			{
				// 玩家信息验证
				double playerAreaAllBetValue = betValue;
				AreaToBetInfoMap::const_iterator btIt = playerData->areaBetInfo.find(areaId);
				if (btIt != playerData->areaBetInfo.end())
				{
					playerAreaAllBetValue += btIt->second.betValue;
				}

				//下注了红，就不能下注黑，反之相同
				if (areaId == com_protocol::ERedBlackArea::ERedBlackRed)
				{
					AreaToBetInfoMap::const_iterator pbIt = playerData->areaBetInfo.find(com_protocol::ERedBlackArea::ERedBlackBlack);
					if (pbIt != playerData->areaBetInfo.end() && pbIt->second.betValue > ZeroLine)
					{
						DEBUG_LOG("checkBetChipInfo already bet ERedBlackBlack betareaId=%u betValue = %.2f ", areaId, pbIt->second.betValue);
						return RedBlackAlreadyBetBlackError;
					}
				}
				else if (areaId == com_protocol::ERedBlackArea::ERedBlackBlack)
				{
					AreaToBetInfoMap::const_iterator pbIt = playerData->areaBetInfo.find(com_protocol::ERedBlackArea::ERedBlackRed);
					if (pbIt != playerData->areaBetInfo.end() && pbIt->second.betValue > ZeroLine)
					{

						DEBUG_LOG("checkBetChipInfo already bet ERedBlackRed betareaId=%u betValue = %.2f ", areaId, pbIt->second.betValue);
						return RedBlackAlreadyBetRedError;
					}
				}

				double areaLimit = getAeraBetLimit(roomData, areaId);
				if (playerAreaAllBetValue > areaLimit) return RedBlackPlayerAreaBetValueMaxLimit;
			}

			playerAllBetValue += betValue;
		}

		if (needCheckPlayerBetInfo)
		{
			// 玩家在各个区域已经下注了的金额
			for (AreaToBetInfoMap::const_iterator it = playerData->areaBetInfo.begin(); it != playerData->areaBetInfo.end(); ++it) playerAllBetValue += it->second.betValue;
		}

		ret = (playerGold >= playerAllBetValue) ? SrvOptSuccess : (int)RedBlackFightGoldInsufficient;

		//停止定时器
		if (ret == SrvOptSuccess && playerData)
		{
			playerData->stopPlayTimer();
		}

		return ret;
	}

};