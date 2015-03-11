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

//parsing from command line routine
int oarg::parse(int argc, char** argv, bool clear)
{
     oarg::OargBase* oarg_ptr;
	std::vector<std::string> splitted;
	int 	index, aux;
	bool past_had_comma;
	std::vector<int> pos_vec;

	for(int i=MAGIC_NUMBER; i<argc; i++)
		Container::arg_vec.push_back(argv[i]);
	
	for(int i=0; i<Container::oargs.size(); i++)
	{	
		if(Container::repeated[i])
			continue;	

		oarg_ptr = Container::oargs[i];
		
		if(oarg_ptr->pos_n_found > 0)
		{
			index = 0;
			pos_vec.insert(pos_vec.begin(),i);	
			
			for(int j=1; j<pos_vec.size(); j++)
			{
				if(Container::oargs[pos_vec[j-1]]->pos_n_found >= Container::oargs[pos_vec[j]]->pos_n_found)
				{
					aux = pos_vec[j];
					pos_vec[j] = pos_vec[j-1];
					pos_vec[j-1] = aux;
					index = j;
				}		
			}
		}

		if(clear)
			oarg_ptr->clear();
		
		for(int j=0; j<Container::arg_vec.size(); j++)
		{
			for(int k=0; k<oarg_ptr->names.size(); k++)
				if(Container::arg_vec[j] == OargBase::clName(oarg_ptr->names[k]))
				{ 
					oarg_ptr->found = true;

					if(oarg_ptr->pos_n_found > 0)
						pos_vec.erase(pos_vec.begin() + index);
		
					past_had_comma = true;

					Container::arg_vec.erase(Container::arg_vec.begin()+j);
					while(j<Container::arg_vec.size() && !OargBase::isClName(Container::arg_vec[j]))
					{	
						std::string& word = Container::arg_vec[j];
		
						if(word[0] != ',' && word[word.size()-1] != ',')
						{
							if(!past_had_comma)
								break;
							past_had_comma = false;
						}
						else
							past_had_comma = true;
					
						splitted = split(word);

						for(int l=0; l<splitted.size(); l++)
							oarg_ptr->str_vals.push_back(splitted[l]);		

						splitted.clear();

						Container::arg_vec.erase(Container::arg_vec.begin()+j);
					}

					oarg_ptr->setVec();
					break;
				}
		}
	}

	for(int i=0; i<Container::arg_vec.size(); i++)
		if(OargBase::isClName(Container::arg_vec[i]))
		{
			OargBase::unknown_options.push_back(Container::arg_vec[i]);
			Container::arg_vec.erase(Container::arg_vec.begin()+i);
			i--;
		}
	
	for(int i=0; i<pos_vec.size(); i++)
	{
		oarg_ptr = Container::oargs[pos_vec[i]]; 
		past_had_comma = true;

		while(Container::arg_vec.size() > 0)
		{			
			std::string& word = Container::arg_vec[0];
		
			if(word[0] != ',' && word[word.size()-1] != ',')
			{
				if(!past_had_comma)
					break;
				past_had_comma = false;
			}
			else
				past_had_comma = true;
		
			splitted = split(word);

			for(int j=0; j<splitted.size(); j++)
				oarg_ptr->str_vals.push_back(splitted[j]);		

			splitted.clear();
			Container::arg_vec.erase(Container::arg_vec.begin());				
		}

		if(oarg_ptr->str_vals.size() > 0)
		{
			oarg_ptr->found = true;
			oarg_ptr->setVec();
		}
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
			std::cout << std::left << std::setw(48) << line;
			line = " " + oarg_ptr->description;
			std::cout << std::left << std::setw(32) << line << std::endl;
		}

     return;
}

void oarg::describeArgs(const std::string& helpmsg)
{
	OargBase::describe(helpmsg);
}

int oarg::OargBase::getId()
{
	return id;
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
