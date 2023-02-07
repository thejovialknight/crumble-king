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

struct PlatformButton {
    int keycode;
    bool held = false;
    bool just_pressed = false;
    bool released = false;

    PlatformButton(int keycode) : keycode(keycode) {}
};

// TODO: Too specific for a platform layer. Figure something out.
struct PlatformInput {
    PlatformButton left = PlatformButton(SDL_SCANCODE_A);
    PlatformButton right = PlatformButton(SDL_SCANCODE_D);
    PlatformButton up = PlatformButton(SDL_SCANCODE_W);
    PlatformButton down = PlatformButton(SDL_SCANCODE_S);
    PlatformButton jump = PlatformButton(SDL_SCANCODE_SPACE);
    PlatformButton pause = PlatformButton(SDL_SCANCODE_ESCAPE);
};

struct PlatformSprite {
    int atlas;
    IRect source;
    double x;
    double y;
    double origin_x;
    double origin_y;
    bool is_flipped;

    PlatformSprite(int atlas, IRect source, int x, int y, int origin_x, int origin_y, bool is_flipped) : atlas(atlas), source(source), x(x), y(y), origin_x(origin_x), origin_y(origin_y), is_flipped(is_flipped) {}
};

struct PlatformSound {
    int handle;
    double volume = 1;

    PlatformSound(int handle, double volume) : handle(handle), volume(volume) {}
};

// TODO: Refactor for pixel art text with atlas
struct PlatformText {
    std::string text;
    double font_size;
    int x;
    int y;
    Vec3 color;

    PlatformText(std::string text, double font_size, int x, int y, Vec3 color) :
        text(text), font_size(font_size), x(x), y(y), color(color) {}
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
    std::vector<PlatformText> texts;
    std::vector<PlatformButton*> buttons;
    Vec3 background_color = Vec3(0, 0, 0);
};

// Platform management
void init_platform(Platform& platform);
void update_platform(Platform& platform);
void deinit_platform();

// Text loading
const char* get_file_text(const char* fname);
void write_file_text(const char* text, const char* fname);

// Sprite handling
int new_texture_handle(Platform& platform, const char* fname);
void put_sprite(Platform& platform, PlatformSprite sprite);

// Sound handling
int new_sound_handle(Platform& platform, const char* fname);
int new_music_handle(Platform& platform, const char* fname);
void buffer_sound(Platform& platform, int handle, double volume);
void set_music(Platform& platform, int handle, double volume);

// Time handling
double get_delta_time(Platform& platform);
