// CS251 Project 1 DNA Profiling
// Boi Huynh UIN:653357799
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include "ourvector.h"
using namespace std;

// create a database structor to store all the vector of STR,
// sequence, DNA,... so it would easy to acces and organize
struct DataBase{
    ourvector<ourvector<char> > vecname;
    ourvector<ourvector<char> > STR;
    ourvector<ourvector<int> > STRPerson;
	ourvector<char>  DNA;
	ourvector<int > DNASTR;
	int location =-1;  // store location of match person in search
}; typedef struct DataBase Database;

// This function extracts the name out of the string line and returns it
string getName1st(string &line) {
    size_t pos = line.find(",");
    string name = line.substr(0, pos);
    line = line.substr(pos + 1, line.size());
    return name;
}

// get the name of every person in data file and push it in ourvector
void getName(string &line, ourvector<ourvector<char> >&vecname) {
    ourvector<char> temp;
	size_t pos = line.find(",");
    string name = line.substr(0, pos);
    line = line.substr(pos + 1, line.size());
    for (unsigned i = 0; i < name.size(); i++) {
        temp.push_back(name[i]);
    }
    vecname.push_back(temp);
	temp.clear();
}

// get STR and store it in the STR vector
void getSTR(string &line, DataBase&db) {
	ourvector<char>temp;
	stringstream s(line);
	while( getline(s, line, ',' ) ) {
     for (unsigned j = 0; j <line.size(); j++) {
         temp.push_back(line[j]);
     }
       db.STR.push_back(temp);
			 temp.clear();
	}
}

// get the longest repeat STR of every person and store it in the vector
void getSTRPerson(string &line, ourvector<ourvector<int> >&STRPerson) {
    ourvector<int> tmp;
    stringstream s(line);
    while ( getline(s, line, ',') ) {  // extract the line
         tmp.push_back(stoi(line));
    }
		STRPerson.push_back(tmp);
}

// get and store name, STR , longest repeat STR in vector and oganize it
void load_db(string fileName, DataBase &database) {
	// make the data is empty before loaded
    database.STR.clear();
    database.vecname.clear();
    database.STRPerson.clear();
    ifstream inFS(fileName);  // load file name
     cout << "Loading database..." << endl;
     if (!inFS.is_open()) {  // check if file can open or not
		cout << "Error: unable to open '" << fileName << "'" << endl;
     }
	string line;  // store every line in file
  	// get information from 1ST Line
  	string firstline;
	getline(inFS, firstline);
  	string name1st = getName1st(firstline);
  	getSTR(firstline, database);
  	// read file and extract data
  	while (getline(inFS, line)) {  // get every line in file
      getName(line, database.vecname);
      getSTRPerson(line, database.STRPerson);
  	}
  	inFS.close();  // close file after use
}

// print STR
void printSTR(DataBase db) {
	for (int i = 0; i < db.STR.size(); i++) {
		for (int j = 0; j < db.STR[i].size(); j++) {
			cout << " " << db.STR[i][j];
		}
	}
	cout << endl;
}

// print the longest repeat STR of every person in file
void printSequence(DataBase &db, int i) {
  for (int k = 0; k < db.STRPerson[i].size(); k++) {
			cout << " " << db.STRPerson[i][k];
		}
  cout << endl;
}
// print name of every person in database
void printname(DataBase &db) {
	for(int i = 0; i < db.vecname.size(); i++) {
		for(int j = 0; j < db.vecname[i].size(); j++) {
			cout << db.vecname[i][j];
		}
		printSequence(db, i);
	}
cout << endl;
}

// store DNA from file to ourvector
void load_dna(string fname, DataBase &db) {
    db.DNA.clear();
	string line;
	ifstream inFS(fname);
	cout << "Loading DNA..." << endl;
	if(!inFS.is_open()) {  // check if file can open or not
		cout << "Error: unable to open '" << fname
		<< "'" << endl;
	}
	while(getline(inFS, line)) {
		for(unsigned i = 0; i < line.size(); i++) {
			db.DNA.push_back(line[i]);
		}
	}
	inFS.close();  // close file after usedß
}
// print the DNA we store
void printDNA(DataBase &db) {
	cout << "DNA loaded: " << endl;
	for(int i = 0; i < db.DNA.size(); i++) {
		cout << db.DNA[i];
	}
	cout << endl;
}

// find if DNA match with the STR
bool match(DataBase &db, ourvector<char> &STR, int &i, int &count) {
    	int j = 0;
			for( j = 0; j< STR.size(); j++) {
			if(i+j >= db.DNA.size()|| db.DNA[i+j] != STR[j]) {
				return false;
			}
			if(j == STR.size()-1) {  // if it match with str
				count++;
				i+=STR.size()-1;  // set i to the next nucleotide
			}
        }
        return true;
}

// collect the number of longest repeat
void process_helper(DataBase &database, ourvector<char>&STR) {
	int max = 0;
	int count = 0;
	for(int i = 0; i< database.DNA.size(); i++) {
		if (count > 0 && database.DNA[i]!= STR[0]) {
			count = 0;
		} else if (database.DNA[i]== STR[0]) {
		    match(database, STR, i, count);
		}
		if(count > max) {
			max = count;
		}
	}
	database.DNASTR.push_back(max);
}

// find the longest repeat STR
void process(DataBase & db) {
	// check if databased and DNA loaded befor run function
    if(db.STR.size() <= 0) {
        cout << "No database loaded." << endl;
    } else if (db.DNA.size() <= 0) {
        cout << "No DNA loaded." << endl;
    } else {
    cout << "Processing DNA..." << endl;
    for (int i=0; i < db.STR.size(); i++) {
        process_helper(db, db.STR[i]);
    }
    }
}

// print the longest repeat in every STR in file.
void print_process(DataBase & database) {
	cout << "DNA processed, STR counts: \n";
	for (int i = 0; i< database.DNASTR.size(); i++) {
	    for (int j = 0; j< database.STR[i].size(); j++) {
	    cout << database.STR[i][j];
	    }
	    cout << ": " << database.DNASTR[i] << endl;
	}
	cout << endl;
}
// check if the STR match with STr in procees
void match_search(DataBase &database, int &count, int &i) {
	for (int j = 0; j < database.STRPerson[i].size(); j++) {
		if (database.STRPerson[i][j] != database.DNASTR[j]) {
			break;
		}
		if (j == database.DNASTR.size()-1) {
			count++;
		}
	}
}

// refersh count ifSTR and DNA not match and process if it is
void search_helper(DataBase &database, int &count, int &i) {
	if (count > 0 && database.STRPerson[i][0] != database.DNASTR[0]) {
		count = 0;
	} else if (database.STRPerson[i][0] == database.DNASTR[0]) {
			match_search(database, count, i);
	}
}

// print search result
void print_search(DataBase &db) {
	if(db.location < 0) {
		cout << "Not found in database." << endl;
	} else if (db.location >= 0) {
		cout << "Found in database!  DNA matches:  ";
		for(int i = 0; i < db.vecname[db.location].size(); i++) {
			cout << db.vecname[db.location][i];
		}
		cout << endl;
	}
}

// search if any one in file match the STR
void search_pro(DataBase &database, int &count) {
    cout << "Searching database..." << endl;
	for(int i = 0; i < database.STRPerson.size(); i++) {
		search_helper(database, count, i);
    	if(count == 1) {
    		database.location = i;
			break;
		}
	}
}

// search if there are a person STR match with DNA in file
void search(DataBase &database) {
	int count = 0;
	database.location = -1;  // refesh the location
	// check if database, DNA loaded yet or have it processed
	if (database.STR.size() <= 0) {
        cout << "No database loaded." << endl;
    } else if (database.DNA.size() <= 0) {
        cout << "No DNA loaded." << endl;
    } else if (database.DNASTR.size() <= 0) {
        cout << "No DNA processed." << endl;
    } else {  // if it all loaded and processed, run the search
	   search_pro(database, count);
	    print_search(database);  // print after run, no display call needed
    }
}

// print out the database
bool display(Database &db) {
	if(db.STR.size() > 0) {  // check if there are database
  		cout << "Database loaded: " << endl;
		printname(db);
	} else {
  		cout << "No database loaded." << endl;
	}
	if(db.DNA.size() <= 0) {  // check if the DNA loaded
	    cout << "No DNA loaded." << endl;
	} else {
	    printDNA(db);
	}
	cout << endl;
	if(db.DNASTR.size()<= 0) {  // check if DNA process
	    cout << "No DNA has been processed." << endl;
	} else {
	    print_process(db);
	}
	return true;
}
// auto run all function in one command with filename for data and dna 
// command : auto "file name" "dna file name"
void autorun(string fn, string fdna, Database &database) {
	load_db(fn,database);
	load_dna(fdna, database);
	process(database);
	display(database);
	search(database);
}
// call the function correspone with the command
void commandfunc(string command, string fn, string fdna, Database &database) {
	if (command == "load_db") {
        load_db(fn, database);
    } else if (command == "display") {
        display(database);
    } else if (command == "load_dna") {
		load_dna(fn, database);
	} else if (command == "process") {
		process(database);
	} else if (command == "search") {
	    search(database);  // print_search(database);
	}
	else if (command == "auto") {
		autorun (fn, fdna, database);  
	}
}

// get the input, seperate the line for command and file name 
// ( in load database and load dna case)
void menu(DataBase& database) {
    string fn, inp, line, command, fdna;
    cout << "Welcome to the DNA Profiling Application." << endl;
    while (command != "#") {
   		cout << "Enter command or # to exit: ";
		getline(cin, inp);
		stringstream s(inp);
        getline(s, command, ' ');
        getline(s, fn, ' ');
				getline(s, fdna, ' ');
        commandfunc(command, fn, fdna, database);
    }
}

int main() {
	DataBase Database;
	// menu accept the user input and process those input
	menu(Database);
    return 0;
}

