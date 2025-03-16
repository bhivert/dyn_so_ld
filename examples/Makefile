.PHONY:
.SUFFIXES:

OUT_DIR	=	.
NAME	=	$(OUT_DIR)/TEST_so_loader

CXX		=	clang++

INCS	=	-I../incs

SRCS_DIR=	.
SRCS	=	$(SRCS_DIR)/main.cpp

OBJS	=	$(SRCS:.cpp=.o)

LXXFLAGS=	-g -ldl -pthread

CXXFLAGS+=	-std=c++11 -Wextra -Wall

all		:	$(NAME)

$(NAME)	:	_make $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) $(LXXFLAGS) -o $@

%.o		:	%.cpp
	$(CXX) -c $(CXXFLAGS) $< $(INCS) -o $@

clean	:	_make_clean
	rm -rf $(OBJS)

fclean	:	_make_fclean clean
	rm -rf $(NAME)

re		:	fclean all

_make:
	make -C fifi
	make -C riri

_make_clean:
	make -C fifi clean
	make -C riri clean

_make_fclean:
	make -C fifi fclean
	make -C riri fclean
