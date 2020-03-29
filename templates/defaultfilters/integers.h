/*
  This file is part of the Cutelee template system.

  Copyright (c) 2009,2010 Stephen Kelly <steveire@gmail.com>

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either version
  2.1 of the Licence, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library.  If not, see <http://www.gnu.org/licenses/>.

*/

#ifndef INTEGERFILTERS_H
#define INTEGERFILTERS_H

// krazy:excludeall=includes

#include "filter.h"

using namespace Cutelee;

class AddFilter : public Filter
{
public:
  QVariant doFilter(const QVariant &input, const QVariant &argument = {},
                    bool autoescape = {}) const override;
};

class GetDigitFilter : public Filter
{
public:
  QVariant doFilter(const QVariant &input, const QVariant &argument = {},
                    bool autoescape = {}) const override;
};

#endif
