/* Generated file, do not edit */

#ifndef CXXTEST_RUNNING
#define CXXTEST_RUNNING
#endif

#define _CXXTEST_HAVE_STD
#define _CXXTEST_HAVE_EH
#include <cxxtest/TestListener.h>
#include <cxxtest/TestTracker.h>
#include <cxxtest/TestRunner.h>
#include <cxxtest/RealDescriptions.h>
#include <cxxtest/TestMain.h>
#include <cxxtest/ErrorPrinter.h>

int main( int argc, char *argv[] ) {
 int status;
    CxxTest::ErrorPrinter tmp;
    CxxTest::RealWorldDescription::_worldName = "cxxtest";
    status = CxxTest::Main< CxxTest::ErrorPrinter >( tmp, argc, argv );
    return status;
}
bool suite_ServerTestSuite_init = false;
#include "test_server.h"

static ServerTestSuite suite_ServerTestSuite;

static CxxTest::List Tests_ServerTestSuite = { 0, 0 };
CxxTest::StaticSuiteDescription suiteDescription_ServerTestSuite( "test_server.h", 69, "ServerTestSuite", suite_ServerTestSuite, Tests_ServerTestSuite );

static class TestDescription_suite_ServerTestSuite_testServerScenario1 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ServerTestSuite_testServerScenario1() : CxxTest::RealTestDescription( Tests_ServerTestSuite, suiteDescription_ServerTestSuite, 81, "testServerScenario1" ) {}
 void runTest() { suite_ServerTestSuite.testServerScenario1(); }
} testDescription_suite_ServerTestSuite_testServerScenario1;

static class TestDescription_suite_ServerTestSuite_testServerScenario2 : public CxxTest::RealTestDescription {
public:
 TestDescription_suite_ServerTestSuite_testServerScenario2() : CxxTest::RealTestDescription( Tests_ServerTestSuite, suiteDescription_ServerTestSuite, 100, "testServerScenario2" ) {}
 void runTest() { suite_ServerTestSuite.testServerScenario2(); }
} testDescription_suite_ServerTestSuite_testServerScenario2;

#include <cxxtest/Root.cpp>
const char* CxxTest::RealWorldDescription::_worldName = "cxxtest";
