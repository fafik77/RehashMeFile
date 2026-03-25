#include "pch.h"
#include "CppUnitTest.h"
#include "../hash-rename-fileCpp/quickdigest5.hpp"
#include "../hash-rename-fileCpp/quickdigest5.cpp"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace MD5Test
{
	TEST_CLASS(MD5Test)
	{
	public:
		/**
		 * @brief Tests different methods of hashing the same string "test", expects same results
		 */
		TEST_METHOD(Md5TestString_ProducesSameHashForAll)
		{
			Hash::MD5 test1, test2, test3;
			const std::string testStr = "test";
			const std::string resultOfTestStr = "098f6bcd4621d373cade4e832627b4f6";

			test1.update(&testStr.front(), testStr.size());
			auto res1 = test1.FinalizeHash();

			for (int i = 0; i != testStr.size(); ++i) {
				test2.update(&testStr.at(i), 1);
			}
			auto res2 = test2.FinalizeHash();

			auto res3 = test3.stringToHash(testStr);

			Assert::AreEqual(res1, resultOfTestStr);
			Assert::AreEqual(res2, resultOfTestStr);
			Assert::AreEqual(res3, resultOfTestStr);
		}
		
		TEST_METHOD(Md5LongData_ProducesSameHashForAll) {
			Hash::MD5 test1, test2, test3;
			static const std::string LongData = "Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
			const std::string resultOfLongData = "db89bb5ceab87f9c0fcc2ab36c189c2c";

			test1.update(&LongData.front(), LongData.size());
			auto res1 = test1.FinalizeHash();
			size_t speed = 59;
			for (size_t i = 0; i < LongData.size(); ) {
				// Cap the chunk size to the remaining data length or continue with speed
				size_t chunkSize = (i + speed > LongData.size()) ? (LongData.size() - i) : speed;
				test2.update(&LongData.at(i), chunkSize);
				i += chunkSize;
				speed += 7;
				// Keep varying the speed arbitrarily 
				if (speed > 100) speed = 19;
			}
			auto res2 = test2.FinalizeHash();

			auto res3 = test3.stringToHash(LongData);

			Assert::AreEqual(res1, resultOfLongData);
			Assert::AreEqual(res2, resultOfLongData);
			Assert::AreEqual(res3, resultOfLongData);
		}
	};
}
