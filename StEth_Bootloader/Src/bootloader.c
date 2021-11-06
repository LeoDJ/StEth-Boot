#include "bootloader.h"
#include "globals.h"

#include "TFTP/tftp.h"

uint8_t tftpBuf[MAX_MTU_SIZE];

void bootloaderInit() {
    TFTP_init(SOCKET_TFTP, &tftpBuf);
    
}

void bootloaderLoop() {
    TFTP_run();
}