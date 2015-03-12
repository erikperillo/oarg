/* oarg - a simple command line/file parser
** by Erik Perillo */

#ifndef __OARG_HEADER__
#define __OARG_HEADER__

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>

#define DEBUG 0 
#define debugmsg(msg)\
std::cout << "[oarg@debug] " << msg << std::endl

#define OARG_VERSION "1.1"
#define MAGIC_NUMBER 1

namespace oarg
{
	std::vector<std::string> split(const std::string& src_str, const std::string& delim = ",");
	int parse(int argc, char** argv, bool clear = true);
	int parse(const std::string& filename, bool clear = true);
	void describeArgs(const std::string& helpmsg = "");
	
	//class for describing a command line argument 
	class OargBase
	{
		public:
		//ctors and dtors
		OargBase(const std::string& names, const std::string& description, int pos_n_found = 0);
		OargBase(const OargBase& oarg);
		OargBase(); 
		~OargBase();
 
		//routines
		virtual OargBase& operator=(const OargBase& oarg);
		static std::string getUnknownOption(int index = 0);
		int getId();
		int getPosNFound();

		private:
		static std::string pureName(const std::string& name);
		static std::string clName(const std::string& name);
		static std::string cfName(const std::string& name);
		static void describe(const std::string& helpmsg = "");
		//holds unknown options in case of error
		static std::vector<std::string> unknown_options;	
		
		protected:
		virtual void setVec() = 0;
		virtual void clear() = 0;
		static bool isClName(const std::string& word);
		//variables
		int id,pos_n_found;
		bool found;
		std::vector<std::string> names;
		std::string description;
		std::vector<std::string> str_vals;

		//friends declaration
		friend int parse(int argc, char** argv, bool clear);
		friend int parse(const std::string& filename, bool clear);
		friend void describeArgs(const std::string&);
	};

	//class which includes values	
	template <class Tp = int>
	class Oarg: public OargBase
	{
		public:
		//ctors and dtors
		Oarg(const std::string& names, const Tp& def_val, const std::string& description, int pos_n_found = 0);
		Oarg(const Oarg& clarg);
		Oarg(); 
		~Oarg();

		//routines
		Oarg& operator=(const Oarg& clarg);

		//getters
		bool wasFound();
		int nParsedVals();
		Tp getVal(int index = 0);
		std::vector<Tp> getValVec();

		private:
		//routines		
		void setVec();
		void clear();
		//variables
		int pos_n_found;
		Tp def_val;
		std::vector<Tp> val_vec;
	};
	
	//container of argument describers
	class Container
	{
		private:
		//routines
		static int add(OargBase* oarg_ptr, bool is_repeated = false);
 		static std::vector<bool> repeated;	
		static std::vector<OargBase*> oargs;
		static std::vector<std::string> arg_vec;

		//declaration of friend class
		friend class OargBase;
		template <class Tp> friend class Oarg;
		friend int parse(int argc, char** argv, bool clear);
		friend int parse(const std::string& filename, bool clear);
	};

	class Comparer
	{
		public:
		Comparer();
		bool operator()(OargBase* a, OargBase* b);
		int getIndex();
		void setIndex(int x);
	
		private:
		int index;
	};

	//including implementation of template methods
	#include "oarg.tpp"
}

#endif
