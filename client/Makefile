EXECUTABLE = DogRun2

INCLUDES = -I./ -I../common/dep/protobuf-2.5.0/src -I../common

SOURCES = $(wildcard src/*.cpp)

COCOS_ROOT = ../../cocos2d-x-2.2
include $(COCOS_ROOT)/cocos2dx/proj.linux/cocos2dx.mk

SHAREDLIBS += -lcocos2d -lextension -lcurl
COCOS_LIBS = $(LIB_DIR)/libcocos2d.so
STATICLIBS += ../server/lib/libprotobuf.a

# add cc obj
SOURCES1 := $(wildcard src/*.cc) $(wildcard ../common/protocol/*.cc) $(wildcard protocol/*.cc)
OBJECTS1 := $(SOURCES1:.cc=.o)
OBJECTS1 := $(subst ../,,$(OBJECTS1))
OBJECTS1 := $(subst $(COCOS_ROOT)/,,$(OBJECTS1))
OBJECTS1 := $(addprefix $(OBJ_DIR)/, $(OBJECTS1))
OBJECTS += $(OBJECTS1) $(OBJ_DIR)/src/tcp.o

INCLUDES += -I$(COCOS_ROOT)/extensions \
	-I$(COCOS_ROOT)/external/chipmunk/include/chipmunk \

$(TARGET): $(OBJECTS) $(STATICLIBS) $(COCOS_LIBS) $(CORE_MAKEFILE_LIST)
	mkdir -p $(@D)
	$(LOG_LINK)$(CXX) $(CXXFLAGS) $(OBJECTS) -o $@ $(SHAREDLIBS) $(STATICLIBS)

$(OBJ_DIR)/%.o: %.cpp $(CORE_MAKEFILE_LIST)
	mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cpp $(CORE_MAKEFILE_LIST)
	mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: %.cc $(CORE_MAKEFILE_LIST)
	mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: ../%.cc $(CORE_MAKEFILE_LIST)
	mkdir -p $(@D)
	$(LOG_CXX)$(CXX) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@

$(OBJ_DIR)/%.o: %.c $(CORE_MAKEFILE_LIST)
	mkdir -p $(@D)
	$(LOG_CXX)$(CC) $(CXXFLAGS) $(INCLUDES) $(DEFINES) $(VISIBILITY) -c $< -o $@
