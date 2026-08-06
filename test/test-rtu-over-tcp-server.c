#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>

#include <unistd.h>
#include <arpa/inet.h>

#include "mb-rtu.h"

#define PORT 1502
#define RX_BUFFER_SIZE 512

#define TEST_RTU_OVER_TCP_DEBUG

static int client_fd;

static void send_data(uint8_t *data, uint16_t len)
{
    send(client_fd, data, len, 0);

    #ifdef TEST_RTU_OVER_TCP_DEBUG
        printf("TX: ");
        for(int i=0;i<len;i++)
            printf("%02X ",data[i]);
        printf("\n");
    #endif
}

int main(void)
{
    int server_fd;
    struct sockaddr_in addr;
    uint8_t rx_buffer[RX_BUFFER_SIZE];

    printf("Starting Modbus RTU Over TCP Slave\n");

    mb_set_tx_handler(send_data);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(server_fd < 0)
    {
        perror("socket");
        return -1;
    }

    memset(&addr,0,sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    if(bind(server_fd,
            (struct sockaddr *)&addr,
            sizeof(addr)) < 0)
    {
        perror("bind");
        return -1;
    }

    listen(server_fd,1);

    printf("Listening TCP port %d\n",PORT);

    while(1)
    {

        printf("Waiting client...\n");

        client_fd = accept(server_fd,NULL,NULL);

        if(client_fd < 0)
        {
            perror("accept");
            continue;
        }

        printf("Client connected\n");

        while(1)
        {
            int len;
            len = (int)recv(client_fd,
                       rx_buffer,
                       sizeof(rx_buffer),
                       0);

            if(len <= 0)
            {
                printf("Client disconnected\n");
                close(client_fd);
                break;
            }

            #ifdef TEST_RTU_OVER_TCP_DEBUG
                printf("RX: ");
                for(int i=0;i<len;i++)
                    printf("%02X ",rx_buffer[i]);
                printf("\n");
            #endif

            // mb_tcp_receive(rx_buffer,len);
            for(int i=0;i<len;i++) mb_rtu_check_new_data(rx_buffer[i]);
        }
    }
    close(server_fd);
    return 0;
}
