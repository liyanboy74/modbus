#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "mb.h"
#include "table.h"

int main()
{
    /*
    mb_error_e error;
    error=mb_check_coils_address(15,1);
    printf("Error Code %02x\r\n",error);
    */

    mb_table_write(TBALE_Coils,0,0xf0f2);
    printf("%04x \r\n",mb_table_read(TBALE_Coils,0));
    printf("%02x \r\n",mb_table_read_bit(TBALE_Coils,1));

    return 0;
}