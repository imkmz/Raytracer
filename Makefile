##
## EPITECH PROJECT, 2025
## raytracer_b
## File description:
## Makefile
##

NAME		=	raytracer
CXX			=	g++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++20

UNAME := $(shell uname)
ifeq ($(UNAME), Darwin)
    SDL_CFLAGS := -I/opt/homebrew/include/SDL2 -D_THREAD_SAFE
    SDL_LDFLAGS := -L/opt/homebrew/lib -lSDL2
else
    SDL_CFLAGS := $(shell sdl2-config --cflags)
    SDL_LDFLAGS := $(shell sdl2-config --libs)
endif

CXXFLAGS	+=	$(SDL_CFLAGS)
LDFLAGS		=	$(SDL_LDFLAGS)

SRC_DIR		=	src
INC_DIR		=	include
OBJ_DIR		=	obj

SRC_DIRS	=	$(SRC_DIR) \
				$(SRC_DIR)/math \
				$(SRC_DIR)/utils \
				$(SRC_DIR)/raytracer \
				$(SRC_DIR)/raytracer/realtime

SRC			=	$(foreach dir,$(SRC_DIRS),$(wildcard $(dir)/*.cpp))

OBJ			=	$(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)

fclean: clean
	rm -rf $(NAME)

re: fclean all

.PHONY: all clean fclean re
