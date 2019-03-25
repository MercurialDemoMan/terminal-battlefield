/***********************************/
/***********************************/
/*     STRATEGY IMPLEMENTATION     */
/***********************************/
/***********************************/

#ifndef array_size
#define array_size(x) (sizeof(x)/sizeof(x[0]))
#endif

//positions
static position_t ally_viruses_positions[] =
{
    { 6, 10, NODE_OPTION_NULL },
    { 7, 11, NODE_OPTION_NULL }
};

static position_t ally_walls_positions[] =
{
    { 8,  16, NODE_OPTION_NULL },
    { 7,  15, NODE_OPTION_NULL },
    { 9,  15, NODE_OPTION_NULL },
    { 10, 14, NODE_OPTION_NULL },
    { 6,  14, NODE_OPTION_NULL }
};

static position_t ally_mines_positions[] =
{
    { }
};

static position_t ally_turrets_positions[] =
{
    //{ 4,  9, NODE_OPTION_TURRET_FORWARD },
    //{ 12, 9, NODE_OPTION_TURRET_FORWARD },
};



//update virus position
void ally_virus(s32 x, s32 y, s8* next_x, s8* next_y)
{
    *next_y = -1;
    *next_x = x % 2 ? 1 : -1;
}

//init algorithm
void ally_init()
{
    for(u32 i = 0; i < array_size(ally_viruses_positions); i++)
    {
        place_node(NODE_TYPE_VIRUS, NODE_COLOR_GREEN, ally_viruses_positions[i].x, ally_viruses_positions[i].y, ally_viruses_positions[i].option);
    }
    
    for(u32 i = 0; i < array_size(ally_walls_positions); i++)
    {
        place_node(NODE_TYPE_WALL, NODE_COLOR_GREEN, ally_walls_positions[i].x, ally_walls_positions[i].y, ally_walls_positions[i].option);
    }
    
    
    for(u32 i = 0; i < array_size(ally_turrets_positions); i++)
    {
        place_node(NODE_TYPE_TURRET, NODE_COLOR_GREEN, ally_turrets_positions[i].x, ally_turrets_positions[i].y, ally_turrets_positions[i].option);
    }
}






