#include "oarg.hpp"

using namespace std;
using namespace oarg;

#define DESCRIPTION "oarg - a simple command line/file parser\nby Erik Perillo"

template <class Tp>
void printvec(vector<Tp> vec)
{
	int i;
	cout << "[" << flush;
	for(i=0; i<(int)(vec.size()-1); i++)
	{
		cout << vec[i] << ", ";}
	if(vec.size() > 0)
	{
		cout << vec[i];}
	cout  << "]" << endl;
}

int main(int argc, char** argv)
{
	std::string fl;

	Arg<bool> help("-----------h -----------help",false,"This help message");
	Arg<bool> version("-v --version::::::::::::::::::::::",false,"Version and program information");
	Arg<int> ival("-i --int-value",-2,"Integer value");
	Arg<> ival2;
	Arg<double> dval("-d double-val",3.141592,"Double value");
	Arg<float> fval("-f --float-val",-0.9,"Float value");
	Arg<string> sval("-s --string-val","string 1","String number one");
	Arg<string> filename("-F --filename","","Filename to read configuration file from");
	ival2 = ival;	

	parse(argc,argv);
	
	if(help.getVal())
	{
		Oarg::describe("Parameters:");
		return 0;
	}

	if(version.getVal())
	{
		cout << DESCRIPTION << endl;
		cout << "Version " << OARG_VERSION << endl;
		return 0;
	}
	
	if((filename.wasFound()?(fl = filename.getVal()):(fl = filename.getRelVal(argc,argv))) != "")
	{
		if(parse(fl) < 0)
		{
			cout << "file '" << fl << "' could not be open!" << endl;
			return -1;
		}
		cout << "values from configuration file '" << fl << "':" << endl;
	}
	else
		cout << "values from Command line:" << endl;
	
	double res = dval.getVal()*ival.getVal();

	cout << "ival = " << (ival.wasFound()?ival.getVal():ival.getRelVal(argc,argv,2)) << endl;
	printvec(ival.getValVec());
	ival.getValVec();
	cout << "dval = " << (dval.wasFound()?dval.getVal():dval.getRelVal(argc,argv,3)) << endl;
	printvec(dval.getValVec());
	dval.getValVec();
	cout << "fval = " << (fval.wasFound()?fval.getVal():fval.getRelVal(argc,argv,4)) << endl;
	printvec(fval.getValVec());
	fval.getValVec();
	cout << "sval = " << (sval.wasFound()?sval.getVal():sval.getRelVal(argc,argv,5)) << endl;
	printvec(sval.getValVec());
	sval.getValVec();
	cout << "res (ival*dval) = " << res << endl;	

	return 0;
}
