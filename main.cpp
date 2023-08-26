#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <windows.h>
#include <numeric>



#include "time.h"

#include "Allmethod.h"
#include "rand.h"
#include "NEH.h"



#include "ES.h"
#include "SIG.h"
#include "CIG.h"
#include "MNIG.h"
#include "DPSO.h"




//主函数

int main()
{
	srand(time(0));
	ReadInstanceFileNameList("..\\Benchmark\\");
	//GenerateInstances();						 //生成一次就行

	int CPU = 2;           //CPU控制系数
	int Psize = 20;
	float Mcoefficien = 0.0;

	vector<vector<int>>EC_Factory_CIG;
	EC_Factory_CIG.resize(90);
	for (int ins = 0; ins < EC_Factory_CIG.size(); ins++)
		EC_Factory_CIG[ins].resize(20, 0);

	vector<vector<int>>EC_Factory_MNIG;
	EC_Factory_MNIG.resize(90);
	for (int ins = 0; ins < EC_Factory_MNIG.size(); ins++)
		EC_Factory_MNIG[ins].resize(20, 0);

	vector<vector<int>>EC_Factory_DPSO;
	EC_Factory_DPSO.resize(90);
	for (int ins = 0; ins < EC_Factory_DPSO.size(); ins++)
		EC_Factory_DPSO[ins].resize(20, 0);

	vector<vector<int>>EC_Factory_ES;
	EC_Factory_ES.resize(90);
	for (int ins = 0; ins < EC_Factory_ES.size(); ins++)
		EC_Factory_ES[ins].resize(20, 0);

	vector<vector<int>>EC_Factory_SIG;
	EC_Factory_SIG.resize(90);
	for (int ins = 0; ins < EC_Factory_SIG.size(); ins++)
		EC_Factory_SIG[ins].resize(20, 0);



	cout << "Test_CPU=" << CPU << endl;
	cout << "算法从左到右依次为：CIG, MN_IG, DPSO, ES, SIG " << endl;

	string FileDirectory = "Result\\";    //
	ostringstream str;
	str << "Test_Algorithms_cpu=" << CPU << ".txt";    //不同的算法
	ofstream ofile;
	ofile.open(FileDirectory + str.str());

	string FileDirectory_RPI = "Result\\";    //
	ostringstream str_RPI;
	str_RPI << "RPI_CPU=" << CPU << ".txt";    //不同的算法
	ofstream RPIfile;
	RPIfile.open(FileDirectory_RPI + str_RPI.str());

	for (int Ins = 0; Ins < 90; Ins++)
	{
		ReadInstances(Ins);
		InitVector();

		cout << " 工厂：" << gFactory << " 工件：" << gJobs << " 阶段：" << gStage << endl;

		long TimeLimit = CPU * gJobs * gStage;
		double min_CIG = INT_MAX;
		double min_SIG = INT_MAX;
		double min_DPSO = INT_MAX;
		double min_MNIG = INT_MAX;
		double min_ES = INT_MAX;

		double min_EC = INT_MAX;     //寻找最小指标

		vector<double> RPI;
		RPI.resize(12);

		ofile << "Jobs:" << gJobs << " factory:" << gFactory << " Stage:" << gStage << "\n";          //输出到 Test_Algorithms_cpu 文件中
		ofile << "从左到右算法依次为：" << " CIG," << " MNIG," << " DPSO," << " ES," << " SIG ";

		ofile << "\n";
		int ECsum_CIG = 0;
		int ECavg_CIG = 0;
		for (int i = 0; i < 10; i++)        //每个测试用例执行10遍
		{
			srand(i + 100 + Ins);
			EC_Factory_CIG[Ins][i] = CIG(TimeLimit, 7, Mcoefficien, 0.5, 0.8, 0.5);
			ofile << EC_Factory_CIG[Ins][i] << "\t";

			ECsum_CIG += EC_Factory_CIG[Ins][i];

			if (EC_Factory_CIG[Ins][i] < min_CIG)   
				min_CIG = EC_Factory_CIG[Ins][i];

			if (min_CIG < min_EC)   //寻找所有算法的最小的目标值，计算RPI
				min_EC = min_CIG;
		}
		ECavg_CIG = ECsum_CIG / 10;

		ofile << "\n";
		int ECsum_MNIG = 0;
		int ECavg_MNIG = 0;
		for (int i = 0; i < 10; i++)        //每个测试用例执行10遍
		{
			srand(i + 100 + Ins);
			EC_Factory_MNIG[Ins][i] = MNIG(TimeLimit, 4, Mcoefficien);
			ofile << EC_Factory_MNIG[Ins][i] << "\t";

			ECsum_MNIG += EC_Factory_MNIG[Ins][i];

			if (EC_Factory_MNIG[Ins][i] < min_MNIG)
				min_MNIG = EC_Factory_MNIG[Ins][i];

			if (min_MNIG < min_EC)   //寻找所有算法的最小的目标值，计算RPI
				min_EC = min_MNIG;
		}
		ECavg_MNIG = ECsum_MNIG / 10;

		ofile << "\n";
		int ECsum_DPSO = 0;
		int ECavg_DPSO = 0;
		for (int i = 0; i < 10; i++)        //每个测试用例执行10遍
		{
			srand(i + 100 + Ins);
			EC_Factory_DPSO[Ins][i] = DPSO(TimeLimit, 50, 200, 1.1, 1.1);
			ofile << EC_Factory_DPSO[Ins][i] << "\t";

			ECsum_DPSO += EC_Factory_DPSO[Ins][i];

			if (EC_Factory_DPSO[Ins][i] < min_DPSO)
				min_DPSO = EC_Factory_DPSO[Ins][i];

			if (min_DPSO < min_EC)   //寻找所有算法的最小的目标值，计算RPI
				min_EC = min_DPSO;
		}
		ECavg_DPSO = ECsum_DPSO / 10;

		ofile << "\n";
		int ECsum_ES = 0;
		int ECavg_ES = 0;
		for (int i = 0; i < 10; i++)        //每个测试用例执行10遍
		{
			srand(i + 100 + Ins);
			EC_Factory_ES[Ins][i] = ES(TimeLimit, 7, 5);
			ofile << EC_Factory_ES[Ins][i] << "\t";

			ECsum_ES += EC_Factory_ES[Ins][i];

			if (EC_Factory_ES[Ins][i] < min_ES)
				min_ES = EC_Factory_ES[Ins][i];

			if (min_ES < min_EC)   //寻找所有算法的最小的目标值，计算RPI
				min_EC = min_ES;
		}
		ECavg_ES = ECsum_ES / 10;

		ofile << "\n";
		int ECsum_SIG = 0;
		int ECavg_SIG = 0;
		for (int i = 0; i < 10; i++)        //每个测试用例执行10遍
		{
			srand(i + 100 + Ins);
			EC_Factory_SIG[Ins][i] = SIG(TimeLimit, CPU);
			ofile << EC_Factory_SIG[Ins][i] << "\t";

			ECsum_SIG += EC_Factory_SIG[Ins][i];

			if (EC_Factory_SIG[Ins][i] < min_SIG)
				min_SIG = EC_Factory_SIG[Ins][i];

			if (min_SIG < min_EC)   //寻找所有算法的最小的目标值，计算RPI
				min_EC = min_SIG;
		}
		ECavg_SIG = ECsum_SIG / 10;

		//输出各算法的RPI
		RPIfile << "Jobs:" << gJobs << " factory:" << gFactory << " Stage:" << gStage << "\n";
		RPIfile << "从左到右算法依次为：" << " CIG," << " MNIG," << " DPSO," << " ES," << " SIG " << "\n";
		RPI[0] = (ECavg_CIG- min_EC) / min_EC * 100;
		RPIfile << RPI[0] << "\t";
		RPI[1] = (ECavg_MNIG - min_EC) / min_EC * 100;
		RPIfile << RPI[1] << "\t";
		RPI[2] = (ECavg_DPSO - min_EC) / min_EC * 100;
		RPIfile << RPI[2] << "\t";
		RPI[3] = (ECavg_ES - min_EC) / min_EC * 100;
		RPIfile << RPI[3] << "\t";
		RPI[4] = (ECavg_SIG - min_EC) / min_EC * 100;
		RPIfile << RPI[4] << "\t";

		RPIfile << endl;
		cout << RPI[0] << "\t" << RPI[1] << "\t" << RPI[2] << "\t" << RPI[3] << "\t" << RPI[4] << endl;
		cout << endl;
		ofile << endl;
	}

	RPIfile.close();
	ofile.close();

	return 0;
}


