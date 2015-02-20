/* oarg - a simple command line/file parser
** by Erik Perillo 
**
** Implementation of template class Arg */

//ctors
template <class Tp> 
Arg<Tp>::Arg(const std::string& names, const Tp& def_val, const std::string& description, int pos_n_found):
Oarg(names,description,pos_n_found), def_val(def_val)
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
	return found;
}

//return number of parsed values
template <class Tp>
int Arg<Tp>::nParsedVals()
{
	return val_vec.size();
}

//gets first "index" th of vector os parsed values
template <class Tp>
Tp Arg<Tp>::getVal(int index)
{
	if(index >= 0 && index < val_vec.size())
		return val_vec[index];

	return def_val;		
}

//returns vector of parsed values and zero lenght vector if nothing was found
template <class Tp>
std::vector<Tp> Arg<Tp>::getValVec()
{
	return val_vec;
}

template <class Tp>
void Arg<Tp>::setVec()
{
	std::stringstream ss;	
	Tp dumb_val;

	for(int i=0; i<str_vals.size(); i++)
	{
		ss.str(str_vals[i]);

		if(ss >> dumb_val)
			val_vec.push_back(dumb_val);

		ss.str(std::string());
		ss.clear();
	}	
}

template <class Tp>
void Arg<Tp>::clear()
{
	found = false;
	str_vals.clear();
	val_vec.clear();	
}
