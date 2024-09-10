
#include "assembler.h"
#include "log.h"

#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <assert.h>

/*
 * Helper function.
 * Returns "1" (true) if the instruction in s is an LMC
 * instruction.
 * */
static int is_instr(char * s){

    if( ! strcasecmp(s, "HLT") )
        return 1;
    else if( ! strcasecmp(s, "INP" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "OUT" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "OTC" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "ADD" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "SUB" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "LDA" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "STA" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "BRA" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "BRZ" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "BRP" ) ){
        return 1;
    }
    else if( ! strcasecmp(s, "DAT" ) ){
        return 1;
    }
    return 0;
}

int assemble(struct lmc * lmc){

    int i;
    int num_instr;
    int machinecode;
    int address;
    int n;
    int num_labels;
    char *b0, *b1, *b2;
    char * buf;
    char * instr;
    char * label;
    char * addr;
    char * tok;
    char * labels[100];
    char * instrs[100];
    char * addrss[100];
    char assembler_mem[100][128];

    // Initialise variables
    num_labels = 0;
    num_instr = 0;

    for(i=0; i < 100; i++){
        lmc->machinecode_mem[i] = 0;
        lmc->label_mem[i][0] = '\0';
        labels[i] = instrs[i] = addrss[i] = NULL;
        strncpy( assembler_mem[i], lmc->assembler_mem[i], 128 );
        assembler_mem[i][127] = '\0';
    }

    // This pass looks at everything, sorts ever instruction
    // into labels, instruction and address, if they are
    // supplied by the programmer.
    for(i=0; i < 100; i++){

        buf = assembler_mem[i];

        n = 0;
        instr = NULL;
        label = NULL;
        addr = NULL;

        b0 = strtok( buf, " " );
        if( b0 == NULL )
            break;

        n++;
        b1 = strtok( NULL, " " );
        if( b1 != NULL ){
            n++;
            b2 = strtok( NULL, " " );
            if( b2 != NULL ){
                n++;
            }
        }

        // What is label, instr and addr, if any?
        if( n == 1 ){
            instr = b0;
        }
        else if( n == 2 ){
            // Complicated case;
            // b0 or b1 can be instr, for example:
            // label HLT
            //   OR
            // ADD label
            if( is_instr(b0) ){
                instr = b0;
                addr = b1;
            }
            else if( is_instr(b1) ){
                label = b0;
                instr= b1;
            }
            else{
                status_field_print(lmc, "No instruction on line %d", i);
                return -1;
            }
        }
        else{ // n == 3
            label = b0;
            instr = b1;
            addr = b2;
        }

        labels[i] = label;
        instrs[i] = instr;
        addrss[i] = addr;

        num_instr++;
    }

    log_printf(lmc, "num instr%d\n",num_instr);
    for(i=0; i < num_instr; i++){

        address = -1;
        label = labels[i];
        instr = instrs[i];
        addr  = addrss[i];

        log_printf(lmc, "considering instruction: %s\n", instr);
        if( label ){
            log_printf(lmc, "label: %s\n", label);
        }
        if( addr ){
            char * errp;
            address = strtol( addr, &errp, 10 );

            // If this triggers, we have to assume that the
            // programmer meant a label, and we'll need to find it
            if( errno || (strlen(errp) != 0) ){
                int j;
                int found = 0;
                for(j=0; j < num_instr; j++){

                    if( labels[j] != NULL ){
                        if( ! strcmp( addr, labels[j] ) ){
                            //log_printf(lmc, "found label at %d\n", j);
                            address = j;
                            found = 1;
                            break;
                        }
                    }
                }
                if( ! found ){
                    //log_printf(lmc, "no label found for: %s\n", addr);
                    status_field_print(lmc, "%s", addr);
                    status_field_print(lmc, "Line %d: No such label:", i);
                    return -1;
                }
            }
        }


        if( ! strcasecmp( instr, "HLT" ) ){
            machinecode = 0;
            address = 0;
        }
        else if( ! strcasecmp( instr, "ADD" ) ){
            machinecode = 1;
            if( address < 0 ){
                status_field_print(lmc, "Line %d: no address", i);
                return -1;
            }
        }
        else if( ! strcasecmp( instr, "SUB" ) ){
            machinecode = 2;
            if( address < 0 ){
                status_field_print(lmc, "Line %d: no address", i);
                return -1;
            }
        }
        else if( ! strcasecmp( instr, "STA" ) ){
            machinecode = 3;
            if( address < 0 ){
                status_field_print(lmc, "Line %d: no address", i);
                return -1;
            }
        }
        else if( ! strcasecmp( instr, "LDA" ) ){
            machinecode = 5;
            if( address < 0 ){
                status_field_print(lmc, "Line %d: no address", i);
                return -1;
            }
        }
        else if( ! strcasecmp( instr, "BRA" ) ){
            machinecode = 6;
            if( address < 0 ){
                status_field_print(lmc, "Line %d: no address", i);
                return -1;
            }
        }
        else if( ! strcasecmp( instr, "BRZ" ) ){
            machinecode = 7;
            if( address < 0 ){
                status_field_print(lmc, "Line %d: no address", i);
                return -1;
            }
        }
        else if( ! strcasecmp( instr, "BRP" ) ){
            machinecode = 8;
            if( address < 0 ){
                status_field_print(lmc, "Line %d: no address", i);
                return -1;
            }
        }
        else if( ! strcasecmp( instr, "INP" ) ){
            machinecode = 9;
            address = 1;
        }
        else if( ! strcasecmp( instr, "OUT" ) ){
            machinecode = 9;
            address = 2;
        }
        else if( ! strcasecmp( instr, "OTC" ) ){
            machinecode = 9;
            address = 22;
        }
        else if( ! strcasecmp( instr, "DAT" ) ){
            machinecode = 0;
        }
        else{
            status_field_print(lmc, "(%s)", instr);
            status_field_print(lmc, "Unknown instruction");
            status_field_print(lmc, "Line %d:", i);
            return -1;
        }

        if( (address < -999) ||
                (address > 999) ){
            log_printf(lmc, "DAT: address out of range (%d)\n", address);
            status_field_print(lmc, "%d", address);
            status_field_print(lmc, "Address out of range");
            status_field_print(lmc, "Line %d:", i);
            return -1;
        }


        lmc->machinecode_mem[i] = machinecode * 100 + address;
    }

    return 0;
}


