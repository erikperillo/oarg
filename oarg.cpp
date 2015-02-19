/* oarg - a simple command line/file parser 
** by Erik Perillo 
**
** Implementation of routines of classes Oarg and Container */ 

#include "oarg.hpp"

//class Oarg implementations

//ctors
oarg::Oarg::Oarg(const std::string& names, const std::string& description):
description(description), found(false)
{
	#if DEBUG
     debugmsg("Oarg(cl,names,def_val,description)");
	#endif

	std::stringstream ss(names);
     std::string name;

     while(ss >> name)
		if((name = pureName(name)) != "")
			this->names.push_back(name);

	id = Container::add(this);
}

oarg::Oarg::Oarg(const oarg::Oarg& oarg): 
found(oarg.found), str_vals(oarg.str_vals), names(oarg.names), description(oarg.description)
{
	#if DEBUG
     debugmsg("Oarg(oarg)");
	#endif

	id = Container::add(this,true);
}

oarg::Oarg::Oarg(): 
id(-1), found(false)
{
	#if DEBUG
     debugmsg("Oarg()");
	#endif
}

//operator= overload
oarg::Oarg& oarg::Oarg::operator=(const oarg::Oarg& oarg)
{
	#if DEBUG
     debugmsg("Oarg::operator=(oarg)");
	#endif

     id = Container::add(this,true);
     names = oarg.names;
	description = oarg.description;

     return *this;
}

//dtor
oarg::Oarg::~Oarg()
{
	#if DEBUG
     debugmsg("~Oarg()");
	#endif
}

//parsing from command line routine
void oarg::parse(int argc, char** argv, bool clear)
{
     oarg::Oarg* oarg_ptr;
	std::string arg;

	for(int i=0; i<Container::oargs.size(); i++)
	{	
		oarg_ptr = Container::oargs[i];

		if(clear)
		{
			oarg_ptr->found = false;
			oarg_ptr->str_vals.clear();
		}

		for(int j=0; j<argc; j++)
		{
			for(int k=0; k<oarg_ptr->names.size(); k++)
				if(argv[j] == Oarg::clName(oarg_ptr->names[k]))
				{
					oarg_ptr->found = true;
					for(int l=j+1; l<argc && !Oarg::isClName(argv[l]); l++)
					{
						arg = std::string(argv[l]);
						if(arg.substr(0,2) == "\\-")
							arg = arg.substr(1,arg.size()-1);
						oarg_ptr->str_vals.push_back(arg);
					}
				}
		}

		oarg_ptr->setVec();
	}
}

//parsing from file routine
int oarg::parse(const std::string& filename, bool clear)
{
	std::ifstream in_file(filename.c_str());
	std::stringstream ss;
	std::string line,word,val;
	oarg::Oarg* oarg_ptr;

	if(!in_file.is_open())
		return -1;

	for(int i=0; i<Container::oargs.size(); i++)
	{
		oarg_ptr = Container::oargs[i];
		
		if(clear)
		{
			oarg_ptr->found = false;
			oarg_ptr->str_vals.clear();
		}
		
		while(std::getline(in_file,line))
		{
			ss.str(line);
			while(ss >> word)
			{
				for(int j=0; j<oarg_ptr->names.size(); j++)
					if(word == Oarg::cfName(oarg_ptr->names[j]))
					{
						oarg_ptr->found = true;
						while(ss >> word)
						{
							if(word[0] == '#')
								break;

							val = word;

							if(val[0] == '"')
							{
								val = val.substr(1,val.size()-1);
								if(val[val.size()-1] != '"')
									while(ss >> word)
									{
										val += " " + word;
										if(word[word.size()-1] == '"')
											break;
									}
								val = val.substr(0,val.size()-1);
							}
							oarg_ptr->str_vals.push_back(val);
						}
					}
			}
			ss.str(std::string());
			ss.clear();
		}
		in_file.clear();
		in_file.seekg(0,std::ios::beg);
	}
	return 1;
}

//removes "-" chars from beggining and ":" chars from end
std::string oarg::Oarg::pureName(const std::string& name)
{	
	std::string ret = std::string(name);

	while(ret.size() > 0 && ret.at(ret.size()-1) == ':')
		ret = ret.substr(0,ret.size()-1);

	while(ret.size() > 0 && ret[0] == '-')
		ret = ret.substr(1,ret.size()-1);
	
	return ret;
}

//formats pure name to clarg name format 
std::string oarg::Oarg::clName(const std::string& pure_name)
{
	std::string ret = std::string(pure_name);
	
	ret.insert(0,(ret.size()>1)?"--":"-");

	return ret;
}

//formats pure name to configuration file name format
std::string oarg::Oarg::cfName(const std::string& pure_name)
{
	return pure_name + ":";
}

//returns true if word is a command line name
bool oarg::Oarg::isClName(const std::string& word)
{
	if(word.size() < 2)
		return (word[0]=='-');
	return (word[0]=='-' && (word[1] < '0' || word[1] > '9'));
}

//lists arguments in command line names format and it's descriptions
void oarg::Oarg::describe(const std::string& helpmsg)
{
     int j;
     std::string line;
	oarg::Oarg* oarg_ptr;

     if(helpmsg!="")
          std::cout << helpmsg << std::endl;

     for(int i=0; i<Container::oargs.size(); i++)
		if(!Container::repeated[i])
		{
			oarg_ptr = Container::oargs[i]; 
			line = "\t";
			for(j=0; j<oarg_ptr->names.size()-1; j++)
				line +=  Oarg::clName(oarg_ptr->names[j]) + ", ";
			line += Oarg::clName(oarg_ptr->names[j]);
			std::cout << std::left << std::setw(48) << line;
			line = " " + oarg_ptr->description;
			std::cout << std::left << std::setw(32) << line << std::endl;
		}

     return;
}

//class Container definitions

//definition of static class Oarg data members
std::vector<oarg::Oarg*> oarg::Container::oargs;
std::vector<bool> oarg::Container::repeated;

//adds Oarg descriptions to list and returns id of Oarg in list
int oarg::Container::add(Oarg* oarg_ptr, bool is_repeated)
{
	oargs.push_back(oarg_ptr);
	repeated.push_back(is_repeated);
	return oargs.size() - 1;
}

//specializations of some template class Arg routines
namespace oarg
{
	//gets value stored in str_vals
	template<> void oarg::Arg<std::string>::setVec()
	{
		for(int i=0; i<str_vals.size(); i++)
			val_vec.push_back(str_vals[i]);
	}

	//gets value stored in str_vals
	template<> void oarg::Arg<bool>::setVec()
	{
		if(found)
			val_vec.push_back(!def_val);
	}

	//gets "pos" nth value not defined after clname 
	template<> std::string Arg<std::string>::getRelVal(int argc, char** argv, int pos)
	{
		std::string val = def_val;		

		for(int i=0; i<argc && !Oarg::isClName(argv[i]); i++)
			if(pos == i)
			{
				val = std::string(argv[i]);
				if(val.substr(0,2) == "\\-")
					val = val.substr(1,val.size()-1);
			}

		return val;
	}
	
	//gets "pos" nth value not defined after clname 
	template<> bool Arg<bool>::getRelVal(int argc, char** argv, int pos)
	{
		bool val = def_val;
		
		for(int i=0; i<argc && !Oarg::isClName(argv[i]); i++)
			if(pos == i)
			{
				if(std::string(argv[i]) == "true")
					val = true;
				else if(std::string(argv[i]) == "false")
					val = false;
			}

		return val;
	}
}
