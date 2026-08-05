#include <cstdlib>
#include <getopt.h>
#include <iostream> 
#include <sstream>
#include <vector>
#include <string>
#include <unistd.h>
#include <sodium.h>
#include "include/utils.h"
using namespace std;

const char MIN_LEN{8};
const int MAX_LEN{1024};

void printHelp(const char* name);
string generatePsw(const int len, string extra);

int main(int argc, char *argv[]){
	if(sodium_init() < 0){
		exitWithError("Libsodium library not initialized");
	}
	int opt;
	bool printToStd{false};

	while((opt = getopt(argc, argv, "hp")) != -1){
		switch(opt){
			case 'h':
				printHelp(argv[0]);	
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
		printHelp(argv[0]);
		exit(0);
	}
	if(optind >= argc)
		exitWithError("No length specified");

	char* arg = argv[optind];
	int len{};
	stringstream stream(arg);
	if (!(stream >> len) || !(stream.eof())){ 
		exitWithError("invalid length");
	}

	string extra;
	if(argv[optind + 1])
		extra = argv[optind + 1];
	else extra = "";

	string newPsw = generatePsw(len, extra);
	if(printToStd)
		cout << newPsw << endl;
	else{
		if(copyToClip(newPsw)){
			cout << "Password copied to clipboard" << endl;
			waitForClear();
		}else
			exitWithError("failed to copy to clipboard");
	}
	pswCleanup(newPsw);

	return 0;
}

// Print helper message
void printHelp(const char* name){
	cout << "Usage: " << name << " [-hp] <length> [extra_symbols]" << endl << endl;
	cout << "Description:" << endl;
	cout << " Randomly generate a new password with the specified length." << endl;
	cout << " It copies the result to the clipboard as default." << endl << endl;
	cout << "Options:" << endl;
	cout << " -h		Display the help message." << endl;
	cout << " -p		Print password to terminal." << endl;
	cout << endl;

	exit(0);
}

// Pawword generation function
string generatePsw(const int len, string extra){
	if(len <= 0)
		exitWithError("Length cannot be zero or negative.");
	if(len < MIN_LEN)
		exitWithError("Length cannot be less than 8 characters.");
	if(len > MAX_LEN)
		exitWithError("Password cannot be longer than 1024 characters.");

	string base = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
	string psw(len, ' ');

	string numbers = "0123456789";
	string lower   = "abcdefghijklmnopqrstuvwxyz";
	string upper   = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	string special = "-_!=+";

	vector<unsigned int> indices(len);
	for (int i = 0; i < len; ++i)
		indices[i] = i;

	// Shuffle
	for (int i = 0; i < len; ++i) {
		unsigned int j;
		randombytes_buf(&j, sizeof(unsigned int));
		j %= len;
		swap(indices[i], indices[j]);
	}

    psw[indices[0]] = getRandChar(numbers);
    psw[indices[1]] = getRandChar(lower);
    psw[indices[2]] = getRandChar(upper);
    psw[indices[3]] = getRandChar(special);
	
	for(int i = 0; i < len; i++)
		if(psw[i] == ' ')
			psw[i] = getRandChar(base + special + extra);

	return psw;
}
