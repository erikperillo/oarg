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
	Arg<bool> version("-v -version::::::::::::::::::::::",false,"Version and program information");
	Arg<int> ival("-i --int-value",-1,"Integer value",2);
	Arg<> ival2;
	Arg<double> dval("-d double-val",3.141592,"Double value",1);
	Arg<float> fval("-f --float-val",-0.9,"Float value",3);
	Arg<string> sval("-s --string-val","string 1","String number one",5);
	Arg<string> filename("-F --filename","","Filename to read configuration file from",4);
	ival2 = ival;	

	parse(argc,argv);

	cout << "values from Command line:" << endl;
	
	double res = dval.getVal()*ival.getVal();

	cout << "ival = " << ival.getVal() << " <> ";
	printvec(ival.getValVec());
	
	cout << "ival2 = " << ival2.getVal() << " <> ";
	printvec(ival2.getValVec());
	
	cout << "dval = " << dval.getVal() << " <> ";
	printvec(dval.getValVec());
	
	cout << "fval = " << fval.getVal() << " <> ";
	printvec(fval.getValVec());
	
	cout << "sval = " << sval.getVal() << " <> ";
	printvec(sval.getValVec());
	
	cout << "version = " << (version.getVal()?"true":"false") << " <> ";
	printvec(version.getValVec());
	
	cout << "help = " << (help.getVal()?"true":"false") << " <> ";
	printvec(help.getValVec());
	
	cout << "filename = " << filename.getVal() << " <> ";
	printvec(filename.getValVec());
	
	cout << "res (ival*dval) = " << res << endl;	


	if(help.getVal())
	{
		describeArgs("Parameters:");
		return 0;
	}

	if(version.getVal())
	{
		cout << DESCRIPTION << endl;
		cout << "Version " << OARG_VERSION << endl;
		return 0;
	}
	
	if(filename.wasFound())
	{
		fl = filename.getVal();
		if(parse(fl) < 0)
		{
			cout << "file '" << fl << "' could not be open!" << endl;
			return -1;
		}
		cout << "values from configuration file '" << fl << "':" << endl;
		
		cout << "ival = " << ival.getVal() << " <> ";
		printvec(ival.getValVec());
		
		cout << "ival2 = " << ival2.getVal() << " <> ";
		printvec(ival2.getValVec());
		
		cout << "dval = " << dval.getVal() << " <> ";
		printvec(dval.getValVec());
		
		cout << "fval = " << fval.getVal() << " <> ";
		printvec(fval.getValVec());
		
		cout << "sval = " << sval.getVal() << " <> ";
		printvec(sval.getValVec());
		
		cout << "version = " << (version.getVal()?"true":"false") << " <> ";
		printvec(version.getValVec());
		
		cout << "help = " << (help.getVal()?"true":"false") << " <> ";
		printvec(help.getValVec());
		
		cout << "filename = " << filename.getVal() << " <> ";
		printvec(filename.getValVec());
		
		cout << "res (ival*dval) = " << res << endl;	
	}
	return 0;
}
