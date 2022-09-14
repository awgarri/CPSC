import threading, os, re, time, sys, json, traceback
from optparse import OptionParser
from ChatClient import CRCClient
from ChatServer import CRCServer
from ChatMessageParser import *
from Testers.NetworkConnectivityTest import NetworkConnectivityTest
from Testers.CRCFunctionalityTest import CRCFunctionalityTest

class CRCLogger(object):
    def __init__(self, logfile):
        self.terminal = sys.stdout
        self.log = logfile
        self.lock = threading.Lock()

    def write(self, message):
        self.lock.acquire()
        try:
            self.terminal.write(message)
            self.log.write(message)
        finally:
            self.lock.release()

    def flush(self):
        #this flush method is needed for python 3 compatibility.
        #this handles the flush command by doing nothing.
        #you might want to specify some extra behavior here.
        self.terminal.flush()
        self.log.flush()

    def print_to_log(self, message):
        self.log.write(message + "\n")

    def print_to_terminal(self, message):
        self.terminal.write(message)


class CRCTestManager(object):

    ######################################################################
    # Initialization
    def __init__(self, CRCServerImpl = None, CRCMessageParserImpl = None, catch_exceptions = False):
        self.catch_exceptions = catch_exceptions
        if CRCServerImpl:
            self.CRCServerImpl = CRCServerImpl
        else:
            self.CRCServerImpl = CRCServer
        
        if CRCMessageParserImpl:
            self.CRCMessageParserImpl = CRCMessageParserImpl
        else:
            self.CRCMessageParserImpl = MessageParser

    ######################################################################
    # Test Management
    def run_tests(self, tests):
        __location__ = os.path.realpath(os.path.join(os.getcwd(), os.path.dirname(__file__)))
        if not os.path.exists(os.path.join(__location__, 'Logs')):
            os.makedirs(os.path.join(__location__, 'Logs'))   

        score = 0
        results = []
        for test in sorted(tests.keys()):
            # Open the test file
            with open(os.path.join(__location__, 'TestCases', '%s.cfg' % test), 'r') as fp:
                test_config = json.load(fp)
                # Redirect all output to a log file for this test
                with open(os.path.join(__location__, 'Logs', '%s.log' % test), 'w') as logfile:
                    #sys.stdout = CRCLogger(logfile)
                    print("\n##############################################")
                    print("Beginning test " + test + "\n")
                    passed, errors, exception = self.run_test(test_config)
                    results.append({
                        'test':test, 
                        'passed':passed, 
                        'errors':errors,
                        'exception':exception
                    })
                    print("\nTest passed:" + str(passed))
                    time.sleep(1)
                    #sys.stdout = sys.__stdout__

        print("\n##############################################")
        for result in results:
            if result['passed']:
                score += tests[result['test']]
            print("%s passed: %r" % (result['test'], result['passed']))
            if result['errors']:
                print("%s" % (result['errors']))
            if result['exception']:
                print(traceback.format_exc())
        
        return score, results

    def run_test(self, test):
        tester = None
        if test["type"] == "network_connectivity":
            tester = NetworkConnectivityTest(self.CRCServerImpl, CRCClient, self.catch_exceptions)
        elif test["type"] == "CRC_functionality":
            tester = CRCFunctionalityTest(self.CRCServerImpl, CRCClient, self.catch_exceptions)
        else:
            return None
        return tester.run_test(test)
        

if __name__ == "__main__":

    test_manager = CRCTestManager()
    basic_score = 0
    message_parsing_score = 0
    CRC_connection_score = 0

    CRC_connection_tests = {
        # Tests basic connection setup, doesn't involve processing messages
        # 15 points
        '1_1_TwoConnections':7,
        '1_2_FourConnections':5,
        '1_3_EightConnections':3,

        # Tests the server registration messages
        # 15 points
        #'2_1_TwoServers':7,
        #'2_2_FourServers':5,
        #'2_3_ElevenServers':3,

        # Tests the user registration messages
        # 15 points
        #'3_1_OneServer_OneClient':3,
        #'3_2_OneServer_TwoClients':4,
        #'3_3_ThreeServers_FourClients':5,
        #'3_4_ElevenServers_EightClients':3,

        # Tests the chat messaging
        #'4_1_Message_Zero_Hops':2,
        #'4_2_Message_One_Hop':1.5,
        #'4_3_Message_Three_Hops':1.5,   # FAIL

        # Tests status messages
        #'5_1_WelcomeStatus':2,
        #'5_2_DuplicateID_Server':1,
        #'5_3_DuplicateID_Client':1,
        #'5_4_UnknownID_Client':1,

        # Test the client quit functionality
        #'6_1_ClientQuit_OneServer':2,
        #'6_2_ClientQuit_ThreeServers':1.5,
        #'6_3_ClientQuit_ElevenServers':1.5, # FAIL
    }

    CRC_connection_score = test_manager.run_tests(CRC_connection_tests)