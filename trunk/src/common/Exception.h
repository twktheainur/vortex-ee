/*"The contents of this file are subject to the Mozilla Public License
Version 1.1 (the "License"); you may not use this file except in
compliance with the License. You may obtain a copy of the License at
http://www.mozilla.org/MPL/

Software distributed under the License is distributed on an "AS IS"
basis, WITHOUT WARRANTY OF ANY KIND, either express or implied. See the
License for the specific language governing rights and limitations
under the License.*/
#ifndef EXCEPTION_H_
#define EXCEPTION_H_
#include <string>
class Exception
{
public:
  Exception(int exception){id = exception;}
  const char * what() const throw(){return error.data();}
  ~Exception() throw (){};
  inline std::string getError(){return error;}
  inline void setError(std::string err){error=err;}
private:
    std::string error;
    int id;
};

#endif /*EXCEPTION_H_*/
