#ifndef IO_H
#define IO_H


typedef struct {
    char *attr_name;
    char *attr_content;
} t_cmd_attr;

typedef struct {
    char *command_line;
    t_cmd_attr *attr;
    int is_current;
} t_cmd_input;

#endif
