#pragma once
#include<vector>
#include<algorithm>
#include "rand.h"
using namespace std;

// -------------for ordering----------
typedef struct
{
	int dim;
	int value;
} Pair;

class PairGreater {
public:
	bool operator () (Pair a, Pair b)    //函数对象 也称之为伪函数
	{
		return a.value > b.value;
	}
};

class PairLess {
public:
	bool operator () (Pair a, Pair b)
	{
		return a.value < b.value;
	}
};
//-------------end ----------------

//--------------------------------------------------
int Jobs[6] = { 50,100,150,200,250,300 };
int Stages[3] = { 5, 8, 10 };
int Factories[5] = { 2, 3, 4, 5, 6};
//-----------------------------------------------------

int gJobs, gFactory, gStage;
vector<string> InstanceFileNameList;

vector<vector<int>> PTime;
vector<vector<vector<int>>> pSetupTime;
vector<int> pMachines;
vector<vector<int>> IdleTime;
vector<vector<int>> BlockTime;
vector<vector<int>> IdleConsumption; //IdleConsumption(i,j) the idle-energy consumption per unit time of the machine j at stage i
vector<vector<int>> BlockConsumption; //BlockConsumption(i,j) the  blocking consumption of the machine j at stage i per unit time
vector<vector<int>> PTimeConsumption; //IdleConsumption(i,j) the  Idle consumption of the machine j at stage i per unit time

vector<vector<int>> STime, CTime;
vector<vector<int>> RecordJobBlockTime;
vector<vector<int>> mSIdleTime; //机器开始空闲的时间
vector<vector<vector<int>>> gRecordMidle, gBRecordMidle;
vector<vector<vector<int>>> gTempRecordMidle, gTempBRecordMidle;
vector<vector<vector<int>>> tempRecordMidle, tempBRecordMidle;

int gBestValue = INT_MAX;

//---------------------------------------------------------------------------------//

//生成实例
void GenerateInstances()   
{
	ifstream insfile;  //wy:用于打开设置实例的文件
	insfile.open("..\\Benchmark\\TestInstances.txt", ios::in);

	ofstream ofile1, ofile2;
	ofile1.open("..\\Benchmark\\InstanceFileNameList.txt");

	for (int Ins = 0; Ins < 90; Ins++)
	{
		srand(Ins + 1234);
		insfile >> gFactory >> gJobs >> gStage;
		PTime.resize(gJobs);
		for (int j = 0; j < gJobs; j++)
		{
			PTime[j].resize(gStage);
		}

		for (int i = 0; i < gJobs ; i++)
		{
			for (int j = 0; j < gStage; j++)
			{
				PTime[i][j] = 1 + rand() % 30; //[1-30]
			}
		}

		pMachines.resize(gStage);
		for (int i = 0; i < gStage; i++)
		{
			pMachines[i] = 1 + rand() % 3; //range of machines 
		}

		PTimeConsumption.resize(gStage);      //单位时间内的处理时间的能耗
		for (int k = 0; k < gStage; k++)
		{
			PTimeConsumption[k].resize(pMachines[k]);   //初始化内存
			PTimeConsumption[k][0] = 5 + rand() % 2;   //[5-6]
			for (int i = 0; i < pMachines[k]; i++)
			{
				PTimeConsumption[k][i] = PTimeConsumption[k][0];
			}
		}

		IdleConsumption.resize(gStage);       //单位时间内的空闲时间的能耗
		for (int k = 0; k < gStage; k++)
		{
			IdleConsumption[k].resize(pMachines[k]);
			IdleConsumption[k][0] = 3 + rand() % 2;   //[3-4]
			for (int i = 0; i < pMachines[k]; i++)
			{
				IdleConsumption[k][i] = IdleConsumption[k][0];
			}
		}

		BlockConsumption.resize(gStage);			 //单位时间内的阻塞时间的能耗
		for (int k = 0; k < gStage; k++)
		{
			BlockConsumption[k].resize(pMachines[k]);
			BlockConsumption[k][0] = 1 + rand() % 2;   //[1-2]
			for (int i = 0; i < pMachines[k]; i++)
			{
				BlockConsumption[k][i] = BlockConsumption[k][0];
			}
		}

		ostringstream FileName;
		FileName << "INS" << Ins << "_" << gFactory << "_" << gJobs << "_" << gStage << ".txt";
		ofile1 << Ins << "\t" << FileName.str() << endl;

		ofile2.open("..\\Benchmark\\" + FileName.str());

		ofile2 << "Factories" << "\t" << gFactory << endl;
		ofile2 << "gJobs" << "\t" << gJobs << endl;
		ofile2 << "gStage" << "\t" << gStage << endl;

		ofile2 << "Number of Machines at each stage" << endl;
		for_each(begin(pMachines), end(pMachines), [&](auto v)
			{
				ofile2 << v << "\t";
			});

		ofile2 << endl;

		ofile2 << "Processing times of jobs" << endl;
		for_each(begin(PTime), end(PTime), [&](const auto& vec)
			{
				for_each(begin(vec), end(vec), [&](auto v)
					{
						ofile2 << v << "\t";
					});
				ofile2 << endl;
			});

		ofile2 << "Energy consumption per unit  time for processing" << endl;
		for_each(begin(PTimeConsumption), end(PTimeConsumption), [&](const auto& vec)
			{
				for_each(begin(vec), end(vec), [&](auto v)
					{
						ofile2 << v << "\t";
					});
				ofile2 << endl;
			});

		ofile2 << "Energy consumption per unit  time for idle" << endl;
		for_each(begin(IdleConsumption), end(IdleConsumption), [&](const auto& vec)
			{
				for_each(begin(vec), end(vec), [&](auto v)
					{
						ofile2 << v << "\t";
					});
				ofile2 << endl;
			});

		ofile2 << "Energy consumption per unit  time for blocking" << endl;
		for_each(begin(BlockConsumption), end(BlockConsumption), [&](const auto& vec)
			{
				for_each(begin(vec), end(vec), [&](auto v)
					{
						ofile2 << v << "\t";
					});
				ofile2 << endl;
			});

		ofile2.close();
	}
	ofile1.close();
}

void ReadInstanceFileNameList(string Dir)
{
	InstanceFileNameList.clear();
	ifstream ifile;
	ifile.open(Dir + "\\" + "InstanceFileNameList.txt");
	while (true)
	{
		int x;
		string FName;
		ifile >> x >> FName;
		if (ifile.peek() != EOF)
		{
			InstanceFileNameList.push_back(Dir + "\\" + FName);
		}
		else
		{
			break;
		}
	}
	ifile.close();
}

void ReadInstances(int InsNo)
{
	ifstream ifile;
	ifile.open(InstanceFileNameList[InsNo]);
	string Str;

	ifile >> Str >> gFactory;
	ifile >> Str >> gJobs;
	ifile >> Str >> gStage;

	getline(ifile, Str);
	getline(ifile, Str);
	pMachines.resize(gStage);
	for_each(begin(pMachines), end(pMachines), [&](auto& v)
		{
			ifile >> v;
		});

	getline(ifile, Str);
	getline(ifile, Str);
	PTime.resize(gJobs);
	for (int j = 0; j < gJobs; j++)
	{
		PTime[j].resize(gStage);
	}

	for_each(begin(PTime), end(PTime), [&](auto& vec)
		{
			for_each(begin(vec), end(vec), [&](auto& v)
				{
					ifile >> v;
				});
		});

	getline(ifile, Str);
	getline(ifile, Str);
	PTimeConsumption.resize(gStage);     
	for (int k = 0; k < gStage; k++)
	{
		PTimeConsumption[k].resize(pMachines[k]);   //初始化内存
	}

	for_each(begin(PTimeConsumption), end(PTimeConsumption), [&](auto& vec)
		{
			for_each(begin(vec), end(vec), [&](auto& v)
				{
					ifile >> v;
				});
		});

	getline(ifile, Str);
	getline(ifile, Str);
	IdleConsumption.resize(gStage);
	for (int k = 0; k < gStage; k++)
	{
		IdleConsumption[k].resize(pMachines[k]);   //初始化内存
	}

	for_each(begin(IdleConsumption), end(IdleConsumption), [&](auto& vec)
		{
			for_each(begin(vec), end(vec), [&](auto& v)
				{
					ifile >> v;
				});
		});

	getline(ifile, Str);
	getline(ifile, Str);
	BlockConsumption.resize(gStage);
	for (int k = 0; k < gStage; k++)
	{
		BlockConsumption[k].resize(pMachines[k]);   //初始化内存
	}

	for_each(begin(BlockConsumption), end(BlockConsumption), [&](auto& vec)
		{
			for_each(begin(vec), end(vec), [&](auto& v)
				{
					ifile >> v;
				});
		});

	ifile.close();
}

//初始化vector
void InitVector()
{
	STime = vector<vector<int>>(gStage, vector<int>(gJobs));
	CTime = vector<vector<int>>(gStage, vector<int>(gJobs));

	mSIdleTime.resize(gStage);
	for (int k = 0; k < gStage; k++)
	{
		mSIdleTime[k].resize(pMachines[k]);
	}

	IdleTime.resize(gStage);
	for (int k = 0; k < gStage; k++)
	{
		IdleTime[k].resize(pMachines[k]);
	}

	BlockTime.resize(gStage);
	for (int k = 0; k < gStage; k++)
	{
		BlockTime[k].resize(pMachines[k]);
	}

	RecordJobBlockTime.resize(gStage);
	for (int k = 0; k < gStage; k++)
	{
		RecordJobBlockTime[k].resize(gJobs);
	}

	gRecordMidle.resize(gJobs);
	//gBRecordMidle.resize(gJobs);
	tempRecordMidle.resize(gJobs);
	//tempBRecordMidle.resize(gJobs);
	gTempRecordMidle.resize(gJobs);
	//gTempBRecordMidle.resize(gJobs);

	for (int j = 0; j < gJobs; j++)
	{
		gRecordMidle[j].resize(gStage);
		//gBRecordMidle[j].resize(gStage);
		tempRecordMidle[j].resize(gStage);
		//tempBRecordMidle[j].resize(gStage);
		gTempRecordMidle[j].resize(gStage);
		//gTempBRecordMidle[j].resize(gStage);
	}
	for (int j = 0; j < gJobs; j++)
	{
		for (int k = 0; k < gStage; k++)
		{
			gRecordMidle[j][k].resize(pMachines[k]);
			//gBRecordMidle[j][k].resize(pMachines[k]);
			tempRecordMidle[j][k].resize(pMachines[k]);
			//tempBRecordMidle[j][k].resize(pMachines[k]);
			gTempRecordMidle[j][k].resize(pMachines[k]);
			//gTempBRecordMidle[j][k].resize(pMachines[k]);
		}
	}
}

//------------------------------生成span上界------------------------------------

int Get_OneFactory_span(const vector<int>& Seq)
{
	if (Seq.size() == 0)
	{
		return 0;
	}

	for (auto& vec : mSIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : STime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : CTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	int mpt1 = -1;

	for (int j = 0; j < Seq.size(); ++j)
	{
		int cur_job = Seq[j];

		for (int k = 0; k < gStage; ++k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//选出最先空闲机器
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (mSIdleTime[k][i] < minIdle)
				{
					minIdle = mSIdleTime[k][i];
					mpt = i;
				}
			}

			if (k == 0)
			{
				//第1阶段机器开始的时间
				STime[k][cur_job] = minIdle;
			}
			else
			{
				//其他阶段
				STime[k][cur_job] = max(minIdle, CTime[k - 1][cur_job]);
				mSIdleTime[k - 1][mpt1] = STime[k][cur_job];
			}
			CTime[k][cur_job] = STime[k][cur_job] + PTime[cur_job][k];
			mSIdleTime[k][mpt] = CTime[k][cur_job];
			mpt1 = mpt;
		}
	}

	int Makespan = -1;
	for (int i = 0; i < pMachines[gStage - 1]; ++i)
	{
		Makespan = max(Makespan, mSIdleTime[gStage - 1][i]);
	}

	return Makespan;
}

int Get_OneFac_span_After_Insert_Job(const vector<int>& Seq, int job, int pos)
{
	int Makespan = 0;
	if (Seq.size() == 0)
		Makespan = 0;
	else
	{
		for (auto& vec : mSIdleTime)
		{
			fill(begin(vec), end(vec), 0);
		}

		BlockTime = mSIdleTime;

		for (auto& vec : STime)
		{
			fill(begin(vec), end(vec), 0);
		}

		CTime = STime;

		int mpt1 = INT_MAX;
		int cur_job;

		for (int j = 0; j < Seq.size() + 1; ++j)
		{
			if (j < pos)
			{
				cur_job = Seq[j];
			}
			else if (j == pos)
			{
				cur_job = job;
			}
			else
			{
				cur_job = Seq[j - 1];
			}

			for (int k = 0; k < gStage; ++k)
			{
				//Select a machine
				int mpt, minIdle = INT_MAX;

				//选出最先空闲机器
				for (int i = 0; i < pMachines[k]; i++)
				{
					if (mSIdleTime[k][i] < minIdle)
					{
						minIdle = mSIdleTime[k][i];
						mpt = i;
					}
				}

				//第1阶段机器开始的时间
				if (k == 0)
				{
					STime[k][cur_job] = minIdle;
					mpt1 = mpt;
				}

				//其他阶段
				else
				{
					STime[k][cur_job] = max(minIdle, CTime[k - 1][cur_job]);

					if (minIdle >= CTime[k - 1][cur_job])
					{
						BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][cur_job];
						//BlockingStartTime[k - 1][cur_job] = CTime[k - 1][cur_job];
						RecordJobBlockTime[k - 1][cur_job] = BlockTime[k - 1][mpt1];
						/*BlockTotalConsumption += BlockTime[k - 1][mpt1] * BlockConsumption[k - 1][mpt1];*/
					}

					CTime[k - 1][cur_job] = STime[k][cur_job] - BlockTime[k - 1][mpt1];
					mSIdleTime[k - 1][mpt1] = STime[k][cur_job];
					mpt1 = mpt;
				}
				CTime[k][cur_job] = STime[k][cur_job] + PTime[cur_job][k];
				mSIdleTime[k][mpt] = CTime[k][cur_job];
			}
		}
		for (int i = 0; i < pMachines[gStage - 1]; ++i)
		{
			Makespan = max(Makespan, mSIdleTime[gStage - 1][i]);
		}
	}
	return Makespan;
}

//找到末尾位置最好的工厂
int FindBestFinalPosFac(vector<vector<int>>& Sol, int Job, int& SelFac)
{
	vector<int> FactorySpan;
	FactorySpan.resize(Sol.size(), 0); //

	for (int f = 0; f < Sol.size(); f++) //
		FactorySpan[f] = Get_OneFactory_span(Sol[f]); //

	//Test the job at all the factories, and select the best position
	int minTSpan = INT_MAX;
	for (int fac = 0; fac < gFactory; fac++)
	{
		if (Sol[fac].size() == 0)
		{
			minTSpan = 0;
			for (int m = 0; m < gStage; m++)
				minTSpan += PTime[Job][m] /** pTimeConsumption[m][0]*/;
			SelFac = fac;
			break;
		}

		int TempSpan = 0; //////
		
		TempSpan = Get_OneFac_span_After_Insert_Job(Sol[fac], Job, Sol[fac].size());

		if (TempSpan < minTSpan)
		{
			minTSpan = TempSpan;
			SelFac = fac;
		}
	}

	//将工件插入最好的位置
	Sol[SelFac].push_back(Job);
	FactorySpan[SelFac] = minTSpan;

	int Span = *max_element(FactorySpan.begin(), FactorySpan.end());
	return Span;
}

//按照NEH1规则分配工厂
int Assign_to_Fac(vector<vector<int>> Sol, const vector<int> FullPermutation)
{
	vector<int> FacSpan(gFactory, 0);
	for (int f = 0; f < gFactory; f++)
	{
		Sol[f].push_back(FullPermutation[f]);
		int span = Get_OneFactory_span(Sol[f]);
		FacSpan[f] = span;
	}

	for (int i = gFactory; i < FullPermutation.size(); i++)
	{
		int SelFac = -1;
		int Job = FullPermutation[i];
		int span = FindBestFinalPosFac(Sol, Job, SelFac);   
		FacSpan[SelFac] = span;
	}
	return *max_element(begin(FacSpan), end(FacSpan));
}

//五种初始化方法来生成makespan，平均求其上界
//随机排序
int RANDInit(vector<vector<int>> Sol, vector<int> FullPermutation)
{
	for (int j = 0; j < gJobs; j++)
	{
		FullPermutation[j] = j;
	}
	shuffle(FullPermutation.begin(), FullPermutation.end(), rand_generator());
	int Span = Assign_to_Fac(Sol, FullPermutation);
	return Span;
}

//处理时间降序
int LPTInit(vector<vector<int>> Sol, vector<int> FullPermutation)
{
	Pair* ch = new Pair[gJobs];
	for (int j = 0; j < gJobs; j++)
	{
		ch[j].dim = j;
		ch[j].value = 0;
		for (int m = 0; m < gStage; m++)
			ch[j].value += PTime[j][m];
	}

	sort(ch, ch + gJobs, PairGreater());
	for (int j = 0; j < gJobs; j++)
		FullPermutation[j] = ch[j].dim;
	delete[]ch;

	int Span = Assign_to_Fac(Sol, FullPermutation);
	return Span;
}

//处理时间升序
int SPTInit(vector<vector<int>> Sol, vector<int> FullPermutation)
{
	Pair* ch = new Pair[gJobs];
	for (int j = 0; j < gJobs; j++)
	{
		ch[j].dim = j;
		ch[j].value = 0;
		for (int m = 0; m < gStage; m++)
			ch[j].value += PTime[j][m];
	}

	sort(ch, ch + gJobs, PairLess());
	for (int j = 0; j < gJobs; j++)
		FullPermutation[j] = ch[j].dim;
	delete[]ch;

	int Span = Assign_to_Fac(Sol, FullPermutation);
	return Span;
}

//第一阶段处理时间降序
int LPTFInit(vector<vector<int>> Sol, vector<int> FullPermutation)
{
	Pair* ch = new Pair[gJobs];
	for (int j = 0; j < gJobs; j++)
	{
		ch[j].dim = j;
		ch[j].value = PTime[j][0];
	}

	sort(ch, ch + gJobs, PairGreater());
	for (int j = 0; j < gJobs; j++)
		FullPermutation[j] = ch[j].dim;
	delete[]ch;

	int Span = Assign_to_Fac(Sol, FullPermutation);
	return Span;
}

//第一阶段处理时间降序
int SPTFInit(vector<vector<int>> Sol, vector<int> FullPermutation)
{
	Pair* ch = new Pair[gJobs];
	for (int j = 0; j < gJobs; j++)
	{
		ch[j].dim = j;
		ch[j].value = PTime[j][0];
	}

	sort(ch, ch + gJobs, PairLess());
	for (int j = 0; j < gJobs; j++)
		FullPermutation[j] = ch[j].dim;
	delete[]ch;

	int Span = Assign_to_Fac(Sol, FullPermutation);
	return Span;
}

//生成makespan约束的上界
int GenerateSpanUpperBound()
{
	vector<vector<int>> Sol;
	Sol.clear();
	Sol.resize(gFactory);

	vector<int> FullPermutation;
	FullPermutation.resize(gJobs);

	int Span1 = RANDInit(Sol, FullPermutation);
	int Span2 = LPTInit(Sol, FullPermutation);
	int Span3 = SPTInit(Sol, FullPermutation);
	int Span4 = LPTFInit(Sol, FullPermutation);
	int Span5 = SPTFInit(Sol, FullPermutation);

	int SpanUpperBound = (Span1 + Span2 + Span3 + Span4 + Span5) / 5;
	return SpanUpperBound;
}


//-------------------------------FIG--------------------------------------------


//从pos位置往后更新RecordMidle
//int UpdateOneFactoryTimeInformation_AfterPos(const vector<int>& Seq, int pos, int& tempSpan,  vector<vector<vector<int>>>& RecordMidle)
//{
//	if (Seq.size() == 0)
//	{
//		return 0;
//	}
//
//	for (auto& vec : mSIdleTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	for (auto& vec : STime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	for (auto& vec : CTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	for (auto& vec : IdleTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	for (auto& vec : BlockTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	for (auto& vec : RecordJobBlockTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	int PTimeTotalConsumption = 0;				//加工总能耗
//	int BlockTotalConsumption = 0;				//阻塞总能耗
//	int IdleTotalConsumption = 0;				//空闲总能耗
//
//	for (int j = 0; j < Seq.size(); j++)			//加工总能耗是已知的
//	{
//		for (int k = 0; k < gStage; k++)
//		{
//			PTimeTotalConsumption = PTimeTotalConsumption + PTime[Seq[j]][k] * PTimeConsumption[k][0];
//		}
//	}
//
//	if (pos > 0)
//	{
//		for (int k = 0; k < gStage; ++k)
//		{
//			for (int i = 0; i < pMachines[k]; i++)
//			{
//				mSIdleTime[k][i] = RecordMidle[Seq[pos - 1]][k][i];
//			}
//		}
//	}
//
//	int mpt1 = -1;
//
//	for (int j = pos; j < Seq.size(); ++j)
//	{
//		int cur_job = Seq[j];
//
//		for (int k = 0; k < gStage; ++k)
//		{
//			//Select a machine
//			int mpt, minIdle = INT_MAX;
//
//			//选出最先空闲机器
//			for (int i = 0; i < pMachines[k]; i++)
//			{
//				if (mSIdleTime[k][i] < minIdle)
//				{
//					minIdle = mSIdleTime[k][i];
//					mpt = i;
//				}
//			}
//
//			if (k == 0)
//			{
//				//第1阶段机器开始的时间
//				STime[k][cur_job] = minIdle;
//			}
//			else
//			{
//				//其他阶段
//				STime[k][cur_job] = max(minIdle, CTime[k - 1][cur_job]);
//
//				if (minIdle >= CTime[k - 1][cur_job])     //存在阻塞，计算阻塞能耗
//				{
//					BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][cur_job];
//					RecordJobBlockTime[k - 1][cur_job] = BlockTime[k - 1][mpt1];
//					BlockTotalConsumption += BlockTime[k - 1][mpt1] * BlockConsumption[k - 1][mpt1];
//				}
//				else   //计算空闲能耗
//				{
//					IdleTime[k][mpt] = CTime[k - 1][cur_job] - minIdle;
//					IdleTotalConsumption += IdleTime[k][mpt] * IdleConsumption[k][mpt];
//				}
//
//				mSIdleTime[k - 1][mpt1] = STime[k][cur_job];
//				RecordMidle[cur_job][k - 1][mpt1] = mSIdleTime[k - 1][mpt1]; //记录机器可用时间
//			}
//			CTime[k][cur_job] = STime[k][cur_job] + PTime[cur_job][k];
//			mSIdleTime[k][mpt] = CTime[k][cur_job];
//			mpt1 = mpt;
//
//			for (int i = 0; i < pMachines[k]; ++i)
//			{
//				RecordMidle[cur_job][k][i] = mSIdleTime[k][i]; //记录机器可用时间
//			}
//		}
//	}
//
//	int Makespan = -1;
//	for (int i = 0; i < pMachines[gStage - 1]; ++i)
//	{
//		Makespan = max(Makespan, mSIdleTime[gStage - 1][i]);
//	}
//	tempSpan = Makespan;
//
//	int TotalEC = 0;
//	TotalEC = PTimeTotalConsumption + BlockTotalConsumption + IdleTotalConsumption;
//	return TotalEC;
//}

//根据记录的值正向解码计算把工件插入后的makespan，工件还没有真正插入
//int GetOneFacEC_AfterInsertJob(const vector<int>& Seq, int job, int pos, int& tempSpan, const vector<vector<vector<int>>>& RecordMidle)
//{
//	for (auto& vec : mSIdleTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	for (auto& vec : STime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//	for (auto& vec : CTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//	for (auto& vec : IdleTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	for (auto& vec : BlockTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	for (auto& vec : RecordJobBlockTime)
//	{
//		fill(begin(vec), end(vec), 0);
//	}
//
//	int PTimeTotalConsumption = 0;				//加工总能耗
//	int BlockTotalConsumption = 0;				//阻塞总能耗
//	int IdleTotalConsumption = 0;				//空闲总能耗
//
//	for (int j = 0; j < Seq.size(); j++)			//加工总能耗是已知的
//	{
//		for (int k = 0; k < gStage; k++)
//		{
//			PTimeTotalConsumption = PTimeTotalConsumption + PTime[Seq[j]][k] * PTimeConsumption[k][0];
//		}
//	}
//
//	if (pos > 0)
//	{
//		for (int k = 0; k < gStage; ++k)
//		{
//			for (int i = 0; i < pMachines[k]; i++)
//			{
//				mSIdleTime[k][i] = RecordMidle[Seq[pos - 1]][k][i];
//			}
//		}
//	}
//
//	int mpt1 = -1;
//	int cur_job;
//
//	for (int j = pos; j < Seq.size() + 1; ++j)
//	{
//		if (j == pos)
//		{
//			cur_job = job;
//		}
//		else
//		{
//			cur_job = Seq[j - 1];
//		}
//
//		for (int k = 0; k < gStage; ++k)
//		{
//			//Select a machine
//			int mpt, minIdle = INT_MAX;
//
//			//选出最先空闲机器
//			for (int i = 0; i < pMachines[k]; i++)
//			{
//				if (mSIdleTime[k][i] < minIdle)
//				{
//					minIdle = mSIdleTime[k][i];
//					mpt = i;
//				}
//			}
//
//			if (k == 0)
//			{
//				//第1阶段机器开始的时间
//				STime[k][cur_job] = minIdle;
//			}
//			else
//			{
//				//其他阶段
//				STime[k][cur_job] = max(minIdle, CTime[k - 1][cur_job]);
//
//				if (minIdle >= CTime[k - 1][cur_job])     //存在阻塞，计算阻塞能耗
//				{
//					BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][cur_job];
//					RecordJobBlockTime[k - 1][cur_job] = BlockTime[k - 1][mpt1];
//					BlockTotalConsumption += BlockTime[k - 1][mpt1] * BlockConsumption[k - 1][mpt1];
//				}
//				else   //计算空闲能耗
//				{
//					IdleTime[k][mpt] = CTime[k - 1][cur_job] - minIdle;
//					IdleTotalConsumption += IdleTime[k][mpt] * IdleConsumption[k][mpt];
//				}
//
//				mSIdleTime[k - 1][mpt1] = STime[k][cur_job];
//			}
//			CTime[k][cur_job] = STime[k][cur_job] + PTime[cur_job][k];
//			mSIdleTime[k][mpt] = CTime[k][cur_job];
//			mpt1 = mpt;
//		}
//	}
//
//	int Makespan = -1;
//	for (int i = 0; i < pMachines[gStage - 1]; ++i)
//	{
//		Makespan = max(Makespan, mSIdleTime[gStage - 1][i]);
//	}
//	tempSpan = Makespan;
//
//	int TotalEC = 0;
//	TotalEC = PTimeTotalConsumption + BlockTotalConsumption + IdleTotalConsumption;
//	return TotalEC;
//}

//工件还没有真正插入
int GetOneFacEC_After_InsertJob(const vector<int>& Seq, int job, int pos, int& tempSpan)
{
	for (auto& vec : mSIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : STime)
	{
		fill(begin(vec), end(vec), 0);
	}
	for (auto& vec : CTime)
	{
		fill(begin(vec), end(vec), 0);
	}
	for (auto& vec : IdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : BlockTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : RecordJobBlockTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	int PTimeTotalConsumption = 0;				//加工总能耗
	int BlockTotalConsumption = 0;				//阻塞总能耗
	int IdleTotalConsumption = 0;				//空闲总能耗

	for (int j = 0; j < Seq.size(); j++)			//加工总能耗是已知的
	{
		for (int k = 0; k < gStage; k++)
		{
			PTimeTotalConsumption = PTimeTotalConsumption + PTime[Seq[j]][k] * PTimeConsumption[k][0];
		}
	}
	for (int k = 0; k < gStage; k++)
	{
		PTimeTotalConsumption += PTime[job][k] * PTimeConsumption[k][0];         //作业还没插入，要加上这个作业的处理能耗
	}

	int mpt1 = -1;
	int cur_job;

	for (int j = 0; j <= Seq.size(); ++j)
	{
		if (j < pos)
		{
			cur_job = Seq[j];
		}
		else if (j == pos)
		{
			cur_job = job;
		}
		else
		{
			cur_job = Seq[j - 1];
		}

		for (int k = 0; k < gStage; ++k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//选出最先空闲机器
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (mSIdleTime[k][i] < minIdle)
				{
					minIdle = mSIdleTime[k][i];
					mpt = i;
				}
			}

			if (k == 0)
			{
				//第1阶段机器开始的时间
				STime[k][cur_job] = minIdle;
			}
			else
			{
				//其他阶段
				STime[k][cur_job] = max(minIdle, CTime[k - 1][cur_job]);

				if (minIdle >= CTime[k - 1][cur_job])     //存在阻塞，计算阻塞能耗
				{
					BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][cur_job];
					RecordJobBlockTime[k - 1][cur_job] = BlockTime[k - 1][mpt1];
					BlockTotalConsumption += BlockTime[k - 1][mpt1] * BlockConsumption[k - 1][mpt1];
				}
				else   //计算空闲能耗
				{
					IdleTime[k][mpt] = CTime[k - 1][cur_job] - minIdle;
					IdleTotalConsumption += IdleTime[k][mpt] * IdleConsumption[k][mpt];
				}

				mSIdleTime[k - 1][mpt1] = STime[k][cur_job];
			}
			CTime[k][cur_job] = STime[k][cur_job] + PTime[cur_job][k];
			mSIdleTime[k][mpt] = CTime[k][cur_job];
			mpt1 = mpt;
		}
	}

	int Makespan = -1;
	for (int i = 0; i < pMachines[gStage - 1]; ++i)
	{
		Makespan = max(Makespan, mSIdleTime[gStage - 1][i]);
	}
	tempSpan = Makespan;

	int TotalEC = 0;
	TotalEC = PTimeTotalConsumption + BlockTotalConsumption + IdleTotalConsumption;
	return TotalEC;
}

//在一个工厂内找到最好的插入位置
int FindBestInsertionPositonInOneFactory(int& BestPos, int Job, const vector<int>& Seq, int& tempSpan, const int SpanUpperBound)
{
	int minEC = INT_MAX;

	for (int Pos = 0; Pos <= Seq.size(); ++Pos)
	{
		int temp_EC = GetOneFacEC_After_InsertJob(Seq, Job, Pos, tempSpan);

		if ( tempSpan<= SpanUpperBound && temp_EC < minEC)
		{
			BestPos = Pos;
			minEC = temp_EC;
		}
	}

	return minEC;    
}

//一个工件在所有的工厂里找到最好的工厂最好的位置
int JobToBestFactoryBestposition_GetEC_AfterInsert(int Job, const vector<vector<int>>& Sol, int& SelFac, int& BestPos, 
	vector<int>& Aspan, const int SpanUpperBound)
{
	//Test the job at all the factories, and select the best position
	int minEC = INT_MAX;
	for (int fac = 0; fac < gFactory; fac++)
	{
		int TempPos = -1;
		int tempFspan = Aspan[fac];      //记录此工厂的span，若不好，则还原span

		int TempEC = FindBestInsertionPositonInOneFactory(TempPos, Job, Sol[fac], Aspan[fac], SpanUpperBound);

		if (TempEC < minEC)
		{
			minEC = TempEC;
			BestPos = TempPos;
			SelFac = fac;
		}
		else
		{
			Aspan[fac] = tempFspan;
		}
	}
	return minEC;    //返回单个工厂的能耗，需要在此函数完后，计算所有工厂的总能耗
}



//-----------------------------------------------------------------------------------------









//----------------------- 其他算法 --------------------------------

// 计算一个工厂的能耗
int Get_OneFac_EC(const vector<int>& Seq, int& tempSpan)
{
	if (Seq.size() == 0)
	{
		return 0;
	}

	for (auto& vec : mSIdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : STime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : CTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : IdleTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : BlockTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	for (auto& vec : RecordJobBlockTime)
	{
		fill(begin(vec), end(vec), 0);
	}

	int PTimeTotalConsumption = 0;				//加工总能耗
	int BlockTotalConsumption = 0;				//阻塞总能耗
	int IdleTotalConsumption = 0;				//空闲总能耗


	for (int j = 0; j < Seq.size(); j++)			//加工总能耗是已知的
	{
		for (int k = 0; k < gStage; k++)
		{
			PTimeTotalConsumption = PTimeTotalConsumption + PTime[Seq[j]][k] * PTimeConsumption[k][0];
		}
	}

	int mpt1 = -1;

	for (int j = 0; j < Seq.size(); ++j)
	{
		int cur_job = Seq[j];

		for (int k = 0; k < gStage; ++k)
		{
			//Select a machine
			int mpt, minIdle = INT_MAX;

			//选出最先空闲机器
			for (int i = 0; i < pMachines[k]; i++)
			{
				if (mSIdleTime[k][i] < minIdle)
				{
					minIdle = mSIdleTime[k][i];
					mpt = i;
				}
			}

			if (k == 0)
			{
				//第1阶段机器开始的时间
				STime[k][cur_job] = minIdle;
			}
			else
			{
				//其他阶段
				STime[k][cur_job] = max(minIdle, CTime[k - 1][cur_job]);

				if (minIdle >= CTime[k - 1][cur_job])     //存在阻塞，计算阻塞时间
				{
					BlockTime[k - 1][mpt1] = minIdle - CTime[k - 1][cur_job];
					RecordJobBlockTime[k - 1][cur_job] = BlockTime[k - 1][mpt1];
					BlockTotalConsumption += BlockTime[k - 1][mpt1] * BlockConsumption[k - 1][mpt1];
				}
				else
				{
					IdleTime[k][mpt] = CTime[k - 1][cur_job] - minIdle;
					IdleTotalConsumption += IdleTime[k][mpt] * IdleConsumption[k][mpt];
				}

				mSIdleTime[k - 1][mpt1] = STime[k][cur_job];
			}
			CTime[k][cur_job] = STime[k][cur_job] + PTime[cur_job][k];
			mSIdleTime[k][mpt] = CTime[k][cur_job];
			mpt1 = mpt;
		}
	}

	int Makespan = -1;
	for (int i = 0; i < pMachines[gStage - 1]; ++i)
	{
		Makespan = max(Makespan, mSIdleTime[gStage - 1][i]);
	}
	tempSpan = Makespan;

	int TotalEC = 0;
	TotalEC = PTimeTotalConsumption + BlockTotalConsumption + IdleTotalConsumption;

	return TotalEC;
}

// 计算所有工厂的总能耗
int Get_AllFac_EC(const vector<vector<int>>& Sol, vector<int>& Aspan)
{
	int TotalEC = 0;
	for (int f = 0; f < Sol.size(); f++)     //计算总能耗
	{
		TotalEC += Get_OneFac_EC(Sol[f], Aspan[f]);
	}
	return TotalEC;
}

//为某个工件Job找到最好的插入位置
int FindBestInsertionPositon(int& BestPos, int Job, vector<int>& Seq, int & tempSpan, const int SpanUpperBound)
{
	int minEC = INT_MAX;
	vector<int> Fij;
	Fij.resize(Seq.size() + 1, 0); //

	for (int i = 0; i < Seq.size(); ++i)
	{
		Fij[i] = Seq[i];
	}

	for (int Pos = 0; Pos < Fij.size(); Pos++)
	{
		int TEC = 0;
		if (Pos == (Fij.size() - 1))
		{
			Fij[Pos] = Job;
		}
		else
		{
			for (int j = Fij.size() - 2; j >= Pos; --j)
			{
				Fij[j + 1] = Fij[j];
			}
			Fij[Pos] = Job;
		}

		TEC =Get_OneFac_EC(Fij, tempSpan);

		if (tempSpan<= SpanUpperBound && TEC < minEC)
		{
			minEC = TEC;
			BestPos = Pos;
		}

		//		Fij.clear();   //这里错了，clear全部清空了，不能赋值
		std::fill(Fij.begin(), Fij.end(), 0); //快速置0
		for (int i = 0; i < Seq.size(); ++i)
		{
			Fij[i] = Seq[i];
		}
	}

	return minEC;
}

int Job_To_BestFactory_Bestposition(int Job, vector<vector<int>>& Sol, int& SelFac, int& BestPos, vector<int> & Aspan, const int SpanUpperBound)
{
	//先将每个工厂的makespan都初始化为0
	vector<int> FactoryEC;
	FactoryEC.resize(Sol.size(), 0); //

	for (int f = 0; f < Sol.size(); f++) //
		FactoryEC[f] = Get_OneFac_EC(Sol[f], Aspan[f]); //    

	vector<bool> EijFlag;
	EijFlag.resize(gFactory);
	for (int fac = 0; fac < gFactory; fac++)
		EijFlag[fac] = false;

	//Test the job at all the factories, and select the best position
	int minEC = INT_MAX;
	for (int fac = 0; fac < gFactory; fac++)
	{
		if (Sol[fac].size() == 0)
		{
			minEC = 0;
			for (int m = 0; m < gStage; m++)
			{
				minEC += PTime[Job][m] * PTimeConsumption[m][0] /** pTimeConsumption[m][0]*/;
				Aspan[fac] += PTime[Job][m];
			}
			BestPos = 0;
			SelFac = fac;
			break;
		}

		if (!EijFlag[fac])
		{
			EijFlag[fac] = true;
		}

		int TempEC = 0, TempPos = -1; //////

		TempEC = FindBestInsertionPositon(TempPos, Job, Sol[fac], Aspan[fac], SpanUpperBound);

		if (TempEC < minEC)
		{
			minEC = TempEC;
			BestPos = TempPos;
			SelFac = fac;
		}
	}

	//将工件插入最好的位置
	Sol[SelFac].insert(Sol[SelFac].begin() + BestPos, Job);
	FactoryEC[SelFac] = minEC;
	EijFlag[SelFac] = false;

	int TEC = accumulate(FactoryEC.begin(), FactoryEC.end(), 0);

	return TEC;
}

int Job_To_SomeFactory_Bestposition(int Job, vector<int>& Seq, int& tempSpan, const int SpanUpperBound) /////
{

	int minEC = 0, BestPos = -1;
	int EC;

	if (Seq.size() == 0)
	{
		for (int m = 0; m < gStage; m++)
		{
			minEC = PTime[Job][m] * PTimeConsumption[m][0];
			tempSpan += PTime[Job][m];
		}
		BestPos = 0;
	}
	else
	{
		minEC = FindBestInsertionPositon(BestPos, Job, Seq, tempSpan, SpanUpperBound);
	}

	//将工件插入最好的位置

	Seq.insert(Seq.begin() + BestPos, Job);
	EC = minEC;

	//if (BestPos != -1)
	//{
	//	Seq.insert(Seq.begin() + BestPos, Job);
	//	EC = minEC;
	//}
	//else
	//{
	//	EC = Get_OneFac_EC(Seq, tempSpan);
	//}

	return EC;
}



int Job_To_BestFactory_Bestposition_DPSO(int Job, vector<vector<int>>& Sol,  vector<int>& Aspan, const int SpanUpperBound)
{
	//先将每个工厂的makespan都初始化为0
	vector<int> FactoryEC;
	FactoryEC.resize(Sol.size(), 0); //

	for (int f = 0; f < Sol.size(); f++) //
		FactoryEC[f] = Get_OneFac_EC(Sol[f], Aspan[f]); //    

	vector<bool> EijFlag;
	EijFlag.resize(gFactory);
	for (int fac = 0; fac < gFactory; fac++)
		EijFlag[fac] = false;

	//Test the job at all the factories, and select the best position
	int minEC = INT_MAX, SelFac = 0,  BestPos = -1;
	for (int fac = 0; fac < gFactory; fac++)
	{
		if (Sol[fac].size() == 0)
		{
			minEC = 0;
			for (int m = 0; m < gStage; m++)
			{
				minEC += PTime[Job][m] * PTimeConsumption[m][0] /** pTimeConsumption[m][0]*/;
				Aspan[fac] += PTime[Job][m];
			}
			BestPos = 0;
			SelFac = fac;
			break;
		}

		if (!EijFlag[fac])
		{
			EijFlag[fac] = true;
		}

		int TempEC = 0, TempPos = -1; //////

		TempEC = FindBestInsertionPositon(TempPos, Job, Sol[fac], Aspan[fac], SpanUpperBound);

		if (TempEC < minEC)
		{
			minEC = TempEC;
			BestPos = TempPos;
			SelFac = fac;
		}
	}

	//将工件插入最好的位置
	Sol[SelFac].insert(Sol[SelFac].begin() + BestPos, Job);
	FactoryEC[SelFac] = minEC;
	EijFlag[SelFac] = false;

	int TEC = accumulate(FactoryEC.begin(), FactoryEC.end(), 0);

	return TEC;
}

int Job_To_SomeFactory_Bestposition_DPSO(int Job, vector<int>& Seq, int& tempSpan, const int SpanUpperBound) /////
{
	int minEC = 0, BestPos = -1;
	if (Seq.size() == 0)
	{
		for (int m = 0; m < gStage; m++)
			minEC = 0; /** pTimeConsumption[m][0]*/
		BestPos = 0;
	}
	else
	{
		minEC = FindBestInsertionPositon(BestPos, Job, Seq, tempSpan, SpanUpperBound);
	}

	//将工件插入最好的位置
	Seq.insert(Seq.begin() + BestPos, Job);
	int EC = Get_OneFac_EC(Seq, tempSpan);

	return EC;
}






int FindBestSwapPositon(int& BestPos, int Job, vector<int> Seq, int& tempSpan, const int SpanUpperBound)
{
	int minEC = Get_OneFac_EC(Seq, tempSpan);
	vector<int> Fij;
	Fij.resize(Seq.size(), 0);
	int tempspan1 = tempSpan;

	for (int i = 0; i < Seq.size(); ++i)
	{
		Fij[i] = Seq[i];
	}
	int temp;
	for (int Pos = 0; Pos < Fij.size(); Pos++)
	{
		int TEC = 0;
		int tempJob = Job;


		temp = Fij[Pos];
		Fij[Pos] = tempJob;
		tempJob = temp;


		TEC = Get_OneFac_EC(Fij, tempspan1);

		if (TEC < minEC && tempspan1<= SpanUpperBound)
		{
			minEC = TEC;
			BestPos = Pos;

		}

		//		Fij.clear();
		std::fill(Fij.begin(), Fij.end(), 0); //快速置0
		for (int i = 0; i < Seq.size(); ++i)
		{
			Fij[i] = Seq[i];
		}
	}

	return minEC;
}

int Job_ToSwap_BestFactory_Bestposition(int maxFac, int i, int Job, vector<vector<int>>& Sol, int& SelFac, int& BestPos,
	vector<int>& Aspan, const int SpanUpperBound)
{
	//先将每个工厂的EC都初始化为0
	vector<int> FactorySpan;
	FactorySpan.resize(Sol.size(), 0); //

	for (int f = 0; f < Sol.size(); f++) //
		FactorySpan[f] =Get_OneFac_EC(Sol[f], Aspan[f]); //

	vector<bool> EijFlag;
	EijFlag.resize(gFactory);
	for (int fac = 0; fac < gFactory; fac++)
		EijFlag[fac] = false;

	int minEC = INT_MAX;
	for (int fac = 0; fac < gFactory; fac++)
	{
		if (fac != maxFac)
		{
			if (Sol[fac].size() == 0)
			{
				minEC = 0;
				for (int m = 0; m < gStage; m++)
					minEC += PTime[Job][m] /** pTimeConsumption[m][0]*/;
				BestPos = 0;
				SelFac = fac;
				break;
			}

			if (!EijFlag[fac])
			{
				EijFlag[fac] = true;
			}

			int TempEC = 0, TempPos = -1; //////
			TempEC = FindBestSwapPositon(TempPos, Job, Sol[fac],Aspan[fac],SpanUpperBound);
			if (TempEC < minEC)
			{
				minEC = TempEC;
				BestPos = TempPos;
				SelFac = fac;
			}
		}
	}
	//交换工件
	if (BestPos != -1)
	{
		Sol[maxFac][i] = Sol[SelFac][BestPos];
		Sol[SelFac][BestPos] = Job;
		FactorySpan[SelFac] = Get_OneFac_EC(Sol[SelFac], Aspan[SelFac]);
		EijFlag[SelFac] = false;
	}


	int Span = accumulate(FactorySpan.begin(), FactorySpan.end(), 0);
	return Span;
}

//--------------------------------------------------------------------------

//获取时间函数
long GetElapsedProcessTime()
{
	FILETIME createTime;
	FILETIME exitTime;
	FILETIME kernelTime;
	FILETIME userTime;

	if (GetProcessTimes(GetCurrentProcess(), &createTime, &exitTime, &kernelTime, &userTime) != 0)
	{
		//  Returns total user time.
		SYSTEMTIME userSystemTime;
		if (FileTimeToSystemTime(&userTime, &userSystemTime) != -1)
			return (userSystemTime.wDay - 1) * 24 * 3600 * 1000
			+ userSystemTime.wHour * 3600 * 1000 +
			userSystemTime.wMinute * 60 * 1000 +
			userSystemTime.wSecond * 1000 +
			userSystemTime.wMilliseconds;
		else
			return 0;
	}
	else
		return 0;
}

double GetTemperature()
{
	double Temperature = 0;
	int totalPtime = 0;
	for (int j = 0; j < gJobs; ++j)
	{
		for (int k = 0; k < gStage; ++k)
		{
			totalPtime += PTime[j][k];
		}
	}
	Temperature = (totalPtime / 10.0 * gJobs * gStage) * 0.5;
	return Temperature;
}

double GetTemperature02(double Tem)
{
	double Temperature = 0;
	int totalPtime = 0;
	for (int j = 0; j < gJobs; ++j)
	{
		for (int k = 0; k < gStage; ++k)
		{
			totalPtime += PTime[j][k];
		}
	}
	Temperature = (totalPtime / 10.0 * gJobs * gStage) * 0.40;
	return Temperature;
}