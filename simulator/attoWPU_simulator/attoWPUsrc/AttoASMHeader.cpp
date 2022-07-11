namespace attoWPU
{
	namespace assembler
	{
		char *attoASMHeader = "\
			ADDR {0}\
			CTRL {8}\
			DATA {16}\
			AJMP {48}\
			STOP {0}\
			\
			/* aPC */\
			APC_W {01H}\
			APC_O {02H}\
			APC_R {03H}\
			\
			/* Attocode memory /*\
			AM_AD {01H}\
			AM_OA {02H}\
			AM_OD {03H}\
			AM_WR {04H}\
			AM_WN {05H}\
			AM_WP {06H}\
			AM_NX {07H}\
			AM_PR {08H}\
			\
			/* TEMP register /*\
			TMP_WRM {01H}\
			TMP_ODM {02H}\
			TMP_WR  {03H}\
			TMP_OD  {04H}\
			TMP_WM  {05H}\
			TMP_OM  {06H}\
			TMP_ME  {07H}\
			TMP_MD  {08H}\
			TMP_CLR {09H}\
			TMP_FLL {0AH}\
			\
			/* Register memory */\
			RG_AD  {01H}\
			RG_AO  {02H}\
			RG_ODM {03H}\
			RG_WRM {04H}\
			RG_WNM {05H}\
			RG_WPM {06H}\
			RG_NX  {07H}\
			RG_PR  {08H}\
			RG_WM  {09H}\
			RG_OM  {0AH}\
			RG_ME  {0BH}\
			RG_MD  {0CH}\
			RG_OD  {0DH}\
			RG_WR  {0EH}\
			RG_WN  {0FH}\
			RG_WP  {10H}\
			RG_RES {11H}\
			\
			/* ALU */\
			ZERO {00H}\
			ADD  {01H}\
			SUB  {02H}\
			MULL {03H}\
			MULH {04H}\
			DIV  {05H}\
			REM  {06H}\
			CR   {07H}\
			BO   {08H}\
			SADD {09H}\
			SSUB {0AH}\
			SMULL{0BH}\
			SMULH{0CH}\
			SDIV {0DH}\
			SREM {0EH}\
			SCR  {0FH}\
			SBO  {10H}\
			ANDB {11H}\
			ORB  {12H}\
			NOTB {13H}\
			XORB {14H}\
			RL   {15H}\
			RR   {16H}\
			ANDL {17H}\
			ORL  {18H}\
			NOTL {19H}\
			XORL {1AH}\
			SL   {1BH}\
			SR   {1CH}\
			NAND {1DH}\
			NOR  {1EH}\
			BOOL {1FH}\
			MAX  {20H}\
			MAXN {21H}\
			MIN  {22H}\
			MINN {23H}\
			EQL  {24H}\
			ZSET {25H}\
			NZSET{26H}\
			\
			/* OUT register */\
			OUT_D {01H}\
			\
			/* FPU */\
			FADD  {01H}\
			FSUB  {02H}\
			FMUL  {03H}\
			FDIV  {04H}\
			FSIN  {05H}\
			FTAN  {06H}\
			FEXP  {07H}\
			FSQRT {08H}\
			FLOG2 {09H}\
			FLOG10{0AH}\
			FLN   {0BH}\
			FISINF{0CH}\
			FTOINT{0DH}\
			FTOFLT{0EH}\
			FMAX  {0FH}\
			FMAXN {10H}\
			FMIN  {11H}\
			FMINN {12H}\
			FABS  {13H}\
			FPOW  {14H}\
			\
			/* Memory controller */\
			M_WRL {01H}\
			M_OAL {02H}\
			M_OD  {03H}\
			M_WR  {04H}\
			M_WN  {05H}\
			M_WP  {06H}\
			M_NX  {07H}\
			M_PR  {08H}\
			M_WRH {09H}\
			M_OAH {0AH}\
			M_SZ  {0BH}\
			M_CL  {0CH}\
			M_32  {0DH}\
			M_16  {0EH}\
			M_8   {0FH}\
			M_4   {10H}\
			M_OCL {11H}\
			\
			/* Small queue */\
			SQ_AD {01H}\
			SQ_OA {02H}\
			SQ_ODM{03H}\
			SQ_WRM{04H}\
			SQ_NX {05H}\
			SQ_PR {06H}\
			SQ_OD {07H}\
			SQ_WR {08H}\
			SQ_ME {09H}\
			SQ_MD {0AH}\
			SQ_O  {0BH}\
			SQ_I  {0CH}\
			SQ_CD {0DH}\
			SQ_FL {0EH}\
			SQ_R  {0FH}\
			SQ_S  {10H}\
			\
			/* LED control */\
			LED_R0 {00H}\
			LED_R1 {01H}\
			LED_R2 {02H}\
			LED_R3 {03H}\
			\
			/* Text display controller */\
			TX_ADR {01H}\
			TX_WR  {02H}\
			TX_WN  {03H}\
			TX_NX  {04H}\
			TX_PR  {05H}\
			TX_OA  {06H}\
			TX_OD  {07H}\
			TX_R   {08H}\
			\
			/* LCD Display controller */\
			LCD_AD {01H}\
			LCD_WR {02H}\
			LCD_WN {03H}\
			LCD_NX {04H}\
			LCD_PR {05H}\
			LCD_AO {06H}\
			LCD_DO {07H}\
			LCD_R  {08H}\
			\
			/* Input controller */\
			IN_DO {01H}\
			IN_R0 {02H}\
			IN_R1 {03H}\
			IN_R2 {04H}\
			IN_R3 {05H}\
			IN_RN {06H}\
			IN_SN {07H}\
			IN_RK {08H}\
			IN_SK {09H}\
			\
			/* Timer controller */\
			TI_DO  {01H}\
			TI_TR0 {02H}\
			TI_TR1 {03H}\
			TI_TR2 {04H}\
			TI_TR3 {05H}\
			TI_TS0 {06H}\
			TI_TS1 {07H}\
			TI_TS2 {08H}\
			TI_TS3 {09H}\
			TI_TF0 {0AH}\
			TI_TF1 {0BH}\
			TI_TF2 {0CH}\
			TI_TF3 {0DH}\
			TI_AF0 {0EH}\
			TI_AF1 {0FH}\
			TI_AF2 {10H}\
			TI_AF3 {11H}\
			TI_OC0 {12H}\
			TI_OC1 {13H}\
			TI_OC2 {14H}\
			TI_OC3 {15H}\
			TI_OV0 {16H}\
			TI_OV1 {17H}\
			TI_OV2 {18H}\
			TI_OV3 {19H}\
			TI_ORT {1AH}\
			\
			/* END */";
	}
}
	