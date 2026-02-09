/******************************************************************************
 * Module: LIB
 * File Name: std_macros.h
 * Description: Standard macros for bit manipulation
 * Author: Abdelrahman Arafa
 * Email: engarafa55@gmail.com
 ******************************************************************************/
#ifndef STD_MACROS_H_
#define STD_MACROS_H_

/*******************************************************************************
 *                             Definitions                              *
 *******************************************************************************/
#define F_CPU 8000000UL
#define REGISTER_SIZE 8

/*******************************************************************************
 *                     Macro Functions Declarations                     *
 *******************************************************************************/
#define SET_BIT(reg, bit) reg |= (1 << bit)
#define CLR_BIT(reg, bit) reg &= (~(1 << bit))
#define TOG_BIT(reg, bit) reg ^= (1 << bit)
#define Read_bIT(reg, bit) (reg & (1 << bit)) >> bit
#define IS_BIT_SET(reg, bit) (reg & (1 << bit)) >> bit
#define IS_BIT_CLR(reg, bit) !((reg & (1 << bit)) >> bit)
#define ROR(reg, num) reg = (reg << (REGISTER_SIZE - num)) | (reg >> (num))
#define ROL(reg, num) reg = (reg >> (REGISTER_SIZE - num)) | (reg << (num))

#endif /* STD_MACROS_H_ */