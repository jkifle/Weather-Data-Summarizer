#ifndef SUPPORT_H
#define SUPPORT_H

#include <vector>
#include <unordered_map>
#include <list>
#include <map>
#include <string>
#include <set>

using namespace std;

struct location {
	string city;
	string state;
	string geocode;

	bool operator<(const location &L) const;
	void print() const; 
};

struct rawdata {
	int month;
	float precip;
	int temp;

	void print() const;
};

struct summary {
	int N;

	float precip_avg;
	float precip_max;
	float precip_min;

	float temp_avg;
	float temp_max;
	float temp_min;

	summary();
	void operator+=(rawdata &rd);
	void print() const;
};

class database {
	public:
		void init_rawdata(string filename);
		void print_rawdata();

		void init_summary();
		void print_summary(string target);

	private:
		void extract_rawdata(string &Line);
		void extract_summary();

		vector<list<rawdata> > rawdata_cache;
		map<location, int> location_map;

		vector<vector<summary> > summary_cache;
		unordered_map<string, set<string> > city_map;
		unordered_map<string, set<string> > state_map;
		unordered_map<string, location> geocode_map;
};

#endif
