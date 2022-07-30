#include "EasyCon_API.h"

/**********************************************************************/
// some incude files for funcs, you need change to your device framework files
/**********************************************************************/


/**********************************************************************/
// EasyCon API, you need to implement all of the AIP
/**********************************************************************/

/* EasyCon read 1 byte from E2Prom or flash 
 * need implement
 */
uint8_t EasyCon_read_byte(uint8_t* addr)
{
    
}

/* EasyCon write 1 byte to E2Prom or flash 
 * need implement
 */
void EasyCon_write_byte(uint8_t* addr,uint8_t value)
{
    
}

/* EasyCon read 2 byte from E2Prom or flash 
 * need implement
 */
uint16_t EasyCon_read_2byte(uint16_t* addr)
{
    
}

/* EasyCon write 2 byte to E2Prom or flash 
 * need implement
 */
void EasyCon_write_2byte(uint16_t* addr,uint16_t value)
{
    
}

/* running led on
 * need implement
 */
void EasyCon_runningLED_on(void)
{
    
}

/* running led off
 * need implement
 */
void EasyCon_runningLED_off(void)
{
    
}

/* data led blink
 * need implement,no block
 */
void EasyCon_blink_led(void)
{

}

/* serial send 1 byte
 * need implement,block
 */
void EasyCon_serial_send(const char DataByte)
{
    EasyCon_blink_led();
}

// about hid report

/* clean echo times in HID
 * need implement
 */
void zero_echo(void)
{

}

/* reset hid report to default.
 * need implement
 */
void reset_hid_report(void)
{

}

/* set left stick in hid report.
 * need implement
 */
void set_left_stick(const uint8_t LX, const uint8_t LY)
{

}

/* set right stick in hid report.
 * need implement
 */
void set_right_ttick(const uint8_t RX, const uint8_t RY)
{

}

/* set button in hid report.
 * need implement
 */
void set_buttons(const uint16_t Button)
{

}

/* set button press in hid report.
 * need implement
 */
void press_buttons(const uint16_t Button)
{

}

/* set buttons release in hid report.
 * need implement
 */
void release_buttons(const uint16_t Button)
{

}

/* set HAT in hid report.
 * need implement
 */
void set_HAT_switch(const uint8_t HAT)
{

}