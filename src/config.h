#ifndef _config_h_
#define _config_h_

// comment this out when building the lib for the server
#include <SDL.h>

// app parameters
#define DEBUG 0
#define FULLSCREEN 0
#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
#define VSYNC 1
#define SCROLL_THRESHOLD 0.1
#define MAX_MESSAGES 4
#define DB_PATH "craft.db"
#define USE_CACHE 1
#define DAY_LENGTH 600
#define INVERT_MOUSE 0

// rendering options
#define SHOW_LIGHTS 1
#define SHOW_PLANTS 1
#define SHOW_CLOUDS 1
#define SHOW_TREES 1
#define SHOW_ITEM 1
#define SHOW_CROSSHAIRS 1
#define SHOW_WIREFRAME 1
#define SHOW_INFO_TEXT 1
#define SHOW_CHAT_TEXT 1
#define SHOW_PLAYER_NAMES 1

// key bindings
#define CRAFT_KEY_FORWARD 'W'
#define CRAFT_KEY_BACKWARD 'S'
#define CRAFT_KEY_LEFT 'A'
#define CRAFT_KEY_RIGHT 'D'
#define CRAFT_KEY_JUMP GLFW_KEY_SPACE
#define CRAFT_KEY_FLY GLFW_KEY_TAB
#define CRAFT_KEY_OBSERVE 'O'
#define CRAFT_KEY_OBSERVE_INSET 'P'
#define CRAFT_KEY_ITEM_NEXT 'E'
#define CRAFT_KEY_ITEM_PREV 'R'
#define CRAFT_KEY_ZOOM GLFW_KEY_LEFT_SHIFT
#define CRAFT_KEY_ORTHO 'F'
#define CRAFT_KEY_CHAT 't'
#define CRAFT_KEY_COMMAND '/'
#define CRAFT_KEY_SIGN '`'

// https://wiki.libsdl.org/SDL_Scancode?highlight=%28%5CbCategoryEnum%5Cb%29%7C%28CategoryKeyboard%29
#define KEY_FORWARD SDL_SCANCODE_W
#define KEY_BACKWARD SDL_SCANCODE_S
#define KEY_LEFT SDL_SCANCODE_A
#define KEY_RIGHT SDL_SCANCODE_D
#define KEY_JUMP SDL_SCANCODE_SPACE
#define KEY_FLY SDL_SCANCODE_TAB
#define KEY_OBSERVE SDL_SCANCODE_O
#define KEY_OBSERVE_INSET SDL_SCANCODE_P
#define KEY_ITEM_NEXT SDL_SCANCODE_E
#define KEY_ITEM_PREV SDL_SCANCODE_R
#define KEY_ZOOM SDL_SCANCODE_LSHIFT
#define KEY_ORTHO SDL_SCANCODE_F
#define KEY_CHAT SDL_SCANCODE_T
#define KEY_COMMAND SDL_SCANCODE_SLASH
#define KEY_SIGN SDL_SCANCODE_GRAVE

enum {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP,
	FLY,
	OBSERVE,
	OBSERVE_INSET,
	ITEM_NEXT,
	ITEM_PREV,
	ZOOM,
	ORTHO,
	CHAT,
	COMMAND,
	SIGN,
	NUM_CONTROLS
};

//int controls[NUM_CONTROLS];




// advanced parameters
#define CREATE_CHUNK_RADIUS 4
#define RENDER_CHUNK_RADIUS 4
#define RENDER_SIGN_RADIUS 2
#define DELETE_CHUNK_RADIUS 8
#define CHUNK_SIZE 32
#define COMMIT_INTERVAL 5

#endif
