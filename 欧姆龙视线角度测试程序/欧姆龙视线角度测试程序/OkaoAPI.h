/*-------------------------------------------------------------------*/
/*  Copyright(C) 2003-2011 by OMRON Corporation                      */
/*  All Rights Reserved.                                             */
/*                                                                   */
/*   This source code is the Confidential and Proprietary Property   */
/*   of OMRON Corporation.  Any unauthorized use, reproduction or    */
/*   transfer of this software is strictly prohibited.               */
/*                                                                   */
/*-------------------------------------------------------------------*/
/* 
    OKAO_SDK Library API
*/
#pragma warning(disable:4996)
#ifndef OKAOCOMAPI_H__
#define OKAOCOMAPI_H__

#define OKAO_API
#include "OkaoDef.h"
#include "DetectionInfo.h"

#ifdef  __cplusplus
extern "C" {
#endif

/*----- Get Version -----*/
OKAO_API INT32      OKAO_GetVersion(UINT8 *pbyMajor, UINT8 *pbyMinor);

/*----- Memory Management -----*/
OKAO_API INT32      OKAO_SetWMemoryArea(VOID *pMemoryAddr, UINT32 unSize);                      /* Work Memory Area Setting */
OKAO_API INT32      OKAO_SetBMemoryArea(VOID *pMemoryAddr, UINT32 unSize);                      /* Backup Memory Area Setting */
OKAO_API INT32      OKAO_FreeWMemoryArea(void);             /* Release Work Memory Area */
OKAO_API INT32      OKAO_FreeBMemoryArea(void);             /* Release Backup Memory Area */

/*----- BGR Data --> Gray Data -----*/
OKAO_API INT32 OKAO_ImgConvertBGRtoGRAY(
    RAWIMAGE    *pSrcImage,         /* (I)  BGR Image Data      */
    INT32       nWidth,             /* (I)  Image Width         */
    INT32       nHeight,            /* (I)  Image Height        */
    RAWIMAGE    *pGrayImage);       /* (O)  Gray Scale Data     */

/*----- YUV422 Data --> GRAY Data -----*/
OKAO_API INT32 OKAO_ImgConvertYUV422toGRAY(
    RAWIMAGE    *pSrcImage,         /* (I)  YUV422 Image Data   */
    INT32       nWidth,             /* (I)  Image Width         */
    INT32       nHeight,            /* (I)  Image Height        */
    RAWIMAGE    *pGrayImage);       /* (O)  Gray Scale Data     */

/*----- BGR Data --> YUV422 Data -----*/
OKAO_API INT32 OKAO_ImgConvertBGRtoYUV422(
    RAWIMAGE    *pSrcImage,         /* (I) BGR Image Data       */
    INT32       nWidth,             /* (I) Image Width          */
    INT32       nHeight,            /* (I) Image Height         */
    RAWIMAGE    *pDstImage);        /* (O) YUV422 Image Data    */

/*----- YUV422 Data --> BGR Data -----*/
OKAO_API INT32 OKAO_ImgConvertYUV422toBGR(
    RAWIMAGE    *pSrcImage,         /* (I) YUV422 Image Data    */
    INT32       nWidth,             /* (I) Image Width          */
    INT32       nHeight,            /* (I) Image Height         */
    RAWIMAGE    *pDstImage);        /* (O) BGR Image Data       */

/*----- RGBAlpha Data --> GRAY Data -----*/
OKAO_API INT32 OKAO_ImgConvertRGBALPHAtoGRAY(
    RAWIMAGE    *pSrcImage,         /* (I)  RGBAlpha Image Data */
    INT32       nWidth,             /* (I)  Image Width         */
    INT32       nHeight,            /* (I)  Image Height        */
    RAWIMAGE    *pGrayImage);       /* (O)  Gray Scale Data     */

/*----- Conversion of Detection Information -----*/
OKAO_API INT32 OKAO_GetRectangleFromInfo(
    DETECTION_INFO *psDetectionInfo,/* (I)  Detection Information */
    POINT *ptLeftTop,               /* (O)  Top-left Corner */
    POINT *ptRightTop,              /* (O)  Top-right Corner */
    POINT *ptLeftBottom,            /* (O)  Bottom-left Corner */
    POINT *ptRightBottom);          /* (O)  Bottom-right Corner */

#ifdef  __cplusplus
}
#endif

#endif  /* OKAOCOMAPI_H__ */

