#pragma once
#include <string>
#include <vector>
using namespace std;


class Utils
{
public:
	Utils();
	~Utils();

	static void reverseArray(char* arr, int size);
	static void splitString(const string& str, vector<string>& output);
	
	static int get1DIndexFrom2D(int x, int y);
	static void set2DArraySizes(int width, int height);

private:
	static int width;
	static int height;

};

