#include "bootloader.h"
#include "globals.h"
#include <stdio.h>

#include "TFTP/tftp.h"

uint8_t tftpBuf[MAX_MTU_SIZE];

void bootloaderInit() {
    TFTP_init(SOCKET_TFTP, tftpBuf);
}

void bootloaderLoop() {
    // TFTP_run();
}

// implement data callback from TFTP library
void save_data(uint8_t *data, uint32_t data_len, uint16_t block_number) {
    printf("(%d) ", data_len);
    fwrite(data, 1, data_len, stdout);
}

void bootloaderRequestFile() {
    uint8_t tftpServer[] = { TFTP_SERVER };
    uint32_t ip = __ntohl(*(uint32_t*)tftpServer);
    TFTP_read_request(ip, "hello.txt");
    uint8_t ret;
    while (1) {
        ret = TFTP_run();
        if (ret != TFTP_PROGRESS)
            break;
    }
}