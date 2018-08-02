/*
 * Hand.h
 *
 *  Created on: Jul 19, 2018
 *      Author: dmitry
 */

#ifndef HAND_H_
#define HAND_H_

#include "Card.h"

#include <algorithm>
#include <array>

class Hand {
public:
	constexpr static const size_t s_size = 5;

	using container = std::array<Card, s_size>;
	using const_iterator = container::const_iterator;

	enum class HandRank
	{
		HighCard = 1, Pair, TwoPair, ThreeOfAKind, Straight, Flush, FullHouse, FourOfAKind, StraightFlush
	};

	template <typename TempContainer>
	Hand(TempContainer && cards) : i_cards{cards[0], cards[1], cards[2], cards[3], cards[4]}
	{
		std::sort(i_cards.begin(), i_cards.end());
		i_value = handRank(i_cards);
	}

	Hand & operator = (container && cards);

	const_iterator begin() const noexcept {return i_cards.begin();}
	const_iterator end() const noexcept {return i_cards.end();}

	unsigned value() const {return i_value;}
private:
	static size_t handRank(container const & hand);
	static size_t coupledRank(container const & hand);
	static size_t nonCoupledRank(container const & hand);
	container i_cards;
	unsigned i_value;
};

inline bool operator == (Hand const & hand1, Hand const & hand2)
{
	return hand1.value() == hand2.value();
}

inline bool operator < (Hand const & hand1, Hand const & hand2)
{
	return hand1.value() < hand2.value();
}

inline bool operator < (Hand::HandRank r1, Hand::HandRank r2)
{
	return static_cast<int>(r1) < static_cast<int>(r2);
}

#endif /* HAND_H_ */
