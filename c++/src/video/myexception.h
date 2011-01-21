/*
 * exception.h
 *
 *  Created on: 18 janv. 2011
 *      Author: P.E. Hladik
 *  Inspired of code from http://www.siteduzero.com/ by NoHar
 */

#ifndef MYEXCEPTION_H
#define MYEXCEPTION_H

#include <exception>

#define	EX_MESSAGE	0
#define EX_WARNING	1
#define EX_ERREUR	2

namespace robotInsa
{
   /// Classe d'exception basique.
   /**
    * Implémentation d'une exception comprenant un niveau d'importance (message, warning, erreur), ainsi
    * qu'un message explicatif.
    */
   class MyException: public std::exception
   {
      public:
         /// Constructeur.
         MyException( unsigned char niveau = EX_MESSAGE, const char* message = "" ) throw ();

         /// Accesseur hérité.
         virtual const char* what() const throw();

         /// Accesseur pour le niveau d'importance.
         unsigned char getNiveau() const throw();

         /// Destructeur.
         virtual ~MyException() throw();

      private:
         /// Niveau d'importance de l'exception.
         unsigned char mNiveau;

         /// Message descriptif.
         const char* mMessage;
   };
}

#endif //!ZVEXCEPTION_H
