#include<stdlib.h>
#include "command.h"

Command* create_command(){
    Command *cmd=malloc(sizeof(Command));

    cmd->argv=malloc(sizeof(char*)*MAX_ARGS);
    cmd->input_file=NULL;
    cmd->output_file=NULL;
    cmd->append=0;
    cmd->background=0;
    cmd->next=NULL;

    return cmd;
}