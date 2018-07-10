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
 * Till class definitions and inline methods.
 **/

#ifndef TILL_HPP
#define TILL_HPP

#include <iostream>
#include <sstream>
#include <unordered_map>

/** The cash register holds zero or more of these bills and coins in the Till.
 *  The value of the enum is its worth in cents.
 */
enum Denomination {
  Penny   = 1,
  Nickel  = 5,
  Dime    = 10,
  Quarter = 25,
  Half    = 50,
  Dollar  = 100,
  Five    = 500,
  Ten     = 1000,
  Twenty  = 2000,
  Fifty   = 5000,
  Hundred = 10000
};

class Till {
private:
  //! This is the LCD display on the cash register
  std::ostream                            m_display;

  //! This is the Till.  All bills and coins are stored here
  std::unordered_map< Denomination, int > m_till;

  //! Overloaded operator << for LCD displays
  inline std::ostream & operator << ( const char *i_message ) {
    m_display << i_message;
    return m_display;
  }

  //! How much money is in the cash register?
  double totalInRegister() const;

  // -------------------------------------------------------
  // Function:    CashRegister::Dispense
  // Purpose:     Remove coins/bills from the Till and give them to the customer
  // -------------------------------------------------------
  inline void dispense( const Denomination &i_d,
                        const int          &i_count ) {
    m_till[i_d] -= i_count;
  }

  // -------------------------------------------------------
  // Function:    Till::reportError
  // Purpose:     There is a problem!
  // -------------------------------------------------------
  inline void reportError( const char *i_text ) {
    m_display << i_text << std::endl;
  }

  //! Display readable denomination info
  std::string displayDenomination( const Denomination &i_d );

public:
  Till( std::ostream &out );

  // -------------------------------------------------------
  // Function:    Till::~Till
  // Purpose:     Destructor - display final amount left in Till
  // -------------------------------------------------------
  inline ~Till() {
    std::cout << "\nFinal amount in register = $" << totalInRegister() << std::endl;
  }

  /** The customer has paid money, that money is already in the Till
   *  Now, dispense change.
   **/
  int makeChange( const double &i_amountPaid,
                  const double &i_amountOwed );
};

#endif