#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include "global.hpp"
using std::vector;
using std::string;
using std::ifstream;
using std::stringstream;

vector<string> filecontent;

// File Preread in (to facilitate error reporting)
void Filepreread(const string &str){
	ifstream fin(str);
	string tmp;
	while(getline(fin, tmp)) filecontent.push_back(tmp);
}

// Array Dimension
// e.g. given [2][3][5], it returns <1, 5, 15, 30>
vector<int> Arraydimprocess(vector<int> p){
	vector<int> arrv (1,1);
	std::reverse(p.begin(),p.end());
	if(!p.size()) return arrv;
	for(auto i : p){
		arrv.push_back(arrv.back() * i);
	}

	return arrv;
}

// From array index to a single number, do the convolution
// e.g. Given <1, 5, 15, 30> and <1, 2, 3>, it returns 1 * 15 + 2 * 5 + 3 * 1 = 28;
// e.g. Given <1, 5, 15, 30> and <1, 2>, it returns 1 * 15 + 2 * 5 = 25.
// e.g. Given <1, 5, 15, 30, 0> and <5, 1, 2, 4> it returns 5 * 30 + 1 * 15 + 2 * 5 + 4 * 1 = 179;
int Arraylinearno(const std::vector<int> &arr, const std::vector<int> &ind){
	int ret = 0;
	const int len = arr.size(), indlen = ind.size();

	// A Scalar
	if(len == 1) return 0;

	// An array
	for(int i = 0;i < indlen;++i){
		ret += (ind[i] * arr[len - 2 - i]);
	}
	return ret;
}

// Encode error message
// e.g. Given <1, 5, 15, 30>, it generates "int [2][3][5]"
string Encodemessage(const vector<int> &p){
	int size = p.size();
	if(size == 1) return "int";
	string ret = "int ";

	// A ptr
	if(p.back() == 0){
		ret += "[]";
		size -= 1;
	}

	stringstream sin;
	string tmp;
	for(int i = size - 1;i >= 1;--i){
		sin << p[i] / p[i - 1];
		sin >> tmp;
		ret = ret + "[" + tmp + "]";
		sin.clear();
	}
	return ret;
}

// Encode error message
// e.g. Given 30, it generates "30"
string Encodemessage(int p){
	string ret;
	stringstream sin;
	sin << p;
	sin >> ret;
	return ret;
}
