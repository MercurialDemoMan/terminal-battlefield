
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#include "output.h"
#include "types.h"

/***********************************/
/***********************************/
/*      ENGINE IMPLEMENTATION      */
/***********************************/
/***********************************/

void ally_virus(s32 x, s32 y, s8* next_x, s8* next_y);
void ally_init();

void enemy_virus(s32 x, s32 y, s8* next_x, s8* next_y);
void enemy_init();



//check if coordinates are in diamond
bool in_field(s32 x, s32 y)
{
    if(x > y + FIELD_WIDTH / 2 ||
       x + FIELD_WIDTH / 2 < y ||
       FIELD_WIDTH / 2 - x > y ||
       FIELD_WIDTH - 1 + FIELD_WIDTH / 2 - x < y)
    {
        return false;
    }
    return true;
}



//insert node into battlefield
//returns 1 on successful placement
bool place_node(node_type type, node_color color, s32 x, s32 y, node_options options)
{
    //if trying to place outside of the playing field
    if(!in_field(x, y))
    {
        return false;
    }
    else
    {
        //if trying to place over other placed node
        if(field[y][x].type != NODE_TYPE_GROUND)
        {
            return false;
        }
        
        if(y == FIELD_HEIGHT / 2 && type != NODE_TYPE_BULLET)
        {
            return false;
        }
        
        //place node onto the field
        field[y][x].type     = type;
        field[y][x].color    = color;
        field[y][x].options  = options;
        field[y][x].alliance = y < FIELD_HEIGHT / 2 ? NODE_ALLIANCE_TOP : NODE_ALLIANCE_BOTTOM;
        field[y][x].timer    = rand() % 8;
        field[y][x].updated  = false;
        
        return true;
    }
}

//print single node
void print_node(node* n)
{
    switch(n->color)
    {
        case NODE_COLOR_WHITE: printf("%s", FNT_WHITE); break;
        case NODE_COLOR_BLACK: printf("%s", FNT_BLACK); break;
        case NODE_COLOR_RED:   printf("%s", FNT_RED);   break;
        case NODE_COLOR_GREEN: printf("%s", FNT_GREEN); break;
        case NODE_COLOR_BLUE:  printf("%s", FNT_CYAN);  break;
        default: break;
    }
    
    switch(n->type)
    {
        case NODE_TYPE_GROUND: printf("· "); break;
        case NODE_TYPE_WALL:   printf("# "); break;
        case NODE_TYPE_TURRET: printf("+ "); break;
        case NODE_TYPE_BULLET: printf("° "); break;
        case NODE_TYPE_VIRUS:
            if(n->alliance == NODE_ALLIANCE_TOP)    { printf("v "); }
            if(n->alliance == NODE_ALLIANCE_BOTTOM) { printf("ʌ "); }
            break;
        case NODE_TYPE_MINE:   printf("* "); break;
        default: break;
    }
}

//reset node
void reset_node(node* n)
{
    n->type     = NODE_TYPE_GROUND;
    n->color    = NODE_COLOR_RED;
    n->options  = NODE_OPTION_NULL;
    n->alliance = NODE_ALLIANCE_NONE;
    n->timer    = 0;
    n->options  = 0;
    n->updated  = false;
}



//print diamant battle field
void print_battlefield()
{
    //clear screen and reset font
    clear(); printf("%s", FNT_RESET);
    
    //print top score
    printf("Enemy Score: 0x%08x\n", score_top);
    
    //print corner indexes
    for(u32 i = 0; i < FIELD_WIDTH; i++)
    {
        if(i == 0 || i == FIELD_WIDTH - 1 || i == FIELD_WIDTH / 2)
        {
            printf("%i ", i);
        }
        else
        {
            printf("  ");
        }
    } printf("\n\n");
    
    //scan each node in 2d array
    for(s32 y = 0; y < FIELD_HEIGHT; y++)
    {
        for(s32 x = 0; x < FIELD_WIDTH; x++)
        {
            //check if node is in diamond
            if(!in_field(x, y))
            {
                printf("  ");
            }
            else
            {
                print_node(&field[y][x]);
            }
        }
        
        //print turns
        if(y == FIELD_HEIGHT / 2) { printf("  %sTurns left: 0x%04x", FNT_RESET, turns_left); }
        
        printf("\n");
    }
    printf("%s\n", FNT_RESET);
    
    //print corner indexes
    for(u32 i = 0; i < FIELD_WIDTH; i++)
    {
        if(i == 0 || i == FIELD_WIDTH - 1 || i == FIELD_WIDTH / 2)
        {
            printf("%i ", i);
        }
        else
        {
            printf("  ");
        }
    } printf("\n");
    
    printf("Ally Score: 0x%08x\n", score_bottom);
}

//returns true if bullet did not hit anything
bool process_bullet(s32 x, s32 y, s8 dx, s8 dy)
{
    //coolide
    return (field[y + dy][x + dx].type == NODE_TYPE_GROUND);
}

//node functions
bool process_field()
{
    u32 num_virus_nodes = 0;
    
    //update bullets
    for(s32 y = 0; y < FIELD_HEIGHT; y++)
    {
        for(s32 x = 0; x < FIELD_WIDTH; x++)
        {
            if(field[y][x].type == NODE_TYPE_BULLET && !field[y][x].updated)
            {
                switch(field[y][x].options)
                {
                    case NODE_OPTION_BULLET_UP:
                    {
                        if(!in_field(x, y - 1)) { reset_node(&field[y][x]); }
                        else
                        {
                            if(process_bullet(x, y, 0, -1))
                            {
                                //move bullet up
                                field[y - 1][x] = field[y][x];
                                field[y - 1][x].updated = true;
                                reset_node(&field[y][x]);
                            }
                            else
                            {
                                //collide
                                reset_node(&field[y][x]);
                                reset_node(&field[y - 1][x]);
                            }
                        }
                        break;
                    }
                    case NODE_OPTION_BULLET_DOWN:
                    {
                        if(!in_field(x, y + 1)) { reset_node(&field[y][x]); }
                        else
                        {
                            if(process_bullet(x, y, 0, 1))
                            {
                                //move bullet up
                                field[y + 1][x] = field[y][x];
                                field[y + 1][x].updated = true;
                                reset_node(&field[y][x]);
                            }
                            else
                            {
                                
                                //collide
                                reset_node(&field[y][x]);
                                reset_node(&field[y + 1][x]);
                            }
                        }
                        break;
                    }
                    case NODE_OPTION_BULLET_UPLEFT:
                    {
                        if(!in_field(x - 1, y - 1)) { reset_node(&field[y][x]); }
                        else
                        {
                            if(process_bullet(x, y, -1, -1))
                            {
                                //move bullet up
                                field[y - 1][x - 1] = field[y][x];
                                field[y - 1][x - 1].updated = true;
                                reset_node(&field[y][x]);
                            }
                            else
                            {
                                //collide
                                reset_node(&field[y][x]);
                                reset_node(&field[y - 1][x - 1]);
                            }
                        }
                        break;
                    }
                    case NODE_OPTION_BULLET_UPRIGHT:
                    {
                        if(!in_field(x + 1, y - 1)) { reset_node(&field[y][x]); }
                        else
                        {
                            if(process_bullet(x, y, 1, -1))
                            {
                                //move bullet up
                                field[y - 1][x + 1] = field[y][x];
                                field[y - 1][x + 1].updated = true;
                                reset_node(&field[y][x]);
                            }
                            else
                            {
                                //collide
                                reset_node(&field[y][x]);
                                reset_node(&field[y - 1][x + 1]);
                            }
                        }
                        break;
                    }
                    case NODE_OPTION_BULLET_DOWNLEFT:
                    {
                        if(!in_field(x - 1, y + 1)) { reset_node(&field[y][x]); }
                        else
                        {
                            if(process_bullet(x, y, -1, 1))
                            {
                                //move bullet up
                                field[y + 1][x - 1] = field[y][x];
                                field[y + 1][x - 1].updated = true;
                                reset_node(&field[y][x]);
                            }
                            else
                            {
                                //collide
                                reset_node(&field[y][x]);
                                reset_node(&field[y + 1][x - 1]);
                            }
                        }
                        break;
                    }
                    case NODE_OPTION_BULLET_DOWNRIGHT:
                    {
                        if(!in_field(x + 1, y + 1)) { reset_node(&field[y][x]); }
                        else
                        {
                            if(process_bullet(x, y, 1, 1))
                            {
                                //move bullet up
                                field[y + 1][x + 1] = field[y][x];
                                field[y + 1][x + 1].updated = true;
                                reset_node(&field[y][x]);
                            }
                            else
                            {
                                //collide
                                reset_node(&field[y][x]);
                                reset_node(&field[y + 1][x + 1]);
                            }
                        }
                        break;
                    }
                    default: break;
                }
            }
        }
    }
    
    //update information nodes and turrets
    for(s32 y = 0; y < FIELD_HEIGHT; y++)
    {
        for(s32 x = 0; x < FIELD_WIDTH; x++)
        {
            switch(field[y][x].type)
            {
                case NODE_TYPE_TURRET:
                {
                    //fire
                    if(field[y][x].timer == 0)
                    {
                        switch(field[y][x].options)
                        {
                            case NODE_OPTION_TURRET_FORWARD:
                            {
                                if(y < FIELD_HEIGHT / 2)
                                {
                                    place_node(NODE_TYPE_BULLET, NODE_COLOR_GREEN, x, y + 1, NODE_OPTION_BULLET_DOWN);
                                }
                                else
                                {
                                    place_node(NODE_TYPE_BULLET, NODE_COLOR_GREEN, x, y - 1, NODE_OPTION_BULLET_UP);
                                }
                                break;
                            }
                            case NODE_OPTION_TURRET_LEFT:
                            {
                                if(y < FIELD_HEIGHT / 2)
                                {
                                    place_node(NODE_TYPE_BULLET, NODE_COLOR_GREEN, x - 1, y + 1, NODE_OPTION_BULLET_DOWNLEFT);
                                }
                                else
                                {
                                    place_node(NODE_TYPE_BULLET, NODE_COLOR_GREEN, x - 1, y - 1, NODE_OPTION_BULLET_UPLEFT);
                                }
                                break;
                            }
                            case NODE_OPTION_TURRET_RIGHT:
                            {
                                if(y < FIELD_HEIGHT / 2)
                                {
                                    place_node(NODE_TYPE_BULLET, NODE_COLOR_GREEN, x + 1, y + 1, NODE_OPTION_BULLET_DOWNRIGHT);
                                }
                                else
                                {
                                    place_node(NODE_TYPE_BULLET, NODE_COLOR_GREEN, x + 1, y - 1, NODE_OPTION_BULLET_UPRIGHT);
                                }
                                break;
                            }
                            default: break;
                        }
                        
                        field[y][x].timer = 0x8;
                    }
                    //load
                    else
                    {
                        field[y][x].timer--;
                    }
                    break;
                }
                    
                case NODE_TYPE_VIRUS:
                {
                    if(!field[y][x].updated)
                    {
                        s8 dx = 0;
                        s8 dy = 0;
                        
                        if(field[y][x].alliance == NODE_ALLIANCE_TOP)
                        {
                            enemy_virus(x, y, &dx, &dy);
                        }
                        else if(field[y][x].alliance == NODE_ALLIANCE_BOTTOM)
                        {
                            ally_virus(x, y, &dx, &dy);
                        }

                        if(dx < -1) { dx = -1; }
                        if(dx >  1) { dx =  1; }
                        
                        if(dy < -1) { dy = -1; }
                        if(dy >  1) { dy =  1; }
                        
                        if(dx == 0 && dy == 0) { break; }
                        
                        if(!(in_field(x + dx, y + dy)))
                        {
                            if(field[y][x].alliance == NODE_ALLIANCE_TOP)
                            {
                                if(y + dy > FIELD_HEIGHT / 2) { score_top    += 1024; reset_node(&field[y][x]); }
                            }
                            else
                            {
                                if(y + dy < FIELD_HEIGHT / 2) { score_bottom += 1024; reset_node(&field[y][x]); }
                            }
                            break;
                        }
                        
                        //checking node move
                        if(field[y + dy][x + dx].type != NODE_TYPE_GROUND)
                        {
                            //kill information node
                            switch(field[y + dy][x + dx].type)
                            {
                                case NODE_TYPE_BULLET:
                                case NODE_TYPE_MINE:   reset_node(&field[y][x]); reset_node(&field[y + dy][x + dx]); break;
                                default: break;
                            }
                        }
                        else
                        {
                            //move information node
                            field[y + dy][x + dx] = field[y][x];
                            field[y + dy][x + dx].updated = true;
                            reset_node(&field[y][x]);
                        }
                    }
                    break;
                }
                    
                default: break;
            }
        }
    }
    
    //reset updated nodes
    for(s32 y = 0; y < FIELD_HEIGHT; y++)
    {
        for(s32 x = 0; x < FIELD_WIDTH; x++)
        {
            if(field[y][x].type == NODE_TYPE_VIRUS) { num_virus_nodes++; }
            field[y][x].updated = false;
        }
    }
    
    if(num_virus_nodes == 0) return false;
    
    return true;
}


/***********************************/
/***********************************/
/*     STRATEGY IMPLEMENTATION     */
/***********************************/
/***********************************/


#include "../strategy_ally.h"
#include "../strategy_enemy.h"


/***********************************/
/***********************************/
/*          MAIN FUNCTION          */
/***********************************/
/***********************************/


//main
int main(int argc, char* argv[])
{
    //init field
    for(u32 y = 0; y < FIELD_HEIGHT; y++)
    {
        for(u32 x = 0; x < FIELD_WIDTH; x++)
        {
            field[y][x].type     = NODE_TYPE_GROUND;
            field[y][x].color    = NODE_COLOR_RED;
            field[y][x].alliance = NODE_ALLIANCE_NONE;
            field[y][x].timer    = 0;
            field[y][x].options  = 0;
        }
    }
    
    score_top  = score_bottom = 0;
    turns_left = MAX_TURNS;
    
    ally_init();
    enemy_init();
    
    //battle loop
    while(process_field() && turns_left != 0)
    {
        print_battlefield();
        turns_left--;
        usleep(500000);
    }
    return 0;
}
