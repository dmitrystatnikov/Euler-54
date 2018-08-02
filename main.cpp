/*
 * main.cpp
 *
 *  Created on: Jul 19, 2018
 *      Author: dmitry
 */

#include "Hand.h"

#include <fstream>
#include <iostream>
#include <map>
#include <stdexcept>
#include <string>
#include <vector>

using namespace std;

map<char, Card::Rank> initRanksConverter()
{
	map<char, Card::Rank> ranks;
	ranks.emplace('2', Card::Rank::Two);
	ranks.emplace('3', Card::Rank::Three);
	ranks.emplace('4', Card::Rank::Four);
	ranks.emplace('5', Card::Rank::Five);
	ranks.emplace('6', Card::Rank::Six);
	ranks.emplace('7', Card::Rank::Seven);
	ranks.emplace('8', Card::Rank::Eight);
	ranks.emplace('9', Card::Rank::Nine);
	ranks.emplace('T', Card::Rank::Ten);
	ranks.emplace('J', Card::Rank::Jack);
	ranks.emplace('Q', Card::Rank::Queen);
	ranks.emplace('K', Card::Rank::King);
	ranks.emplace('A', Card::Rank::Ace);
	return ranks;
}

map<char, Card::Suit> initSuitsConverter()
{
	map<char, Card::Suit> suits;
	suits.emplace('C', Card::Suit::Clubs);
	suits.emplace('S', Card::Suit::Spaids);
	suits.emplace('D', Card::Suit::Diamonds);
	suits.emplace('H', Card::Suit::Hearts);
	return suits;
}

Card string2Card(string const & str)
{
	static const map<char, Card::Rank> s_ranks(initRanksConverter());
	static const map<char, Card::Suit> s_suits(initSuitsConverter());
	auto itR = s_ranks.find(str[0]);
	if (itR == s_ranks.end())
		throw runtime_error("Wrong rank: " + str);
	auto itS = s_suits.find(str[1]);
	if (itS == s_suits.end())
		throw runtime_error("Wrong suit: " + str);
	return Card(itR->second, itS->second);
}

int main(int argc, char * argv[])
{
	const char * fileName = argv[1];
	cout << fileName << endl;

	fstream iof(fileName, fstream::in | fstream::out);
	if (!iof)
		cout << "No such file" << endl;
	iof >> std::ws;
	size_t counter = 0;
	while (iof)
	{
		string str[5];
		vector<Card> hands[2];
		size_t value = 0;
		try
		{
			for (auto & hand : hands)
			{
				hand.reserve(Hand::s_size);
				for (auto & c : str)
				{
					iof >> c;
					if (!c.empty())
						hand.emplace_back(string2Card(c));
					else
						break;
				}
			}
			if (hands[0].size() == 5 && hands[1].size() == 5)
			{
				Hand hand1(hands[0]), hand2(hands[1]);
				if (hand2.value() < hand1.value())
					++counter;
			}
		}
		catch (std::exception & ex)
		{
			cout << ex.what() << endl;
		}
	}
	cout << counter << endl;
	iof.close();

	return 0;
}


