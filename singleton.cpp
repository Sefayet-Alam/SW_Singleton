#include <iostream>
#include <string>
#include <gtest/gtest.h>

using namespace std;

// ===== Singleton Class =====
class Singleton {
private:
    static Singleton* instance;  // Static pointer to the single instance
    string value;

    // Private constructor prevents external instantiation
    Singleton(const string val) : value(val) {}

public:
    // Deleted copy constructor and assignment operator to prevent copies
    Singleton(const Singleton&) = delete;
    Singleton& operator=(const Singleton&) = delete;

    // Static method to get the singleton instance
    static Singleton* getInstance(const string val) {
        // If instance doesn't exist, create it
        if (instance == nullptr) {
            instance = new Singleton(val);
        }
        return instance;
    }

    string getValue() const {
        return value;
    }

    // Static method to reset singleton instance (for testing only)
    static void resetInstance() {
        delete instance;
        instance = nullptr;
    }
};

// Initialize static member to nullptr
Singleton* Singleton::instance = nullptr;

// ===== Google Test Cases =====

TEST(SingletonTest, SameInstanceReturned) {
    Singleton::resetInstance();  // Reset before test
    Singleton* s1 = Singleton::getInstance("First");
    Singleton* s2 = Singleton::getInstance("Second");

    // Expect same pointer
    EXPECT_EQ(s1, s2);

    // Expect original value retained (not "Second")
    EXPECT_EQ(s1->getValue(), "First");
}

TEST(SingletonTest, UniqueInstanceAlways) {
    Singleton::resetInstance();
    Singleton* s1 = Singleton::getInstance("A");
    Singleton* s2 = Singleton::getInstance("A");

    // Should be same instance even if value passed again
    EXPECT_EQ(s1, s2);
}

TEST(SingletonTest, ResetReinitializesInstance) {
    Singleton::resetInstance();
    Singleton* s1 = Singleton::getInstance("X");
    Singleton::resetInstance();  // Reset instance

    Singleton* s2 = Singleton::getInstance("Y");

    // Now s1 and s2 should not be same
    EXPECT_NE(s1, s2);
    EXPECT_EQ(s2->getValue(), "Y");
}

TEST(SingletonTest, IncorrectValueAfterFirstInitFails) {
    Singleton::resetInstance();
    Singleton* s1 = Singleton::getInstance("Config1");
    Singleton* s2 = Singleton::getInstance("Config2");

    // Intentional failure: Expect value to be second (which should not happen)
    EXPECT_NE(s2->getValue(), "Config2");  // Passes
    EXPECT_EQ(s2->getValue(), "Config1");  // Passes
    EXPECT_EQ(s2->getValue(), "Config2");  // ❌ Will fail (used as negative test)
}

TEST(SingletonTest, CopyAndAssignmentShouldBeBlocked) {
    Singleton::resetInstance();
    Singleton* s1 = Singleton::getInstance("Blocked");

    // Uncommenting below should give compile error, hence test by commenting
    // Singleton s2(*s1); // ❌ Copy constructor is deleted
    // Singleton s3 = *s1; // ❌ Assignment operator is deleted

    // Since we can't test compile-time errors here, we simply assert the instance works
    EXPECT_EQ(s1->getValue(), "Blocked");
}

// ===== Main Runner =====
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

/*
g++ singleton.cpp /home/sefayet/googletest/build/lib/libgtest.a -lpthread -I /home/sefayet/googletest/googletest/include/ -o singleton_test
./singleton_test
echo "# SW_Singleton" >> README.md
git init
git add .
git commit -m "first commit"
git branch -M main
git remote add origin https://github.com/Sefayet-Alam/SW_Singleton.git
git push -u origin main
*/