#include <stdint.h>
#include <stdbool.h>
#include <SDL2/SDL.h>
#include "mylib.h"
#include <string.h>
#include <assert.h>

typedef struct
{
    SDL_Window *window;
    SDL_Renderer *renderer;
    SDL_Texture *texture;
    uint16_t tft_fb0[ HEIGHT ][ WIDTH ];
    uint16_t tft_fb[ HEIGHT ][ WIDTH ];
    uint16_t tft_fb1[ HEIGHT ][ WIDTH ];

} monitor_t;

uint16_t palette[ 256 ];
uint16_t *PALETTE = &palette[ 0 ];
uint16_t *videoBuffer;
uint32_t buttons = ~( 0 );
uint16_t __key_curr, __key_prev;

static monitor_t monitor = { 0 };
monitor_t *m = &monitor;

int quit_filter(void *userdata, SDL_Event *event)
{
    (void) userdata;

    if( SDL_WINDOWEVENT == event->type )
    {
        if( SDL_WINDOWEVENT_CLOSE == event->window.event )
        {
            exit( 0 );
        }
    }
    else if( SDL_QUIT == event->type )
    {
        exit( 0 );
    }

    return 1;
}

volatile uint32_t* frame_buffer_init(void)
{
    buttons = ~( 0 );
    /* Initialise the SDL */
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialise! SDL_Error: %s\n", SDL_GetError() );
        exit( -1 );
    }

    SDL_SetEventFilter( quit_filter, NULL );

    m->window = SDL_CreateWindow( "mine sweeper", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                  WIDTH * 3, HEIGHT * 3, 0 );
    assert( m->window );

    m->renderer = SDL_CreateRenderer( m->window, -1, SDL_RENDERER_SOFTWARE );
    assert( m->renderer );

    m->texture = SDL_CreateTexture( m->renderer, SDL_PIXELFORMAT_BGR555, SDL_TEXTUREACCESS_STATIC,
                                    WIDTH, HEIGHT );
    assert( m->texture );

    SDL_SetTextureBlendMode( m->texture, SDL_BLENDMODE_BLEND );

    videoBuffer = &m->tft_fb[0][0];

    return (void*) m->tft_fb;
}

int frame_buffer_switch(int offset)
{
    (void) offset;

    int rslt = SDL_UpdateTexture( m->texture, NULL, m->tft_fb, WIDTH * sizeof( uint16_t ));
    assert( 0 == rslt );
    rslt = SDL_RenderClear( m->renderer );
    assert( 0 == rslt );

    /* Update the renderer with the texture containing the rendered image */
    rslt = SDL_RenderCopy( m->renderer, m->texture, NULL, NULL );
    assert( 0 == rslt );

    SDL_RenderPresent( m->renderer );

    return 0;
}

void drawRect(int row, int col, int height, int width, uint16_t color)
{
    int endw = row + width;
    int endh = col + height;
    if( endw > WIDTH )
    {
        endw = WIDTH;
    }
    if( endh > HEIGHT )
    {
        endh = HEIGHT;
    }

    for( int h = col; h < endh; h++ )
    {
        for( int w = row; w < endw; w++ )
        {
            m->tft_fb[ h ][ w ] = color;
        }
    }
}
void drawRect4(int row, int col, int height, int width, uint8_t index)
{
    drawRect( row, col, height, width, palette[ index ]);
}

void background_color(int height, int width, uint16_t color)
{
    for( int h = 0; h < height; h++ )
    {
        for( int w = 0; w < width; w++ )
        {
            m->tft_fb[ h ][ w ] = color;
        }
    }
}

void drawFullscreenImage(const uint16_t *image)
{
    memcpy( m->tft_fb, image, sizeof( m->tft_fb ));
}

void fillinScreen(volatile uint16_t color)
{
    background_color( HEIGHT, WIDTH, color );
}

void fillScreen4(uint8_t color)
{
    background_color( HEIGHT, WIDTH, palette[ color ]);
}

void drawImage4(int row, int col, int width, int height, const uint16_t *image)
{
    const uint8_t *img = (void*)image;
    for( int c = 0; c < height; c++ )
    {
        for( int r = 0; r < width; r++ )
        {
            m->tft_fb[ col + c ][ row + r ] = palette[ *img ];
            img++;
        }
    }
}

void setPixel4(int row, int col, uint8_t index)
{
    if( palette[ index ])
    {
        m->tft_fb[ col ][ row ] = palette[ index ];
    }
}

void drawBackground(const uint16_t *image)
{
    drawImage4( 0, 0, WIDTH, HEIGHT, image );
}

uint32_t poll_controller(uint32_t delay)
{
    SDL_Event event;
    uint32_t mask = 0;
    int keyPressed = 0;

    while( SDL_PollEvent( &event ) )
    {
        switch( event.key.keysym.sym )
        {
        case SDLK_a :
            mask |= BUTTON_A;
            break;

        case SDLK_b :
            mask |= BUTTON_B;
            break;

        case SDLK_q :
            mask |= BUTTON_L;
            break;

        case SDLK_w :
            mask |= BUTTON_R;
            break;

        case SDLK_SPACE :
            mask |= BUTTON_SELECT;
            break;

        case SDLK_RETURN2 :
        case SDLK_RETURN :
            mask |= BUTTON_START;
            break;

        case SDLK_KP_0 :
        case SDLK_0 :
            break;

        case SDLK_RIGHT:
        case SDLK_KP_PLUS:
            mask |= BUTTON_RIGHT;
            break;

        case SDLK_LEFT:
        case SDLK_KP_MINUS:
            mask |= BUTTON_LEFT;
            break;

        case SDLK_UP:
            mask |= BUTTON_UP;
            break;

        case SDLK_DOWN:
            mask |= BUTTON_DOWN;
            break;

        case SDLK_ESCAPE:
            exit( 1 );
            break;
        }

        switch( event.type )
        {
        case SDL_KEYDOWN:
            keyPressed = 1;
            break;

        case SDL_KEYUP:
            keyPressed = 0;
            break;

        default:
            break;

        }
    }

    if( keyPressed )
    {
        buttons &= ~( mask );
    }
    else
    {
        buttons |= mask;
    }

    SDL_Delay( delay ); /* Sleep for 5 millisecond */

    return buttons;
}

uint32_t button_get( void )
{
    return buttons;
}

void waitForVblank(void)
{
    poll_controller(16);
    frame_buffer_switch(0);
}

void delay(int n)
{
    SDL_Delay( n );
}

void flipPage()
{
    waitForVblank();
}
