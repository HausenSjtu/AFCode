#include "stdio.h"
#include <stdlib.h>
//#include <string>
//using namespace std;

void main()
{
   FILE *fp1;//�����ļ���ָ�룬���ڴ򿪶�ȡ���ļ�
   FILE *fp2;//�����ļ���ָ�룬���ڴ�д�������ļ�
   FILE *fp3;
   char text[1024];//����һ���ַ������飬���ڴ洢��ȡ���ַ�
   char *s="Am I right?";
   fp1 = fopen("D:\\c_workspace\\test\\a.txt","r");//ֻ����ʽ���ļ�a.txt
   fp2 = fopen("D:\\c_workspace\\test\\b.txt","w");
   fp3 = fopen("D:\\c_workspace\\test\\c.txt","w");
   while(fgets(text,1024,fp1)!=NULL)//���ж�ȡfp1��ָ���ļ��е����ݵ�text��
   {
        puts(text);//�������Ļ
        fputs(text,fp2);//������д��fp2��ָ���ļ���
   }
   fprintf(fp3,"%s",s);         //++���ȼ�����*
   fclose(fp1);//�ر��ļ�a.txt���д򿪾�Ҫ�йر�
   fclose(fp2);//�ر��ļ�b.txt
   fclose(fp3);//�ر��ļ�b.txt
   system("pause");
}