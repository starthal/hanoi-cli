/* Towers of Hanoi CLI demo */
/* Author: Stephen Albert */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <error.h>
#include <unistd.h>
#include <string.h>
#include <stdbool.h>

/* Terminal escape sequences */
#define TERM_CSI "\33["
#define TERM_RETURN_TO_ORIGIN TERM_CSI "1;1f"
#define TERM_CLEAR_SCREEN TERM_CSI "2J"

/* The delay between displaying the initial state and starting 
 * the demo */
#define INITIAL_DELAY_MS 1000

void print_state(bool pretty);

/* Each tower is an int array, where the zeroth element is the lowest position
 * on the tower. A zero represents no disk in a given position and a positive 
 * number represents a disk of that size. 
 */
int *towers[3];

/* The number of disks, and therefore also the height of each tower */
int tower_size;

/* The number of moves that have been executed so far */
int nmoves;

/* The delay between moves when we are displaying the towers */
int move_delay_ms;

/* Whether we should pretty-print the state */
bool pretty_print;

int term_columns;

/* Print a compact representation */
void print_state_compact()
{
    int i, j;
    
    for (i = 0; i < 3; i++)
    {
        printf("[Tower %c]: ", '0' + i);
        for (j = 0; j < tower_size; j++)
        {
            if (towers[i][j] == 0)
                printf("   ");
            else
                printf("%2d ", towers[i][j]);
        }
        printf("\n");
    }
    
    printf(" - - - - - - - - - - \n");

}

/* Print an ASCII art representation */
void print_state_pretty()
{
    int i, j;

    int tower_w =  1 + tower_size * 4;
    int canvas_h = tower_size + 2;
    int canvas_w = (tower_w + 2) * 3;
    int towers_x[3]; 
    int x, y;

    if (canvas_w > term_columns)
    {
        error(EXIT_FAILURE, 0, "Terminal width too small");
    }

    for (i = 0; i < 3; i++)
        towers_x[i] = (tower_w + 2) * i + (tower_w / 2);

    for (y = 0; y < canvas_h; y++)
    {
        for (x = 0; x < towers_x[2] + tower_w/2 + 1; x++)
        {
            int done = 0;
            for (i = 0; i < 3; i++)
            {
                j = tower_size - y;

                /* If part of a tower */
                if (x >= towers_x[i] - tower_w / 2 && x <= towers_x[i] + tower_w / 2)
                {
                    if (y == canvas_h - 1)
                    {
                        printf("=");
                        done = 1;
                    }
                    else if (towers[i][j] > 0 || (j > 0 && towers[i][j - 1] > 0))
                    {
                        int disk_radius = towers[i][j] * 2;
                        int disk_start_x = towers_x[i] - disk_radius; 
                        int disk_end_x = towers_x[i] + disk_radius; 

                        int below_disk_radius = 0;
                        if (j > 0)
                            below_disk_radius = towers[i][j - 1] * 2;
                        
                        int below_disk_start_x = towers_x[i] - below_disk_radius;
                        int below_disk_end_x = towers_x[i] + below_disk_radius;

                        if (x == disk_start_x || x == disk_end_x) 
                        {
                            printf("|");
                            done = 1;
                        }
                        else if (x > disk_start_x && x < disk_end_x)
                        {
                            printf("_");
                            done = 1;
                        }
                        else if (x > below_disk_start_x && x < below_disk_end_x)
                        {
                            printf("_");
                            done = 1;
                        }
                    }
                    else if (x == towers_x[i])
                    {
                        printf("|");
                        done = 1;
                    }
                }
            }

            if (!done)
                printf(" ");
        }
        printf("\n");
    }
}

void print_state(bool pretty)
{
    if (pretty)
        print_state_pretty();
    else
        print_state_compact();
}

/* Move n disks from one tower to another */
void move_disks(int ndisks, int src, int dest, int depth)
{
    //printf("Move %d disks from tower %d to tower %d\n", ndisks, src, dest);
    if (ndisks == 1)
    {
        /* Move a single disk */
        int src_pos = 0;
        int dest_pos = 0;

        /* Find the top disk */
        while (towers[src][src_pos + 1] != 0) src_pos++;  
        while (towers[dest][dest_pos] != 0) dest_pos++;

        towers[dest][dest_pos] = towers[src][src_pos];
        towers[src][src_pos] = 0;

        nmoves++;

        /* Display the current state */
        printf(TERM_RETURN_TO_ORIGIN);
        printf("After %d moves: \n", nmoves);
        print_state(pretty_print);
        usleep(move_delay_ms * 1000);

        /* Check legality of move */
        if (dest_pos > 0 && towers[dest][dest_pos] >= towers[dest][dest_pos - 1])
        {
            error(EXIT_FAILURE, 0, "Illegal move occurred.");
        }
    }
    else
    {
        int other;

        /* A small LUT tells us which position is the "other"
         * given the source and dest positions */
        static const int other_lut[3][3] = 
            {  /* 0   1   2 */
                {-1,  2,  1}, /* 0 */
                { 2, -1,  0}, /* 1 */
                { 1,  0, -1}, /* 2 */
            };

        other = other_lut[src][dest];

        assert(other != -1);
    
        move_disks(ndisks - 1, src, other, depth + 1);
        move_disks(1, src, dest, depth + 1);
        move_disks(ndisks - 1, other, dest, depth + 1);
    }
}

int main(int argc, char** argv)
{
    int i, j;

    pretty_print = false;
    move_delay_ms = 1000;
    tower_size = 0;

    /* Parse options */ 
    int arg_pos = 0;
    for (i = 1; i < argc; i++)
    {
        if (argv[i][0] == '-' && strlen(argv[i]) > 1) 
        {
            /* Switch arguments */
            switch (argv[i][1])
            {
                case 'p': 
                    pretty_print = true; 
                    break;
                default: 
                    error(EXIT_FAILURE, 0, "Bad argument \"%s\"", argv[i]);
            }
        }
        else
        {
            /* Positional arguments */
            switch (arg_pos)
            {
                case 0:
                    tower_size = strtol(argv[i], NULL, 0);
                    break;
                case 1:
                    move_delay_ms = strtol(argv[i], NULL, 0);
                    break;
                default:
                    error(EXIT_FAILURE, 0, "Bad argument \"%s\"", argv[i]);
            }
            arg_pos++;
        }
    }

    if (tower_size == 0 || move_delay_ms <= 0)
    {
        printf("Towers of Hanoi simulation\n");
        printf("Usage: %s [-p] ndisks [delay_ms]\n", argv[0]);
        printf("    ndisks: number of disks in the tower\n");
        printf("    delay_ms: delay between moves (ms)\n");
        printf("    -p: pretty-print output\n");
        return -1;
    }
    
    char* columns_str = getenv("COLUMNS");
    if (columns_str != NULL)
    {
        term_columns = atoi(columns_str);
    }
    if (term_columns == 0)
        term_columns = 512;

    /* Initialize structures */
    for (i = 0; i < 3; i++)
    {
        towers[i] = (int*) malloc(sizeof(int) * (tower_size + 1));
        for (j = 0; j < tower_size + 1; j++)
        {
            if (i == 0)
            {
                towers[i][j] = tower_size - j;
            }
            else
            {
                towers[i][j] = 0;
            }
        }
    }

#ifndef NO_PRINT
    printf(TERM_CLEAR_SCREEN TERM_RETURN_TO_ORIGIN);
    printf("Initial state: \n");
    print_state(pretty_print);

    usleep(INITIAL_DELAY_MS * 1000);
#endif

    move_disks(tower_size, 0, 2, 0);

    printf("Solution took %d moves\n", nmoves);

    return 0;    
}
    
