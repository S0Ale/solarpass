#include <getopt.h>
#include <iostream> 
#include <sodium/randombytes.h>
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sodium.h>
#include "include/utils.h"
using namespace std;

const unsigned int MAX_LEN{1024};

// Print helper message
void printUsage(const char* name){
	cout << "Usage: " << name << " [-hpse] <length> [extra_symbols]" << endl << endl;
	cout << "Description:" << endl;
	cout << " Randomly generate a new password with the specified length." << endl;
	cout << " It copies the result to the clipboard as default." << endl << endl;
	cout << "Options:" << endl;
	cout << " -h		Display the help message." << endl;
	cout << " -p		Print password to terminal." << endl;
	cout << " -s		Use common symbols for the password generation." << endl;
	//cout << " -e		Add a custom set of simbols for the password generation." << endl;
	cout << endl;

	exit(0);
}

unsigned int getRandIndex(const unsigned int len){
    unsigned int index;
	randombytes_buf(&index, sizeof(unsigned int)); 
	return index % len;
}

char getRandChar(string basePool) {
    unsigned char index;
	randombytes_buf(&index, 1); 
    return basePool[index % basePool.length()];
}

// Pawword generation function
string generatePsw(const unsigned int len, string extra){
	if(len <= 0)
		exitWithError("Length cannot be zero or negative.");
	if(len > MAX_LEN)
		exitWithError("Password cannot be longer than 1024 characters.");

	string base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	string psw(len, ' ');

	string numbers = "0123456789";
	string lower   = "abcdefghijklmnopqrstuvwxyz";
	string upper   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string special = "-_!=+";

	vector<unsigned int> indices(len);
	for (unsigned int i = 0; i < len; ++i)
		indices[i] = i;

	// Shuffle
	for (unsigned int i = 0; i < len; ++i) {
		unsigned int j;
		randombytes_buf(&j, sizeof(unsigned int));
		j %= len;
		swap(indices[i], indices[j]);
	}

    psw[indices[0]] = getRandChar(numbers);
    psw[indices[1]] = getRandChar(lower);
    psw[indices[2]] = getRandChar(upper);
    psw[indices[3]] = getRandChar(special);
	
	unsigned int i = 0;
	for(; i < len; i++)
		if(psw[i] == ' ')
			psw[i] = getRandChar(base + special + extra);

	return psw;
}

// TODO: enforce category policies, add options for common symbols and to insert a custom set
int main(int argc, char *argv[]){
	int opt;
	bool printToStd{false};

	while((opt = getopt(argc, argv, "hp")) != -1){
		switch(opt){
			case 'h':
				printUsage(argv[0]);	
				exit(0);
			case 'p':
				std::cout << "Printing the new password to stdin..." << endl;
				printToStd = true;
				break;
			case 'e':
				break;
		}
	}

	if(argc <= 1){
		printUsage(argv[0]);
		exit(0);
	}
	if(optind >= argc)
		exitWithError("No length specified");

	char* arg = argv[optind];
	unsigned int len{};
	stringstream stream(arg);
	if (!(stream >> len) || !(stream.eof())){ 
		exitWithError("invalid length");
	}

	// This throws an error, because second arg can be null
	string extra;
	if(argv[optind + 1])
		extra = argv[optind + 1];
	else extra = "";

	string newPsw = generatePsw(len, extra);
	if(printToStd)
		cout << newPsw << endl;
	else{
		if(copyToClip(newPsw))
			cout << "Password copied to clipboard" << endl;
		else
			exitWithError("failed to copy to clipboard");
	}

	return 0;
}
