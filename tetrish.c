#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

struct Board
{
    uint8_t size;
    uint8_t rows[20];
};

struct Input
{
    uint8_t direction : 2;
};

struct Piece
{
    uint8_t rotation : 2;
};

struct Game
{
    struct Board *board;
    struct Input *input;
    struct Piece *piece;
};

static void
handle_render(struct Game *game)
{
    while (true)
    {
        system("clear");
        for (int8_t y = 0; y < game->board->size >> 4; y++)
        {
            printf("<!");
            for (int8_t x = game->board->size & 0x0F; x > 0; x--)
            {
                if (game->board->rows[y] >> (x - 1) & 0x01)
                {
                    printf("▮");
                }
                else
                {
                    printf(".");
                }
            }

            printf("!>");

            printf("\n");
        }
        sleep(1);
    }
}

static void handle_input(struct Game *game)
{
    int c;
    while ((c = getchar()) != EOF)
    {
        switch (c)
        {
        case 'a':
            game->input->direction = 0b10;
            break;

        case 'b':
            game->input->direction = 0b01;
            break;
        }
    }
}

static void handle_piece(struct Game *game)
{
    while (true)
    {

        sleep(1);
    }
}

void main(int argc, char **argv)
{
    struct Board board = {
        .size = 0b10001000,
        .rows = {0},
    };

    struct Input input = {
        .direction = 0,
    };

    struct Piece piece = {
        .rotation = 0,
    };

    struct Game game = {
        .board = &board,
        .input = &input,
        .piece = &piece,
    };

    pthread_t render_thread;
    pthread_create(&render_thread, NULL, (void *(*)(void *))handle_render, &game);

    pthread_t input_thread;
    pthread_create(&input_thread, NULL, (void *(*)(void *))handle_input, &game);

    pthread_t piece_thread;
    pthread_create(&piece_thread, NULL, (void *(*)(void *))handle_piece, &game);

    pthread_join(input_thread, NULL);
    pthread_detach(render_thread);
    pthread_detach(piece_thread);

    return EXIT_SUCCESS;
}