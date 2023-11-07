CXX = g++
CXXFLAGS = -Wall -std=c++11
TARGET = "TFT Board Solver"
TEST_DRIVER = "test"
EMBEDED_FOLDER = Embeded_text

OBJ_FILES = $(EMBEDED_FOLDER)/Prompts.o \
			Parser.o \
			Board.o \
			Solver.o \
			Hasher.o \
			Interface.o \
            main.o \
            $(EMBEDED_FOLDER)/TFT_Icon.o

TD_OBJ_FILES = $(EMBEDED_FOLDER)/Prompts.o \
			   Parser.o \
			   Board.o \
			   Solver.o \
			   Hasher.o \
			   Interface.o \
			   test_driver.o \
			   $(EMBEDED_FOLDER)/TFT_Icon.o

all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(TARGET) $(OBJ_FILES)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EMBEDED_FOLDER)/TFT_Icon.o: $(EMBEDED_FOLDER)/TFT_Icon.rc
	windres -i $(EMBEDED_FOLDER)/TFT_Icon.rc -o $(EMBEDED_FOLDER)/TFT_Icon.o

run: $(TARGET)
	./$(TARGET)

test: $(TEST_DRIVER)

$(TEST_DRIVER): $(TD_OBJ_FILES)
	$(CXX) $(LDFLAGS) -o $(TEST_DRIVER) $(TD_OBJ_FILES)

test_driver.o: test_driver.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	del /Q $(TARGET).exe $(TEST_DRIVER).exe *.o
	del /Q $(EMBEDED_FOLDER)\*.o
