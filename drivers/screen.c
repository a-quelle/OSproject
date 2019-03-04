#include "screen.h"
#include "ports.h"
#include "../kernel/util.h"

int get_cursor_offset();
void set_cursor_offset(int offset);
int print_char(char c, int col, int row, char attr);
int get_offset(int col, int row);
int get_offset_row(int offset);
int get_offset_col(int offset);

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

int print_char(char c, int col, int row, char attr){
    unsigned char* vidMem = (unsigned char*) VIDEO_ADDRESS;
    if(!attr) 
        attr= WHITE_ON_BLACK;

    if(col >= MAX_COLS || row >= MAX_ROWS){
        vidMem[2*MAX_COLS*MAX_ROWS-2] = 'E';
        vidMem[2*MAX_COLS*MAX_ROWS-1] = RED_ON_WHITE;
        return get_offset(col, row);
    }

    int offset;
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
            memory_copy(get_offset(0,i) + VIDEO_ADDRESS,
                get_offset(0, i-1) + VIDEO_ADDRESS,
                MAX_COLS * 2);        
            
        char* last_line = get_offset(0, MAX_ROWS - 1) + VIDEO_ADDRESS;
        for(i = 0; i < MAX_COLS * 2; ++i)
            last_line[i] = 0;
        
        offset -= MAX_COLS * 2;
    }

    set_cursor_offset(offset);
    return offset;
}

int get_cursor_offset(){
    outb(REG_SCREEN_CTRL, 14);
    int position = inb(REG_SCREEN_DATA) << 8;
    outb(REG_SCREEN_CTRL, 15);
    position += inb(REG_SCREEN_DATA);
    return position * 2; //offset = position*cellSize
}

void set_cursor_offset(int offset){
    offset /= 2;
    outb(REG_SCREEN_CTRL, 14);
    outb(REG_SCREEN_DATA, (unsigned char)(offset >> 8));
    outb(REG_SCREEN_CTRL, 15);
    outb(REG_SCREEN_DATA, (unsigned char)(offset & 0xff));
}

void clear_screen(){
    int screenSize = MAX_COLS*MAX_ROWS;
    int i;
    char* screen = VIDEO_ADDRESS;

    for(i = 0; i < screenSize; ++i){
        screen[2*i] = ' ';
        screen[2*i+1] = 0;
    }
    set_cursor_offset(get_cursor_offset(0,0));
}

int get_offset(int col, int row) { 
    return 2 * (row * MAX_COLS + col); 
}
int get_offset_row(int offset) { 
    return offset / (2 * MAX_COLS); 
}
int get_offset_col(int offset) { 
    return (offset - (get_offset_row(offset)*2*MAX_COLS))/2; 
}