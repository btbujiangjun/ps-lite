TEST_SRC = $(wildcard tests/test_*.cc)
TEST = $(patsubst tests/test_%.cc, tests/test_%, $(TEST_SRC))

# GTEST_LIB=$(GTEST_PATH)/lib/
# GTEST_INC=$(GTEST_PATH)/include/

tests/% : tests/%.cc build/libps.a
	$(CXX) -std=c++0x $(CFLAGS) -MM -MT tests/$* $< >tests/$*.d
	$(CXX) -std=c++0x $(CFLAGS) -o $@ $(filter %.cc %.a, $^) $(PS_LDFLAGS_SO) -pthread

-include tests/*.d
