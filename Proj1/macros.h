#ifndef MACROS_H_
/**
 * @brief FLAG de inicio de controlo
 * 
 */
#define FLAG 0x7E
/**
 * @brief Flag A para o emissor
 * 
 */
#define Aemi 0x03
/**
 * @brief Flag A para o recetor
 * 
 */
#define Arec 0x01
/**
 * @brief Flag C caso esteja a ser enviada a mensagem SET
 * 
 */
#define Cset 0x03
/**
 * @brief Flag C caso esteja a ser enviada a mensagem DISC
 * 
 */
#define Cdisc 0x11
/**
 * @brief Flag C caso esteja a ser enviada a mensagem UA
 * 
 */
#define Cua 0x08
/**
 * @brief Flag C caso esteja a ser enviada a mensagem RR com S=1
 * 
 */
#define Crr1 0x85
/**
 * @brief Flag C caso esteja a ser enviada a mensagem RR com S=0
 * 
 */
#define Crr0 0x05
/**
 * @brief Flag C caso esteja a ser enviada a mensagem REJ com S=1
 * 
 */
#define Crej1 0x81
/**
 * @brief Flag C caso esteja a ser enviada a mensagem REJ com S=0
 * 
 */
#define Crej0 0x01
/**
 * @brief Flag S caso esteja a ser enviada a trama com informação e S=0
 * 
 */
#define S0 0x00
/**
 * @brief Flag s caso esteja a ser enviada a trama com informação e S=1
 * 
 */
#define S1 0x40
/**
 * @brief Flag relacionada com Stuffing. Byte de ESCAPE
 * 
 */
#define ESCAPE 0x7d
/**
 * @brief Flag relacionada com Stuffing. Byte para a primeira parte
 * 
 */
#define ESCAPEF 0x5e
/**
 * @brief Flag relacionada com Stuffing. Byte para a segunda parte
 * 
 */
#define ESCAPEE 0x5d
/**
 * @brief Maximo de leituras de bytes que o programa pode fazer numa única tentativa
 * 
 */
#define MaximumRead 10000
/**
 * @brief Tempo do alarme
 * 
 */
#define WAITTIME 3

//Application layer
#define DataC 0x01
#define StartC  0x02
#define EndC  0x03
#define PackageSize 128
#define PerError 80

#define BAUDRATE B57600
#define _POSIX_SOURCE 1 /* POSIX compliant source */
#define FALSE 0
#define TRUE 1

#define clear() printf("\033[H\033[J")

#endif
