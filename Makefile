# Colors
C_GREEN="\033[0;32m"
C_RED="\033[0;31m"
C_BLUE="\033[0;34m"
C_RESET="\033[0m"

# Standard compiler variables
CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -Ofast -std=c++98

RM = rm -rf

NAME = test
NAME_SPACE = NO_NS
BUILD = build

COMMON_SRCS = common/main common/iterator_test common/type_traits \
			  common/algorithm_test common/pair_test common/vector_test \
			  common/stack_test common/map_test common/set_test
COMMON_OBJS = $(COMMON_SRCS:=.o)
COMMON_HEADERS = common/common.hpp common/tests.hpp
INCLUDES = -Icommon -Iiterator -Itype_traits -Ialgorithm -Iutility -Ivector -Istack -Ired_black_tree -Ifunctional -Imap -Iset
HEADERS = iterator/iterator.hpp iterator/iterator_traits.hpp type_traits/type_traits.hpp \
		  algorithm/algorithm.hpp utility/utility.hpp vector/vector.hpp stack/stack.hpp \
		  functional/functional.hpp map/map.hpp set/set.hpp

# Rules
all: $(NAME)

$(NAME): $(HEADERS) $(COMMON_HEADERS) $(addprefix build/, $(COMMON_OBJS))
	@echo $(C_GREEN)linking $(C_RED)\($(addprefix build/, $(COMMON_OBJS))\) $(C_RESET)
	@$(CXX) $(CXXFLAGS) $(addprefix build/, $(COMMON_OBJS)) -o $(NAME)
	@echo $(C_GREEN)Done! $(C_RESET)

$(BUILD)/%.o:%.cpp $(COMMON_HEADERS) $(HEADERS)
	@echo $(C_BLUE)compiling $(C_RED)\($<\) $(C_BLUE)using namespace $(NAME_SPACE) $(C_RESET)
	@mkdir -p $(shell dirname $@)
	@$(CXX) $(CXXFLAGS) $(INCLUDES) -DUSING_$(NAME_SPACE) -c $< -o $@ 

clean:
	@echo $(C_RED)removing object files $(C_RESET)
	@$(RM) $(BUILD)

fclean: clean
	@echo $(C_RED)removing executable $(C_RESET)
	@$(RM) $(NAME)

re: fclean all
