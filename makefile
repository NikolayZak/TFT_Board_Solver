CXX = g++
CXXFLAGS = -Wall -std=c++11

TARGET = "TFT Board Solver"
TEST_DRIVER = "test"
EMBEDED_FOLDER = embeded_text
TEST_FOLDER = test_files

OBJ_FILES = $(EMBEDED_FOLDER)/Prompts.o \
            Parser.o \
            Board.o \
            Solver.o \
            Multithreading.o \
            Hasher.o \
            Interface.o \
            main.o \
            $(EMBEDED_FOLDER)/TFT_Icon.o

TD_OBJ_FILES = $(EMBEDED_FOLDER)/Prompts.o \
               Parser.o \
               Board.o \
               Solver.o \
               Multithreading.o \
               Hasher.o \
               Interface.o \
               $(TEST_FOLDER)/test_driver.o \
               $(EMBEDED_FOLDER)/TFT_Icon.o

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) -o $(TARGET) $(OBJ_FILES)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EMBEDED_FOLDER)/TFT_Icon.o: $(EMBEDED_FOLDER)/TFT_Icon.rc
	windres -i $(EMBEDED_FOLDER)/TFT_Icon.rc -o $(EMBEDED_FOLDER)/TFT_Icon.o

run: $(TARGET)
	./$(TARGET)

test: $(TEST_DRIVER)

$(TEST_DRIVER): $(TD_OBJ_FILES)
	$(CXX) -o $(TEST_DRIVER) $(TD_OBJ_FILES)

test_driver.o: test_driver.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET).exe $(TEST_DRIVER).exe *.o
	rm -f $(EMBEDED_FOLDER)/*.o
	rm -f $(TEST_FOLDER)/*.o