/* oarg - a simple command line parser 
** version 1.0 
** by Erik Perillo 
*/ 

//implementation of template function of class Container
using namespace std;

template <class Tp>
Tp Oarg<Tp>::parse(int argc, char** argv)
{
	std::string arg;
     std::stringstream ss;
	Tp retval = def_val; 

	if(!Container::invoked)
	{
		for(int i=0; i<argc; i++)
			Container::argv.push_back(argv[i]);
		Container::invoked = true;
	}

     for(int i=0; i<Container::argv.size()-1; i++)
	{
          for(int j=0; j<Container::cl_names[id].size(); j++)
               if(Container::argv[i] == Container::cl_names[id][j])
               {
				ss.str(Container::argv[i+1]);
				ss >> retval;
                    Container::argv.erase(Container::argv.begin()+i+1);
 
				return retval;                    
               } 
	}

     return retval;
}

template <class Tp> 
std::vector<Tp> Oarg<Tp>::parseVec(int argc, char** argv, int n_args)
{
     int i,j,k;
     std::vector<Tp> retvec;
	std::stringstream ss;
     Tp retval;

     for(i=0; i<n_args; i++)
          retvec.push_back(def_val);
     
	if(!Container::invoked)
	{
          for(i=0; i<argc; i++)
               Container::argv.push_back(argv[i]);
	Container::invoked = true;
	}

     for(i=0; i<Container::argv.size(); i++)
     {
          for(j=0; j<Container::cl_names[id].size(); j++)
               if(Container::argv[i] == Container::cl_names[id][j])
               {
                    for(k=0,++i; i<Container::argv.size() && Container::argv[i][0] != '-' && ((n_args>0)?(k<n_args):true); k++)
                    {
					ss.str(Container::argv[i]); 
					ss >> retval;
                         if(n_args>0)
						retvec[k] = retval;
					else
						retvec.push_back(retval);

					ss.str( std::string() );
					ss.clear();
                         Container::argv.erase(Container::argv.begin()+i);
                    }
                    return retvec;
               } 
     }

     return retvec;
}

