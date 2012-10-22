CODE_BASE =		C:/Users/dinkem1/Codebase/SO2RProject/
SRC_CODE_PATH =		$(CODE_BASE)SO2REmbedded/
TEST_CODE_PATH =	$(CODE_BASE)SO2RTests/


CXXPATH =  
INCLUDE_PATH =	-I$(TEST_CODE_PATH)incl/CppUTest \
                -I$(TEST_CODE_PATH)incl/CppUTestExt \
				-I$(SRC_CODE_PATH)incl \
				-I$(SRC_CODE_PATH)arduinoIncludes \
				-I$(TEST_CODE_PATH)mockIncludes \
                
				 

CC_COMPILER	= -O2 -Wall -fmessage-length=0 -D__AVR_ATmega2560__
CC_INCLUDE	= $(INCLUDE_PATH) 

CXXFLAGS = 	$(CC_COMPILER) $(CC_INCLUDE)

OBJS =		src/runAllTests.o \
			src/usartTests.o \
			src/otrspTests.o \
			src/so2rHardwareTests.o \
			src/mocks/mockHardware.o \
			src/mocks/mockSupport.o \
			$(SRC_CODE_PATH)src/usart/usart.o \
			$(SRC_CODE_PATH)src/so2rHardware.o \
			$(SRC_CODE_PATH)src/otrsp/otrsp.o \
			$(SRC_CODE_PATH)src/otrsp/otrspCommand.o \
			$(SRC_CODE_PATH)src/otrsp/otrspQuery.o \

LIBS = $(TEST_CODE_PATH)lib/CppUTest/libCppUTest.a \
       $(TEST_CODE_PATH)lib/CppUTest/libCppUTestExt.a


AUTO_IMPORT = -Xlinker --enable-auto-import

TARGET =		./bin/runAllTests.exe


$(TARGET):	$(OBJS)
	$(CXX)  $(AUTO_IMPORT) -o $(TARGET) $(OBJS)  $(LIBS) $(INCLUDE_PATH)

all:	$(TARGET)	

debug:	CXX += -DDEBUG -g
debug:	$(TARGET)	

clean:
	rm -f $(OBJS) $(TARGET)
	rm -f $(OBJS) $(DEBUGTARGET)

