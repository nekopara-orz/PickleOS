/*
 * =====================================================================================
 *
 *       Filename:  timer.h
 *
 *    Description:  定义 PIT(周期中断定时器) 相关函数
 *
 *        Version:  1.0
 *        Created:  2020年08月03日 
 *       Revision:  none
 *       Compiler:  gcc

 * =====================================================================================
 */

#ifndef INCLUDE_TIMER_H_
#define INCLUDE_TIMER_H_

#include "types.h"
void init_timer(uint32_t frequency);

#endif 	// INCLUDE_TIMER_H_
