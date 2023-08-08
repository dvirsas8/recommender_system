CC=g++
CXXFLAGS= -Wall -Wvla -Wextra -Werror -g -std=c++14
LDFLAGS= -lm
HEADERS= Movie.h RecommenderSystem.h RSUser.h RecommenderSystemLoader.h RSUsersLoader.h
OBJS= Movie.o RSUser.o RecommenderSystem.o RecommenderSystemLoader.o RSUsersLoader.o

%.o : %.c


rs: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ main.cpp $^

presubmit: $(OBJS)
	$(CC) $(LDFLAGS) -o $@ presubmit.cpp $^

$(OBJS) : $(HEADERS)
$(OBJS_TEST) : $(HEADERS_TEST)

.PHONY: clean clean_test
clean:
	rm -rf *.o
	rm -rf rs