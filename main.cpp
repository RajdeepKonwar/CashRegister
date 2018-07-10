/**
 * @file This file is part of CashRegister.
 *
 * @section LICENSE
 * MIT License
 *
 * Copyright (c) 2018 Rajdeep Konwar
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 * @section DESCRIPTION
 * Main program.
 **/

#include <limits>

#include "Till.hpp"

int main( int    argc,
          char **argv ) {
  Till l_till( std::cout );
  double l_paid, l_owed;
  std::string l_line;

  char l_prompt = 'y';
  while( l_prompt != 'n' && l_prompt != 'N' ) {
    //! Enter amount paid by customer (for cashier thus std::cout)
    std::cout << std::endl << "Enter amount paid: $";
    while( std::getline( std::cin, l_line ) ) {
      std::stringstream l_ss( l_line );
      if( l_ss >> l_paid ) {
        if( l_ss.eof() && l_paid >= 0.0 ) {
          break;  //! Success
        }
      }

      //! Catch for non-numeric inputs from cashier
      std::cout << "Please enter numbers (>= 0) only!\n";
      return EXIT_FAILURE;
    }

    //! Enter amount owed by customer (for cashier thus std::cout)
    std::cout << "Enter amount owed: $";
    while( std::getline( std::cin, l_line ) ) {
      std::stringstream l_ss( l_line );
      if( l_ss >> l_owed ) {
        if( l_ss.eof() && l_owed >= 0.0 ) {
          break;  //! Success
        }
      }

      //! Catch for non-numeric inputs from cashier
      std::cout << "Please enter numbers (>= 0) only!\n";
      return EXIT_FAILURE;
    }

    //! Conduct transaction
    if( l_till.makeChange( l_paid, l_owed ) == -1 )
      return EXIT_FAILURE;

    //! Prompt for further transactions (for cashier thus std::cout)
    std::cout << std::endl << "More transactions? (y/n)";
    std::cin >> l_prompt;   //! Anything other than 'n' prompts yes
    //! Ignore until newline
    std::cin.ignore( std::numeric_limits< std::streamsize >::max(), '\n' );
  }

  return EXIT_SUCCESS;
}