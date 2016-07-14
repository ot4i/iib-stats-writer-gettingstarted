/********************************************************* {COPYRIGHT-TOP} ***
* Copyright 2016 IBM Corporation
*
* All rights reserved. This program and the accompanying materials
* are made available under the terms of the MIT License
* which accompanies this distribution, and is available at
* http://opensource.org/licenses/MIT
********************************************************** {COPYRIGHT-END} **/

#include "GettingStartedStatsWriter.hpp"

#include <exception>

extern "C" {

  /*
   * This function is called by the IBM Integration Bus runtime when it loads the LIL file.
   * It is expecting a CsiStatsWriter object to be created using csiCreateStatsWriter() and
   * returned. The CsiStatsWriter is registered after this function has been called.
   */
  CsiStatsWriter LilFactoryExportPrefix * LilFactoryExportSuffix bipGetStatsWriter() {
    try {
      GettingStartedStatsWriter* instance = new GettingStartedStatsWriter();
      return instance->writer();
    } catch (const std::exception& e) {
      return nullptr;
    }
  }

}

namespace {

  /*
   * This is the C callback for getAttributeName(); simply forward the call onto the C++ object.
   */
  CciSize getAttributeNameCallback(int* rc, int index, CciChar* buffer, CciSize bufferLength, void* context) {
    GettingStartedStatsWriter* writer = reinterpret_cast<GettingStartedStatsWriter*>(context);
    return writer->getAttributeName(rc, index, buffer, bufferLength);
  }

  /*
   * This is the C callback for getAttribute(); simply forward the call onto the C++ object.
   */
  CciSize getAttributeCallback(int* rc, const CciChar* name, CciChar* buffer, CciSize bufferLength, void* context) {
    GettingStartedStatsWriter* writer = reinterpret_cast<GettingStartedStatsWriter*>(context);
    return writer->getAttribute(rc, name, buffer, bufferLength);
  }

  /*
   * This is the C callback for setAttribute(); simply forward the call onto the C++ object.
   */
  void setAttributeCallback(int* rc, const CciChar* name, const CciChar* value, void* context) {
    GettingStartedStatsWriter* writer = reinterpret_cast<GettingStartedStatsWriter*>(context);
    writer->setAttribute(rc, name, value);
  }

  /*
   * This is the C callback for write(); simply forward the call onto the C++ object.
   */
  void writeCallback(const CsiStatsRecord* record, void* context) {
    GettingStartedStatsWriter* writer = reinterpret_cast<GettingStartedStatsWriter*>(context);
    writer->write(record);
  }

  /*
   * This is the name of the statistics writer resource. This is the name that should
   * be passed to the mqsichangeproperties or mqsireportproperties commands to change
   * or report on properties of this statistics writer, for example:
   *
   * mqsireportproperties NODE -e SERVER -o GettingStartedStatsWriter -r
   * mqsichangeproperties NODE -e SERVER -o GettingStartedStatsWriter -n property1,property2 -v value1,value2
   */
  const std::u16string RESOURCE_NAME(u"GettingStartedStatsWriter");

  /*
   * This is the name of the "format" written by this statistics writer. This is the
   * name that should be passed to mqsichangeflowstats command to enable writing
   * statistics using this statistics writer for a specific message flow or set of
   * message flows, for example:
   *
   * mqsichangeflowstats NODE -s -e SERVER -f FLOW -c active -o gettingstarted
   */
  const std::u16string FORMAT_NAME(u"gettingstarted");

  /*
   * This is the name of a property of this statistics writer. The value of the
   * property can be accessed using the mqsichangeproperties or mqsireportproperties
   * commands (see above).
   */
  const std::u16string PROPERTY1_NAME(u"property1");

  /*
   * This is the name of a property of this statistics writer. The value of the
   * property can be accessed using the mqsichangeproperties or mqsireportproperties
   * commands (see above).
   */
  const std::u16string PROPERTY2_NAME(u"property2");

}

/*
 * Constructor.
 */
GettingStartedStatsWriter::GettingStartedStatsWriter()
 : iWriter(nullptr) {

  /*
   * Create the virtual function table for the statistics writer.
   * These functions will be called by the IBM Integration Bus
   * runtime when it needs to interact with the statistics writer.
   */
  CsiStatsWriterVft vft = { CSI_STATS_WRITER_VFT_DEFAULT };
  vft.getAttributeName = &getAttributeNameCallback;
  vft.getAttribute = &getAttributeCallback;
  vft.setAttribute = &setAttributeCallback;
  vft.write = &writeCallback;

  /*
   * Create the statistics writer, passing it the virtual function
   * table, resource name, and format name that are defined above.
   * If this function returns an error, then throw an exception so
   * that we will return nullptr as the result of the bipGetStatsWriter()
   * call.
   */
  int returnCode = CCI_FAILURE;
  iWriter = csiCreateStatsWriter(
    &returnCode,
    RESOURCE_NAME.c_str(),
    FORMAT_NAME.c_str(),
    &vft,
    this
  );
  if (returnCode != CCI_SUCCESS) {
    const char16_t* traceText = u"Call to csiCreateStatsWriter failed";
    const char16_t* inserts[] = { traceText, RESOURCE_NAME.c_str(), FORMAT_NAME.c_str() };
    cciLogWithInsertsW(nullptr, CCI_LOG_ERROR,__FILE__, __LINE__, __func__, u"BIPmsgs", 2113, traceText, inserts, sizeof(inserts) / sizeof(inserts[0]));
    throw std::exception();
  }

}

/*
 * Destructor.
 */
GettingStartedStatsWriter::~GettingStartedStatsWriter() {

}

/*
 * Called by the IBM Integration Bus runtime to get the name of the property at the
 * specified index. If no property exists at the specified index, then this function
 * should return CCI_ATTRIBUTE_UNKNOWN. If the specified buffer is too small for the
 * name of the property, then this function should return CCI_BUFFER_TOO_SMALL and
 * the required size of the buffer.
 */
CciSize GettingStartedStatsWriter::getAttributeName(int* rc, int index, CciChar* buffer, CciSize bufferLength) const {
  switch (index) {
  case 0:
    if (bufferLength < PROPERTY1_NAME.length()) {
      if (rc) *rc = CCI_BUFFER_TOO_SMALL;
      return PROPERTY1_NAME.length();
    }
    if (rc) *rc = CCI_SUCCESS;
    return PROPERTY1_NAME.copy(buffer, PROPERTY1_NAME.length());
  case 1:
    if (bufferLength < PROPERTY2_NAME.length()) {
      if (rc) *rc = CCI_BUFFER_TOO_SMALL;
      return PROPERTY2_NAME.length();
    }
    if (rc) *rc = CCI_SUCCESS;
    return PROPERTY2_NAME.copy(buffer, PROPERTY2_NAME.length());
  default:
    *rc = CCI_ATTRIBUTE_UNKNOWN;
    return 0;
  }
}

/*
 * Called by the IBM Integration Bus runtime to get the value of the property with the
 * specified name. If no property exists with the specified name, then this function
 * should return CCI_ATTRIBUTE_UNKNOWN. If the specified buffer is too small for the
 * value of the property, then this function should return CCI_BUFFER_TOO_SMALL and
 * the required size of the buffer.
 */
CciSize GettingStartedStatsWriter::getAttribute(int* rc, const CciChar* name, CciChar* buffer, CciSize bufferLength) const {
  if (PROPERTY1_NAME == name) {
    if (bufferLength < iProperty1.length()) {
      if (rc) *rc = CCI_BUFFER_TOO_SMALL;
      return iProperty1.length();
    }
    if (rc) *rc = CCI_SUCCESS;
    return iProperty1.copy(buffer, iProperty1.length());
  } else if (PROPERTY2_NAME == name) {
    if (bufferLength < iProperty2.length()) {
      if (rc) *rc = CCI_BUFFER_TOO_SMALL;
      return iProperty2.length();
    }
    if (rc) *rc = CCI_SUCCESS;
    return iProperty2.copy(buffer, iProperty2.length());
  } else {
    if (rc) *rc = CCI_ATTRIBUTE_UNKNOWN;
    return 0;
  }
}

/*
 * Called by the IBM Integration Bus runtime to set the value of the property with the
 * specified name. If no property exists with the specified name, then this function
 * should return CCI_ATTRIBUTE_UNKNOWN.
 */
void GettingStartedStatsWriter::setAttribute(int* rc, const CciChar* name, const CciChar* value) {
  if (PROPERTY1_NAME == name) {
    iProperty1.assign(value);
    if (rc) *rc = CCI_SUCCESS;
  } else if (PROPERTY2_NAME == name) {
    iProperty2.assign(value);
    if (rc) *rc = CCI_SUCCESS;
  } else {
    if (rc) *rc = CCI_ATTRIBUTE_UNKNOWN;
  }
}

/*
 * Called by the IBM Integration Bus runtime to write the specified statistics record.
 * The statistics record contains data for one message flow with statistics enabled, so
 * this function will be called once per message flow.
 */
void GettingStartedStatsWriter::write(const CsiStatsRecord* record) {

  /*
   * Write the statistics record to the system log.
   */
  const char16_t* traceText = u"Called to write a statistics record";
  const char16_t* inserts[] = {
    traceText,
    RESOURCE_NAME.c_str(),
    FORMAT_NAME.c_str(),
    record->messageFlow.applicationName,
    record->messageFlow.libraryName,
    record->messageFlow.messageFlowName,
    iProperty1.c_str(),
    iProperty2.c_str()
  };
  cciLogWithInsertsW(nullptr, CCI_LOG_INFORMATION, __FILE__, __LINE__, __func__, u"BIPmsgs", 2118, traceText, inserts, sizeof(inserts) / sizeof(inserts[0]));

}
