//---------------------------------��ͷ�ļ��������ռ�������֡�----------------------------
//		����������������ʹ�õ�ͷ�ļ��������ռ�
//------------------------------------------------------------------------------------------------
#include "Data.h"
void CatchLawData();
void CatchData();
void StareTrain();
void KnowNUM();

RNG g_rng(12345);
//-----------------------------------��main( )������------------------------------------------
//		����������̨Ӧ�ó������ں��������ǵĳ�������￪ʼ
//-----------------------------------------------------------------------------------------------
char select='0';
void main()
{

	while(select!='5')
	{
		select='0';
		system("cls");
		printf("============����ʼ===========\n");
		printf("    �˵���\n");
		printf("         1:��׼���ݲɼ�\n");
		//printf("         2:���ݲɼ�\n");
		printf("         3:��ʼѵ��\n");
		printf("         4:ʶ��\n");
		printf("         5.�˳�\n");
		printf("    ѡ��");
		scanf_s("%c",&select);
		switch(select)
		{
		case '1':
			CatchLawData();
			StareTrain();
			break;
		/*case '2':
			CatchData();
			break;*/
		case '3':
			StareTrain();
			break;
		case '4':
			//NumberRec::init(0);
			//StareTrain();
			KnowNUM();
			break;
		}
	}
}
/*
�ɼ���׼����
*/
void CatchLawData()
{
	int witch=-1;
	while(witch!=10)
	{
		system("cls");
		printf("==========��׼���ݲɼ�=========\n");
		printf("    �˵���\n");
		printf("         0:�ɼ�����0\n");
		printf("         1:�ɼ�����1\n");
		printf("         2:�ɼ�����2\n");
		printf("         3:�ɼ�����3\n");
		printf("         4.�ɼ�����4\n");
		printf("         5:�ɼ�����5\n");
		printf("         6:�ɼ�����6\n");
		printf("         7:�ɼ�����7\n");
		printf("         8.�ɼ�����8\n");
		printf("         9:�ɼ�����9\n");
		printf("         10:�ɼ�������\n");
		printf("         11:����\n\n");
		printf("     ѡ��ɼ������֣�");
		getchar();
		scanf("%d",&witch);
		if(witch==11)
			break;
		if(witch==10)
			printf("��ʼ�ɼ�������\n");
		else
			printf("��ʼ�ɼ�����%d\n",witch);
		printf("���òɼ���������");
		int DataNum=0;
		scanf("%d",&DataNum);
		printf("���òɼ�����(200ms)��");
		int DataTime=0;
		scanf("%d",&DataTime);
		CollectData(DataNum,DataTime,witch,1);
		if(witch==10)
			printf("�����ֲɼ���ɣ�\n");
		else
			printf("����%d�ɼ���ɣ�\n",witch);
		system("pause");
	}
}
/*
�ռ�����
*/
void CatchData()
{
	FILE *f=fopen("train-data\\0\\num.txt","r");
	char c[20]={};
	fread(c,20,1,f);
	printf("%s\n%d\n",c,atoi(c));
	fclose(f);
	f=fopen("train-data\\0\\num.txt","w");
	char str[20]={};
	sprintf(str,"%d",21);
	fwrite(&str,20,1,f);
	fclose(f);
	system("pause");
}
///*
//��ʼѵ��
//*/
void StareTrain()
{
	printf("��ʼѵ����\n");
	NumberRec::init();
	printf("ѵ����ɣ�\n");
	system("pause");
}
/*
��ʼʶ��
*/
void KnowNUM()
{
	CollectData(0,3,0,0);
}