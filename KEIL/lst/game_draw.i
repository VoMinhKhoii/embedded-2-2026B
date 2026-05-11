# 1 "game_draw.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 400 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "game_draw.c" 2
# 1 ".\\game_draw.h" 1



# 1 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdint.h" 1 3
# 56 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdint.h" 3
typedef signed char int8_t;
typedef signed short int int16_t;
typedef signed int int32_t;
typedef signed long long int int64_t;


typedef unsigned char uint8_t;
typedef unsigned short int uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long int uint64_t;





typedef signed char int_least8_t;
typedef signed short int int_least16_t;
typedef signed int int_least32_t;
typedef signed long long int int_least64_t;


typedef unsigned char uint_least8_t;
typedef unsigned short int uint_least16_t;
typedef unsigned int uint_least32_t;
typedef unsigned long long int uint_least64_t;




typedef signed int int_fast8_t;
typedef signed int int_fast16_t;
typedef signed int int_fast32_t;
typedef signed long long int int_fast64_t;


typedef unsigned int uint_fast8_t;
typedef unsigned int uint_fast16_t;
typedef unsigned int uint_fast32_t;
typedef unsigned long long int uint_fast64_t;






typedef signed int intptr_t;
typedef unsigned int uintptr_t;



typedef signed long long intmax_t;
typedef unsigned long long uintmax_t;
# 5 ".\\game_draw.h" 2






extern uint16_t currentScore; // Declare currentScore as extern

extern uint8_t level; // Declare level as extern
extern uint8_t minutes; // Declare as extern
extern uint8_t seconds; // Declare as extern
extern char acString[32]; // This should be large enough to hold the score, level, and time

extern uint16_t currentHighScore; // NEW: highest ever this session
void DrawStoneBorder(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void DisplayGameField(void);
void StartGameField(void);
void GameOverField(void);
void LeaderBoard(void);

void AddToLeaderboard(uint16_t score,
                      uint8_t level,
                      uint8_t minutes,
                      uint8_t seconds);
# 2 "game_draw.c" 2
# 1 "../../task2-complete\\EBI_LCD_Module.h" 1
# 28 "../../task2-complete\\EBI_LCD_Module.h"
// Characters
extern uint8_t Font8x16[];
extern uint16_t Font16x32[];
extern uint8_t minutes; // minutes counter
extern uint8_t seconds; // seconds counter
extern uint8_t timer_running; // 0 = paused (splash/Game Over), 1 = play clock

// Sub-functions
void ILI9341_Initial(void);
void Timer3_Init(void);
void LCD_WR_REG(uint16_t cmd);
void LCD_WR_DATA(uint16_t dat);
uint16_t LCD_RD_DATA(void);
void LCD_SetWindow(uint16_t x_s, uint16_t x_e, uint16_t y_s, uint16_t y_e);
void LCD_PutString(uint16_t x, uint16_t y, uint8_t *s, uint32_t fColor, uint32_t bColor);
void LCD_PutChar16x32(uint16_t x, uint16_t y, uint16_t c, uint32_t fColor, uint32_t bColor);
void LCD_BlankArea(uint16_t X, uint16_t Y, uint16_t W, uint16_t H, uint16_t color);
uint16_t Get_TP_X(void);
uint16_t Get_TP_Y(void);
void TimerDelay_Start(uint8_t ticks);
uint8_t TimerDelay_Done(void);
# 3 "game_draw.c" 2
# 1 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 1 3
# 53 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
    typedef unsigned int size_t;
# 68 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
    typedef __builtin_va_list __va_list;
# 87 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
typedef struct __fpos_t_struct {
    unsigned long long int __pos;





    struct {
        unsigned int __state1, __state2;
    } __mbstate;
} fpos_t;
# 108 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
typedef struct __FILE FILE;
# 119 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
struct __FILE {
    union {
        long __FILE_alignment;



        char __FILE_size[84];

    } __FILE_opaque;
};
# 138 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern FILE __stdin, __stdout, __stderr;
extern FILE *__aeabi_stdin, *__aeabi_stdout, *__aeabi_stderr;
# 224 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int remove(const char * ) __attribute__((__nonnull__(1)));







extern __attribute__((__nothrow__)) int rename(const char * , const char * ) __attribute__((__nonnull__(1,2)));
# 243 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) FILE *tmpfile(void);






extern __attribute__((__nothrow__)) char *tmpnam(char * );
# 265 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int fclose(FILE * ) __attribute__((__nonnull__(1)));
# 275 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int fflush(FILE * );
# 285 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) FILE *fopen(const char * __restrict ,
                           const char * __restrict ) __attribute__((__nonnull__(1,2)));
# 329 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) FILE *freopen(const char * __restrict ,
                    const char * __restrict ,
                    FILE * __restrict ) __attribute__((__nonnull__(2,3)));
# 342 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) void setbuf(FILE * __restrict ,
                    char * __restrict ) __attribute__((__nonnull__(1)));






extern __attribute__((__nothrow__)) int setvbuf(FILE * __restrict ,
                   char * __restrict ,
                   int , size_t ) __attribute__((__nonnull__(1)));
# 370 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
#pragma __printf_args
extern __attribute__((__nothrow__)) int fprintf(FILE * __restrict ,
                    const char * __restrict , ...) __attribute__((__nonnull__(1,2)));
# 393 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
#pragma __printf_args
extern __attribute__((__nothrow__)) int _fprintf(FILE * __restrict ,
                     const char * __restrict , ...) __attribute__((__nonnull__(1,2)));





#pragma __printf_args
extern __attribute__((__nothrow__)) int printf(const char * __restrict , ...) __attribute__((__nonnull__(1)));






#pragma __printf_args
extern __attribute__((__nothrow__)) int _printf(const char * __restrict , ...) __attribute__((__nonnull__(1)));





#pragma __printf_args
extern __attribute__((__nothrow__)) int sprintf(char * __restrict , const char * __restrict , ...) __attribute__((__nonnull__(1,2)));








#pragma __printf_args
extern __attribute__((__nothrow__)) int _sprintf(char * __restrict , const char * __restrict , ...) __attribute__((__nonnull__(1,2)));






#pragma __printf_args
extern __attribute__((__nothrow__)) int __ARM_snprintf(char * __restrict , size_t ,
                     const char * __restrict , ...) __attribute__((__nonnull__(3)));
# 460 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
#pragma __printf_args
extern __attribute__((__nothrow__)) int _snprintf(char * __restrict , size_t ,
                      const char * __restrict , ...) __attribute__((__nonnull__(3)));





#pragma __scanf_args
extern __attribute__((__nothrow__)) int fscanf(FILE * __restrict ,
                    const char * __restrict , ...) __attribute__((__nonnull__(1,2)));
# 503 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
#pragma __scanf_args
extern __attribute__((__nothrow__)) int _fscanf(FILE * __restrict ,
                     const char * __restrict , ...) __attribute__((__nonnull__(1,2)));





#pragma __scanf_args
extern __attribute__((__nothrow__)) int scanf(const char * __restrict , ...) __attribute__((__nonnull__(1)));








#pragma __scanf_args
extern __attribute__((__nothrow__)) int _scanf(const char * __restrict , ...) __attribute__((__nonnull__(1)));





#pragma __scanf_args
extern __attribute__((__nothrow__)) int sscanf(const char * __restrict ,
                    const char * __restrict , ...) __attribute__((__nonnull__(1,2)));
# 541 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
#pragma __scanf_args
extern __attribute__((__nothrow__)) int _sscanf(const char * __restrict ,
                     const char * __restrict , ...) __attribute__((__nonnull__(1,2)));
# 555 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int _vfscanf(FILE * __restrict , const char * __restrict , __va_list) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) int _vscanf(const char * __restrict , __va_list) __attribute__((__nonnull__(1)));
extern __attribute__((__nothrow__)) int _vsscanf(const char * __restrict , const char * __restrict , __va_list) __attribute__((__nonnull__(1,2)));
extern __attribute__((__nothrow__)) int __ARM_vsscanf(const char * __restrict , const char * __restrict , __va_list) __attribute__((__nonnull__(1,2)));

extern __attribute__((__nothrow__)) int vprintf(const char * __restrict , __va_list ) __attribute__((__nonnull__(1)));







extern __attribute__((__nothrow__)) int _vprintf(const char * __restrict , __va_list ) __attribute__((__nonnull__(1)));





extern __attribute__((__nothrow__)) int vfprintf(FILE * __restrict ,
                    const char * __restrict , __va_list ) __attribute__((__nonnull__(1,2)));
# 584 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int vsprintf(char * __restrict ,
                     const char * __restrict , __va_list ) __attribute__((__nonnull__(1,2)));
# 594 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int __ARM_vsnprintf(char * __restrict , size_t ,
                     const char * __restrict , __va_list ) __attribute__((__nonnull__(3)));
# 609 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int _vsprintf(char * __restrict ,
                      const char * __restrict , __va_list ) __attribute__((__nonnull__(1,2)));





extern __attribute__((__nothrow__)) int _vfprintf(FILE * __restrict ,
                     const char * __restrict , __va_list ) __attribute__((__nonnull__(1,2)));





extern __attribute__((__nothrow__)) int _vsnprintf(char * __restrict , size_t ,
                      const char * __restrict , __va_list ) __attribute__((__nonnull__(3)));
# 635 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
#pragma __printf_args
extern __attribute__((__nothrow__)) int __ARM_asprintf(char ** , const char * __restrict , ...) __attribute__((__nonnull__(2)));
extern __attribute__((__nothrow__)) int __ARM_vasprintf(char ** , const char * __restrict , __va_list ) __attribute__((__nonnull__(2)));
# 649 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int fgetc(FILE * ) __attribute__((__nonnull__(1)));
# 659 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) char *fgets(char * __restrict , int ,
                    FILE * __restrict ) __attribute__((__nonnull__(1,3)));
# 673 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int fputc(int , FILE * ) __attribute__((__nonnull__(2)));
# 683 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int fputs(const char * __restrict , FILE * __restrict ) __attribute__((__nonnull__(1,2)));






extern __attribute__((__nothrow__)) int getc(FILE * ) __attribute__((__nonnull__(1)));
# 704 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
    extern __attribute__((__nothrow__)) int (getchar)(void);
# 713 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) char *gets(char * ) __attribute__((__nonnull__(1)));
# 725 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int putc(int , FILE * ) __attribute__((__nonnull__(2)));
# 737 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
    extern __attribute__((__nothrow__)) int (putchar)(int );






extern __attribute__((__nothrow__)) int puts(const char * ) __attribute__((__nonnull__(1)));







extern __attribute__((__nothrow__)) int ungetc(int , FILE * ) __attribute__((__nonnull__(2)));
# 778 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) size_t fread(void * __restrict ,
                    size_t , size_t , FILE * __restrict ) __attribute__((__nonnull__(1,4)));
# 794 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) size_t __fread_bytes_avail(void * __restrict ,
                    size_t , FILE * __restrict ) __attribute__((__nonnull__(1,3)));
# 810 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) size_t fwrite(const void * __restrict ,
                    size_t , size_t , FILE * __restrict ) __attribute__((__nonnull__(1,4)));
# 822 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int fgetpos(FILE * __restrict , fpos_t * __restrict ) __attribute__((__nonnull__(1,2)));
# 833 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int fseek(FILE * , long int , int ) __attribute__((__nonnull__(1)));
# 850 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int fsetpos(FILE * __restrict , const fpos_t * __restrict ) __attribute__((__nonnull__(1,2)));
# 863 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) long int ftell(FILE * ) __attribute__((__nonnull__(1)));
# 877 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) void rewind(FILE * ) __attribute__((__nonnull__(1)));
# 886 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) void clearerr(FILE * ) __attribute__((__nonnull__(1)));







extern __attribute__((__nothrow__)) int feof(FILE * ) __attribute__((__nonnull__(1)));




extern __attribute__((__nothrow__)) int ferror(FILE * ) __attribute__((__nonnull__(1)));




extern __attribute__((__nothrow__)) void perror(const char * );
# 917 "C:\\Users\\Admin\\AppData\\Local\\Keil_v5\\ARM\\ARMCLANG\\bin\\..\\include\\stdio.h" 3
extern __attribute__((__nothrow__)) int _fisatty(FILE * ) __attribute__((__nonnull__(1)));



extern __attribute__((__nothrow__)) void __use_no_semihosting_swi(void);
extern __attribute__((__nothrow__)) void __use_no_semihosting(void);
# 4 "game_draw.c" 2
uint16_t currentScore = 0;
uint8_t level = 1;
uint16_t currentHighScore = 0;
typedef struct {
    uint16_t score;
    uint8_t level;
    uint8_t minutes;
    uint8_t seconds;
} LeaderEntry;
static LeaderEntry leaderboard[100];
static uint8_t leaderboard_count = 0;
char acString[32];
void AddToLeaderboard(uint16_t score,
                      uint8_t level,
                      uint8_t minutes,
                      uint8_t seconds)
{
    LeaderEntry newEntry;
    uint8_t pos;
    uint8_t i;


    newEntry.score = score;
    newEntry.level = level;
    newEntry.minutes = minutes;
    newEntry.seconds = seconds;


    pos = leaderboard_count;
    for (i = 0; i < leaderboard_count; i++) {
        if (score > leaderboard[i].score) {
            pos = i;
            break;
        }
    }


    if (leaderboard_count < 100) {

        for (i = leaderboard_count; i > pos; i--) {
            leaderboard[i] = leaderboard[i - 1];
        }
        leaderboard[pos] = newEntry;
        leaderboard_count++;
    }
    else {
        if (pos < 100) {
            for (i = 99; i > pos; i--) {
                leaderboard[i] = leaderboard[i - 1];
            }
            leaderboard[pos] = newEntry;
        }

    }


    if (score > currentHighScore) {
        currentHighScore = score;
    }
}

void DrawStoneBorder(uint16_t x, uint16_t y, uint16_t w, uint16_t h)
{
    uint16_t i, j, color;
    for (i = 0; i + 10 <= w; i += 10) {
        color = ((i / 10) & 1) ? 0x6B4D : 0x7BEF;
        LCD_BlankArea(x+i,y,10,10,color);
    }
    for (i = 0; i + 10 <= w; i += 10) {
        color = ((i / 10) & 1) ? 0x7BEF : 0x6B4D;
        LCD_BlankArea(x+i,y+h- 10,10,10,color);
    }
    for (j = 0; j + 10 <= h; j += 10) {
        color = ((j / 10) & 1) ? 0x6B4D : 0x7BEF;
        LCD_BlankArea(x,y+j,10,10,color);
    }
    for (j = 0; j + 10 <= h; j += 10) {
        color = ((j / 10) & 1) ? 0x7BEF : 0x6B4D;
        LCD_BlankArea(x+w- 10,y+j,10,10,color);
    }
}

void DisplayGameField(void)
{
  char hiString[10];
    LCD_BlankArea(0,0,240,320,0x0000);
    DrawStoneBorder(0, 0, 240, 320);
    DrawStoneBorder(0, 0, 160, 320);
    DrawStoneBorder(150, 0, 90, 100);
    DrawStoneBorder(150, 0, 90, 245);
    LCD_PutString(179, 10, (uint8_t*)"NEXT", 0xFFFF, 0x0000);
    LCD_PutString(167,100, (uint8_t*)"HISCORE",0xFFFF, 0x0000);
    LCD_PutString(176,135, (uint8_t*)"SCORE", 0xFFFF, 0x0000);
    LCD_PutString(178,165, (uint8_t*)"TIME", 0xFFFF, 0x0000);
    LCD_PutString(176,195, (uint8_t*)"LEVEL", 0xFFFF, 0x0000);
    LCD_PutString(173,245, (uint8_t*)"SW1 TO", 0xFFFF, 0x0000);
    LCD_PutString(176,260, (uint8_t*)"PAUSE", 0xFFFF, 0x0000);
    LCD_PutString(173,275, (uint8_t*)"SW2 TO", 0xFFFF, 0x0000);
    LCD_PutString(171,290, (uint8_t*)"RESTART",0xFFFF, 0x0000);

    sprintf(hiString, "%u", currentHighScore);
    LCD_PutString(176, 115, (uint8_t*)hiString, 0xFFFF, 0x0000);
}

void StartGameField(void)
{
  LCD_BlankArea(0, 0, 240 // LCD Width at pixels, 320 // LCD Hight at pixels, 0x0000);
  LCD_PutString(85,10,(uint8_t*)"TETRIS ", 0xFFE0, 0x0000);
  LCD_PutString(0,160,(uint8_t*)"\tPress SW1 to start", 0xFFE0, 0x0000);
  LCD_PutString(0,234,(uint8_t*)"Nguyen Tri Khai - s3921136", 0x051F, 0x0000);
  LCD_PutString(0,250,(uint8_t*)"Le Nguyen Hao - s3963228", 0x051F, 0x0000);
}

void GameOverField(void)
{
    LCD_BlankArea(0, 0, 240 // LCD Width at pixels, 320 // LCD Hight at pixels, 0x0000);
    LCD_PutString(80,10,(uint8_t*)"GAME OVER", 0xFFE0, 0x0000);
    LCD_PutString(70,250,(uint8_t*)"Press SW2 to restart", 0xFFE0, 0x0000);
    LCD_PutString(35,300,(uint8_t*)"Press SW1 for LeaderBoard", 0xFFE0, 0x0000);
    // Display the final score, level, and time
    char timeString[6];
    sprintf(timeString, "%02d:%02d", minutes, seconds);
    LCD_PutString(105, 100, (uint8_t*)"TIME", 0xFFFF, 0x0000);
    LCD_PutString(105, 130, (uint8_t*)timeString, 0xFFFF, 0x0000);
    LCD_PutString(25, 100, (uint8_t*)"SCORE", 0xFFFF, 0x0000);
    sprintf(acString, "%d", currentScore);
    LCD_PutString(25, 130, (uint8_t*)acString, 0xFFFF, 0x0000);
    LCD_PutString(185, 100, (uint8_t*)"LEVEL", 0xFFFF, 0x0000);
    sprintf(acString, "%d", level);
    LCD_PutString(185, 130, (uint8_t*)acString, 0xFFFF, 0x0000);
}





void LeaderBoard(void)
{
    char entryString[32];
    uint8_t i;
    uint8_t toShow;
    uint16_t y;


    LCD_BlankArea(0, 0, 240 // LCD Width at pixels, 320 // LCD Hight at pixels, 0x0000);


    LCD_PutString(70, 10, (uint8_t*)"LEADERBOARD", 0xFFE0, 0x0000);


    LCD_PutString( 10, 30, (uint8_t*)"Rank", 0xFFFF, 0x0000);
    LCD_PutString(55,30, (uint8_t*)"Score", 0xFFFF, 0x0000);
    LCD_PutString(120, 30, (uint8_t*)"Level", 0xFFFF, 0x0000);
    LCD_PutString(180, 30, (uint8_t*)"Time", 0xFFFF, 0x0000);


    if (leaderboard_count < 10) {
        toShow = leaderboard_count;
    } else {
        toShow = 10;
    }


    for (i = 0; i < toShow; i++)
    {
        y = 50 + (uint16_t)i * 20;


        sprintf(entryString, "%2u", (uint16_t)(i + 1));
        LCD_PutString( 10, y, (uint8_t*)entryString, 0xFFFF, 0x0000);


        sprintf(entryString, "%5u", leaderboard[i].score);
        LCD_PutString( 55, y, (uint8_t*)entryString, 0xFFFF, 0x0000);


        sprintf(entryString, "L%1u", leaderboard[i].level);
        LCD_PutString(120, y, (uint8_t*)entryString, 0xFFFF, 0x0000);


        sprintf(entryString, "%02u:%02u",
                leaderboard[i].minutes,
                leaderboard[i].seconds);
        LCD_PutString(180, y, (uint8_t*)entryString, 0xFFFF, 0x0000);
    }


    LCD_PutString( 60, 300, (uint8_t*)"SW2 TO RESTART", 0xFFFF, 0x0000);
}
