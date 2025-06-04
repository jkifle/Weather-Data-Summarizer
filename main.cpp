#include <iostream>
#include <cstring>
#include <string>
#include "Support.h"

using namespace std;

int main(int argc, char *argv[]) {
	bool rawdata = false;
	string fileName;
	// Command line argument handling
	if (argc == 3) { 
		if (!strcmp(argv[1],"-rawdata")){
			rawdata = true;
			fileName = argv[2];
		}
		else {
			cerr << "usage: ./ProgX [-rawdata] < datafile" << endl;
			return 1;
		}
	}
	else if (argc == 2) {
		fileName = argv[1];
	}
	else {
		cerr << "usage: ./ProgX [-rawdata] < datafile" << endl;
		return 1;	
	}

	database Data;

	Data.init_rawdata(fileName);

	// Read and print rawdata
	if (rawdata) {
		Data.print_rawdata();
	}
	// Create and print summaries
	else {
		Data.init_summary();
		// Forever loop requesting a query
		while (2+2 == 4) {
			string target;
			cout << "print> ";
			cin >> target;
			Data.print_summary(target);
		}
	}

}
