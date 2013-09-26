#ifndef COMMON_H_INCLUDED
#define COMMON_H_INCLUDED

#include <string>
#include <exception>

#define THROW_EX(str) throw std::runtime_error(str)

typedef unsigned int uint;
typedef uint Id;
typedef std::string Word;


#endif //COMMON_H_INCLUDED
