
#include <iostream>
#include <fstream>
#include <string> 
#include <sstream> 
#include "json.hpp"

using json = nlohmann::json;
using namespace std;

class Test
{
public:
	char c = '0';
	int i = 0;
	float f = 0;

	Test(char _c, int _i, float _f) : c(_c), i(_i), f(_f)
	{

	}

};


void PrintValues(Test* test)
{
	cout << "c: " << test->c << endl;
	cout << "i: " << test->i << endl;
	cout << "f: " << test->f << endl;
};



int main()
{
	
	//Part A
	if (false)
	{
		bool read = true;
		string text;

		Test* test = new Test('P', 5, 0.5);


		if (read)
		{
			fstream rf("test1.bin", ios::in | ios::binary);

			while (getline(rf, text)) 
			{
				cout << text << endl;
			}

			rf.close();
		}
		else
		{
			fstream wf("test1.bin", ios::out | ios::binary);

			wf << test->c << endl;
			wf << test->i << endl;
			wf << test->f << endl;

			wf.close();
		}

		
		PrintValues(test);
		delete test;
	}

	//Part B
	if (false)
	{
		fstream file("test2.txt", ios::in | ios::binary);
		string text;

		while (getline(file, text))
		{
			stringstream ss(text);
			
			if (text[0] != ' ' && text[0] != '#')
			{
				while (getline(ss, text, ':'))
				{
					cout << text << endl;
				}
				
			}
		}




	}

	//Part C
	if (true)
	{
		fstream file("test3.json", ios::in | ios::binary);

		json j = json::parse(file);

		for (json::iterator it = j.begin(); it != j.end(); ++it) {
			std::cout << *it << '\n';
		}

	}



	return 0;
}