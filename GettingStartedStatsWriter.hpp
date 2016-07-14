/********************************************************* {COPYRIGHT-TOP} ***
* Copyright 2016 IBM Corporation
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the MIT License
* which accompanies this distribution, and is available at
* http://opensource.org/licenses/MIT
********************************************************** {COPYRIGHT-END} **/

#ifndef GettingStartedStatsWriter_hpp
#define GettingStartedStatsWriter_hpp

#include <BipCsi.h>
#include <string>

class GettingStartedStatsWriter {

public:

  GettingStartedStatsWriter();
  ~GettingStartedStatsWriter();

  CciSize getAttributeName(int* rc, int index, CciChar* buffer, CciSize bufferLength) const;
  CciSize getAttribute(int* rc, const CciChar* name, CciChar* buffer, CciSize bufferLength) const;
  void setAttribute(int* rc, const CciChar* name, const CciChar* value);

  void write(const CsiStatsRecord* record);

  CsiStatsWriter* writer() const { return iWriter; }

private:

  CsiStatsWriter* iWriter;
  std::u16string iProperty1;
  std::u16string iProperty2;

};

#endif // GettingStartedStatsWriter_hpp
