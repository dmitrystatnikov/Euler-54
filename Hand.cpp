/*
 * Hand.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: dmitry
 */

#include "Hand.h"

#include <array>
#include <functional>
#include <iostream>
#include <numeric>
#include <utility>
#include <vector>

using namespace std;

Hand & Hand::operator = (container && cards)
{
	i_cards = std::move(cards);
	sort(i_cards.begin(), i_cards.end());
	i_value = handRank(cards);
	return *this;
}

size_t Hand::handRank(container const & hand)
{
	auto coupled = coupledRank(hand);
	auto nonCoupled = nonCoupledRank(hand);
	return max(coupled, nonCoupled);
}

size_t Hand::coupledRank(container const & hand)
{
	constexpr static auto shiftSize = (static_cast<size_t>(Card::Rank::Ace) + 1);
	vector< pair<pair<size_t, size_t>, HandRank> > coupledRanks;
	coupledRanks.reserve(6);
	coupledRanks.emplace_back(make_pair(1,1), HandRank::HighCard);
	coupledRanks.emplace_back(make_pair(2,1), HandRank::Pair);
	coupledRanks.emplace_back(make_pair(2,2), HandRank::TwoPair);
	coupledRanks.emplace_back(make_pair(3,1), HandRank::ThreeOfAKind);
	coupledRanks.emplace_back(make_pair(3,2), HandRank::FullHouse);
	coupledRanks.emplace_back(make_pair(4,1), HandRank::FourOfAKind);
	array<pair<size_t, size_t>, shiftSize> counters;
	auto cardValue = static_cast<size_t>(Card::Rank::Two);
	generate(counters.begin(), counters.end(), [&cardValue](){return make_pair(0, cardValue++);});
	for (auto const & c : hand)
	{
		++(counters[c.value()].first);
	}
	vector< pair<size_t, size_t> > countedCards;
	countedCards.reserve(hand.size());
	copy_if(counters.begin(), counters.end(), back_inserter(countedCards), [](pair<size_t, size_t> const & p) {return p.first > 0;});
	sort(countedCards.begin(), countedCards.end(), greater< pair<size_t, size_t> >());
	auto it = lower_bound(coupledRanks.begin(), coupledRanks.end(), make_pair(countedCards[0].first, countedCards[1].first),
							[](pair<pair<size_t, size_t>, HandRank> const & val, pair<size_t, size_t> const & key) {return val.first < key;});
	return accumulate(countedCards.begin(), countedCards.end(), static_cast<size_t>(it->second),
						[](size_t tmp, pair<size_t, size_t> const & p)
						{
							auto result = tmp;
							for (size_t i = 0; i < p.first; ++i)
								result = result * shiftSize + p.second;
							return result;
						});
}

size_t Hand::nonCoupledRank(container const & hand)
{
	auto s = hand.begin()->suit();
	auto flush = all_of(hand.begin(), hand.end(), [s](Card const & c) {return c.suit() == s;});
	auto expectedValue = hand.begin()->value();
	auto straight = all_of(hand.begin() + 1, hand.end(), [&expectedValue](Card const & c) {return c.value() == ++expectedValue;});
	auto handRank = HandRank::HighCard;
	if (flush && straight)
		handRank = HandRank::StraightFlush;
	if (flush && !straight)
		handRank = HandRank::Flush;
	if (straight && !flush)
		handRank = HandRank::Straight;
	auto shiftSize = (static_cast<size_t>(Card::Rank::Ace) + 1);
	return  accumulate(hand.crbegin(), hand.crend(), static_cast<size_t>(handRank),
						[shiftSize](size_t tmp, Card const & card) {return tmp * shiftSize + card.value();});
}
