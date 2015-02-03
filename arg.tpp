/* oarg - a simple command line/file parser
** by Erik Perillo 
**
** Implementation of template class Arg */

//ctors
template <class Tp> 
Arg<Tp>::Arg(const std::string& names, const Tp& def_val, const std::string& description):
Oarg(names,description), def_val(def_val)
{
	#if DEBUG
	debugmsg("Arg(names,def_val,description)");
	#endif
}

template <class Tp> 
Arg<Tp>::Arg(const Arg& arg): Oarg(arg), def_val(arg.def_val)
{
	#if DEBUG
	debugmsg("Arg(arg)");
	#endif
}

template <class Tp> 
Arg<Tp>::Arg(): Oarg()
{
	#if DEBUG
	debugmsg("Arg()");
	#endif
}

//dtor
template <class Tp> 
Arg<Tp>::~Arg()
{
	#if DEBUG
	debugmsg("~Arg()");
	#endif
}

//operator= overload
template <class Tp> 
Arg<Tp>& Arg<Tp>::operator=(const Arg& arg)
{
	#if DEBUG
	debugmsg("Arg::operator=(arg)");
	#endif

	Oarg::operator=(arg);
	def_val = arg.def_val;

	return *this;
}

//encapsulation methods
template <class Tp>
bool Arg<Tp>::wasFound()
{
	return this->found;
}

//gets first "index" th of vector os parsed values
template <class Tp>
Tp Arg<Tp>::getVal(int index)
{
	std::stringstream ss;
	Tp ret_val;

	if(found && index >= 0 && index < str_vals.size())
	{
		ss.str(str_vals[index]);
		return (ss >> ret_val)?ret_val:def_val;
	}

	return def_val;
}

//gets "pos" th value not defined by clname in parsing
template <class Tp>
Tp Arg<Tp>::getRelVal(int argc, char** argv, int pos)
{
	std::stringstream ss;
	Tp ret_val;

	for(int i=0; i<argc && !Oarg::isClName(argv[i]); i++)
		if(i == pos)
		{
			ss.str(argv[i]);
			return (ss >> ret_val)?ret_val:def_val;
		}

	return def_val;
}

//returns vector of parsed values and zero lenght vector if nothing was found
template <class Tp>
std::vector<Tp> Arg<Tp>::getValVec()
{
	std::string str = "";
	std::stringstream ss;
	std::vector<Tp> ret_vec;
	Tp val;
	
	for(int i=0; i<str_vals.size(); i++)
		str += str_vals[i] + " ";
	
	ss.str(str);

	while(ss >> val)
		ret_vec.push_back(val);

	return ret_vec;
}
