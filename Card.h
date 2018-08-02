/*
 * Card.h
 *
 *  Created on: Jul 19, 2018
 *      Author: dmitry
 */

#ifndef CARD_H_
#define CARD_H_

#include <cstdint>

class Card {
public:
	enum class Rank
	{
		Two = 0, Three, Four, Five, Six, Seven,	Eight, Nine, Ten, Jack, Queen, King, Ace
	};

	enum class Suit
	{
		Clubs = 0, Spaids, Diamonds, Hearts
	};

	Card(Rank value, Suit suit) : i_rank(value), i_suit(suit) {}
	constexpr Rank rank() const noexcept {return i_rank;}
	constexpr Suit suit() const noexcept {return i_suit;}
	constexpr std::size_t value() const noexcept {return static_cast<std::size_t>(i_rank);}

private:
	Rank i_rank;
	Suit i_suit;
};

inline bool operator < (Card const & card1, Card const & card2)
{
	return card1.value() < card2.value();
}


#endif /* CARD_H_ */
