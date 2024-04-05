#include <iostream>
#include "json.hpp"
using namespace std;

struct Msg
{
	int a;
	string b;
};

int main()
{
	printf("hello world!\n");
	nlohmann::json j;
	string name;
	string message;
	name = "名字"	;
	j["name"] = name;
	j["message"] = "消息";
	message = j["message"];
	string namestr = j["name"];
	cout << "json's name :" << namestr << endl;
	cout << "the string message :"<< message << endl;
	
	return 0;
}
