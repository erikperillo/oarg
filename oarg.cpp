/* oarg - a simple command line/file parser 
** by Erik Perillo 
**
** Implementation of routines of classes OargBase and Container */ 

#include "oarg.hpp"

//class OargBase implementations
//ctors
oarg::OargBase::OargBase(const std::string& names, const std::string& description, int pos_n_found):
description(description), found(false), pos_n_found(pos_n_found)
{
	#if DEBUG
     debugmsg("OargBase(cl,names,def_val,description)");
	#endif

	std::stringstream ss(names);
     std::string name;

     while(ss >> name)
		if((name = pureName(name)) != "")
			this->names.push_back(name);

	id = Container::add(this);
}

oarg::OargBase::OargBase(const oarg::OargBase& oarg): 
found(oarg.found), str_vals(oarg.str_vals), names(oarg.names), description(oarg.description)
{
	#if DEBUG
     debugmsg("OargBase(oarg)");
	#endif

	id = Container::add(this,true);
}

oarg::OargBase::OargBase(): 
id(-1), found(false), pos_n_found(0)
{
	#if DEBUG
     debugmsg("OargBase()");
	#endif
}

//operator= overload
oarg::OargBase& oarg::OargBase::operator=(const oarg::OargBase& oarg)
{
	#if DEBUG
     debugmsg("OargBase::operator=(oarg)");
	#endif

	id = oarg.id;
     Container::add(this,true);
     names = oarg.names;
	description = oarg.description;

     return *this;
}

//dtor
oarg::OargBase::~OargBase()
{
	#if DEBUG
     debugmsg("~OargBase()");
	#endif
}

//definition of oarg static vars
std::vector<std::string> oarg::OargBase::unknown_options;

std::string oarg::OargBase::getUnknownOption(int index)
{
	if(index < unknown_options.size())
		return unknown_options[index];
	return std::string("");
}

//splits a string
std::vector<std::string> oarg::split(const std::string& src_str, const std::string& delim)
{
     std::string str = src_str;
     std::vector<std::string> str_vec;

     for(int i=0; i<str.size(); i++)
     {
          for(int j=0; j<delim.size(); j++)
          {
               if(str[i] == delim[j])
               {
                    if(i > 0 && str[i-1] == '\\')
                    {
                         str.erase(i-1,1);
                         i--;
                    }
                    else
                    {
                         if(str.substr(0,i) != "")
                              str_vec.push_back(str.substr(0,i));
                         str = str.substr(i+1,str.size()-i);
                         i=-1;
                         break;
                    }
               }
          }
     }
     if(str != "")
          str_vec.push_back(str);

     return str_vec;
}

inline void oarg::setVals(oarg::OargBase* oarg_ptr, std::vector<std::string>::iterator& arg)
{
	bool past_had_comma = true;
	std::vector<std::string> splitted;

	while(arg != Container::arg_vec.end() && !OargBase::isClName(*arg))
	{
		if((*arg)[0] != ',' && !past_had_comma)
			break;
		if((*arg)[(*arg).size()-1] != ',')
			past_had_comma = false;
		else
			past_had_comma = true;

		splitted = split(*arg);
		oarg_ptr->str_vals.insert(oarg_ptr->str_vals.end(), splitted.begin(), splitted.end());
		splitted.clear();

		arg = Container::arg_vec.erase(arg);
	}

	oarg_ptr->setVec();
}

//parsing from command line routine
int oarg::parse(int argc, char** argv, bool clear)
{
	oarg::OargBase* oarg_ptr;
	oarg::Comparer comparer;
	std::vector<oarg::OargBase*> pos_vec;
	std::vector<std::string>::iterator it;
	int i=0;

	//copying argv to arg_vec
	Container::arg_vec = std::vector<std::string>(argv + MAGIC_NUMBER, argv + argc);
	
	for(std::vector<oarg::OargBase*>::iterator oarg_it = Container::oargs.begin(); oarg_it != Container::oargs.end(); oarg_it++)
	{	
		if(Container::repeated[i])
			continue;	
		i++;

		if(clear)
			(*oarg_it)->clear();
		
		for(std::vector<std::string>::iterator arg = Container::arg_vec.begin(); arg != Container::arg_vec.end(); arg++)
		{
			for(std::vector<std::string>::iterator name = (*oarg_it)->names.begin(); name != (*oarg_it)->names.end(); name++)
				if(*arg == OargBase::clName(*name))
				{
					(*oarg_it)->found = true;
					setVals(*oarg_it,(arg=Container::arg_vec.erase(arg)));
					arg--;
					break;
				}
		}
	
		if(!(*oarg_it)->found && (*oarg_it)->pos_n_found > 0)
			pos_vec.push_back(*oarg_it);
	}
	//collecting possible wrong args
	for(std::vector<std::string>::iterator it = Container::arg_vec.begin(); it != Container::arg_vec.end();)
		if(OargBase::isClName(*it))
		{
			OargBase::unknown_options.push_back(*it);
			it = Container::arg_vec.erase(it);
		}
		else
			it++;

	sort(pos_vec.begin(), pos_vec.end(), comparer);
	
	//collecting args not specified by keyword
	for(std::vector<oarg::OargBase*>::iterator oarg_it = pos_vec.begin(); oarg_it != pos_vec.end(); oarg_it++)
	{
		it = Container::arg_vec.begin();
		setVals(*oarg_it,it);
		
		if((*oarg_it)->str_vals.size() > 0)
			(*oarg_it)->found = true;
	}	

	for(int i=0; i<Container::oargs.size(); i++)
		if(Container::repeated[i])
		{
			oarg_ptr = Container::oargs[i];
			oarg_ptr->str_vals = Container::oargs[oarg_ptr->getId()]->str_vals;
			oarg_ptr->setVec();
		}

	return OargBase::unknown_options.size();
}

//parsing from file routine
int oarg::parse(const std::string& filename, bool clear)
{
	std::ifstream in_file(filename.c_str());
	std::stringstream ss;
	std::string line,word,val;
	oarg::OargBase* oarg_ptr;

	if(!in_file.is_open())
		return -1;

	for(int i=0; i<Container::oargs.size(); i++)
	{
		oarg_ptr = Container::oargs[i];
		
		if(clear)
			oarg_ptr->clear();
		
		while(std::getline(in_file,line))
		{
			ss.str(line);
			while(ss >> word)
			{
				for(int j=0; j<oarg_ptr->names.size(); j++)
					if(word == OargBase::cfName(oarg_ptr->names[j]))
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
						oarg_ptr->setVec();
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
std::string oarg::OargBase::pureName(const std::string& name)
{	
	std::string ret = std::string(name);

	while(ret.size() > 0 && ret.at(ret.size()-1) == ':')
		ret = ret.substr(0,ret.size()-1);

	while(ret.size() > 0 && ret[0] == '-')
		ret = ret.substr(1,ret.size()-1);
	
	return ret;
}

//formats pure name to clarg name format 
std::string oarg::OargBase::clName(const std::string& pure_name)
{
	std::string ret = std::string(pure_name);
	
	ret.insert(0,(ret.size()>1)?"--":"-");

	return ret;
}

//formats pure name to configuration file name format
std::string oarg::OargBase::cfName(const std::string& pure_name)
{
	return pure_name + ":";
}

//returns true if word is a command line name
bool oarg::OargBase::isClName(const std::string& word)
{
	if(word.size() < 2)
		return (word[0]=='-');
	return (word[0]=='-' && (word[1] < '0' || word[1] > '9'));
}

int oarg::OargBase::getId()
{
	return id;
}

int oarg::OargBase::getPosNFound()
{
	return pos_n_found;
}

//lists arguments in command line names format and it's descriptions
void oarg::OargBase::describe(const std::string& helpmsg)
{
     int j;
     std::string line;
	oarg::OargBase* oarg_ptr;

     if(helpmsg!="")
          std::cout << helpmsg << std::endl;

     for(int i=0; i<Container::oargs.size(); i++)
		if(!Container::repeated[i])
		{
			oarg_ptr = Container::oargs[i]; 
			line = "\t";
			for(j=0; j<oarg_ptr->names.size()-1; j++)
				line +=  OargBase::clName(oarg_ptr->names[j]) + ", ";
			line += OargBase::clName(oarg_ptr->names[j]);
			std::cout << std::left << std::setw(40) << line;
			line = " " + oarg_ptr->description;
			std::cout << std::left << std::setw(32) << line << std::endl;
		}

     return;
}

void oarg::describeArgs(const std::string& helpmsg)
{
	OargBase::describe(helpmsg);
}

//class Container definitions

//definition of static class OargBase data members
std::vector<oarg::OargBase*> oarg::Container::oargs;
std::vector<bool> oarg::Container::repeated;
std::vector<std::string> oarg::Container::arg_vec;

//adds OargBase descriptions to list and returns id of OargBase in list
int oarg::Container::add(OargBase* oarg_ptr, bool is_repeated)
{
	oargs.push_back(oarg_ptr);
	repeated.push_back(is_repeated);
	return oargs.size() - 1;
}

bool oarg::Comparer::operator()(oarg::OargBase* a, oarg::OargBase* b)
{
	return (a->getPosNFound() < b->getPosNFound());
}

//specializations of some template class Oarg routines
namespace oarg
{
	//gets value stored in str_vals
	template<> void oarg::Oarg<std::string>::setVec()
	{
		for(int i=0; i<str_vals.size(); i++)
			val_vec.push_back(str_vals[i]);
	}

	//gets value stored in str_vals
	template<> void oarg::Oarg<bool>::setVec()
	{
		val_vec.push_back((found?((str_vals.size()>0)?((str_vals[0]=="true")?true:false):(!def_val)):(def_val)));
	}

}
