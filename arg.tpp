/* oarg - a simple command line/file parser
** by Erik Perillo 
**
** Implementation of template class Oarg */

//ctors
template <class Tp> 
Oarg<Tp>::Oarg(const std::string& names, const Tp& def_val, const std::string& description, int pos_n_found):
OargBase(names,description,pos_n_found), def_val(def_val)
{
	#if DEBUG
	debugmsg("Oarg(names,def_val,description)");
	#endif
}

template <class Tp> 
Oarg<Tp>::Oarg(const Oarg& arg): OargBase(arg), def_val(arg.def_val)
{
	#if DEBUG
	debugmsg("Oarg(arg)");
	#endif
}

template <class Tp> 
Oarg<Tp>::Oarg(): OargBase()
{
	#if DEBUG
	debugmsg("Oarg()");
	#endif
}

//dtor
template <class Tp> 
Oarg<Tp>::~Oarg()
{
	#if DEBUG
	debugmsg("~Oarg()");
	#endif
}

//operator= overload
template <class Tp> 
Oarg<Tp>& Oarg<Tp>::operator=(const Oarg& arg)
{
	#if DEBUG
	debugmsg("Oarg::operator=(arg)");
	#endif

	OargBase::operator=(arg);
	def_val = arg.def_val;

	return *this;
}

//encapsulation methods
template <class Tp>
bool Oarg<Tp>::wasFound()
{
	return found;
}

//return number of parsed values
template <class Tp>
int Oarg<Tp>::nParsedVals()
{
	return val_vec.size();
}

//gets first "index" th of vector os parsed values
template <class Tp>
Tp Oarg<Tp>::getVal(int index)
{
	if(index >= 0 && index < val_vec.size())
		return val_vec[index];

	return def_val;		
}

//returns vector of parsed values and zero lenght vector if nothing was found
template <class Tp>
std::vector<Tp> Oarg<Tp>::getValVec()
{
	return val_vec;
}

template <class Tp>
void Oarg<Tp>::setVec()
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
void Oarg<Tp>::clear()
{
	found = false;
	str_vals.clear();
	val_vec.clear();	
}
