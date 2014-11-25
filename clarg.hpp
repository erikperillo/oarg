/* clarg - a simple command line parser
** version 1.0
** by Erik Perillo
*/

#ifndef __CLARG_HEADER__
#define __ClARG_HEADER__

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include <sstream>

namespace clarg
{
     //struct for describing a command line argument 
     struct Clarg
     {
          std::vector<std::string> names;
          std::string              description;

          Clarg(const char*, const char*);       
     };

     //stores information about command line arguments created and parses argv
     class Container
     {
          int                      read_args;
          std::vector<std::string> argv;
          std::vector<Clarg>       clargs;     

          public:

          Container(int argc, char** argv);

          #include "parseNum.tpp" //template <class Tp> Tp parseNum(const Clarg&, const Tp&);
          bool parseBool(const Clarg&, const bool&);
          std::string parseString(const Clarg&, const char*);

          void describe(const char*);

          int getReadArgs();
     };
}

#endif
