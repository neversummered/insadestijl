/*
 * exception.cpp
 *
 *  Created on: 18 janv. 2011
 *      Author: P.E. Hladik
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#include "myexception.h"

namespace robotInsa
{
    MyException::MyException( unsigned char niveau, const char* message ) throw ()
        :mNiveau( niveau ), mMessage( message )
    {}

    const char* MyException::what() const throw()
    {
        return mMessage;
    }

    unsigned char MyException::getNiveau() const throw()
    {
        return mNiveau;
    }

    MyException::~MyException() throw()
    {}
}
