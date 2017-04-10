//---------------------------------【头文件、命名空间包含部分】----------------------------
//		描述：包含程序所使用的头文件和命名空间
//------------------------------------------------------------------------------------------------
#include "Data.h"
void CatchLawData();
void CatchData();
void StareTrain();
void KnowNUM();

RNG g_rng(12345);
//-----------------------------------【main( )函数】------------------------------------------
//		描述：控制台应用程序的入口函数，我们的程序从这里开始
//-----------------------------------------------------------------------------------------------
char select='0';
void main()
{

	while(select!='5')
	{
		select='0';
		system("cls");
		printf("============程序开始===========\n");
		printf("    菜单：\n");
		printf("         1:标准数据采集\n");
		//printf("         2:数据采集\n");
		printf("         3:开始训练\n");
		printf("         4:识别\n");
		printf("         5.退出\n");
		printf("    选择：");
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
采集标准数据
*/
void CatchLawData()
{
	int witch=-1;
	while(witch!=10)
	{
		system("cls");
		printf("==========标准数据采集=========\n");
		printf("    菜单：\n");
		printf("         0:采集数字0\n");
		printf("         1:采集数字1\n");
		printf("         2:采集数字2\n");
		printf("         3:采集数字3\n");
		printf("         4.采集数字4\n");
		printf("         5:采集数字5\n");
		printf("         6:采集数字6\n");
		printf("         7:采集数字7\n");
		printf("         8.采集数字8\n");
		printf("         9:采集数字9\n");
		printf("         10:采集非数字\n");
		printf("         11:返回\n\n");
		printf("     选择采集的数字：");
		getchar();
		scanf("%d",&witch);
		if(witch==11)
			break;
		if(witch==10)
			printf("开始采集非数字\n");
		else
			printf("开始采集数字%d\n",witch);
		printf("设置采集样本数：");
		int DataNum=0;
		scanf("%d",&DataNum);
		printf("设置采集周期(200ms)：");
		int DataTime=0;
		scanf("%d",&DataTime);
		CollectData(DataNum,DataTime,witch,1);
		if(witch==10)
			printf("非数字采集完成！\n");
		else
			printf("数字%d采集完成！\n",witch);
		system("pause");
	}
}
/*
收集数据
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
//开始训练
//*/
void StareTrain()
{
	printf("开始训练：\n");
	NumberRec::init();
	printf("训练完成！\n");
	system("pause");
}
/*
开始识别
*/
void KnowNUM()
{
	CollectData(0,3,0,0);
}