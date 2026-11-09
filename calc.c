#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <stdarg.h>
#include <time.h>

#include "calc.h"

static int is_running = 0;

enum error_codes {
    ERR_NONE,
    ERR_UNKNOWN_OPERATOR,
    ERR_INVALID_ARGUMENTS,
    ERR_STACK_UNDERFLOW,
    ERR_STACK_OVERFLOW,
    ERR_DIVIDE_BY_ZERO,
    ERR_INTERNAL_ERROR,
    ERR_INVALID_VARIABLE_NAME,
    ERR_VARIABLE_NOT_WRITEABLE,

    ERR_COUNT
};

static int had_error = ERR_NONE;

static void error(enum error_codes code);

int running(void) {
    return is_running;
}

static double get_variable(const char* name);

int getContrast(void) {
    return (int) get_variable("__contrast");
}

#define STACK_SIZE 128

static double stack[STACK_SIZE] = {0};
static int stack_ptr = 0;

static struct variable {
    char* name;
    double value;
    int writeable;
}* variables = NULL;
static int variable_count = 0;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#ifndef M_E
#define M_E 2.71828182845904523536
#endif

static char* duplicate_string(const char* str) {
    size_t len = strlen(str);
    char* copy = malloc(len + 1);
    if (copy) {
        strcpy(copy, str);
    }
    return copy;
}

static int set_variable(const char* name, double value) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            if (variables[i].writeable) {
                printf("Set variable %s to %f\n", name, value);
                variables[i].value = value;
                return 1; // Variable updated successfully
            } else {
                printf("Attempted to set read-only variable %s\n", name);
                error(ERR_VARIABLE_NOT_WRITEABLE);
                return 0; // Variable is read-only
            }
        }
    }
    // Variable not found, add new variable
    variables = realloc(variables, (variable_count + 1) * sizeof(struct variable));
    if (variables == NULL) {
        error(ERR_INTERNAL_ERROR);
        return 0; // Memory allocation failed
    }
    variables[variable_count].name = duplicate_string(name);
    variables[variable_count].value = value;
    variables[variable_count].writeable = 1; // New variables are writeable by default
    variable_count++;
    return 1; // Variable added successfully
}

static void set_variable_readonly(const char* name, double value) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            variables[i].value = value;
            variables[i].writeable = 0; // Set to read-only
            return;
        }
    }
    // Variable not found, add new variable
    variables = realloc(variables, (variable_count + 1) * sizeof(struct variable));
    if (variables == NULL) {
        error(ERR_INTERNAL_ERROR);
        return;
    }
    variables[variable_count].name = duplicate_string(name);
    variables[variable_count].value = value;
    variables[variable_count].writeable = 0; // New variables are read-only
    variable_count++;
}

static double get_variable(const char* name) {
    for (int i = 0; i < variable_count; i++) {
        if (strcmp(variables[i].name, name) == 0) {
            return variables[i].value;
        }
    }
    error(ERR_INVALID_VARIABLE_NAME);
    return NAN;
}

void initialize(void) {
    srand((unsigned int)time(NULL)); // Seed the random number generator

    set_variable_readonly("pi", M_PI);
    set_variable_readonly("e", M_E);
    set_variable_readonly("phi", (1 + sqrt(5)) / 2);
    set_variable_readonly("tau", 2 * M_PI);
    set_variable_readonly("ans", 0); // last answer

    set_variable("__precision", 6); // default precision
    set_variable("__contrast", 3); // default contrast

    is_running = 1;

    xselectoutput();
    xclear();
    xprintf("rpncalc v1.0");
    xselectinput();
    xclear();
}

static void push(double value) {
    if (stack_ptr >= STACK_SIZE) {
        error(ERR_STACK_OVERFLOW);
        return;
    }
    printf("Pushing: %f\n", value);
    stack[stack_ptr++] = value;
}

static double pop(void) {
    if (stack_ptr == 0) {
        return NAN;
    }
    printf("Popping: %f\n", stack[stack_ptr - 1]);
    return stack[--stack_ptr];
}

static double drop(void) {
    if (stack_ptr == 0) {
        error(ERR_STACK_UNDERFLOW);
        return NAN;
    }
    printf("Dropping: %f\n", stack[stack_ptr - 1]);
    stack_ptr--;
    return NAN;
}

static double swap(void) {
    if (stack_ptr < 2) {
        error(ERR_STACK_UNDERFLOW);
        return NAN;
    }
    double temp = stack[stack_ptr - 1];
    stack[stack_ptr - 1] = stack[stack_ptr - 2];
    stack[stack_ptr - 2] = temp;
    return NAN;
}

static double top(void) {
    if (stack_ptr == 0) {
        error(ERR_STACK_UNDERFLOW);
        return NAN;
    }
    return stack[stack_ptr - 1];
}

static double op_plus(double a, double b) { return a + b; }
static double op_minus(double a, double b) { return a - b; }
static double op_multiply(double a, double b) { return a * b; }
static double op_divide(double a, double b) {
    if (b == 0) {
        error(ERR_DIVIDE_BY_ZERO);
        return NAN;
    }
    return a / b;
}
static double op_modulus(double a, double b) {
    if (b == 0) {
        error(ERR_DIVIDE_BY_ZERO);
        return NAN;
    }
    return fmod(a, b);
}
static double op_negate(double a) { return -a; }

static double clear_stack(void) {
    xselectoutput();
    xclear();
    stack_ptr = 0;
    return NAN;
}

static double op_dumpstack(void) {
    for (int i = 0; i < stack_ptr; i++) {
        printf("%.*f\n", (int) get_variable("__precision"), stack[i]);
    }
    return NAN;
}

static double op_factorial(double n) {
    if (n < 0 || floor(n) != n) {
        error(ERR_INVALID_ARGUMENTS);
        return NAN;
    }
    double result = 1;
    for (int i = 1; i <= (int)n; i++) {
        result *= i;
    }
    return result;
}

static double op_gcd(double a, double b) {
    if (a < 0 || b < 0 || floor(a) != a || floor(b) != b) {
        error(ERR_INVALID_ARGUMENTS);
        return NAN;
    }
    while (b != 0) {
        double temp = b;
        b = fmod(a, b);
        a = temp;
    }
    return a;
}

static double op_lcm(double a, double b) {
    if (a < 0 || b < 0 || floor(a) != a || floor(b) != b) {
        error(ERR_INVALID_ARGUMENTS);
        return NAN;
    }
    if (a == 0 || b == 0) {
        return 0;
    }
    return fabs(a * b) / op_gcd(a, b);
}

static double op_rand(void) {
    return (double)rand() / RAND_MAX;
}

static double op_npr(double n, double r) {
    if (n < 0 || r < 0 || floor(n) != n || floor(r) != r || r > n) {
        error(ERR_INVALID_ARGUMENTS);
        return NAN;
    }
    double result = 1;
    for (int i = 0; i < (int)r; i++) {
        result *= (n - i);
    }
    return result;
}

static double op_ncr(double n, double r) {
    if (n < 0 || r < 0 || floor(n) != n || floor(r) != r || r > n) {
        error(ERR_INVALID_ARGUMENTS);
        return NAN;
    }
    return op_npr(n, r) / op_factorial(r);
}

static struct opdata {
    char* name;
    void* op;
    int argc;
} ops[] = {
    { .name = "__dumpstack", .op = op_dumpstack, .argc = 0 },

    { .name = "+", .op = op_plus, .argc = 2 },
    { .name = "-", .op = op_minus, .argc = 2 },
    { .name = "*", .op = op_multiply, .argc = 2 },
    { .name = "/", .op = op_divide, .argc = 2 },
    { .name = "add", .op = op_plus, .argc = 2 },
    { .name = "sub", .op = op_minus, .argc = 2 },
    { .name = "mul", .op = op_multiply, .argc = 2 },
    { .name = "div", .op = op_divide, .argc = 2 },
    { .name = "mod", .op = op_modulus, .argc = 2 },
    { .name = "neg", .op = op_negate, .argc = 1 },

    { .name = "clear", .op = clear_stack, .argc = 0 },
    { .name = "dup", .op = top, .argc = 0 },
    { .name = "swap", .op = swap, .argc = 0 },
    { .name = "drop", .op = drop, .argc = 0 },

    { .name = "sin", .op = sin, .argc = 1 },
    { .name = "cos", .op = cos, .argc = 1 },
    { .name = "tan", .op = tan, .argc = 1 },
    { .name = "asin", .op = asin, .argc = 1 },
    { .name = "acos", .op = acos, .argc = 1 },
    { .name = "atan", .op = atan, .argc = 1 },
    { .name = "sinh", .op = sinh, .argc = 1 },
    { .name = "cosh", .op = cosh, .argc = 1 },
    { .name = "tanh", .op = tanh, .argc = 1 },
    { .name = "asinh", .op = asinh, .argc = 1 },
    { .name = "acosh", .op = acosh, .argc = 1 },
    { .name = "atanh", .op = atanh, .argc = 1 },
    { .name = "atan2", .op = atan2, .argc = 2 },
    
    { .name = "ln", .op = log, .argc = 1 },
    { .name = "log", .op = log10, .argc = 1 },
    { .name = "log2", .op = log2, .argc = 1 },
    { .name = "exp", .op = exp, .argc = 1 },
    { .name = "pow", .op = pow, .argc = 2 },
    { .name = "sqrt", .op = sqrt, .argc = 1 },
    { .name = "cbrt", .op = cbrt, .argc = 1 },
    { .name = "abs", .op = fabs, .argc = 1 },
    { .name = "fact", .op = op_factorial, .argc = 1 },

    { .name = "gcd", .op = op_gcd, .argc = 2 },
    { .name = "lcm", .op = op_lcm, .argc = 2 },

    { .name = "floor", .op = floor, .argc = 1 },
    { .name = "ceil", .op = ceil, .argc = 1 },
    { .name = "round", .op = round, .argc = 1 },
    { .name = "trunc", .op = trunc, .argc = 1 },
    { .name = "rand", .op = op_rand, .argc = 0 },

    { .name = "npr", .op = op_npr, .argc = 2 },
    { .name = "ncr", .op = op_ncr, .argc = 2 },
};

static char xoutput[X_WIDTH] = {0};
static size_t xoutput_cursor = 0;

static char xinput[X_WIDTH] = {0};
static size_t xinput_cursor = 0;

static char* selected_buffer = xoutput;
static size_t* selected_cursor = &xoutput_cursor;

void xselectinput(void) {
    selected_buffer = xinput;
    selected_cursor = &xinput_cursor;
}

void xselectoutput(void) {
    selected_buffer = xoutput;
    selected_cursor = &xoutput_cursor;
}

const char* xgetoutput(void) {
    return xoutput;
}

const char* xgetinput(void) {
    return xinput;
}

size_t xgetinputcursor(void) {
    return xinput_cursor;
}

void xclear(void) {
    for (int i = 0; i < X_WIDTH; i++) {
        memset(selected_buffer, 0, X_WIDTH);
    }
    *selected_cursor = 0;
}

void xputchar(char c) {
    if (c == '\n') {
        xclear();
    } else if (c == '\b') {
        if (*selected_cursor > 0) {
            (*selected_cursor)--;
            selected_buffer[*selected_cursor] = '\0';
        }
    } else {
        if (*selected_cursor < X_WIDTH) {
            selected_buffer[*selected_cursor] = c;
            (*selected_cursor)++;
        }
    }
}

void xputs(const char* str) {
    for (const char* p = str; *p != '\0'; p++) {
        xputchar(*p);
    }
}

void xprintf(const char* format, ...) {
    char buffer[256];
    va_list args;
    va_start(args, format);
    vsnprintf(buffer, sizeof(buffer), format, args);
    va_end(args);
    xputs(buffer);
}

static void error(enum error_codes code) {
    had_error = code;
}

static struct opdata* findOp(char* name) {
    for (size_t i = 0; i < sizeof(ops) / sizeof(ops[0]); i++) {
        if (strcmp(ops[i].name, name) == 0) {
            return &ops[i];
        }
    }
    return NULL;
}

static void doCalc(char* opChar) {
    double result = NAN;
    struct opdata* op = findOp(opChar);

    int displayPrecision = (int) get_variable("__precision");

    if (op != NULL) {
        double args[2] = {0};
        if (op->argc > 0) {
            if (stack_ptr < op->argc) {
                error(ERR_STACK_UNDERFLOW);
                return;
            }
            for (int i = op->argc - 1; i >= 0; i--) {
                args[i] = pop();
            }
        }
        switch (op->argc) {
            case 0:
                result = ((double (*)(void))op->op)();
                return;
            case 1:
                result = ((double (*)(double))op->op)(args[0]);
                break;
            case 2:
                result = ((double (*)(double, double))op->op)(args[0], args[1]);
                break;
            default:
                error(ERR_INTERNAL_ERROR);
                return;
        }

        if (!isnan(result)) {
            set_variable_readonly("ans", result);
            push(result);
            xselectoutput();
            xclear();
            xprintf("%.*f", displayPrecision, result);
        } else {
            for (int i = 0; i < op->argc; i++) {
                push(args[i]);
            }
        }
    } else {
        double value = get_variable(opChar);
        if (opChar[0] == '=') {
            error(ERR_NONE);
            opChar++; // Skip the '=' character
            value = pop();
            int res = set_variable(opChar, value);
            if (res != 1) {
                push(value);
            }
        } else if (!isnan(value)) {
            error(ERR_NONE);
            push(value);
        } else {
            error(ERR_UNKNOWN_OPERATOR);
        }
        xselectoutput();
        xclear();
        xprintf("%.*f", displayPrecision, stack[stack_ptr - 1]);
    }
}

static void pushInput() {
    char* input = xinput;
    size_t index = xinput_cursor;

    if (index > 0) {
        if (strspn(input, "0123456789.") != index) {
            doCalc(input);
        } else {
            double value = atof(input);
            if (isnan(value)) {
                error(ERR_INVALID_ARGUMENTS);
            } else {
                push(value);
            }
        }
    }

    xselectinput();
    xclear();

    xselectoutput();
    xclear();
    if (stack_ptr > 0) {
        xprintf("%.*f", (int) get_variable("__precision"), stack[stack_ptr - 1]);
    }
    if (had_error) {
        xclear();
        xprintf("E%02d", had_error);
        had_error = 0;
    }
}

void press(char c) {
    xselectinput();
    xputchar(c);
}

void press_enter(void) {
    pushInput();
}

void press_backspace(void) {
    xselectinput();
    xputchar('\b');
}

void press_arrow(enum arrow_directions direction) {
    (void)direction; // Unused parameter
}
