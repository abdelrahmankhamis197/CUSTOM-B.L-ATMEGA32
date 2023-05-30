
#include "DIO_interface.h"

const DIO_PinStatus_type  PinsStatusArray[TOTAL_PINS]={
	INFREE,				/* Port A Pin 0 ADC0*/
	OUTPUT,				/* Port A Pin 1 ADC1*/
	OUTPUT,				/* Port A Pin 2 */
	OUTPUT,				/* Port A Pin 3 */
	OUTPUT,				/* Port A Pin 4 */
	OUTPUT,				/* Port A Pin 5 */
	OUTPUT,				/* Port A Pin 6 */
	OUTPUT,				/* Port A Pin 7 */
	OUTPUT,				/* Port B Pin 0   / */
	OUTPUT,				/* Port B Pin 1   /*/
	OUTPUT,			/* Port B Pin 2 / INT2*/
	OUTPUT,				/* Port B Pin 3	/OC0*/
	OUTPUT,				/* Port B Pin 4	/SPI_SS*/
	OUTPUT,				/* Port B Pin 5	/SPI_MOSI*/
	OUTPUT,				/* Port B Pin 6	/SPI_MISO*/
	OUTPUT,				/* Port B Pin 7	/SPI_SCLK*/
	OUTPUT,				/* Port C Pin 0	SCLK*/
	OUTPUT,				/* Port C Pin 1	SDA*/
	OUTPUT,				/* Port C Pin 2 */
	OUTPUT,				/* Port C Pin 3 */
	OUTPUT,				/* Port C Pin 4 */
	OUTPUT,				/* Port C Pin 5 */
	OUTPUT,				/* Port C Pin 6 */
	OUTPUT,				/* Port C Pin 7 */
	INFREE,				/* Port D Pin 0 /Rx_Uart*/
	OUTPUT,				/* Port D Pin 1 /Tx_Uart*/
	INPULLUP,				/* Port D Pin 2 /INT0*/
	INPULLUP,				/* Port D Pin 3 / INT1 */
	INPULLUP,				/* Port D Pin 4 OCR1B  */
	OUTPUT,				/* Port D Pin 5 OCR1A  */
	INFREE,				/* Port D Pin 6 /   ICP*/
	OUTPUT				/* Port D Pin 7 */
};