#include <iostream>
#include <iomanip>
#include <string>
#include "Support.h"
#include <fstream>
#include <sstream>
#include <queue>

using namespace std;

// Location member functions
bool location::operator<(const location &L) const{
	// Matches state first then city
	if (L.state == state) return (L.city > city);
	return (L.state > state);
}

void location::print() const{
	for (int i=0;i<42;i++) cout << "-";
	cout << endl;

	cout << city << ", " << state << " (" <<
		geocode << ")" << endl;

	for (int i=0;i<42;i++) cout << "-";
	cout << endl;
}

// Raw data member functions
void rawdata::print() const{
	cout << fixed << setprecision(2);
	cout << setw(5) << month << " " << setw(5) << setprecision(2) << precip << " " << setw(5) << temp << endl;
}

// Summary member functions
summary::summary(){
	N = 0;

	precip_avg = 0;
	precip_max = 0;
	precip_min = 0;

	temp_avg = 0;
	temp_max = 0;
	temp_min = 0;
}

void summary::operator+=(rawdata &D){
	// Essentially, incorporate data

	// Initialize stats values
	if (N==0) {
		precip_min = D.precip;
		precip_max = D.precip;

		temp_min = D.temp;
		temp_max = D.temp;
	}
	else { 
		if (D.precip > precip_max) precip_max = D.precip;
		if (D.precip < precip_min) precip_min = D.precip;
		if (D.temp > temp_max) temp_max = D.temp;
		if (D.temp < temp_min) temp_min = D.temp;
	}
	precip_avg += D.precip;
	temp_avg += D.temp;	
	N += 1;
}

void summary::print() const{
	cout << fixed << setprecision(2);
	cout << ":" << setw(6) << setw(6) << 
		precip_min <<setw(6) << precip_max << setw(6) << precip_avg/N
		<< " :  " << fixed << setprecision(1) << temp_min << "  " << temp_max << 
		"  " << temp_avg/N << endl;
}

// Database member functions
void database::init_rawdata(string filename){
	// Set up file reading
	ifstream fin(filename.c_str());
	if (!fin.is_open()){
		cerr << filename << " failed to open." << endl;
		return;
	}
	string Line;
	while (getline(fin,Line)){
		extract_rawdata(Line);

		istringstream sin(Line);

		location loc;
		rawdata rd;
		// Read to rawdata and location structs
		sin >> rd.month >> loc.city >> loc.state >> loc.geocode;
		sin >> rd.precip >> rd.temp;

		int index;
		map<location, int>::iterator p0 = location_map.find(loc);

		// Set index 
		if (p0 == location_map.end()) {
			index = location_map.size();
			location_map[loc] = index;
			rawdata_cache.push_back(list<rawdata>());
		} else {
			index = p0->second;
		}

		// Add raw data
		rawdata_cache[index].push_back(rd);
	}
}

void database::print_rawdata(){
	// Handling all printing related to raw data;
	for (map<location,int>::iterator p0 = location_map.begin(); p0 != location_map.end(); p0++){
		p0->first.print();
		list<rawdata>& Raw = rawdata_cache[p0->second];
		for (list<rawdata>::iterator p1 = Raw.begin(); p1 != Raw.end(); p1++) {
			p1->print();
		}
	}
}

void database::init_summary(){
	extract_summary();
	// Populate hash tables
	for (map<location, int>::iterator p0 = location_map.begin(); p0 != location_map.end(); p0++) {
		const location& loc = p0->first;

		geocode_map[loc.geocode] = loc;
		city_map[loc.city].insert(loc.geocode);
		state_map[loc.state].insert(loc.geocode);
	}
}

void database::print_summary(string target){
	queue<string> geocodes;

	// Input is a geocode
	if (target.length() == 3) {
		geocodes.push(target);
	}
	// Finds corresponding state geocodes
	else if (state_map.find(target) != state_map.end()) {
		for (set<string>::iterator p0 = state_map[target].begin(); p0 != state_map[target].end(); p0++){
			geocodes.push(*p0);
		}
	}
	// Finds corresponding city's geocodes
	else if (city_map.find(target) != city_map.end()){
		for (set<string>::iterator p0 = city_map[target].begin(); p0 != city_map[target].end(); p0++){
			geocodes.push(*p0);
		}
	}


	// Index corresponds to month-1
	string months[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun", "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};

	while (!geocodes.empty()){
		location Loc = geocode_map[geocodes.front()];
		// Checks a geocode and then prints out
		if (geocode_map.find(geocodes.front()) != geocode_map.end()){
			int index = location_map[Loc];
			Loc.print();	
			for (int i = 0; i < 12; i++) {
				cout << months[i];
				summary_cache[index][i].print();
			}
		}
		geocodes.pop();
	}
}

void database::extract_rawdata(string &Line){
	// To iterate through each character replacing ' ' -> '_'
	for (size_t i=0; i<Line.size(); i++) {
		if (Line[i] == ' ') {
			Line[i] = '_';
		}
	}

	// ',' -> ' '
	for (size_t i=0; i<Line.size(); i++) {
		if (Line[i] == ',') {
			Line[i] = ' ';
		}
	}

}

void database::extract_summary(){
	summary_cache.resize(rawdata_cache.size());

	for (size_t i = 0; i < rawdata_cache.size(); i++){
		summary_cache[i].resize(12);

		// Iterate and update summary_cache with raw data
		for (list<rawdata>::iterator p0 = rawdata_cache[i].begin(); p0 != rawdata_cache[i].end(); p0++){
			int month = p0->month - 1;
			summary_cache[i][month] += *p0;
		}
	}

}

