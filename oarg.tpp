/* 
Copyright (c) 2015, Erik Perillo
All rights reserved.

Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the documentation and/or other materials provided with the distribution.

3. Neither the name of the copyright holder nor the names of its contributors may be used to endorse or promote products derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

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

template <class Tp> 
Oarg<Tp>::~Oarg()
{
	#if DEBUG
	debugmsg("~Oarg()");
	#endif
}

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

template <class Tp>
bool Oarg<Tp>::wasFound()
{
	return found;
}

template <class Tp>
int Oarg<Tp>::nParsedVals()
{
	return val_vec.size();
}

template <class Tp>
Tp Oarg<Tp>::getVal(int index)
{
	if(index >= 0 && index < val_vec.size())
		return val_vec[index];

	return def_val;		
}

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

	for(std::vector<std::string>::iterator val = str_vals.begin(); val != str_vals.end(); val++)
	{
		ss.str(*val);

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
