#include "platform.h"

void init_platform(Platform& platform)
{
    platform.actual_width = 1920;
    platform.actual_height = 1080;
    platform.logical_width = 640;
    platform.logical_height = 360;
    if(!SDL_Init(SDL_INIT_EVERYTHING))
        std::cout << "Failed to initialize SDL!" << std::endl;
    if(!SDL_CreateWindow("Crumble King", 20, 20, platform.actual_width, platform.actual_height, SDL_WINDOW_SHOWN)) 
        std::cout << "Failed to create window!" << std::endl;
    if (!SDL_CreateRenderer(platform.window, -1, SDL_RENDERER_ACCELERATED))
        std::cout << "Failed to create renderer!" << std::endl;
    if (!IMG_Init(IMG_INIT_PNG))
        std::cout << "Failed to initialize SDL_image!" << std::endl;
    if (!TTF_Init() == -1)
        std::cout << "Failed to initialize SDL_ttf!" << std::endl;
    if (Mix_OpenAudio(44100, AUDIO_S16SYS, 1, 4096) != 0)
        std::cout << "Failed to initialize SDL_mixer!" << std::endl;

    platform.font = TTF_OpenFont("resources/fonts/default.ttf", 32);
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
    float pixel_scalar = platform.actual_height / platform.logical_height;
    for (PlatformSprite& sprite : platform.sprites) {
        SDL_Rect source;
        source.x = sprite.source.position.x;
        source.y = sprite.source.position.y;
        source.w = sprite.source.size.x;
        source.h = sprite.source.size.y;

        SDL_Rect destination;
        destination.x = (sprite.x + sprite.origin_x) * pixel_scalar;
        destination.y = (sprite.y + sprite.origin_y) * pixel_scalar;
        destination.w = sprite.source.size.x * pixel_scalar;
        destination.h = sprite.source.size.y * pixel_scalar;

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
        //PlaySound(platform.sound_assets[sound.handle]);
    }
    platform.sounds.clear();

    // INPUT (NEXT FRAME)
    query_button(platform.input.left);
    query_button(platform.input.right);
    query_button(platform.input.up);
    query_button(platform.input.down);
    query_button(platform.input.jump);
    query_button(platform.input.pause);

    /* Get window_should_close
    if(!platform.window_should_close) {
        platform.window_should_close = WindowShouldClose();
    }*/
}

void deinit_platform()
{
    //CloseWindow();
}

void query_button(PlatformButton& button)
{
    button.held = IsKeyDown(button.keycode);
    button.just_pressed = IsKeyPressed(button.keycode);
    button.released = IsKeyReleased(button.keycode);
}

const char* get_file_text(const char* fname)
{
    FILE* f;
    if ((f = fopen(fname, "r")) == NULL) {
        printf("Error opening file!");
        return "";
    }
    const char* txt;
    char ch;
    do {
        ch = fgetc(f);
        printf("%c", ch);

        // Checking if character is not EOF.
        // If it is EOF stop reading.
    } while (ch != EOF);
    fscanf(f, txt);
    return txt;
}

void write_file_text(const char* text, const char* fname) 
{
    FILE* f;
    f = fopen(fname, "w");
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
    Mix_Chunk sound_asset = LoadSound(fname);
    platform.sound_assets.emplace_back(sound_asset);
    return platform.sound_assets.size() - 1;
}

void put_sprite(Platform& platform, PlatformSprite sprite)
{
    platform.sprites.push_back(sprite);
}

void buffer_sound(Platform& platform, int handle, double volume)
{
    platform.sounds.push_back(PlatformSound(handle, volume));
}

void stop_sound(Platform& platform, int handle)
{
    StopSound(platform.sound_assets[handle]);
}
