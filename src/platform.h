#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <stdlib.h>
#include "vec3.h"
#include "settings.h"
#include "rect.h"
#include "random.h"
#include <unordered_map>

struct PlatformButton {
    int keycode;
    bool held = false;
    bool just_pressed = false;
    bool released = false;

    PlatformButton(int keycode) : keycode(keycode) {}
};

struct PlatformInput {
    PlatformButton left = PlatformButton(SDL_SCANCODE_A);
    PlatformButton right = PlatformButton(SDL_SCANCODE_D);
    PlatformButton up = PlatformButton(SDL_SCANCODE_W);
    PlatformButton down = PlatformButton(SDL_SCANCODE_S);
    PlatformButton jump = PlatformButton(SDL_SCANCODE_SPACE);
    PlatformButton pause = PlatformButton(SDL_SCANCODE_ESCAPE);
    PlatformButton debug_select = PlatformButton(SDL_SCANCODE_TAB);
};

struct PlatformSprite {
    int atlas;
    IRect source;
    double x;
    double y;
    double origin_x;
    double origin_y;
    bool is_flipped;
    Vec3 color;

    PlatformSprite(int atlas, IRect source, int x, int y, int origin_x, int origin_y, bool is_flipped, Vec3 color) : atlas(atlas), source(source), x(x), y(y), origin_x(origin_x), origin_y(origin_y), is_flipped(is_flipped), color(color) {}
};

struct PlatformSound {
    int handle;
    double volume = 1;

    PlatformSound(int handle, double volume) : handle(handle), volume(volume) {}
};

// TODO: Refactor for pixel art text with atlas
struct PlatformText {
    std::string text;
    int font;
    int x;
    int y;
    Vec3 color;

    PlatformText(std::string text, int font, int x, int y, Vec3 color) :
        text(text), font(font), x(x), y(y), color(color) {}
};

struct PlatformFont {
    int atlas;
    std::unordered_map<char, int> chars = {
        {'A', 0},
        {'B', 1},
        {'C', 2},
        {'D', 3},
        {'E', 4},
        {'F', 5},
        {'G', 6},
        {'H', 7},
        {'I', 8},
        {'J', 9},
        {'K', 10},
        {'L', 11},
        {'M', 12},
        {'N', 13},
        {'O', 14},
        {'P', 15},
        {'Q', 16},
        {'R', 17},
        {'S', 18},
        {'T', 19},
        {'U', 20},
        {'V', 21},
        {'W', 22},
        {'X', 23},
        {'Y', 24},
        {'Z', 25},
        {'0', 26},
        {'1', 27},
        {'2', 28},
        {'3', 29},
        {'4', 30},
        {'5', 31},
        {'6', 32},
        {'7', 33},
        {'8', 34},
        {'9', 35},
        {'-', 36},
        {'*', 37},
        {'!', 38},
        {'©', 39},
        {'.', 40},
        {':', 41},
        {' ', 42}
    };
    int width = 8;
    int height = 8;

    PlatformFont(int atlas) : atlas(atlas) {}
};

struct Platform {
    Uint32 ticks_count;
    int actual_width;
    int actual_height;
    int logical_width;
    int logical_height;
    double pixel_scalar;
    bool window_should_close = false;
    PlatformInput input;
    SDL_Window* window;
    SDL_Renderer* renderer;
    TTF_Font* font;
    std::vector<SDL_Texture*> texture_assets;
    std::vector<Mix_Chunk*> sound_assets;
    std::vector<Mix_Music*> music_assets;
    std::vector<PlatformSprite> sprites;
    std::vector<PlatformSound> sounds;
    std::vector<PlatformFont> fonts;
    std::vector<PlatformText> texts;
    std::vector<PlatformButton*> buttons;
    Vec3 background_color = Vec3(0, 0, 0);
};

// Platform management
void init_platform(Platform& platform);
void update_platform(Platform& platform);

// Text loading
const char* get_file_text(const char* fname);
void write_file_text(const char* text, const char* fname);

// Sprite handling
int new_texture_handle(Platform& platform, const char* fname);
void put_sprite(Platform& platform, PlatformSprite sprite);

// Font handling
int new_font_handle(Platform& platform, int texture);

// Sound handling
int new_sound_handle(Platform& platform, const char* fname);
int new_music_handle(Platform& platform, const char* fname);
void buffer_sound(Platform& platform, int handle, double volume);
void set_music(Platform& platform, int handle, double volume);
void stop_music();

// Time handling
double get_delta_time(Platform& platform);
