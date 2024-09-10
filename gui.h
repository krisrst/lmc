
#ifndef GUI_H
#define GUI_H

#include "lmc.h"

/*
 * If you change anything here, only touch the integers, and not the macros.
 * The positions of the other GUI elements are calculated from the initial values.
 * */

#define GUI_ASSEMBLY_FIELD_START_Y 2
#define GUI_ASSEMBLY_FIELD_START_X 2
#define GUI_ASSEMBLY_FIELD_WIDTH 40
#define GUI_ASSEMBLY_FIELD_HEIGHT 52

#define GUI_MACHINECODE_FIELD_START_Y GUI_ASSEMBLY_FIELD_START_Y
#define GUI_MACHINECODE_FIELD_START_X (GUI_ASSEMBLY_FIELD_START_X +\
        GUI_ASSEMBLY_FIELD_WIDTH + 2)
#define GUI_MACHINECODE_FIELD_WIDTH 20
#define GUI_MACHINECODE_FIELD_HEIGHT GUI_ASSEMBLY_FIELD_HEIGHT

#define GUI_OUTPUT_FIELD_START_Y GUI_MACHINECODE_FIELD_START_Y
#define GUI_OUTPUT_FIELD_START_X (GUI_MACHINECODE_FIELD_START_X +\
        GUI_MACHINECODE_FIELD_WIDTH + 2)
#define GUI_OUTPUT_FIELD_WIDTH (OUTPUT_FIELD_WIDTH + 2)
#define GUI_OUTPUT_FIELD_HEIGHT (OUTPUT_FIELD_HEIGHT + 2)

#define GUI_CPU_FIELD_START_Y (GUI_OUTPUT_FIELD_START_Y +\
        GUI_OUTPUT_FIELD_HEIGHT + 2)
#define GUI_CPU_FIELD_START_X (GUI_OUTPUT_FIELD_START_X)
#define GUI_CPU_FIELD_WIDTH GUI_OUTPUT_FIELD_WIDTH
#define GUI_CPU_FIELD_HEIGHT 20

#define GUI_PC_OFF_X GUI_CPU_FIELD_START_X + 2
#define GUI_PC_OFF_Y GUI_CPU_FIELD_START_Y + 2
#define GUI_PC_WIDTH 26
#define GUI_PC_HEIGHT 2

#define GUI_IR_OFF_X GUI_PC_OFF_X + 0
#define GUI_IR_OFF_Y GUI_PC_OFF_Y + GUI_PC_HEIGHT + 2
#define GUI_IR_WIDTH 26
#define GUI_IR_HEIGHT 2

#define GUI_AR_OFF_X GUI_IR_OFF_X + 0
#define GUI_AR_OFF_Y GUI_IR_OFF_Y + GUI_IR_HEIGHT + 2
#define GUI_AR_WIDTH 26
#define GUI_AR_HEIGHT 2

#define GUI_ACC_OFF_X GUI_AR_OFF_X + 0
#define GUI_ACC_OFF_Y GUI_AR_OFF_Y + GUI_AR_HEIGHT + 2
#define GUI_ACC_WIDTH 26
#define GUI_ACC_HEIGHT 2

#define GUI_ALU_FIELD_START_Y (GUI_CPU_FIELD_START_Y +\
        GUI_CPU_FIELD_HEIGHT + 4)
#define GUI_ALU_FIELD_START_X (GUI_CPU_FIELD_START_X)
#define GUI_ALU_FIELD_WIDTH 15
#define GUI_ALU_FIELD_HEIGHT 4

#define GUI_INPUT_FIELD_START_Y (GUI_ALU_FIELD_START_Y +\
        GUI_ALU_FIELD_HEIGHT + 4)
#define GUI_INPUT_FIELD_START_X (GUI_ALU_FIELD_START_X)
#define GUI_INPUT_FIELD_WIDTH 10
#define GUI_INPUT_FIELD_HEIGHT 3

#define GUI_STATUS_FIELD_START_Y (GUI_INPUT_FIELD_START_Y +\
        GUI_INPUT_FIELD_HEIGHT + 1)
#define GUI_STATUS_FIELD_START_X (GUI_INPUT_FIELD_START_X)
#define GUI_STATUS_FIELD_WIDTH (STATUS_FIELD_WIDTH + 2)
#define GUI_STATUS_FIELD_HEIGHT (STATUS_FIELD_HEIGHT + 2)

#define GUI_HELP_FIELD_START_Y 20
#define GUI_HELP_FIELD_START_X 40
#define GUI_HELP_FIELD_WIDTH 100
#define GUI_HELP_FIELD_HEIGHT 20

#define GUI_MEMORY_FIELD_START_Y GUI_OUTPUT_FIELD_START_Y
#define GUI_MEMORY_FIELD_START_X (GUI_OUTPUT_FIELD_START_X +\
        GUI_OUTPUT_FIELD_WIDTH + 8)
#define GUI_MEMORY_FIELD_WIDTH 100
#define GUI_MEMORY_FIELD_HEIGHT 52

#define GUI_CELL_OFF_X 6
#define GUI_CELL_OFF_Y 2
#define GUI_CELL_WIDTH 9
#define GUI_CELL_HEIGHT 5

int gui_init(struct lmc * lmc);
void gui_destroy(struct lmc * lmc);

#endif // GUI_H
