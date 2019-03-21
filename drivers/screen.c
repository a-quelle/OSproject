#include "screen.h"
#include "../cpu/ports.h"
#include "../libc/mem.h"

static int get_cursor_offset();
static void set_cursor_offset(int offset);
static int print_char(char c, int col, int row, char attr);
static int get_offset(int col, int row);
static int get_offset_row(int offset);
static int get_offset_col(int offset);

void clear_screen(){
    int screenSize = MAX_COLS*MAX_ROWS;
    int i;
    char* screen = (char*) VIDEO_ADDRESS;

    for(i = 0; i < screenSize; ++i){
        screen[2*i] = ' ';
        screen[2*i+1] = 0;
    }
    set_cursor_offset(get_cursor_offset(0,0));
}

void kprint_at(char* message, int col, int row){
    int offset;
    if(col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else{
        offset = get_cursor_offset();
        row = get_offset_row(offset);
        col = get_offset_col(offset);    
    }

    while((*message) != 0){
        offset = print_char(*message, col, row, WHITE_ON_BLACK);
        row = get_offset_row(offset);
        col = get_offset_col(offset);
        ++message;
    }
}

void kprint(char* message){
    kprint_at(message, -1, -1);
}

void kprint_backspace(){
    int offset = get_cursor_offset();
    int row = get_offset_row(offset);
    int col = get_offset_col(offset);
    if(col > 2){
        --col;
        offset -= 2;
        print_char(0x00, col, row, 0x00);
        set_cursor_offset(offset);
    }
}

/*************************
 * Private functions
 * **********************/

static int print_char(char c, int col, int row, char attr){
    uint8_t* vidMem = (uint8_t*) VIDEO_ADDRESS;
    if(!attr) 
        attr= WHITE_ON_BLACK;

    if(col >= MAX_COLS || row >= MAX_ROWS){
        vidMem[2*MAX_COLS*MAX_ROWS-2] = 'E';
        vidMem[2*MAX_COLS*MAX_ROWS-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    uint32_t offset;
    if(col >= 0 && row >= 0)
        offset = get_offset(col, row);
    else
        offset = get_cursor_offset();
    
    if(c == '\n'){
        row = get_offset_row(offset);
        offset = get_offset(0, row+1);
    } else{
        vidMem[offset] = c;
        vidMem[offset + 1] = attr;
        offset +=2;
    }

    if(offset >= MAX_ROWS * MAX_COLS * 2){
        int i;
        for(i = 1; i < MAX_ROWS; ++i)
            memory_copy((uint8_t*)get_offset(0,i) + VIDEO_ADDRESS,
                (uint8_t*)get_offset(0, i-1) + VIDEO_ADDRESS,
                MAX_COLS * 2);        
            
        uint8_t* last_line = (uint8_t*)get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS;
        for(i = 0; i < MAX_COLS * 2; ++i)
            last_line[i] = 0;
        
        offset -= MAX_COLS * 2;
    }

    set_cursor_offset(offset);
    return offset;
}

static int get_cursor_offset(){
    outb(REG_SCREEN_CTRL, 14);
    int position = inb(REG_SCREEN_DATA) << 8;
    outb(REG_SCREEN_CTRL, 15);
    position += inb(REG_SCREEN_DATA);
    return position * 2; //offset = position*cellSize
}

static void set_cursor_offset(int offset){
    offset /= 2;
    outb(REG_SCREEN_CTRL, 14);
    outb(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    outb(REG_SCREEN_CTRL, 15);
    outb(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

static int get_offset(int col, int row) { 
    return 2 * (row * MAX_COLS + col); 
}
static int get_offset_row(int offset) { 
    return offset / (2 * MAX_COLS); 
}
static int get_offset_col(int offset) { 
    return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; 
}