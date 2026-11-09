#ifndef CALC_H
#define CALC_H

#define X_WIDTH 20

int running(void);
int getContrast(void);

enum arrow_directions {
    ARROW_UP,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT
};

void initialize(void);

void press(char num);
void press_enter(void);
void press_backspace(void);
void press_arrow(enum arrow_directions direction);

void xsetstream(FILE* s);
void xselectinput(void);
void xselectoutput(void);
void xdump(void);
void xclear(void);
void xputchar(char c);
void xputs(const char* str);
void xprintf(const char* format, ...);
const char* xgetoutput(void);
const char* xgetinput(void);
size_t xgetinputcursor(void);

#endif
