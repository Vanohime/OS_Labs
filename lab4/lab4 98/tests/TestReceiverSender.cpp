#include <gtest/gtest.h>
#include "Receiver.h"
#include "Sender.h"
#include <sstream>
#include <fstream>

class IOTestFixture : public ::testing::Test {
protected:
    void SetUp() override {
        orig_cin = std::cin.rdbuf();
        orig_cout = std::cout.rdbuf();
        orig_cerr = std::cerr.rdbuf();

        std::cin.rdbuf(test_input.rdbuf());
        std::cout.rdbuf(test_output.rdbuf());
        std::cerr.rdbuf(test_error.rdbuf());
    }

    void TearDown() override {
        std::cin.rdbuf(orig_cin);
        std::cout.rdbuf(orig_cout);
        std::cerr.rdbuf(orig_cerr);
    }

    void provideInput(const std::string& input) {
        test_input.str(input);
        test_input.clear(); 
    }

    std::string getOutput() {
        return test_output.str();
    }

    std::string getError() {
        return test_error.str();
    }

    void clearStreams() {
        test_output.str("");
        test_error.str("");
    }

private:
    std::streambuf* orig_cin;
    std::streambuf* orig_cout;
    std::streambuf* orig_cerr;
    std::stringstream test_input;
    std::stringstream test_output;
    std::stringstream test_error;
};

class ReceiverTest : public IOTestFixture {
};

class SenderTest : public IOTestFixture {
};

TEST_F(ReceiverTest, ConstructorWithParameters) {
    Receiver r("testfile.bin", 5, 3);
    EXPECT_EQ(r.get_filename(), "testfile.bin");
}

TEST_F(ReceiverTest, DefaultConstructor) {
    Receiver r;
    //EXPECT_EQ(r.get_user_choice(), 0);
    EXPECT_TRUE(r.get_filename().empty());
}

TEST_F(ReceiverTest, GetUserChoiceValidInput) {
    provideInput("1\n");
    Receiver r;
    EXPECT_EQ(r.get_user_choice(), 1);
}

TEST_F(ReceiverTest, GetUserChoiceInvalidThenValid) {
    provideInput("abc\n2\n1\n");
    Receiver r;
    EXPECT_EQ(r.get_user_choice(), 1);

    std::string output = getOutput();
    EXPECT_NE(output.find("Ошибка"), std::string::npos);
}

TEST_F(ReceiverTest, ReadInputValid) {
    provideInput("test_input.bin\n5\n3\n");
    Receiver r;
    r.read_input();

    EXPECT_EQ(r.get_filename(), "test_input.bin");
}

TEST_F(SenderTest, ConstructorWithFilename) {
    Sender s("testfile.bin");
    EXPECT_EQ(s.get_filename(), "testfile.bin");
}

TEST_F(SenderTest, DefaultConstructor) {
    Sender s;
    EXPECT_TRUE(s.get_filename().empty());
}

TEST_F(SenderTest, GetUserChoiceValidInput) {
    provideInput("0\n");
    Sender s;
    EXPECT_EQ(s.get_user_choice(), 0);
}

TEST_F(SenderTest, GetUserChoiceInvalidThenValid) {
    provideInput("invalid\n5\n0\n");
    Sender s;
    EXPECT_EQ(s.get_user_choice(), 0);

    std::string output = getOutput();
    EXPECT_NE(output.find("Ошибка"), std::string::npos);
}


