#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

#include "mb.h"
#include "master.h"

int main()
{
    mb_error_e error;
    error=mb_check_coils_address(15,1);
    printf("Error Code %02x\r\n",error);
    return 0;
}