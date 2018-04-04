#include "datetime.h"
#include <time.h>
#include <string>
#include <sstream>
#include <cctype>
#include <iomanip>
#include <stdexcept>

DateTime::DateTime(){
	time_t raw;
	struct tm now;

	time(&raw);
	now = *localtime(&raw);
	hour = now.tm_hour;
	min = now.tm_min;
	sec = now.tm_sec;
	year = now.tm_year + 1900;
	month = now.tm_mon;
	day = now.tm_mday;
}


DateTime::DateTime(int hh, int mm, int ss, int year, int month, int day){
	hour = hh;
	min = mm;
	sec = ss;
	this->year = year;
	this->month = month;
	this->day = day;
}


bool DateTime::operator<(const DateTime& other) const{
	if(year < other.year){
		return true;
	}
	else if(year > other.year){
		return false;
	}

	if(month < other.month){
		return true;
	}
	else if(month > other.month){
		return false;
	}

	if(day < other.day){
		return true;
	}
	else if(day > other.day){
		return false;
	}

	if(hour < other.hour){
		return true;
	}
	else if(hour >other.hour){
		return false;
	}

	if(min < other.min){
		return true;
	}
	else if(min > other.min){
		return false;
	}

	if(sec < other.sec){
		return true;
	}
	else{
		return false;
	}
}



std::ostream& operator<<(std::ostream& os, const DateTime& other){
	os << std::setw(4) << std::setfill('0') << other.year << "-";
	os << std::setw(2) << std::setfill('0') << other.month << "-";
	os << std::setw(2) << std::setfill('0') << other.day << " ";
	os << std::setw(2) << std::setfill('0') << other.hour << ":";
	os << std::setw(2) << std::setfill('0') << other.min << ":";
	os << std::setw(2) << std::setfill('0') << other.sec;
	return os;
}

std::istream& operator>>(std::istream& is, DateTime& dt){
	std::string dateBuffer;
	std::string timeBuffer;
	is >> dateBuffer;
	is >> timeBuffer;

	//std::cout << dateBuffer << " " << timeBuffer << std::endl;


	if(dateBuffer.size() != 10 ||dateBuffer[4] != '-' || dateBuffer[7] !='-' || 
	timeBuffer.size() != 8 || timeBuffer[2] !=':' || timeBuffer[5] != ':'){
		std::cout << "DateTime format wrong." << std::endl;
    	throw std::invalid_argument("DateTime format wrong in istream");
	} 

	int i = 0;
	while(dateBuffer[i]){
		if(ispunct(dateBuffer[i])){
			dateBuffer[i] = ' '; 
		}
		i++;
	}

	i=0;
	while(timeBuffer[i]){
		if(ispunct(timeBuffer[i])){
			timeBuffer[i] = ' '; 
		}
		i++;
	}

	std::stringstream ds(dateBuffer);
	ds >> dt.year >> dt.month >> dt.day;
	std::stringstream ts(timeBuffer);
	ts >> dt.hour >> dt.min >> dt.sec;

	return is;
	
}

