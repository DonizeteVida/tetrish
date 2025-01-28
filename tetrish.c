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

static void render(struct Board *board)
{
    while (true)
    {
        system("clear");
        for (int8_t y = 0; y < board->size >> 4; y++)
        {
            printf("<!");

            for (int8_t x = board->size & 0x0F; x > 0; x--)
            {
                if (board->rows[y] >> (x - 1) & 0x01)
                {
                    printf("▮");
                }
                else
                {
                    printf(".");
                }
            }

            printf("!>\n");
        }
        sleep(1);
    }
}

void main(int argc, char **argv)
{
    struct Board board = {
        .size = 0b10001000,
        .rows = {0},
    };

    board.rows[0] = 0b11111110;

    pthread_t render_thread;
    pthread_create(&render_thread, NULL, (void *(*)(void *))render, (void *)&board);
    pthread_join(render_thread, NULL);
}