#pragma once

//还未修改，原来是分布式阻塞

int NEH2_en_B(vector<vector<int>>& Sol, vector<int>& Aspan, int SpanUpperBound)
{
	Sol.clear();
	Sol.resize(gFactory);  //设置容量

	vector<int> FullPermutation;
	FullPermutation.resize(gJobs);
	for (int j = 0; j < gJobs; j++)
	{
		FullPermutation[j] = j;
	}
	NEHInit(FullPermutation);

	int FinalEC = 0;
	for (int i = 0; i < FullPermutation.size(); i++)
	{
		int Job = FullPermutation[i];
		int SelFac = 0;
		int BestPos = 0;
		FinalEC = Job_To_BestFactory_Bestposition(Job, Sol, SelFac, BestPos, Aspan, SpanUpperBound);
		if (Sol[SelFac].size() > 1)
		{
			if (BestPos == 0)
			{
				int RetryJob = Sol[SelFac][BestPos + 1];
				Sol[SelFac].erase(Sol[SelFac].begin() + BestPos + 1);
				Job_To_SomeFactory_Bestposition(RetryJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
			}
			else if (BestPos == (Sol[SelFac].size() - 1))
			{
				int RetryJob = Sol[SelFac][BestPos - 1];
				Sol[SelFac].erase(Sol[SelFac].begin() + BestPos - 1);
				Job_To_SomeFactory_Bestposition(RetryJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
			}
			else
			{
				int RandNum = wyt_rand(2);// rand() % 2;
				if (RandNum == 0)
				{
					int RetryJob = Sol[SelFac][BestPos + 1];
					Sol[SelFac].erase(Sol[SelFac].begin() + BestPos + 1);
					Job_To_SomeFactory_Bestposition(RetryJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
				}
				else
				{
					int RetryJob = Sol[SelFac][BestPos - 1];
					Sol[SelFac].erase(Sol[SelFac].begin() + BestPos - 1);
					Job_To_SomeFactory_Bestposition(RetryJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
				}
			}
		}
	}

	FinalEC = Get_AllFac_EC(Sol, Aspan);

	return FinalEC;
}

int Split(vector<int> pw, vector <int>& order, int m, int n)
{
	int i, j, k, To;
	int T;
	k = (m + n) / 2;
	T = pw[k];
	To = order[k];

	i = m;
	j = n;
	pw[k] = pw[m];
	order[k] = order[m];

	while (i != j)
	{
		while ((pw[j] <= T) && (i < j))  j = j - 1;
		if (i < j)
		{
			pw[i] = pw[j];
			order[i] = order[j];
			i = i + 1;
			while ((pw[i] >= T) && (i < j)) { i = i + 1; }
			if (i < j)
			{
				pw[j] = pw[i];
				order[j] = order[i];
				j = j - 1;
			}
		}
	}
	pw[i] = T;
	order[i] = To;
	return i;
}

void QKSort(vector<int> pw, vector <int>& order, int m, int n)// ordered according to no-increasing pw[i];
{
	int i;
	if (m < n)
	{
		i = Split(pw, order, m, n);
		QKSort(pw, order, i + 1, n);
		QKSort(pw, order, m, i - 1);
	}
}

void PWInit(vector<vector <int>>& Seq, vector<int>& Aspan, int SpanUpperBound) //PAN-WANG 1
{
	Seq.clear();
	Seq.resize(gFactory);
	vector <int> chrom(gJobs);
	int i, j;
	vector<int> ch(gJobs);


	// get Total process time of each job
	for (i = 0; i < gJobs; i++)
	{
		chrom[i] = i;
		ch[i] = 0;
		for (j = 0; j < gStage; j++)
			ch[i] += PTime[i][j];
	}

	// order jobs according to the no-decreasing total process time;
	for (i = 0; i < gJobs; i++)
		ch[i] = -ch[i];
	QKSort(ch, chrom, 0, gJobs - 1);
	int FinalEC = 0;
	for (int i = 0; i < chrom.size(); i++)
	{
		int Job = chrom[i];
		int SelFac = 0;
		int BestPos = 0;
		FinalEC = Job_To_BestFactory_Bestposition(Job, Seq, SelFac, BestPos, Aspan, SpanUpperBound);
		if (Seq[SelFac].size() > 1)
		{
			if (BestPos == 0)
			{
				int RetryJob = Seq[SelFac][BestPos + 1];
				Seq[SelFac].erase(Seq[SelFac].begin() + BestPos + 1);
				Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
			}
			else if (BestPos == (Seq[SelFac].size() - 1))
			{
				int RetryJob = Seq[SelFac][BestPos - 1];
				Seq[SelFac].erase(Seq[SelFac].begin() + BestPos - 1);
				Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
			}
			else
			{
				int RandNum = wyt_rand(2);// rand() % 2;
				if (RandNum == 0)
				{
					int RetryJob = Seq[SelFac][BestPos + 1];
					Seq[SelFac].erase(Seq[SelFac].begin() + BestPos + 1);
					Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
				}
				else
				{
					int RetryJob = Seq[SelFac][BestPos - 1];
					Seq[SelFac].erase(Seq[SelFac].begin() + BestPos - 1);
					Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
				}
			}
		}
	}
}

void PFInit(vector<vector<int>>& Seq, vector<int>& Aspan, int alpha, int SpanUpperBound) // PAN-WANG 2  alpha = 70
{
	Seq.clear();
	Seq.resize(gFactory);
	vector <int> chrom(gJobs);
	vector<vector<int>> DIS_Block(gJobs);
	vector<int>ch(gJobs);
	vector<int>ch1(gJobs);
	vector<bool> bSelected(gJobs);
	for (int i = 0; i < gJobs; i++)
		DIS_Block[i].resize(gJobs, 0);
	//unsigned short DIS_Block[gJobs][gJobs];
	int i, j;
	vector<int> ToTime(gJobs, 0);
	//int ToTime[MAX_JOBS];
	int Alpha = 50;
	// get distance DIS_Block defined by the total blocking time;
	for (i = 0; i < gJobs; i++)
	{
		for (j = 0; j < gJobs; j++)
		{
			DIS_Block[i][j] = 0;
			if (i == j) continue;
			for (int x = 1; x < gStage; x++)
			{
				DIS_Block[i][j] += max(0, PTime[i][x] - PTime[j][x - 1]);
			}
		}
	}

	// get the total distance of each job;
	int MinFPTime = INT_MAX, MFJob, TempTime;
	for (i = 0; i < gJobs; i++)
	{
		TempTime = ch1[i];
		//	TempTime=alpha*pTime[i*gMachines]+(100-alpha)*ch1[i];
		if (TempTime < MinFPTime)
		{
			MinFPTime = TempTime;
			MFJob = i;
		}
	}

	//get the last job of initial sequence;   //把加工总时间次小的为序列的最后一个工件
	int MinLPTime = INT_MAX, MLJob;
	for (i = 0; i < gJobs; i++)
	{
		if (i == MFJob) continue;
		TempTime = ch1[i];
		//	TempTime=alpha*pTime[(i+1)*gMachines-1]+(100-alpha)*ch1[i];
		if (TempTime < MinLPTime)
		{
			MinLPTime = TempTime;
			MLJob = i;
		}
	}

	// produce a initial sequence
	for (i = 0; i < gJobs; i++)
		bSelected[i] = false;
	ch[0] = MFJob;
	ch[gJobs - 1] = MLJob;
	bSelected[MFJob] = true;
	bSelected[MLJob] = true;

	int MinBlocking = INT_MAX, MBJob;
	for (i = 1; i < gJobs - 1; i++)
	{
		MinBlocking = INT_MAX;
		for (j = 0; j < gJobs; j++)
		{
			if (bSelected[j]) continue;
			if (alpha * DIS_Block[ch[i - 1]][j] + (100 - alpha) * ch1[j] < MinBlocking)
			{
				MinBlocking = alpha * DIS_Block[ch[i - 1]][j] + (100 - alpha) * ch1[j];
				MBJob = j;
			}
		}
		ch[i] = MBJob;
		bSelected[MBJob] = true;
	}

	for (i = 0; i < gJobs; i++)
		chrom[i] = ch[i];

	int FinalEC = 0;
	for (int i = 0; i < chrom.size(); i++)
	{
		int Job = chrom[i];
		int SelFac = 0;
		int BestPos = 0;
		FinalEC = Job_To_BestFactory_Bestposition(Job, Seq, SelFac, BestPos, Aspan, SpanUpperBound);
		if (Seq[SelFac].size() > 1)
		{
			if (BestPos == 0)
			{
				int RetryJob = Seq[SelFac][BestPos + 1];
				Seq[SelFac].erase(Seq[SelFac].begin() + BestPos + 1);
				Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
			}
			else if (BestPos == (Seq[SelFac].size() - 1))
			{
				int RetryJob = Seq[SelFac][BestPos - 1];
				Seq[SelFac].erase(Seq[SelFac].begin() + BestPos - 1);
				Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
			}
			else
			{
				int RandNum = wyt_rand(2);// rand() % 2;
				if (RandNum == 0)
				{
					int RetryJob = Seq[SelFac][BestPos + 1];
					Seq[SelFac].erase(Seq[SelFac].begin() + BestPos + 1);
					Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
				}
				else
				{
					int RetryJob = Seq[SelFac][BestPos - 1];
					Seq[SelFac].erase(Seq[SelFac].begin() + BestPos - 1);
					Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
				}
			}
		}
	}
	//FinalMakespan = GetFspan_BE(Sol);
	//return FinalMakespan;
}

void Destruction_B(vector<vector<int>>& Seq, vector<int>& DesJobSeq, vector<int>& Aspan, int PLen)
{
	DesJobSeq.clear();

	vector<int>FactorySpan(Seq.size(), 0);
	for (int f = 0; f < Seq.size(); f++)
		FactorySpan[f] = Get_OneFac_EC(Seq[f], Aspan[f]);

	int CriticalFacPt = -1;
	int TempFacSpan = INT_MIN;
	for (int i = 0; i < FactorySpan.size(); i++)
	{
		if (FactorySpan[i] > TempFacSpan)
		{
			TempFacSpan = FactorySpan[i];
			CriticalFacPt = i;
		}
	}

	for (int i = 0; i < PLen / 2; i++)
	{
		int Pt = wyt_rand(Seq[CriticalFacPt].size());
		DesJobSeq.push_back(Seq[CriticalFacPt][Pt]);
		Seq[CriticalFacPt].erase(Seq[CriticalFacPt].begin() + Pt);
	}

	while (DesJobSeq.size() < PLen)
	{
		int Fpt = wyt_rand(Seq.size());
		if ((Seq[Fpt].size() > 1) && (Fpt != CriticalFacPt))
		{
			int pt = wyt_rand(Seq[Fpt].size());
			DesJobSeq.push_back(Seq[Fpt][pt]);
			Seq[Fpt].erase(Seq[Fpt].begin() + pt);
		}
	}
}

int Construction_B(vector<vector<int>>& Seq, vector<int>DesJobSeq, vector<int>& Aspan, int SpanUpperBound)
{
	for (int i = 0; i < DesJobSeq.size(); i++)
	{
		int Job = DesJobSeq[i];
		int SelFac = 0;
		int BestPos = 0;
		Job_To_BestFactory_Bestposition(Job, Seq, SelFac, BestPos, Aspan, SpanUpperBound);

		if (Seq[SelFac].size() > 1)
		{
			if (BestPos == 0)
			{
				int RetryJob = Seq[SelFac][BestPos + 1];
				Seq[SelFac].erase(Seq[SelFac].begin() + BestPos + 1);
				Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
			}
			else if (BestPos == (Seq[SelFac].size() - 1))
			{
				int RetryJob = Seq[SelFac][BestPos - 1];
				Seq[SelFac].erase(Seq[SelFac].begin() + BestPos - 1);
				Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
			}
			else
			{
				int RandNum = wyt_rand(2); //rand() % 2;
				if (RandNum == 0)
				{
					int RetryJob = Seq[SelFac][BestPos + 1];
					Seq[SelFac].erase(Seq[SelFac].begin() + BestPos + 1);
					Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
				}
				else
				{
					int RetryJob = Seq[SelFac][BestPos - 1];
					Seq[SelFac].erase(Seq[SelFac].begin() + BestPos - 1);
					Job_To_SomeFactory_Bestposition(RetryJob, Seq[SelFac], Aspan[SelFac], SpanUpperBound);
				}
			}
		}
	}
	int FinalMakespan = Get_AllFac_EC(Seq, Aspan);
	return FinalMakespan;
}


void RandExchange1(vector<vector<int>>& Seq, vector<int>& Aspan, int exchange_jobs, int SpanUpperBound)
{

	int EJ = 0;
	int Energy = Get_AllFac_EC(Seq, Aspan);
	while (EJ < exchange_jobs)
	{
		vector<vector<int>> TempSeq = Seq;
		vector<int> tempASpan = Aspan;
		//随机找到两个工厂
		int  MaxFpt, MinFpt;
		do
		{
			MaxFpt = wyt_rand(gFactory);
			MinFpt = wyt_rand(gFactory);
		} while (MaxFpt == MinFpt);


		//在两个工厂中各选出一个工件
		int MaxJobpt, MinJobpt;
		int Job1, Job2;

		MaxJobpt = wyt_rand(Seq[MaxFpt].size());
		MinJobpt = wyt_rand(Seq[MinFpt].size());

		Job1 = Seq[MaxFpt][MaxJobpt];
		Job2 = Seq[MinFpt][MinJobpt];

		//交换选出的两个工件
		int t = Job1;
		Seq[MaxFpt][MaxJobpt] = Job2;
		Seq[MinFpt][MinJobpt] = t;
		int N_Energy = Get_AllFac_EC(Seq, Aspan);
		int Span = *max_element(Aspan.begin(), Aspan.begin());
		if (Span > SpanUpperBound || N_Energy > Energy)  //不符合条件还原
		{
			Seq = TempSeq;
			Aspan = tempASpan;
		}
		EJ++;
	}
}

void Exter_Factory_Insert_B(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	//随机找到两个工厂
	int  MaxFpt, MinFpt;
	do
	{
		MaxFpt = wyt_rand(Seq.size());
		MinFpt = wyt_rand(Seq.size());
	} while (MaxFpt == MinFpt);

	//选出完工时间最大的那个工厂
	int MaxFactoryEC= Get_OneFac_EC(Seq[MaxFpt], Aspan[MaxFpt]);
	int MinFactoryEC= Get_OneFac_EC(Seq[MinFpt], Aspan[MinFpt]);

	int tempMinspan = Aspan[MinFpt];   //记住这个工厂的span，后面判断工件是否能插入

	int TempEC= MaxFactoryEC;
	int TempFpt = MaxFpt;

	if (MaxFactoryEC< MinFactoryEC)
	{
		MaxFactoryEC= MinFactoryEC;
		MaxFpt = MinFpt;
		MinFactoryEC= TempEC;
		MinFpt = TempFpt;
	}

	//在完工时间大的工厂随机选出一个工件
	vector<int>MinTempSeq = Seq[MinFpt];
	int pt = wyt_rand(Seq[MaxFpt].size());
	int Job = Seq[MaxFpt][pt];

	Seq[MaxFpt].erase(Seq[MaxFpt].begin() + pt);

	//将随机取得的工件重新插入到另外一个工厂的最佳位置
	//int TempBetterEC= Job_To_SomeFactory_Bestposition(Job, Seq[MinFpt], Aspan[MinFpt], SpanUpperBound);

	int  BestPos = -1;

	int TempBetterEC = FindBestInsertionPositon(BestPos, Job, Seq[MinFpt], Aspan[MinFpt], SpanUpperBound);

	//将工件插入最好的位置
	if (BestPos != -1)
	{
		if (TempBetterEC < MaxFactoryEC)
		{
			Seq[MinFpt].insert(Seq[MinFpt].begin() + BestPos, Job);
			Get_OneFac_EC(Seq[MaxFpt], Aspan[MaxFpt]);
		}
		else
		{
			Seq[MaxFpt].insert(Seq[MaxFpt].begin() + pt, Job);
			Aspan[MinFpt] = tempMinspan;
		}
	}
	else
	{
		Seq[MaxFpt].insert(Seq[MaxFpt].begin() + pt, Job);
		Aspan[MinFpt] = tempMinspan;
	}

	//if (TempBetterEC < MaxFactoryEC && Aspan[MinFpt] != tempMinspan) //确保工件没有插入工厂Minfpt
	//{
	//	Seq[MaxFpt].erase(Seq[MaxFpt].begin() + pt);
	//	Get_OneFac_EC(Seq[MaxFpt], Aspan[MaxFpt]);
	//}
	//else   //还原
	//{
	//	Seq[MinFpt] = MinTempSeq;
	//	Aspan = tempAspan;
	//}
}

void Exter_CriticalFactory_Swap_B(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	vector<vector<int>>TempSeq = Seq;
	vector<int> tempAspan = Aspan;

	//找到关键工厂
	int TempSpan = 0;
	int CriticalFactory = 0;
	for (int f = 0; f < Seq.size(); f++)
	{
		int TempSpan1 = Get_OneFac_EC(Seq[f],Aspan[f]);
		if (TempSpan < TempSpan1)
		{
			TempSpan = TempSpan1;
			CriticalFactory = f;
		}
	}

	//随机选取一个不同于关键工厂的工厂
	int Fpt;
	do
	{
		Fpt = wyt_rand(Seq.size());
	} while (Fpt == CriticalFactory);

	//在关键工厂和被选取的工厂中各随机选取一个工件
	int CriticalJobpt, Jobpt;
	int CriticalJob, Job;

	CriticalJobpt = wyt_rand(Seq[CriticalFactory].size());
	Jobpt = wyt_rand(Seq[Fpt].size());
	CriticalJob = Seq[CriticalFactory][CriticalJobpt];
	Job = Seq[Fpt][Jobpt];

	//交换两个工厂的工件
	Seq[CriticalFactory].erase(Seq[CriticalFactory].begin() + CriticalJobpt);
	Seq[Fpt].erase(Seq[Fpt].begin() + Jobpt);

	Seq[CriticalFactory].insert(Seq[CriticalFactory].begin() + CriticalJobpt, Job);
	Seq[Fpt].insert(Seq[Fpt].begin() + Jobpt, CriticalJob);

	//判断是否接受新的解
	int NewCriticalTempSpan = Get_OneFac_EC(Seq[CriticalFactory], Aspan[CriticalFactory]);
	int NewTempSpan = Get_OneFac_EC(Seq[Fpt], Aspan[Fpt]);
	int NewFacTempSpan;
	if (Aspan[CriticalFactory] <= SpanUpperBound && Aspan[Fpt] <= SpanUpperBound)
	{
		NewFacTempSpan = max(NewCriticalTempSpan, NewTempSpan);

		if (NewFacTempSpan > TempSpan)
		{
			Seq = TempSeq;
			Aspan = tempAspan;
		}
	}
	else
	{
		Seq = TempSeq;
		Aspan = tempAspan;
	}
}

int Exter_CriticalFactory_Swap_B2(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	vector<int>FactoryEC(Seq.size(), 0);
	for (int f = 0; f < Seq.size(); f++)
		FactoryEC[f] = Get_OneFac_EC(Seq[f], Aspan[f]);

	int CriticalFacPt = -1;
	int Fpt;
	int Counter = 0;
	vector<int>tempAspan;

	while (true)
	{
		tempAspan = Aspan;
		if (Counter == 0)
		{
			int CriticalFacEC = INT_MIN;

			for (int i = 0; i < FactoryEC.size(); i++) //找到关键工厂
			{
				if (FactoryEC[i] > CriticalFacEC)
				{
					CriticalFacEC = FactoryEC[i];
					CriticalFacPt = i;
				}
			}

			//随机选取一个不同于关键工厂的工厂
			do
			{
				Fpt = wyt_rand(Seq.size());
			} while (Fpt == CriticalFacPt);
		}

		int RandPos1 = wyt_rand(Seq[CriticalFacPt].size());
		int Job1 = Seq[CriticalFacPt][RandPos1];

		int RandPos2 = wyt_rand(Seq[Fpt].size());
		int Job2 = Seq[Fpt][RandPos2];

		vector<vector<int>> TempSeq = Seq;

		Seq[CriticalFacPt][RandPos1] = Job2;
		Seq[Fpt][RandPos2] = Job1;

		int EC1 = Get_OneFac_EC(Seq[CriticalFacPt], Aspan[CriticalFacPt]);
		int EC2 =Get_OneFac_EC(Seq[Fpt], Aspan[Fpt]);

		if (max(Aspan[CriticalFacPt], Aspan[Fpt]) >SpanUpperBound || max(EC1, EC2) >= FactoryEC[CriticalFacPt]) //若交换无效
		{
			Seq = TempSeq;
			Aspan = tempAspan;
			Counter++;
		}
		else
		{

			//更新FactorySpan
			FactoryEC[CriticalFacPt] = EC1;
			FactoryEC[Fpt] = EC2;

			Counter = 0;
		}

		if (Counter >= Seq[CriticalFacPt].size())
		{
			break;
		}
	}
	int TotalEC = accumulate(FactoryEC.begin(), FactoryEC.end(), 0);
	return TotalEC;
}

//阻塞（关键工厂内部交换）
void Inter_CriticalFactory_Swap_B(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	//找到关键工厂
	int TempEC = 0;
	int CriticalFactory = 0;
	for (int f = 0; f < Seq.size(); f++)
	{
		int TempEC1 = Get_OneFac_EC(Seq[f], Aspan[f]);
		if (TempEC < TempEC1)
		{
			TempEC = TempEC1;
			CriticalFactory = f;
		}
	}

	vector<int>TempSeq = Seq[CriticalFactory];
	int tempspan = Aspan[CriticalFactory];

	//在关键工厂中随机选取两个不同的工件
	int Jobpt1, Jobpt2;
	do
	{
		Jobpt1 = wyt_rand(Seq[CriticalFactory].size());
		Jobpt2 = wyt_rand(Seq[CriticalFactory].size());
	} while (Jobpt1 == Jobpt2);

	//交换两个工件的位置
	int TempJob = Seq[CriticalFactory][Jobpt1];
	Seq[CriticalFactory][Jobpt1] = Seq[CriticalFactory][Jobpt2];
	Seq[CriticalFactory][Jobpt2] = TempJob;

	//判断是否接受新的解
	int TempEC1 =Get_OneFac_EC(Seq[CriticalFactory], Aspan[CriticalFactory]);

	if (Aspan[CriticalFactory] <= SpanUpperBound && TempEC1 < TempEC)
	{
		TempEC = TempEC1;
		TempSeq = Seq[CriticalFactory];
	}
	else
	{
		Seq[CriticalFactory] = TempSeq;
		Aspan[CriticalFactory] = tempspan;
	}
}

void Initial(vector<vector<vector<int>>>& Seq, vector<vector<int>>& AllSpan, int P, int SpanUpperBound)
{

	Seq.clear();
	Seq.resize(P);
	int Span = NEH2_en_B(Seq[0], AllSpan[0], SpanUpperBound);
	//Span = NEH2_en_BE(Seq[1]);
	PWInit(Seq[1], AllSpan[1], SpanUpperBound);
	PFInit(Seq[2], AllSpan[2], 70, SpanUpperBound);
	int RandomE_J = gJobs / 2;
	int i = 3;
	while (i < P)
	{
		int d = wyt_rand(3);
		Seq[i].clear();
		Seq[i].resize(Seq[d].size());
		for (int j = 0; j < gFactory; j++)
			Seq[i][j].resize(Seq[d][j].size());
		for (int j = 0; j < gFactory; j++)
			for (int m = 0; m < Seq[d][j].size(); m++)
				Seq[i][j][m] = Seq[d][j][m];
		RandExchange1(Seq[i], AllSpan[i], RandomE_J, SpanUpperBound);
		i++;
	}
}

int VLS(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	int iteration = 1;
	int Age = 0;
	while (iteration <= gFactory)
	{
		int k = 1;
		int Energy =Get_AllFac_EC(Seq, Aspan);
		vector<vector<int>> Temp = Seq;
		while (k <= 3)
		{

			if (k == 1) Exter_Factory_Insert_B(Temp, Aspan, SpanUpperBound);
			//else if (k == 2) RandExchange(Temp, 3);
			else if (k == 2) Exter_CriticalFactory_Swap_B(Temp, Aspan, SpanUpperBound);
			else Inter_CriticalFactory_Swap_B(Temp, Aspan, SpanUpperBound);


			//RandExchange(Seq, 3);

			int TEnergy = Get_AllFac_EC(Temp, Aspan);
			if (TEnergy < Energy)
			{
				Seq = Temp;
				k = 1;
				iteration = 1;
				Energy = TEnergy;
			}
			else
			{
				k++;
				//Seq = Temp;
			}

		}
		iteration = iteration + 1;
	}
	int FEnergy = Get_AllFac_EC(Seq, Aspan);
	return FEnergy;
}


int VLS2(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	int iteration = 1;
	int Age = 0;
	int EC = Get_AllFac_EC(Seq, Aspan);
	while (iteration <= gFactory)
	{
		int k = 1;

		vector<vector<int>> Temp = Seq;
		while (k <= 3)
		{

			if (k == 1) Exter_Factory_Insert_B(Temp, Aspan, SpanUpperBound);
			//else if (k == 2) RandExchange(Temp, 3);
			else if (k == 2) Exter_CriticalFactory_Swap_B2(Temp, Aspan, SpanUpperBound);
			else Inter_CriticalFactory_Swap_B(Temp, Aspan, SpanUpperBound);


			//RandExchange(Seq, 3);

			int TEnergy = Get_AllFac_EC(Temp, Aspan);
			if (TEnergy < EC)
			{
				Seq = Temp;
				k = 1;
				iteration = 1;
				EC = TEnergy;
			}
			else
			{
				k++;
				//Seq = Temp;
			}

		}
		iteration = iteration + 1;
	}
	return EC;
}

void CheckChrom1(vector<vector<int>> Sol, vector<int>& Aspan, int TotalEC)
{
	vector<bool> bExist(gJobs, false);

	for (int f = 0; f < Sol.size(); f++)
	{
		for (int j = 0; j < Sol[f].size(); j++)
			bExist[Sol[f][j]] = true;
	}

	for (int i = 0; i < bExist.size(); i++)
	{
		if (!bExist[i]) printf("/nchrom is error! Job %d is lost", i);
	}

	if (TotalEC > 0)
	{
		int TempEC = Get_AllFac_EC(Sol, Aspan);
		//		printf("%d\n", TempSpan);
		//	printf("%d\n", Span);
		if (TempEC != TotalEC)
			printf("\n Value is Error! %d, %d", TempEC, TotalEC);
	}
}

int ES(long TimeLimit, int PLen, int P)//PLen = 7,P = 5
{
	long InitTime = GetElapsedProcessTime();
	int SpanUpperBound = GenerateSpanUpperBound();  //生成makespan上界

	 vector<vector<int>>AllSpan (P, vector<int>(gFactory, 0));

	vector<vector<vector<int>>> Seq;
	vector<vector<vector<int>>> Seq_N(3 * P);
	for (int i = 0; i < 3 * P; i++)
		Seq_N[i].resize(gFactory);

	vector<int> Energy(P, 0);
	int Best_Num = INT_MAX;
	vector<vector<int>>BestSeqSofar;
	int BestValueSofar;
	//生成初始解
	Initial(Seq, AllSpan, P, SpanUpperBound);

	for (int i = 0; i < 3; i++)
	{
		Energy[i] = Get_AllFac_EC(Seq[i], AllSpan[i]);
	}

	for (int i = 3; i < P; i++)
		Energy[i] = VLS2(Seq[i], AllSpan[i], SpanUpperBound);
	for (int i = 0; i < P; i++)
	{
		if (Energy[i] < Best_Num)
		{
			BestSeqSofar = Seq[i];
			Best_Num = i;
			BestValueSofar = Energy[i];
		}
	}


	int cnt = 0;
	while (GetElapsedProcessTime() - InitTime < TimeLimit)
	{
		while (cnt < 3 * P)
		{
			int k = wyt_rand(P);
			vector<vector<int>>TempSeq = Seq[k]; //rand() % P


			default_random_engine gen;

			normal_distribution<double> dis(0, 1);

			uniform_int_distribution<> dis1(2, 7);

			int dSi = dis1(gen);

			int N = dis(gen);

			int dS_new = dSi * (exp(N));

			if (dS_new < 2) dS_new = 2;
			if (dS_new > 7) dS_new = 7;



			vector<int>DesJobSeq;
			Destruction_B(TempSeq, DesJobSeq, AllSpan[k], dS_new);//PLen = 7
			Construction_B(TempSeq, DesJobSeq, AllSpan[k], SpanUpperBound);

			int TempSpan = Get_AllFac_EC(TempSeq, AllSpan[k]);
			TempSpan = VLS(TempSeq, AllSpan[k], SpanUpperBound);

			if (TempSpan < BestValueSofar)
			{
				BestSeqSofar = TempSeq;
				BestValueSofar = TempSpan;
				Seq_N[cnt] = TempSeq;
				Best_Num = k;
			}
			cnt++;
		}

	}

	CheckChrom1(BestSeqSofar, AllSpan[Best_Num], BestValueSofar);

	//cout << BestValueSofar << "   ";

	return BestValueSofar;

}

//int ES_Curve(long TimeLimit, int PLen, int P, int No)
//{
//	long InitTime = GetElapsedProcessTime();
//	vector<vector<vector<int>>> Seq;
//	vector<vector<vector<int>>> Seq_N(3 * P);
//	for (int i = 0; i < 3 * P; i++)
//		Seq_N[i].resize(gFactory);
//
//	vector<int> Energy(P, 0);
//	int Best_Num = INT_MAX;
//	vector<vector<int>>BestSeqSofar;
//	int BestValueSofar;
//	//生成初始解
//	Initial(Seq, P);
//
//
//	for (int i = 0; i < 3; i++)
//	{
//		Energy[i] = GetFspan_Block(Seq[i]);
//	}
//
//	for (int i = 3; i < P; i++)
//		Energy[i] = VLS2(Seq[i]);
//	for (int i = 0; i < P; i++)
//	{
//		if (Energy[i] < Best_Num)
//		{
//			BestSeqSofar = Seq[i];
//			Best_Num = i;
//			BestValueSofar = Energy[i];
//		}
//	}
//
//	int cnt = 0;
//
//	//建立一个txt文件
//	string FileDirectory = "Result\\";
//
//	ofstream ofile;
//	ofile.open(FileDirectory + "ES_Curve" + ".txt"); //不同的算法
//	int LocalValue; long LocalTime;
//	//输出初始解以及初始时间
//	LocalValue = BestValueSofar;
//	LocalTime = GetElapsedProcessTime() - InitTime;
//	ofile << LocalTime << "\t" << LocalValue << "\t" << endl;
//	int gen = 0;
//
//	while (GetElapsedProcessTime() - InitTime < TimeLimit)
//	{
//		while (cnt < 3 * P)
//		{
//			vector<vector<int>>TempSeq = Seq[wyt_rand(P)]; //rand() % P
//
//
//			default_random_engine gen;
//
//			normal_distribution<double> dis(0, 1);
//
//			uniform_int_distribution<> dis1(2, 7);
//
//			int dSi = dis1(gen);
//
//			int N = dis(gen);
//
//			int dS_new = dSi * (exp(N));
//
//			if (dS_new < 2) dS_new = 2;
//			if (dS_new > 7) dS_new = 7;
//
//
//
//			vector<int>DesJobSeq;
//			Destruction_B(TempSeq, DesJobSeq, dS_new);//PLen = 7
//			Construction_B(TempSeq, DesJobSeq);
//
//			int TempSpan = GetFspan_Block(TempSeq);
//			TempSpan = VLS(TempSeq);
//
//			if (TempSpan < BestValueSofar)
//			{
//				BestSeqSofar = TempSeq;
//				BestValueSofar = TempSpan;
//				Seq_N[cnt] = TempSeq;
//
//			}
//			cnt++;
//
//
//		}
//		LocalValue = BestValueSofar;                             //
//		LocalTime = GetElapsedProcessTime() - InitTime;         //
//
//		//将得到的LocalValue以及LocalTime写入文件当中
//
//		gen++;
//		if (gen == 80000)
//		{
//			ofile << LocalTime << "\t" << LocalValue << "\t" << endl;
//			gen = 0;
//		}
//
//	}
//	ofile.close();
//	CheckChrom1(BestSeqSofar, BestValueSofar);
//	return BestValueSofar;
//}
