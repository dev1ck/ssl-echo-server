CXX = g++
# CXXFLAGS = -Wall -g
TARGET = server
SRCS = main.cpp ClientHandler.cpp Server.cpp SSLManager.cpp
OBJS = $(SRCS:.cpp=.o)
LDFLAGS = -lpthread -lssl -lcrypto

$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $<

clean:
	rm -f $(TARGET) $(OBJS)
