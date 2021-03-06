/***************************************************************************
* Copyright (c) Date: Mon Nov 24 16:26:00 CST 2008 QUALCOMM INCORPORATED 
* All Rights Reserved 
* Modified by QUALCOMM INCORPORATED on Mon Nov 24 16:26:00 CST 2008 
****************************************************************************/ 

#include "typedef.h"
#include "hexagon_protos.h"
#include "hexagon_types.h"

/*___________________________________________________________________________
  | NOTE:                                                                    |
  |  The following functions will not match the ETSI standard definitions    | 
  |  at certain corner cases.                                                |
  |   - L_mac, L_msu, mac_r, msu_r                                           |
  |                                                                          |
  |  All 32-bit multiplications yield more precision than ETSI standard      |
  |  definitions                                                             |
  |__________________________________________________________________________|
 */

/*___________________________________________________________________________
  |                                                                          |
  |   Constants and Globals                                                  |
  |                                                                          |
  |__________________________________________________________________________|
 */

#define MAX_64      (Word64)0x7fffffffffffffffLL
#define MIN_64      (Word64)0x8000000000000000LL

#define MAX_32      (Word32)0x7fffffffL
#define MIN_32      (Word32)0x80000000L

#define MAX_16      (Word16)+32767 /* 0x7fff */
#define MIN_16      (Word16)-32768 /* 0x8000 */

/*___________________________________________________________________________
  |                                                                          |
  |   Prototypes for basic arithmetic operators                              |
  |__________________________________________________________________________|
 */

//----------------------------------------------------------------------
// Word16 extract_h(Word32 L_var1);    
// extract high 16-bits
//----------------------------------------------------------------------
#define   extract_h(x)       ((Word16)Q6_R_asrh_R( (x) ))       

//----------------------------------------------------------------------
// Word16 extract_l(Word32 L_var1);  
// extract low 16-bits
//----------------------------------------------------------------------
#define   extract_l(x)       ((Word16)(x))

//----------------------------------------------------------------------
// Word16 add(Word16 var1, Word16 var2); 
// short add with saturation
//----------------------------------------------------------------------
#define   add(x, y)          ((Word16)Q6_R_add_RlRl_sat( (x), (y) )) 

//----------------------------------------------------------------------
// Word16 sub(Word16 var1, Word16 var2);  
// short sub with saturation
//----------------------------------------------------------------------
#define   sub(x, y)          ((Word16)Q6_R_sub_RlRl_sat( (x), (y) )) 

//----------------------------------------------------------------------
// Word16 negate(Word16 var1);               
// Short negate
//----------------------------------------------------------------------
#define   negate(x)          ((Word16)Q6_R_sath_R(-Q6_R_sxth_R((x)))) 

//----------------------------------------------------------------------
// Word16 avg(Word16 var1, Word16 var2);               
// Short average 
//----------------------------------------------------------------------
#define   avg(x, y)          ((Word16)Q6_P_vavgh_PP_rnd((x),(y))) 

//----------------------------------------------------------------------
// Word16 avg(Word16 var1, Word16 var2);               
// Short average negative
//----------------------------------------------------------------------
#define   navg(x, y)          ((Word16)Q6_P_vnavgh_PP_rnd_sat((x),(y))) 

//----------------------------------------------------------------------
// Word16 shr(Word16 var1, Word16 var2);   
// short shift right with saturation
//----------------------------------------------------------------------
#define   shr(x,n)           ((Word16)Q6_R_sath_R(Q6_R_asr_RR_sat(Q6_R_sxth_R((x)),(n))))

//----------------------------------------------------------------------
// Word16 shl(Word16 var1, Word16 var2);   
// short shift left with saturation
//----------------------------------------------------------------------
#define   shl(x,n)           ((Word16)Q6_R_sath_R(Q6_R_asl_RR_sat(Q6_R_sxth_R((x)),(n))))

//----------------------------------------------------------------------
// Word16 shr_r(Word16 var1, Word16 var2);
// shift right by const with round
//----------------------------------------------------------------------
//#define   shr_r(x,c)         ((Word16)Q6_R_asr_RI_rnd(Q6_R_sxth_R((x)),(c)-1))

//----------------------------------------------------------------------
// Word16 mult(Word16 var1, Word16 var2);    
// Short fractional mult
//----------------------------------------------------------------------
#define   mult(x, y)         ((Word16)Q6_R_asrh_R(Q6_R_mpy_RlRl_s1_sat( (x), (y) )))

//----------------------------------------------------------------------
// Word16 mult_r(Word16 var1, Word16 var2);  
// Short fractional mult with round
//----------------------------------------------------------------------
#define   mult_r(x, y)       ((Word16)Q6_R_asrh_R(Q6_R_mpy_RlRl_s1_rnd_sat( (x), (y) )))

//----------------------------------------------------------------------
// Word16 mac_r(Word32 L_var3, Word16 var1, Word16 var2);
// Mac with rounding
//----------------------------------------------------------------------
#define   mac_r(z,x,y)       (rnd(L_mac( (z), (x), (y) )))

//----------------------------------------------------------------------
// Word16 msu_r(Word32 L_var3, Word16 var1, Word16 var2);
// Msu with rounding
//----------------------------------------------------------------------
#define   msu_r(z,x,y)       (rnd(L_msu( (z), (x), (y) )))

//----------------------------------------------------------------------
// Word16 abs_s(Word16 var1);       
// short abs
//----------------------------------------------------------------------
#define   abs_s(x)           ((Word16)Q6_R_sath_R(Q6_R_abs_R( (x) ))) 

//----------------------------------------------------------------------
// Word16 rnd(Word32 L_var1);  
// round 32-bit to 16-bit
//----------------------------------------------------------------------
#if __HEXAGON_ARCH__ < 4
#define   rnd(x)             ((Word16)Q6_R_sath_R(Q6_R_asrrnd_RI((x),16)))
#else
#define   rnd(x)             (Q6_R_round_RI_sat((x),16))
#endif

//----------------------------------------------------------------------
// Word16 sature(Word32 L_var1); 
// sature 32-bit integer to 16-bit
//----------------------------------------------------------------------
#define   saturate(x)          ((Word16)Q6_R_sath_R( (x) ))

//----------------------------------------------------------------------
// Word16 norm_s(Word16 var1);  
// Short norm,    
//----------------------------------------------------------------------
#define   norm_s(x)          (Q6_R_normamt_R(Q6_R_aslh_R( (x) )))

//----------------------------------------------------------------------
// Word16 max(Word16 x, Word16 y);
// max
//----------------------------------------------------------------------
#define   max(x, y)          ((Word16)Q6_R_max_RR( (Word32)(x), (Word32)(y) ))

//----------------------------------------------------------------------
// Word16 min(Word16 x, Word16 y);
// min
//----------------------------------------------------------------------
#define   min(x, y)          ((Word16)Q6_R_min_RR( (Word32)(x), (Word32)(y) ))

//----------------------------------------------------------------------
// Word32 L_deposit_h(Word16 var1);   
// 16 bit var1 -> MSB,      
//----------------------------------------------------------------------
#define   L_deposit_h(x)     (Q6_R_aslh_R( (x) ))

//----------------------------------------------------------------------
// Word32 L_deposit_l(Word16 var1);       
// 16 bit var1 -> LSB,      
//----------------------------------------------------------------------
#define   L_deposit_l(x)     (Q6_R_sxth_R( (x) ))

//----------------------------------------------------------------------
// Word32 combine(Word16 h, Word16 l);
// deposit h and l as MSB and LSB of long 
//----------------------------------------------------------------------
#define   combine(h, l)      (Q6_R_combine_RlRl( (h), (l) ))

//----------------------------------------------------------------------
// Word32 L_add(Word32 L_var1, Word32 L_var2);   
// Long add with sat 
//----------------------------------------------------------------------
#define   L_add(x, y)        (Q6_R_add_RR_sat( (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_sub(Word32 L_var1, Word32 L_var2);  
// Long sub with sat
//----------------------------------------------------------------------
#define   L_sub(x, y)        (Q6_R_sub_RR_sat( (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_negate(Word32 L_var1);              
// Long negate with sat
//----------------------------------------------------------------------
#define   L_negate(x)        (Q6_R_neg_R_sat( (x) ))

//----------------------------------------------------------------------
// Word32 L_avg(Word32 L_var1, Word32 L_var2);   
// Long average with rnd 
//----------------------------------------------------------------------
#define   L_avg(x, y)        ((Word32)Q6_P_vavgw_PP_rnd( (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_navg(Word32 L_var1, Word32 L_var2);   
// Long average with rnd and sat
//----------------------------------------------------------------------
#define   L_navg(x, y)        ((Word32)Q6_P_vnavgw_PP_rnd_sat( (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_mult(Word16 var1, Word16 var2);  
// Long fractional mult (with sat)
//----------------------------------------------------------------------
#define   L_mult(x, y)       (Q6_R_mpy_RlRl_s1_sat( (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_mac(Word32 L_var3, Word16 var1, Word16 var2); 
// Long fractional MAC (with sat)
//----------------------------------------------------------------------
#define   L_mac(z,x,y)       (Q6_R_mpyacc_RlRl_s1_sat( (z), (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_msu(Word32 L_var3, Word16 var1, Word16 var2); 
// mult and sub  (with sat
//----------------------------------------------------------------------
#define   L_msu(z,x,y)       (Q6_R_mpynac_RlRl_s1_sat( (z), (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_msu(Word32 L_var3, Word16 var1, Word16 var2); 
// mult and sub  (with sat
//----------------------------------------------------------------------
#define   L_dmac(z,x,y,u,v)  ((Word32)Q6_P_vdmpyacc_PP_s1_sat( (z),combine((x),(u)),combine((y),(v)) ))

//----------------------------------------------------------------------
// Word32 L_macNs(Word32 L_var3, Word16 var1, Word16 var2);
// Mac without sat
//----------------------------------------------------------------------
#define   L_macNs(z,x,y)     (Q6_R_mpyacc_RlRl_s1( (z), (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_msuNs(Word32 L_var3, Word16 var1, Word16 var2);
// Msu without sat
//----------------------------------------------------------------------
#define   L_msuNs(z,x,y)     (Q6_R_mpynac_RlRl_s1( (z), (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_shl(Word32 L_var1, Word16 var2); 
// Long shift left with sat 
//----------------------------------------------------------------------
#define   L_shl(x,n)         (Q6_R_asl_RR_sat( (x),(n) ))

//----------------------------------------------------------------------
// Word32 L_shr(Word32 L_var1, Word16 var2); 
// Long shift right with sat  
//----------------------------------------------------------------------
#define   L_shr(x,n)         (Q6_R_asr_RR_sat( (x),(n) ))

//----------------------------------------------------------------------
// Word32 L_shr_r(Word32 L_var1, Word16 var2);
// Long shift right by const with round
//----------------------------------------------------------------------
//#define   L_shr_r(x,c)       (Q6_R_asr_RI_rnd((x),((c)-1)))

//----------------------------------------------------------------------
// Word32 L_asl(Word32 L_var1, Word16 var2); 
// Long shift left without sat 
//----------------------------------------------------------------------
#define   L_asl(x,n)         (Q6_R_asl_RR( (x),(n) ))

//----------------------------------------------------------------------
// Word32 L_asr(Word32 L_var1, Word16 var2); 
// Long shift right without sat  
//----------------------------------------------------------------------
#define   L_asr(x,n)         (Q6_R_asr_RR( (x),(n) ))

//----------------------------------------------------------------------
// Word32 L_abs(Word32 L_var1);   
// Long abs with sat
//----------------------------------------------------------------------
#define   L_abs(x)           (Q6_R_abs_R_sat( (x) ))

//----------------------------------------------------------------------
// Word16 norm_l(Word32 L_var1);    
// long norm,    
//----------------------------------------------------------------------
#define   norm_l(x)          (Q6_R_normamt_R( (x) ))

//----------------------------------------------------------------------
// Word32 L_max(Word32 x, Word32 y);
// max
//----------------------------------------------------------------------
#define   L_max(x, y)          (Q6_R_max_RR( (x), (y) ))

//----------------------------------------------------------------------
// Word32 L_min(Word32 x, Word32 y);
// min
//----------------------------------------------------------------------
#define   L_min(x, y)          (Q6_R_min_RR( (x), (y) ))


/*___________________________________________________________________________
  |                                                                          |
  |   Prototypes for basic 32-bit multiplication operators                   |
  |__________________________________________________________________________|
 */

//----------------------------------------------------------------------
// Word32 L_mult32x16(Word32 L_var1, Word16 var2);
// 32x16 multiply (fractional) without rounding
//----------------------------------------------------------------------
#if __HEXAGON_ARCH__ < 4
#define   L_mult32x16(x, y)  ((Word32)Q6_P_vmpyweh_PP_s1_sat( (x), (y) ))
#else
#define   L_mult32x16(x, y)  (Q6_R_mpy_RRl_s1_sat( (x), (y) ))    
#endif

//----------------------------------------------------------------------
// Word32 L_mult32x16r(Word32 L_var1, Word16 var2);
// 32x16 multiply (fractional) with rounding
//----------------------------------------------------------------------
#define   L_mult32x16r(x, y) (Q6_R_mpy_RRl_s1_rnd_sat( (x), (y) ))    

//----------------------------------------------------------------------
// Word32 L_mult32(Word32 L_var1, Word32 L_var2);     
// 32x32 fractional multiply with sat 
//----------------------------------------------------------------------
#if __HEXAGON_ARCH__ < 4
#define   L_mult32(x, y)     (Q6_R_sat_P(Q6_P_asr_PI(Q6_P_mpy_RR((x), (y)),31)))  
#else
#define   L_mult32(x, y)     (Q6_R_mpy_RR_s1_sat((x), (y)))
#endif

//----------------------------------------------------------------------
// Word32 L_mac32(Word32, L_var3, Word32 L_var1, Word32 L_var2);     
// 32x32 fractional multiply and accumulation with sat 
//----------------------------------------------------------------------
#if __HEXAGON_ARCH__ < 4
#define   L_mac32(z, x, y)   (Q6_R_sat_P((Word64)(z)+Q6_P_asr_PI(Q6_P_mpy_RR((x), (y)),31)))
#else
#define   L_mac32(z, x, y)     (Q6_R_mpyacc_RR_s1_sat((z), (x), (y)))
#endif

//----------------------------------------------------------------------
// Word32 L_msu32(Word32, L_var3, Word32 L_var1, Word32 L_var2);     
// 32x32 fractional multiply and negative accumulation with sat 
//----------------------------------------------------------------------
#if __HEXAGON_ARCH__ < 4
#define   L_msu32(z, x, y)   (Q6_R_sat_P((Word64)(z)-Q6_P_asr_PI(Q6_P_mpy_RR((x), (y)),31)))
#else
#define   L_msu32(z, x, y)     (Q6_R_mpynac_RR_s1_sat((z), (x), (y)))
#endif

//----------------------------------------------------------------------
// Word32 L_imult32h(Word32 L_var1, Word32 L_var2);     
// 32x32 integer multiply and  take upper result(no sat)
//----------------------------------------------------------------------
#define   L_imult32h(x, y)   (Q6_R_mpy_RR( (x), (y) ))  

//----------------------------------------------------------------------
// Word32 L_imult32hr(Word32 L_var1, Word32 L_var2);     
// 32x32 integer multiply and  take upper result( with round)
//----------------------------------------------------------------------
#define   L_imult32hr(x, y)  (Q6_R_mpy_RR_rnd( (x), (y) ))  

//----------------------------------------------------------------------
// Word32 L_imult32uh(Word32 var1, Word32 var2);     
// unsigned 32x32 integer multiply and  take upper result(no sat)
//----------------------------------------------------------------------
//#define   L_imult32uh(x, y)  (Q6_R_mpyu_RR( (x), (y) ))  

//----------------------------------------------------------------------
// Word64  LL_imult32(Word32 x, Word32 y);
// signed 32x32 multiply with 64-bit result 
//----------------------------------------------------------------------
#define   LL_imult32(x, y)    (Q6_P_mpy_RR((x),(y)))

//----------------------------------------------------------------------
// UWord64  LL_imult32u(UWord32 x, UWord32 y);
// unsigned 32x32 multiply with 64-bit result 
//----------------------------------------------------------------------
#define   LL_imult32u(x, y)   (Q6_P_mpyu_RR((x),(y)))


/*___________________________________________________________________________
  |                                                                          |
  |   Prototypes for basic 64-bit arithmetic operators                       |
  |__________________________________________________________________________|
 */

//----------------------------------------------------------------------
// Word32 L_sat(Word64 L_var);              
// saturate 64 bit to 32 bit               
//----------------------------------------------------------------------
#define   L_sat(x)           (Q6_R_sat_P( (x) ))

//----------------------------------------------------------------------
// Word32 L_extract_h32(Word64 LL_var1);              
// extract 32 MSB of a 64 bit word
//----------------------------------------------------------------------
#define L_extract_h32(x)     HEXAGON_V64_GET_W1((x))

//----------------------------------------------------------------------
// Word32 L_extract_h32(Word64 LL_var1);              
// extract 32 MSB of a 64 bit word
//----------------------------------------------------------------------
#define L_extract_l32(x)     HEXAGON_V64_GET_W0((x))

//----------------------------------------------------------------------
// Word64 L_combine(Word32 h, Word32 l);
// deposit x and y as MSB and LSB of a long long 
//----------------------------------------------------------------------
#define   L_combine(h, l)      ((Word64)HEXAGON_V64_CREATE_W( (h), (l) ))

//----------------------------------------------------------------------
// Word64  LL_shl(Word64 x, Word32 n);
// long long shift left 
//----------------------------------------------------------------------
#define   LL_shl(x, n)       (Q6_P_asl_PR((x),(n)))

//----------------------------------------------------------------------
// Word64  LL_shr(Word64 x, Word32 n);
// long long shift right 
//----------------------------------------------------------------------
#define   LL_shr(x, n)       (Q6_P_asr_PR((x),(n)))


/*___________________________________________________________________________
  |                                                                          |
  |   Prototypes for complex arithmetic operators                            |
  |__________________________________________________________________________|
 */
#define real(L_var1)                (extract_l((L_var1)))                   /* Extract real part -- low  16bits */
#define imag(L_var1)                (extract_h((L_var1)))                   /* Extract imag part -- high 16bits */
#define complex(var_r, var_i)       (combine((var_i),(var_r)))              /* Construct complex number         */
#define L_real(LL_var1)             (L_extract_l32((LL_var1)))              /* Extract real part -- low  32bits */
#define L_imag(LL_var1)             (L_extract_h32((LL_var1)))              /* Extract real part -- low  32bits */
#define L_complex(L_var_r,L_var_i)  (L_combine((L_var_i),(L_var_r)))        /* Construct long complex number    */

//----------------------------------------------------------------------
// CWord2x16 conjugate(CWord2x16 x);     
// Complex add (with sat)
//----------------------------------------------------------------------
#define   conjugate(x)         ((CWord2x16)Q6_P_vconj_P_sat( (x) ))

//----------------------------------------------------------------------
// CWord2x16 cadd(CWord2x16 var1, CWord2x16 var2);     
// Complex add (with sat)
//----------------------------------------------------------------------
#define   cadd(x, y)         (Q6_R_vaddh_RR_sat( (x), (y) ))

//----------------------------------------------------------------------
// CWord2x16 csub(CWord2x16 var1, CWord2x16 var2);     
// Complex sub (with sat)
//----------------------------------------------------------------------
#define   csub(x, y)         (Q6_R_vsubh_RR_sat( (x), (y) ))

//----------------------------------------------------------------------
// Word2x16 cmult_r(Word2x16 var1, Word2x16 var2);     
// Complex mult and round
//----------------------------------------------------------------------
#define   cmult_r(x, y)      (Q6_R_cmpy_RR_s1_rnd_sat( (x), (y) )) 

//----------------------------------------------------------------------
// Word2x16 cmult_cr(Word2x16 var1, Word2x16 var2);     
// Complex mult
//----------------------------------------------------------------------
#define   cmult_cr(x, y)     (Q6_R_cmpy_RR_conj_s1_rnd_sat( (x), (y) )) 

//----------------------------------------------------------------------
// CWord2x32 L_cmult(Word2x16 var1, Word2x16 var2);     
// Complex mult
//----------------------------------------------------------------------
#define   L_cmult(x,y)       (Q6_P_cmpy_RR_s1_sat( (x), (y) )) 

//----------------------------------------------------------------------
// CWord2x32 L_cmult_c(CWord2x16 x, CWord2x16 y);     
// L_cmpy(var1, conj(var2))
//----------------------------------------------------------------------
#define   L_cmult_c          Q6_P_cmpy_RR_conj_s1_sat 

//----------------------------------------------------------------------
// CWord2x32 L_cmac(CWord2x32 var, CWord2x16 x, CWord2x16 y);     
// Complex mac
//----------------------------------------------------------------------
#define   L_cmac             Q6_P_cmpyacc_RR_s1_sat 

//----------------------------------------------------------------------
// CWord2x32 L_cmac_c(CWord2x32 var, CWord2x16 x, CWord2x16 y);     
// L_cmac(var, var1, conj(var2))
//----------------------------------------------------------------------
#define   L_cmac_c           Q6_P_cmpyacc_RR_conj_s1_sat 

//----------------------------------------------------------------------
// CWord2x32 L_cmsu(CWord2x32 var, CWord2x16 x, CWord2x16 y);     
// Complex msu
//----------------------------------------------------------------------
#define   L_cmsu             Q6_P_cmpynac_RR_s1_sat 

//----------------------------------------------------------------------
// CWord2x32 L_cmsu_c(CWord2x32 var, CWord2x16 x, CWord2x16 y);     
// L_cmsu(var, var1, conj(var2))
//----------------------------------------------------------------------
#define   L_cmsu_c           Q6_P_cmpynac_RR_conj_s1_sat 


/*___________________________________________________________________________
  |                                                                          |
  |   Prototypes for spefical operators                                      |
  |__________________________________________________________________________|
 */

//----------------------------------------------------------------------
// Word16 clb(Word32 x);
// count leading bits
//----------------------------------------------------------------------
#define   clb(x)             (Q6_R_clb_R( (x) ))

//----------------------------------------------------------------------
// Word16 cl0(Word32 x);
// count leading zero bits
//----------------------------------------------------------------------
#define   cl0(x)             (Q6_R_cl0_R( (x) ))

//----------------------------------------------------------------------
// UWord32 bitreverse(UWord32 x);
// bit reverse
//----------------------------------------------------------------------
#define   bitreverse(x)          (Q6_R_brev_R( (x) ))

//----------------------------------------------------------------------
// UWord32 parity(UWord64 x, UWord64 y)
// Get parity sum
//----------------------------------------------------------------------
#define   parity(x, y)       (Q6_R_parity_PP((Word64)(x), (Word64)(y)))


/*___________________________________________________________________________
  |                                                                          |
  |   Prototypes for vector operators                                        |
  |__________________________________________________________________________|
 */

//----------------------------------------------------------------------
// Word64 L_Vadd(Word64 x, Word64 y);
// Vector add long
//----------------------------------------------------------------------
#define   L_Vadd             Q6_P_vaddw_PP_sat

//----------------------------------------------------------------------
// Word64 L_Vadd(Word64 x, Word64 y);
// Vector sub long
//----------------------------------------------------------------------
#define   L_Vsub             Q6_P_vsubw_PP_sat

//----------------------------------------------------------------------
// Word64 L_Vasl(Word64 x, Word32 n)
// Vector shift right long
//----------------------------------------------------------------------
#define   L_Vasr             Q6_P_vasrw_PR

//----------------------------------------------------------------------
// Word64 L_Vasl(Word64 x, Word32 n)
// Vector shift left long
//----------------------------------------------------------------------
#define   L_Vasl             Q6_P_vaslw_PR

//----------------------------------------------------------------------
// Word64 L_Vsxt(Word32 x)
// Vector sign extend short to long
//----------------------------------------------------------------------
#define   L_Vsxt             Q6_P_vsxthw_R     

//----------------------------------------------------------------------
// Word32 L_Vrnd(Word64 x)
// Vector rnd long to short
//----------------------------------------------------------------------
#define   Vrnd               Q6_R_vrndwh_P_sat

//----------------------------------------------------------------------
// Word32 L_Vsat(Word64 x)
// Vector sat long to short
//----------------------------------------------------------------------
#define   Vsat               Q6_R_vsatwh_P

//----------------------------------------------------------------------
// Word64 L_Vmpy32x16(Word64 x, Word64 y);
// Vector 32x16 mult
//----------------------------------------------------------------------
//#define   L_Vmpy32x16        Q6_P_vmpyweh_PP_s1_rnd_sat

//----------------------------------------------------------------------
// Word64 L_Vmpy32x16u(Word64 x, Word64 y);
// Vector 32-bit sign mult by 16-bit unsigned
//----------------------------------------------------------------------
//#define   L_Vmpy32x16u       Q6_P_vmpyweuh_PP_s1_rnd_sat

//----------------------------------------------------------------------
// Word32 L_Vextract_l(Word64 x, Word64 y)
// Vector extract low
//----------------------------------------------------------------------
//#define   Vextract_l         Q6_P_vtrunewh_PP

//----------------------------------------------------------------------
// Word32 L_Vextract_h(Word64 x, Word64 y)
// Vector extract high
//----------------------------------------------------------------------
//#define   Vextract_h         Q6_P_vtrunowh_PP



