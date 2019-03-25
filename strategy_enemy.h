/***********************************/
/***********************************/
/*     STRATEGY IMPLEMENTATION     */
/***********************************/
/***********************************/

#ifndef array_size
#define array_size(x) (sizeof(x)/sizeof(x[0]))
#endif

//positions
static position_t enemy_viruses_positions[] =
{
    { 8,  4,  NODE_OPTION_NULL },
    { 8,  5,  NODE_OPTION_NULL },
    { 5,  4,  NODE_OPTION_NULL },
    { 11, 4,  NODE_OPTION_NULL },
};

static position_t enemy_walls_positions[] =
{
    { 8,  0,  NODE_OPTION_NULL },
    { 7,  1,  NODE_OPTION_NULL },
    { 8,  1,  NODE_OPTION_NULL },
    { 9,  1,  NODE_OPTION_NULL },
    { 6,  2,  NODE_OPTION_NULL },
    { 10, 2,  NODE_OPTION_NULL },
    { 5,  3,  NODE_OPTION_NULL },
    { 11, 3,  NODE_OPTION_NULL },
};

static position_t enemy_mines_positions[] =
{
    
};

static position_t enemy_turrets_positions[] =
{
    { 7,  2,  NODE_OPTION_TURRET_FORWARD },
    { 9,  2,  NODE_OPTION_TURRET_FORWARD },
};

#define PLACE_ARRAY(type, color, array)                           \
for(u32 i = 0; i < array_size(array); i++)                        \
{                                                                 \
place_node(type, color, array[i].x, array[i].y, array[i].option); \
}


//update virus position
void enemy_virus(s32 x, s32 y, s8* next_x, s8* next_y)
{
    *next_y = 1;
    *next_x = x % 2 ? 1 : -1;
}

//init algorithm
void enemy_init()
{
    PLACE_ARRAY(NODE_TYPE_VIRUS, NODE_COLOR_RED,  enemy_viruses_positions);
    PLACE_ARRAY(NODE_TYPE_WALL,  NODE_COLOR_RED,  enemy_walls_positions);
    PLACE_ARRAY(NODE_TYPE_MINE,  NODE_COLOR_RED,  enemy_mines_positions);
    PLACE_ARRAY(NODE_TYPE_TURRET, NODE_COLOR_RED, enemy_turrets_positions);
}






