#include "platform.h"

void init_platform(Platform& platform)
{
    platform.actual_width = 1280;
    platform.actual_height = 720;
    platform.logical_width = 640;
    platform.logical_height = 360;

    SDL_Init(SDL_INIT_EVERYTHING);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    Mix_OpenAudio(96000, AUDIO_S16SYS, 1, 4096);
    platform.window = SDL_CreateWindow("Crumble King", 20, 20, platform.actual_width, platform.actual_height, SDL_WINDOW_SHOWN);
    platform.renderer = SDL_CreateRenderer(platform.window, -1, SDL_RENDERER_ACCELERATED);
    platform.font = TTF_OpenFont("resources/fonts/Glory-Regular.ttf", 32);
    platform.ticks_count = 0;

    platform.buttons.push_back(&platform.input.left);
    platform.buttons.push_back(&platform.input.right);
    platform.buttons.push_back(&platform.input.up);
    platform.buttons.push_back(&platform.input.down);
    platform.buttons.push_back(&platform.input.jump);
    platform.buttons.push_back(&platform.input.pause);

    set_random_seed();
}

void update_platform(Platform& platform)
{
    // BACKGROUND
    SDL_SetRenderDrawColor(platform.renderer, 
        platform.background_color.r, 
        platform.background_color.g, 
        platform.background_color.b, 
        SDL_ALPHA_OPAQUE
    );
    SDL_RenderClear(platform.renderer);
    
    // SPRITES
    platform.pixel_scalar = platform.actual_height / platform.logical_height;
    for (PlatformSprite& sprite : platform.sprites) {
        SDL_Rect source;
        source.x = sprite.source.position.x;
        source.y = sprite.source.position.y;
        source.w = sprite.source.size.x;
        source.h = sprite.source.size.y;

        SDL_Rect destination;
        destination.x = (sprite.x - sprite.origin_x) * platform.pixel_scalar;
        destination.y = (sprite.y - sprite.origin_y) * platform.pixel_scalar;
        destination.w = sprite.source.size.x * platform.pixel_scalar;
        destination.h = sprite.source.size.y * platform.pixel_scalar;

        SDL_RendererFlip flip = SDL_FLIP_NONE;
        if (sprite.is_flipped) {
            flip = SDL_FLIP_HORIZONTAL;
        }

        SDL_RenderCopyEx(platform.renderer, 
            platform.texture_assets[sprite.atlas], 
            &source, 
            &destination,
            0, 
            NULL, 
            flip
        );
    }
    platform.sprites.clear();

    // TEXT
    for (PlatformText& text : platform.texts) {
        SDL_Color color = SDL_Color{ (uint8_t)(text.color.r * 255), (uint8_t)(text.color.g), (uint8_t)(text.color.b), SDL_ALPHA_OPAQUE };
        SDL_Surface* surface = TTF_RenderText_Solid(platform.font, text.text.c_str(), color);
        if (surface == NULL) {
            std::cout << "Unable to render text surface!" << std::endl;
            break;
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(platform.renderer, surface);
        if (texture == NULL) {
            std::cout << "Unable to create texture from rendered text!" << std::endl;
            break;
        }

        int w = surface->w;
        int h = surface->h;
        SDL_FreeSurface(surface);
        SDL_Rect destination = { text.x, text.y, w, h };
        SDL_RenderCopy(platform.renderer, texture, NULL, &destination);
    }
    platform.texts.clear();

    SDL_RenderPresent(platform.renderer);

    // SOUNDS
    for(PlatformSound& sound : platform.sounds) {
        Mix_PlayChannel(-1, platform.sound_assets[sound.handle], 0);
    }
    platform.sounds.clear();

    // INPUT (NEXT FRAME)
    for (PlatformButton* button : platform.buttons) {
        button->just_pressed = false;
    }

    SDL_Event e;
    while (SDL_PollEvent(&e)) {
        switch (e.type) {
        case SDL_QUIT:
            platform.window_should_close = true;
            break;
        case SDL_KEYDOWN:
            for(PlatformButton* btn : platform.buttons) {
                if(e.key.keysym.scancode == btn->keycode) {
                    if(!btn->held) 
                        btn->just_pressed = true;
                    btn->held = true;
                }
            }
            break;
        case SDL_KEYUP:
            for(PlatformButton* btn : platform.buttons) {
                if(e.key.keysym.scancode == btn->keycode) {
                    if(btn->held) 
                        btn->released = true;
                    btn->held = false;
                    btn->just_pressed = false;
                }
            }
            break;
        default:
            break;
        }
    }
}

const char* get_file_text(const char* fname)
{
    FILE* f;
    char* txt;
    long bytes;
    
    f = fopen(fname, "r");
    if(f == NULL)
        return "";
    
    // Get byte length of file
    fseek(f, 0L, SEEK_END);
    bytes = ftell(f);
    fseek(f, 0L, SEEK_SET);	

    // Allocate memory for string
    txt = (char*)calloc(bytes, sizeof(char));	
    if(txt == NULL)
        return "";

    fread(txt, sizeof(char), bytes, f);
    fclose(f);
    return txt;
}

void write_file_text(const char* fname, const char* text)
{
    FILE* f;
    f = fopen(fname, "w");
    if (f == NULL) {
        std::cout << "Cannot open file for writing!" << std::endl;
        return;
    }
    fprintf(f, text);
    fclose(f);
}

int new_texture_handle(Platform& platform, const char* fname)
{
    SDL_Texture* texture_asset = IMG_LoadTexture(platform.renderer, fname);
    platform.texture_assets.emplace_back(texture_asset);
    return platform.texture_assets.size() - 1;
}

int new_sound_handle(Platform& platform, const char* fname)
{
    Mix_Chunk* snd = Mix_LoadWAV(fname);
    if (snd == NULL)
    {
        printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        return 0;
    }
    platform.sound_assets.push_back(snd);
    return platform.sound_assets.size() - 1;
}

int new_music_handle(Platform& platform, const char* fname)
{
    Mix_Music* snd = Mix_LoadMUS(fname);
    if (snd == NULL)
    {
        printf("Failed to load music! SDL_mixer Error: %s\n", Mix_GetError());
        return 0;
    }
    platform.music_assets.push_back(snd);
    return platform.music_assets.size() - 1;
}

void put_sprite(Platform& platform, PlatformSprite sprite)
{
    platform.sprites.push_back(sprite);
}

void buffer_sound(Platform& platform, int handle, double volume)
{
    platform.sounds.push_back(PlatformSound(handle, volume));
}

void set_music(Platform & platform, int handle, double volume)
{
    Mix_PlayMusic(platform.music_assets[handle], -1);
}

void stop_music() {
    Mix_HaltMusic();
}

double get_delta_time(Platform& platform)
{
    double delta_time = (SDL_GetTicks() - platform.ticks_count) / 1000.0f;
    platform.ticks_count = SDL_GetTicks();
    if (delta_time > 0.05)
        delta_time = 0.05;
    return delta_time;
}

