// Flash access code curtesy of http://kevincuzner.com/2018/06/28/building-a-usb-bootloader-for-an-stm32/
// https://www.st.com/content/ccc/resource/technical/document/programming_manual/10/98/e8/d4/2b/51/4b/f5/CD00283419.pdf/files/CD00283419.pdf/jcr:content/translations/en.CD00283419.pdf

#include "flash.h"

/**
 * Certain functions, such as flash write, are easier to do if the code is
 * executed from the RAM. This decoration relocates the function there and
 * prevents any inlining that might otherwise move the function to flash.
 */
#define _RAM __attribute__((section (".data#"), noinline))

/**
 * RAM-located function which actually performs page erases.
 *
 * address: Page-aligned address to erase
 */
static _RAM bool nvm_flash_do_page_erase(uint32_t *address)
{
    //erase operation
    // FLASH->PECR |= FLASH_PECR_ERASE | FLASH_PECR_PROG;
    FLASH->CR |= FLASH_CR_PER;  // page erase
    *address = (uint32_t)0;
    FLASH->CR |= FLASH_CR_STRT; // start operation
    //wait for completion
    while (FLASH->SR & FLASH_SR_BSY) { }
    if (FLASH->SR & FLASH_SR_EOP)
    {
        //completed without incident
        FLASH->SR = FLASH_SR_EOP;
        return true;
    }
    else
    {
        //there was an error, reset error flag
        // FLASH->SR = FLASH_SR_FWWERR | FLASH_SR_PGAERR | FLASH_SR_WRPERR;
        FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
        return false;
    }

}

// TODO: change to correct half word write behaviour !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
 
/**
 * RAM-located function which actually performs half-page writes on previously
 * erased pages.
 *
 * address: Half-page aligned address to write
 * data: Array to 16 32-bit words to write
 */
static _RAM bool nvm_flash_do_write_half_page(uint32_t *address, uint32_t *data)
{
    uint8_t i;
 
    //half-page program operation
    // FLASH->PECR |= FLASH_PECR_PROG | FLASH_PECR_FPRG;
    FLASH->CR |= FLASH_CR_PG;
    for (i = 0; i < 16; i++)
    {
        *address = data[i]; //the actual address written is unimportant as these words will be queued
    }
    //wait for completion
    while (FLASH->SR & FLASH_SR_BSY) { }
    if (FLASH->SR & FLASH_SR_EOP)
    {
        //completed without incident
        FLASH->SR = FLASH_SR_EOP;
        return true;
    }
    else
    {
        //there was an error, reset error flag
        // FLASH->SR = FLASH_SR_FWWERR | FLASH_SR_NOTZEROERR | FLASH_SR_PGAERR | FLASH_SR_WRPERR;
        FLASH->SR |= FLASH_SR_PGERR | FLASH_SR_WRPRTERR;
        return false;
 
    }
}

/**
 * Unlocks the flash
 */
static void nvm_unlock_flash(void)
{
    if (FLASH->CR & FLASH_CR_LOCK)
    {
        FLASH->KEYR = 0x8c9daebf;
        FLASH->KEYR = 0x13141516;
    }
}
 
/**
 * Locks all unlocked NVM regions and registers
 */
static void nvm_lock(void)
{
    if (!(FLASH->CR & FLASH_CR_LOCK))
    {
        FLASH->CR |= FLASH_CR_LOCK;
    }
}
 
 
bool nvm_flash_erase_page(uint32_t *address)
{
    bool result = false;
 
    if ((uint32_t)address & 0x7F)
        return false; //not page aligned
 
    nvm_unlock_flash();
    result = nvm_flash_do_page_erase(address);
    nvm_lock();
    return result;
}
 
bool nvm_flash_write_half_page(uint32_t *address, uint32_t *data)
{
    bool result = false;
 
    if ((uint32_t)address & 0x3F)
        return false; //not half-page aligned
 
    nvm_unlock_flash();
    result = nvm_flash_do_write_half_page(address, data);
    nvm_lock();
    return result;
}