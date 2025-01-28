#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <inttypes.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#define THE_T \
0b\
0100\
1110,   \
0b\
0000\
0000

#define THE_SQUARE \
0b\
1100\
1100,   \
0b\
0000\
0000

#define THE_STICK \
0b\
1000\
1000,   \
0b\
1000\
1000

#define THE_L_LEFT \
0b\
0100\
0100,   \
0b\
1100\
0000

#define THE_L_RIGHT \
0b\
1000\
1000,   \
0b\
1100\
0000

#define THE_DOG_LEFT \
0b\
1100\
0110,   \
0b\
0000\
0000

#define THE_DOG_RIGHT \
0b\
0110\
1100,   \
0b\
0000\
0000

#define ROWS 20
#define COLUMNS 16

struct Board
{
    uint16_t rows[ROWS];
};

struct Input
{
    uint8_t right;
    uint8_t left;
};

struct Piece
{
    uint8_t x;
    uint8_t y;
    uint8_t rotation;
    uint8_t data[2];
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
    system("clear");
    for (int8_t y = 0; y < ROWS; y++)
    {
        printf("<!");
        for (int8_t x = 0; x < COLUMNS; x++)
        {
            if (game->board->rows[y] >> (COLUMNS - x - 1) & 0x01)
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
}

static void handle_input(struct Game *game)
{
    int c;
    while ((c = getchar()) != EOF)
    {
        switch (c)
        {
            case 'a': { game->input->left--; break; }
            case 'd': { game->input->right++; break; }
        }
    }
}

static void handle_piece(struct Game *game)
{
    struct Input *input = game->input;
    struct Piece *piece = game->piece;

    clean: {
        bool isOdd = true;
        for (uint8_t y = piece->y; y < (piece->y + 4); y++)
        {
            uint8_t mask = piece->data[(y - piece->y) < 2 ? 0 : 1] >> (isOdd ? 4 : 0) & 0x0F;
            game->board->rows[y] &= ~(mask << (COLUMNS - 1 - piece->x));
            isOdd = !isOdd;
        }
    }

    piece->y++;
    piece->x += input->right + input->left;

    render: {
        bool isOdd = true;
        for (uint8_t y = piece->y; y < (piece->y + 4); y++)
        {
            uint8_t mask = piece->data[(y - piece->y) < 2 ? 0 : 1] >> (isOdd ? 4 : 0) & 0x0F;
            game->board->rows[y] |= (mask << (COLUMNS - 1 - piece->x));
            isOdd = !isOdd;
        }
    }

    game->input->right = 0;
    game->input->left = 0;
}

int main(int argc, char **argv)
{
    struct Board board = {
        .rows = {0},
    };

    board.rows[0] = 0b1110000000000111;
    board.rows[1] = 0b1100000000000011;
    board.rows[2] = 0b1000000000000001;
    board.rows[3] = 0b1000000000000001;

    struct Input input = {
        .right = 0,
        .left = 0,
    };

    struct Piece piece = {
        .x = 8,
        .y = 8,
        .rotation = 0,
        .data = { THE_DOG_RIGHT }
    };

    struct Game game = {
        .board = &board,
        .input = &input,
        .piece = &piece,
    };

    pthread_t input_thread;
    pthread_create(&input_thread, NULL, (void *(*)(void *))handle_input, &game);

    while (true)
    {
        handle_piece(&game);
        handle_render(&game);
        sleep(1);
    }

    pthread_join(input_thread, NULL);

    return EXIT_SUCCESS;
}