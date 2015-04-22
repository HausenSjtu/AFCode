#include "stdio.h"
#include <stdlib.h>
//#include <string>
//using namespace std;

void main()
{
   FILE *fp1;//定义文件流指针，用于打开读取的文件
   FILE *fp2;//定义文件流指针，用于打开写操作的文件
   FILE *fp3;
   char text[1024];//定义一个字符串数组，用于存储读取的字符
   char *s="Am I right?";
   fp1 = fopen("D:\\c_workspace\\test\\a.txt","r");//只读方式打开文件a.txt
   fp2 = fopen("D:\\c_workspace\\test\\b.txt","w");
   fp3 = fopen("D:\\c_workspace\\test\\c.txt","w");
   while(fgets(text,1024,fp1)!=NULL)//逐行读取fp1所指向文件中的内容到text中
   {
        puts(text);//输出到屏幕
        fputs(text,fp2);//将内容写到fp2所指向文件中
   }
   fprintf(fp3,"%s",s);         //++优先级高于*
   fclose(fp1);//关闭文件a.txt，有打开就要有关闭
   fclose(fp2);//关闭文件b.txt
   fclose(fp3);//关闭文件b.txt
   system("pause");
}