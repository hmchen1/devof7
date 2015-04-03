#ifndef _COMMON_H_
#define _COMMON_H_

#ifndef TARGET
    #error "Must specify TARGET define"
#endif
#define MODULE_CALLTYPE

//Magic macro to check enum size
//#define ctassert(n,e) extern unsigned char n[(e)?0:-1]
#define ctassert(COND,MSG) typedef char static_assertion_##MSG[(COND)?1:-1]

#include <stdint.h>
typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef int8_t   s8;
typedef int16_t  s16;
typedef int32_t  s32;

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "../../std.h"
#include "target.h"

#define CHAN_MAX_VALUE 10000
#define CHAN_MIN_VALUE -10000
#define _tr(x) x
#define _tr_noop(x) x

enum {
    PRIORITY_HIGHEST = (0 << 6),
    PRIORITY_HIGH    = (1 << 6),
    PRIORITY_LOW     = (2 << 6),
    PRIORITY_LOWEST  = (3 << 6),
};

extern volatile s32 Channels[MAX_PPM_IN_CHANNELS];
extern u8 ChannelMap[MAX_PPM_IN_CHANNELS];

struct limit {
    u8 flags;
    u8 failsafe;
};

#define CH_FAILSAFE_EN 1

#define PROTODEF(proto, module, map, cmd, name) proto,
enum Protocols {
    PROTOCOL_NONE,
    #include "../../protocol/protocol.h"
    PROTOCOL_COUNT,
};
#undef PROTODEF
extern const char * const ProtocolNames[PROTOCOL_COUNT];

enum TxPower {
    TXPOWER_100uW,
    TXPOWER_300uW,
    TXPOWER_1mW,
    TXPOWER_3mW,
    TXPOWER_10mW,
    TXPOWER_30mW,
    TXPOWER_100mW,
    TXPOWER_150mW,
    TXPOWER_LAST,
};

enum {
    CYRF6936,
    A7105,
    CC2500,
    NRF24L01,
    MULTIMOD,
    MULTIMODCTL,
    TX_MODULE_LAST,
};
int SPI_ConfigSwitch(unsigned csn_high, unsigned csn_low);
/*
void pabort(const char *s);
void SPI_Init(int idx);
void SPI_WriteRegisterMulti(u8 address, const u8 data[], u8 length);
void SPI_ReadRegisterMulti(u8 address, u8 data[], u8 length);
void SPI_WriteRegister(u8 address, u8 data);
u8 SPI_ReadRegister(u8 address);
*/

void PROTOCOL_SetBindState(u32 i);
const char **PROTOCOL_GetOptions();
u8 PROTOCOL_AutoBindEnabled();
int PROTOCOL_DefaultNumChannels();
int PROTOCOL_NumChannels();
int PROTOCOL_SetSwitch(int module);
int PROTOCOL_SticksMoved(int init);
void PROTOCOL_Init(u8 force);
void PROTOCOL_Deinit();
void PROTOCOL_Bind();

void CLOCK_StopTimer();
void CLOCK_StartTimer(unsigned t, u16 (*_cmd)());
void CLOCK_ResetWatchdog();

u32 CLOCK_getms();

u32 Crc(const void *, u32 size);
u32 rand32_r(u32 *seed, u8 update); //LFSR based PRNG
u32 rand32(); //LFSR based PRNG

extern volatile u8 priority_ready;

extern const char UTX_Version[33];
/* Target defs */
void MCU_SerialNumber(u8 *var, int len);

void CLOCK_Init(void);
u32 CLOCK_getms(void);
void CLOCK_StartTimer(unsigned us, u16 (*cb)(void));
void CLOCK_StopTimer();
void CLOCK_SetMsecCallback(int cb, u32 msec);
void CLOCK_StartWatchdog();
void CLOCK_ResetWatchdog();
#endif /*_COMMON_H_ */
