#ifndef COMMON_H_
#define COMMON_H_

/* cstdlib includes */
#include <stdint.h>

/* macros */
#define BIT(x) (1UL << (x))

/* generic io pin definitions */
/* each pin has the corresponding bit set, i.e. pin 4 -> bit 4 set */
#define PIN0    0x0001U // 0000 0000 0000 0001
#define PIN1    0x0002U // 0000 0000 0000 0010
#define PIN2    0x0004U // 0000 0000 0000 0100
#define PIN3    0x0008U // 0000 0000 0000 1000
#define PIN4    0x0010U // 0000 0000 0001 0000
#define PIN5    0x0020U // 0000 0000 0010 0000
#define PIN6    0x0040U // 0000 0000 0100 0000
#define PIN7    0x0080U // 0000 0000 1000 0000
#define PIN8    0x0100U // 0000 0001 0000 0000
#define PIN9    0x0200U // 0000 0010 0000 0000
#define PIN10   0x0400U // 0000 0100 0000 0000
#define PIN11   0x0800U // 0000 1000 0000 0000
#define PIN12   0x1000U // 0001 0000 0000 0000
#define PIN13   0x2000U // 0010 0000 0000 0000
#define PIN14   0x4000U // 0100 0000 0000 0000
#define PIN15   0x8000U // 1000 0000 0000 0000
#define PIN_ALL 0xFFFFU // 1111 1111 1111 1111

#endif // COMMON_H_