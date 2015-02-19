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

#define DEBUG 0 
#define debugmsg(msg)\
std::cout << "[oarg@debug] " << msg << std::endl

#define OARG_VERSION "1.1"

namespace oarg
{
	void parse(int argc, char** argv, bool clear = true);
	int parse(const std::string& filename, bool clear = true);

	//class for describing a command line argument 
	class Oarg
	{
		public:
		//ctors and dtors
		Oarg(const std::string& names, const std::string& description = "");
		Oarg(const Oarg& oarg);
		Oarg(); 
		~Oarg();
 
		//routines
		virtual Oarg& operator=(const Oarg& oarg);
		static void describe(const std::string& helpmsg = "");

		private:
		static std::string pureName(const std::string& name);
		static std::string clName(const std::string& name);
		static std::string cfName(const std::string& name);

		protected:
		virtual void setVec() = 0;
		static bool isClName(const std::string& word);
		//variables
		int id;
		bool found;
		std::vector<std::string> names;
		std::string description;
		std::vector<std::string> str_vals;	

		//friends declaration
		friend void parse(int argc, char** argv, bool clear);
		friend int parse(const std::string& filename, bool clear);
	};

	//class which includes values	
	template <class Tp = int>
	class Arg: public Oarg
	{
		public:
		//ctors and dtors
		Arg(const std::string& names, const Tp& def_val, const std::string& description = "");
		Arg(const Arg& clarg);
		Arg(); 
		~Arg();

		//routines
		Arg& operator=(const Arg& clarg);

		//getters
		bool wasFound();
		int nParsedVals();
		Tp getVal(int index = 0);
		Tp getRelVal(int argc, char** argv, int pos = 1);
		std::vector<Tp> getValVec();

		private:
		//routines		
		void setVec();
		//variables
		Tp def_val;
		std::vector<Tp> val_vec;
	};

	//including implementation of template methods
	#include "arg.tpp"

	//container of argument describers
	class Container
	{
		private:
		//routines
		static int add(Oarg* oarg_ptr, bool is_repeated = false);
 		static std::vector<bool> repeated;	
		static std::vector<Oarg*> oargs;

		//declaration of friend class
		friend class Oarg;
		friend void parse(int argc, char** argv, bool clear);
		friend int parse(const std::string& filename, bool clear);
	};

}

#endif
