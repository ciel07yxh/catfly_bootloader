#ifndef FRAME_PHASE_H_INCLUDED
#define FRAME_PHASE_H_INCLUDED

#include "usart_driver.h"

#define MAX_MTU_LEN     255
#define MAX_FILE_LEN    128


#define ACK_FRAME    0x05
#define FILE_FRAME  'F'
#define APP2BOOT    (0x4c)

#define SUCCESS     0x80
#define FAILED      0x00

//app frame
typedef struct {
  uint8_t command;                      /**< Sequence number */
  uint8_t status;
}frame_ack_t;

typedef struct {
  uint8_t length;                      /**< Sequence number */
  uint8_t buf[MAX_FILE_LEN];
}frame_file_t;

typedef struct {
  uint8_t seq;                      /**< Sequence number */
}frame_app2boot_t;
//play load
typedef struct {
  uint8_t type;
  uint8_t seq;                   /**< Frame type field  */
  uint8_t payload_len;
  uint8_t buf[MAX_MTU_LEN];
}uart_frame_t;

typedef struct zxy_framer{
uint8_t seq;
void (*send)(struct zxy_framer *phaser,uart_frame_t *frame);
void (*input)(struct zxy_framer *phaser,uint8_t *buf,uint8_t len);

}zxy_framer;

extern zxy_framer __framer;
#endif // FRAME_PHASE_H_INCLUDED
