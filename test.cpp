/* 
Copyright (c) 2015, Erik Perillo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "oarg.hpp"
#include <cstdlib>

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

std::vector<int> sort(const std::vector<int>& src_vec)
{
	std::vector<int> ret_vec;
	int aux;


	for(int i=0; i<src_vec.size(); i++)
	{
		ret_vec.push_back(src_vec[i]);
		for(int j=ret_vec.size()-1; j>0; j--)
			if(ret_vec[j] < ret_vec[j-1])
			{
				aux = ret_vec[j-1];
				ret_vec[j-1] = ret_vec[j];
				ret_vec[j] = aux;
			}
	}
	
	return ret_vec;
}

int main(int argc, char** argv)
{
	std::string fl;

	Oarg<bool> help("-----------h -----------help",false,"This help message");
	Oarg<bool> version("-v -version::::::::::::::::::::::",false,"Version and program information");
	Oarg<int> ival("-i --int-value",-1,"Integer value",2);
	Oarg<> ival2;
	Oarg<double> dval("-d double-val",3.141592,"Double value",1);
	Oarg<float> fval("-f --float-val",-0.9,"Float value",3);
	Oarg<string> sval("-s --string-val","string 1","String number one",5);
	Oarg<string> filename("-F --filename","","Filename to read configuration file from",4);
	ival2 = ival;	

	if(parse(argc,argv) != 0)
	{
		cout << "ERROR: unknown option '" << OargBase::getUnknownOption() << "'" << endl;
		return -1;
	}

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
