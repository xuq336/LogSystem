/*
    C语言实现简单log日志记录功能
    实现log日志记录功能，一种宏定义形式实现，一种函数形式实现。记录于此。
*/
#ifndef __LOG_H__
#define __LOG_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>

enum {ERROR,WARNING,INFO,DEBUG};

//宏定义形式
#define log(log_level, ...) \
    do { \
        time_t current_time = time(NULL); \
        char file_name[64] = {'\0'}; \
        strftime(file_name, sizeof(file_name), "%Y-%m-%d-log.txt", localtime(&current_time)); \
        char log_str[64] = {'\0'};\
        strftime(log_str, sizeof(log_str), "%Y-%m-%d %H:%M:%S", localtime(&current_time));\
        printf("[%s] (%s %s:%d)\n", log_str, __FILE__, __func__, __LINE__);\
        switch(log_level)\
        {\
        case ERROR:\
            printf("[error] ");\
            break;\
        case WARNING:\
            printf("[warning] ");\
            break;\
        case INFO:\
            printf("[info] ");\
            break;\
        case DEBUG:\
            printf("[debug] ");\
            break;\
        default:\
            break;\
        }\
        printf(__VA_ARGS__);\
        printf("\n");\
        fflush(stdout);\
        FILE *fp = NULL; \
        fp = fopen(file_name,"a+"); \
        if(fp != NULL) \
        { \
            fprintf(fp, "[%s] (%s %s:%d)\n", log_str, __FILE__, __func__, __LINE__);\
            switch(log_level)\
            {\
            case ERROR:\
                fprintf(fp, "[error] ");\
                break;\
            case WARNING:\
                fprintf(fp, "[warning] ");\
                break;\
            case INFO:\
                fprintf(fp, "[info] ");\
                break;\
            case DEBUG:\
                fprintf(fp, "[debug] ");\
                break;\
            default:\
                break;\
            }\
            fprintf(fp, __VA_ARGS__);\
            fprintf(fp, "\n");\
            fclose(fp); \
        } \
    }while(0);

//函数形式
//void log(int error_level, const char *format, ...)
//{
//    va_list args;
//    FILE *fp=NULL;
//    char time_str[32];
//    char file_name[256];

//    va_start (args, format);
//    time_t time_log = time(NULL);
//    strftime(file_name,sizeof(file_name),"%Y-%m-%d_log_history.log",localtime(&time_log));

//    if((fp=fopen(file_name,"a+"))!=NULL)
//    {
//        strftime(time_str,sizeof(time_str),"%Y-%m-%d %H:%M:%S",localtime(&time_log));
//        if(error_level==(int)ERROR)
//        {
//            fprintf (fp, "[%s]-[%s]-[%s]-[%d] :> ",time_str,"ERROR",__FILE__,__LINE__);
//            vfprintf (fp,format,args);
//            fprintf (fp,"\n");

//        }
//        else if(error_level==(int)WARNING)
//        {
//            fprintf (fp, "[%s]-[%s]-[%s]-[%d] :> ",time_str,"WARNINGs",__FILE__,__LINE__);
//            vfprintf (fp,format,args);
//            fprintf (fp,"\n");
//        }
//        else if(error_level==(int)INFO)
//        {
//            fprintf (fp, "[%s]-[%s]-[%s]-[%d] :> ",time_str,"INFO",__FILE__,__LINE__);
//            vfprintf (fp,format,args);
//            fprintf (fp,"\n");
//        }
//        else if(error_level==(int)DEBUG)
//        {
//            fprintf (fp, "[%s]-[%s]-[%s]-[%d] :> ",time_str,"DEBUG",__FILE__,__LINE__);
//            vfprintf (fp,format,args);
//            fprintf (fp,"\n");
//        }
//        fclose(fp);
//    }

//    va_end (args);
//}


#ifdef __cplusplus
}
#endif
#endif
