.PHONY:
.SUFFIXES:

OUT_DIR	=	..
NAME	=	$(OUT_DIR)/fifi.so

INCS	=	-I..

SRCS_DIR=	.
SRCS	=	$(SRCS_DIR)/riri.cpp

OBJS	=	$(SRCS:.cpp=.o)

LXXFLAGS =

CXXFLAGS+=	-g -std=c++11 -Wextra -Wall

all		:	$(NAME)

$(NAME)	:	$(OBJS)
	$(CXX) -shared $(CXXFLAGS) $(LXXFLAGS) $(OBJS) -o $@

%.o		:	%.cpp
	$(CXX) -c -fPIC $(CXXFLAGS) $< $(INCS) -o $@

clean	:
	rm -rf $(OBJS)

fclean	:	clean
	rm -rf $(NAME)

re		:	fclean all
