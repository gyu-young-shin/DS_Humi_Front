/* 
 * File:   Rs485.h
 * Author: jjy99
 *
 * Created on 2021년 3월 15일 (월), 오후 1:44
 */

#ifndef RS485_H
#define	RS485_H

#ifdef	__cplusplus
extern "C" {
#endif

#define	URX2_LEN		16
#define	UTX2_LEN		16

void Rs485_Init(void) ;    
void Rs485_proc(void);
void SendChar(unsigned char send_c);



#ifdef	__cplusplus
}
#endif

#endif	/* RS485_H */

