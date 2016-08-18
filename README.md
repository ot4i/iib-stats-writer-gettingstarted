# iib-stats-writer-gettingstarted

## Introduction
In IBM Integration Bus, message flow statistics and accounting data can be collected
to record performance and operating details of one or more message flows. Message flow statistics and accounting data captures dynamic information about the runtime behavior
of a message flow. For example, it indicates how many messages are processed and how
large those messages are, as well as processor usage and elapsed processing times.

The IBM Integration Bus runtime has built-in support for publishing the message flow
statistics and accounting data in XML or JSON format on MQ or MQTT topics, writing
SMF records on z/OS, or recording to user trace.

IBM Integration Bus v10.0.0.5 introduces a new C plugin API that can be used to handle
message flow statistics and accounting data directly from the IBM Integration Bus
runtime.

This repository contains a small sample C plugin written using this new API, to serve
as a starting point for plugin developers looking to write their own plugins.

## Installation
There are no releases yet. In order to try this sample, you must build the code yourself.

### Build dependencies

#### C++ compiler
You need a recent C++ compiler that supports C++11. For example, GNU GCC 4.8+ (Linux),
Microsoft Visual Studio 2015 (Windows), or Xcode 7.3 (Mac OS X). Any other compiler
that supports C++11 should be sufficient, but no other compilers have been tested.

#### CMake
This sample uses the excellent CMake build system. CMake can generate a variety of
project files for use with your favourite IDE or make system.
CMake can be downloaded from: https://cmake.org

#### IBM Integration Bus
You need an installation of IBM Integration Bus v10.0.0.5 or later. The most recent
open beta build can be downloaded from: https://ibm.biz/iibopenbeta

### Build instructions

1. Extract the source from GitHub:

  `git clone https://github.com/ot4i/iib-stats-writer-gettingstarted`

2. Change into the source directory:

  `cd iib-stats-writer-gettingstarted`

3. Run CMake to generate project files or makefiles:

  `cmake -DIIB_INSTALL_DIR=/opt/ibm/iib-10.0.0.5`

4. Run CMake to perform the build:

  `cmake --build .`

5. If the build completes successfully, then a file named **gettingstartedsw.lil**
   will have been created in the current directory. Check that this file exists.

### Installation instructions

1. Stop all integration nodes that are using the installation:

  `mqsistop NODE`

2. Copy the built file into the installation directory:

  `sudo cp -f gettingstartedsw.lil /opt/ibm/iib-10.0.0.5/server/lil/` (Linux and Mac OS X)  
  `copy /y gettingstartedsw.lil "C:\Program Files\IBM\IIB\10.0.0.5\server\bin\"` (Windows)

3. Start all integration nodes again:

  `mqsistart NODE`

## Testing

The sample plugin writes a snippet of the statistic record to the system log. The sample
plugin also has two controllable properties that are included in the system log messages.
To configure and enable the sample plugin once it has been installed, follow these steps:

1. Deploy at least one message flow that can be configured for collection of message flow
   statistics and accounting data.

   *Don't have a message flow? Check out the tutorials gallery in the toolkit!*

2. Review the configured values of the installed plugin:

  `mqsireportproperties NODE -e SERVER -o GettingStartedStatsWriter -r`

       GettingStartedStatsWriter
        uuid='GettingStartedStatsWriter'
        userTraceLevel='none'
        traceLevel='none'
        userTraceFilter='none'
        traceFilter='debugTrace'
        property1=''
        property2=''

       BIP8071I: Successful command completion.

3. Modify the configured values of the installed plugin:

  `mqsichangeproperties NODE -e SERVER -o GettingStartedStatsWriter -n property1,property2 -v myValue1,myValue2`

       BIP8071I: Successful command completion.

  `mqsireportproperties NODE -e SERVER -o GettingStartedStatsWriter -r`

       GettingStartedStatsWriter
        uuid='GettingStartedStatsWriter'
        userTraceLevel='none'
        traceLevel='none'
        userTraceFilter='none'
        traceFilter='debugTrace'
        property1='myValue1'
        property2='myValue2'

       BIP8071I: Successful command completion.

4. Enable message statistics and the installed plugin:

  `mqsichangeflowstats IB10NODE -s -e default -j -c active -t basic -n advanced -o gettingstarted`

       BIP8071I: Successful command completion.

5. Wait for at least 20 seconds (the default period in-between snapshot statistic
   messages), and check the system log for a BIP2118E message from the installed,
   configured, and enabled plugin:

       BIP2118E: Integration node internal error: diagnostic information 'Called to write a statistics record', 'GettingStartedStatsWriter', 'gettingstarted', 'MYAPP', 'MYLIB', 'MYFLOW', 'myValue1', 'myValue2'.

6. Disable message flow statistics and the installed plugin:

  `mqsichangeflowstats IB10NODE -s -e default -j -c inactive -o usertrace`

       BIP8071I: Successful command completion.
