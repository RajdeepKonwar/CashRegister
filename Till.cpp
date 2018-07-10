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
 * Till class methods.
 **/

#include <string>
#include <vector>

#include "Till.hpp"

// -------------------------------------------------------
// Function:    Till::Till
// Purpose:     Constructor - Initialize map with hard-coded values and
//              display initial amount in Till
// -------------------------------------------------------
Till::Till( std::ostream &out ) : m_display(out.rdbuf()) {
  /** Initial counts of each denomination in register (hard-coded)
   *  Can be improved upon by passing as constructor input or as input to
   *  another helper function handling initialization of till (not implemented)
   **/
  m_till[Penny]   = 100;
  m_till[Nickel]  = 100;
  m_till[Dime]    = 100;
  m_till[Quarter] = 100;
  m_till[Half]    = 100;
  m_till[Dollar]  = 100;
  m_till[Five]    = 100;
  m_till[Ten]     = 100;
  m_till[Twenty]  = 50;
  m_till[Fifty]   = 20;
  m_till[Hundred] = 10;

  //! Initial amount in register (for cashier thus std::cout)
  std::cout << "Initial amount in register = $" << totalInRegister() << std::endl;
}

// -------------------------------------------------------
// Function:    Till::totalInRegister
// Purpose:     How much money is in the cash register?
// -------------------------------------------------------
double Till::totalInRegister() const {
  int l_total = 0;
  std::unordered_map< Denomination, int >::const_iterator l_it = m_till.begin();

  for( ; l_it != m_till.end(); ++l_it )
    l_total += ((int) l_it->first) * l_it->second;

  return ((double) l_total / 100.0);
}

// -------------------------------------------------------
// Function:    Till::makeChange
// Purpose:     Return change to customer in form of proper denominations
//              and handle various edge-cases
// -------------------------------------------------------
int Till::makeChange( const double &i_amountPaid,
                      const double &i_amountOwed ) {
  double l_change = i_amountPaid - i_amountOwed;
  std::string l_message;

  //! Transaction information (displayed on LCD for customer)
  m_display << "\n[LCD] Paid: $" << i_amountPaid << std::endl;
  m_display <<   "[LCD] Owed: $" << i_amountOwed << std::endl;

  //! If amount paid is less than amount owed
  if( l_change < 0.0 ) {
    std::ostringstream l_oss;
    l_oss << -l_change;
    std::string l_pay = l_oss.str();
    l_message = "[LCD] Please pay $" + l_pay + " more!";
    reportError( const_cast< char * >(l_message.c_str()) );

    return 0;
  }

  //! If there is not sufficient cash left in register
  if( l_change > totalInRegister() ) {
    //! Instruction for cashier to refill register
    l_message = "[LCD] Please wait for cashier to refill register. Thank you for your patience!";
    reportError( const_cast< char * >(l_message.c_str()) );
    std::cout << "Insufficient funds in register! Please refill..\n";

    return -1;
  }

  //! Display change in dollars (displayed on LCD for customer)
  m_display << "[LCD] Diff: $" << l_change << std::endl;

  //! Convert change to cents
  int l_cents = l_change * 100.0 + 0.5;
  //! Vector of denominations (biggest to smallest)
  std::vector< Denomination > l_denom{ Hundred, Fifty,  Twenty,
                                       Ten,     Five,   Dollar, Half,
                                       Quarter, Dime,   Nickel, Penny };
  std::vector< Denomination >::iterator l_it;
  int l_count[11], l_i = 0;

  for( l_it = l_denom.begin(); l_it != l_denom.end(); ++l_it ) {
    //! Get count of denomination in loop
    l_count[l_i] = l_cents / (*l_it);

    //! If count is positive
    if( l_count[l_i] ) {
      //! If count is greater than count in Till, set it to Till's count
      if( l_count[l_i] > m_till[*l_it] )
        l_count[l_i] = m_till[*l_it];

      //! Deduct dispensed amount in cents
      l_cents -= l_count[l_i] * (*l_it);
    }

    l_i++;  //! Update counter
  }

  //! If all the change could be converted to denominations
  if( !l_cents ) {
    l_i = 0;

    for( l_it = l_denom.begin(); l_it != l_denom.end(); ++l_it ) {
      if( l_count[l_i] ) {
        dispense( *l_it, l_count[l_i] );
        //! Info for cashier hence std::cout
        std::cout << "Denomination: " << displayDenomination( *l_it )
                  << "\tCount: " << l_count[l_i] << std::endl;
      }

      l_i++;  //! Update counter
    }
  } else {
    std::cout << "Insufficient change in register! Please refill..\n";
    l_message = "[LCD] Please wait for cashier to refill register. Thank you for your patience!";
    reportError( const_cast< char * >(l_message.c_str()) );

    return -1;
  }

  //! Amount left after each transaction (for cashier thus std::cout)
  std::cout << "Amount left in register = $" << totalInRegister() << std::endl;

  return 0;
}

// -------------------------------------------------------
// Function:    Till::DisplayDenomination
// Purpose:     Display readable denomination info (\u00A2 is ascii for cent)
// -------------------------------------------------------
std::string Till::displayDenomination( const Denomination &i_d ) const {
  std::string l_den;

  switch( i_d ) {
    case Hundred:
      l_den = "$100";     break;
    case Fifty:
      l_den = "$50";      break;
    case Twenty:
      l_den = "$20";      break;
    case Ten:
      l_den = "$10";      break;
    case Five:
      l_den = "$5";       break;
    case Dollar:
      l_den = "$1";       break;
    case Half:
      l_den = "50\u00A2"; break;
    case Quarter:
      l_den = "25\u00A2"; break;
    case Dime:
      l_den = "10\u00A2"; break;
    case Nickel:
      l_den = "5\u00A2";  break;
    case Penny:
      l_den = "1\u00A2";  break;
    default:
      l_den = "";         break;
  }

  return l_den;
}