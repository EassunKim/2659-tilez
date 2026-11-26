/*------------------------------------------------------------
* File: types.h
*
* PURPOSE: Defines the types that will be used for out project (UINT8, UINT16,
*          UINT32, INT16, BOOL)
*
* Course:  COMP 2659
* Team Members: Matthew Kim, Ujjwal Bhandari
* Author: Matthew Kim
*------------------------------------------------------------*/

#ifndef TYPES_H
#define TYPES_H
 
/*unsigned values*/
typedef unsigned char  UINT8;
typedef unsigned int   UINT16; 
typedef unsigned long  UINT32; 

/*Signed values*/
typedef signed char INT8;
typedef int INT16;
typedef long INT32;

/*Boolean*/
typedef UINT8 BOOL;

#define FALSE 0
#define TRUE 1
 
#endif