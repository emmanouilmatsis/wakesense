#include <iostream>
#include <vector>
using namespace std;

int main()
{
	cout.setf(ios :: fixed);
	cout.setf(ios :: showpoint);
	cout.precision(2);

  vector<double> vec(10, 0.0);
	for (unsigned int i = 0; i < vec.size(); i++)
		cout << vec[i] << endl;
	
	return 0;
}
