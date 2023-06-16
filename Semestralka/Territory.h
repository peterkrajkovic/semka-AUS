#pragma once
#include <string>
#include <iostream>
class Territory {
private :
	int sortNumber_;
	int numberOfCitizens_ = 0;
	std::string code_, officialTitle_, mediumTitle_, shortTitle_, note_, type_;
public:
	Territory(int sortNumber, std::string code, std::string officialTitle, std::string mediumTitle, std::string shortTitle, std::string note, std::string type) {
		sortNumber_ = sortNumber;
		code_ = code;
		officialTitle_ = officialTitle;
		mediumTitle_ = mediumTitle;
		shortTitle_ = shortTitle;
		note_ = note;
		type_ = type;
	}
	void setNumberOfCitizens(int count) {
		numberOfCitizens_ = count;
	}
	int getNumberOfCitizens() {
		return numberOfCitizens_;
	}
	std::string getOfficial() {
		return officialTitle_;
	}
	std::string getMedium() {
		return mediumTitle_;
	}
	std::string getShort() {
		return shortTitle_;
	}
	std::string getNote() {
		return note_;
	}
	int getSortNumber() {
		return sortNumber_;
	}
	std::string getType() {
		return type_;
	}
	void toString() {
		std::cout << officialTitle_ << " s poètom obyvate¾ov " << numberOfCitizens_ << std::endl;
	}
};