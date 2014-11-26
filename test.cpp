#include "clarg.hpp"

using namespace std;

int main(int argc, char** argv)
{
     clarg::Container container(argc,argv);
   
     int ival = container.parse<int>(clarg::Clarg("-i --int-val","Integer var value"));
     float fval = container.parse<float>(clarg::Clarg("-f --float-val","Float var value"));
     string sval = container.parse<string>(clarg::Clarg("-s --string-val","String var value"));
     bool bval = container.parse<bool>(clarg::Clarg("-b --bool-val","Boolean var value"));
     bool help = container.parse<bool>(clarg::Clarg("-h","This help message"),false);

     if(help || container.getReadArgs()==0)
     {
          container.describe("Usage:");
          return 0;
     }

     cout << "ival = " << ival << endl;
     cout << "fval = " << fval << endl;
     cout << "sval = " << sval << endl;
     cout << "bval = " << bval << endl;

     return 0;
}
