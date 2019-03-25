#ifndef TYPES_H
#define TYPES_H

typedef signed char        s8;
typedef signed short       s16;
typedef signed int         s32;
typedef signed long long   s64;
typedef unsigned char      u8;
typedef unsigned short     u16;
typedef unsigned int       u32;
typedef unsigned long long u64;

//node types
typedef enum
{
    NODE_TYPE_GROUND,
    NODE_TYPE_WALL,
    NODE_TYPE_TURRET,
    NODE_TYPE_BULLET,
    NODE_TYPE_VIRUS,
    NODE_TYPE_MINE
} node_type;

//node colors
typedef enum
{
    NODE_COLOR_WHITE,
    NODE_COLOR_BLACK,
    NODE_COLOR_RED,
    NODE_COLOR_GREEN,
    NODE_COLOR_BLUE
} node_color;

//node alliance
typedef enum
{
    NODE_ALLIANCE_NONE,
    NODE_ALLIANCE_TOP,
    NODE_ALLIANCE_BOTTOM
} node_alliance;

//node options
typedef enum
{
    NODE_OPTION_NULL,
    //turret options
    NODE_OPTION_TURRET_FORWARD,
    NODE_OPTION_TURRET_LEFT,
    NODE_OPTION_TURRET_RIGHT,
    //bullet options
    NODE_OPTION_BULLET_UP,
    NODE_OPTION_BULLET_DOWN,
    NODE_OPTION_BULLET_UPRIGHT,
    NODE_OPTION_BULLET_UPLEFT,
    NODE_OPTION_BULLET_DOWNRIGHT,
    NODE_OPTION_BULLET_DOWNLEFT,
} node_options;

//node
typedef struct
{
    node_type     type;
    node_color    color;
    node_alliance alliance;
    node_options  options;
    
    u8 timer;
    bool updated;
    
} node;

//2d vector
typedef struct
{
    s32 x, y;
    u8 option;
} position_t;

#define FIELD_WIDTH  17
#define FIELD_HEIGHT 17
#define MAX_TURNS    60

#define FIELD_COUNT  (FIELD_WIDTH * FIELD_HEIGHT)

//field
static node  field[FIELD_HEIGHT][FIELD_WIDTH];
static u32   score_top;
static u32   score_bottom;
static u32   turns_left;

#endif
