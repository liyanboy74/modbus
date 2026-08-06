#include <stdio.h>
#include <stdint.h>
#include <string.h>

#include <unistd.h>
#include <arpa/inet.h>

#include "mb-tcp.h"
#include "mb-packet.h"

#define SERVER_IP      "127.0.0.1"
#define SERVER_PORT    1502
#define RX_BUFFER_SIZE 512

#define TEST_TCP_DEBUG

static int sockfd;

static void send_data(uint8_t *data, uint16_t len)
{
    send(sockfd, data, len, 0);

    #ifdef TEST_TCP_DEBUG
        printf("TX TCP (%d): ", len);
        for(uint16_t i = 0; i < len; i++)
            printf("%02X ", data[i]);
        printf("\n");
    #endif
}

static void master_process(mb_packet_s Packet)
{
    #ifdef TEST_TCP_DEBUG
        printf("RX Function: %02X\n", Packet.function);
    #endif
}

int main(void)
{
    struct sockaddr_in addr;
    uint8_t rx_buffer[RX_BUFFER_SIZE];

    printf("Starting Modbus TCP Master\n");

    mb_set_tx_handler(send_data);
    mb_set_master_process_handler(master_process);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    memset(&addr, 0, sizeof(addr));

    addr.sin_family = AF_INET;
    addr.sin_port   = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &addr.sin_addr);

    if(connect(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
    {
        perror("connect");
        return -1;
    }

    printf("Connected\n");

    while(1)
    {
        mb_tx_packet_handler(mb_packet_request_read_holding_registers(1,0,10));

        int len = (int)recv(sockfd,
                       rx_buffer,
                       sizeof(rx_buffer),
                       0);

        if(len <= 0)
            break;

        #ifdef TEST_TCP_DEBUG
                printf("RX TCP (%d): ", len);
                for(int i = 0; i < len; i++)
                    printf("%02X ", rx_buffer[i]);
                printf("\n");
        #endif

        mb_tcp_receive(rx_buffer, (uint16_t)len);

        sleep(1);
    }

    close(sockfd);

    return 0;
}
