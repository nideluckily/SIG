#pragma once


void NEHInit(vector<int>& JobSeq)
{
	JobSeq.clear();
	JobSeq.resize(gJobs);
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
		JobSeq[j] = ch[j].dim;
	delete[]ch;
}

void NEHInit02(vector<int>& JobSeq)
{
	JobSeq.clear();
	JobSeq.resize(gJobs);
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
		JobSeq[j] = ch[j].dim;
	delete[]ch;
}


int NEHF(vector<vector<int>>& Sol, vector<int>& FactoryEC, vector<int>&Aspan, const int SpanUpperBound)
{
	Sol.clear();
	Sol.resize(gFactory);

	vector<int>FullPermutation;
	FullPermutation.resize(gJobs);
	for (int j = 0; j < gJobs; j++)
		FullPermutation[j] = j;
	NEHInit(FullPermutation);

	for (int f = 0; f < Sol.size(); f++)
	{
		Sol[f].push_back(FullPermutation[f]);

		int ECf = Get_OneFac_EC(Sol[f], Aspan[f]);
		FactoryEC[f] = ECf;
	}

	for (int i = gFactory; i < FullPermutation.size(); i++)
	{
		int SelFac = -1, BestPos = -1;
		int Job = FullPermutation[i];
		int EC = JobToBestFactoryBestposition_GetEC_AfterInsert(Job, Sol, SelFac, BestPos, Aspan, SpanUpperBound);
		//将工件插入最好的位置
		Sol[SelFac].insert(Sol[SelFac].begin() + BestPos, Job);
		FactoryEC[SelFac] = EC;
	}
	return accumulate(FactoryEC.begin(), FactoryEC.end(), 0);
}


int NEHF2(vector<vector<int>>& Sol, vector<int>& FactoryEC, vector<int>& Aspan, const int SpanUpperBound)
{
	Sol.clear();
	Sol.resize(gFactory);

	vector<int>FullPermutation;
	FullPermutation.resize(gJobs);
	for (int j = 0; j < gJobs; j++)
		FullPermutation[j] = j;
	NEHInit(FullPermutation);

	for (int f = 0; f < Sol.size(); f++)
	{
		Sol[f].push_back(FullPermutation[f]);

		int ECf = Get_OneFac_EC(Sol[f], Aspan[f]);
		FactoryEC[f] = ECf;
	}

	for (int i = gFactory; i < FullPermutation.size(); i++)
	{
		int SelFac = -1, BestPos = -1;
		int Job = FullPermutation[i];
		int EC = JobToBestFactoryBestposition_GetEC_AfterInsert(Job, Sol, SelFac, BestPos, Aspan, SpanUpperBound);
		//将工件插入最好的位置
		Sol[SelFac].insert(Sol[SelFac].begin() + BestPos, Job);
		FactoryEC[SelFac] = EC;
		if (Sol[SelFac].size() > 1)
		{
			int tempBpos = -1;
			if (BestPos == 0)
			{
				int RetryJob = Sol[SelFac][BestPos + 1];
				Sol[SelFac].erase(Sol[SelFac].begin() + BestPos + 1);
				int EC1 = FindBestInsertionPositonInOneFactory(tempBpos, RetryJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
				Sol[SelFac].insert(begin(Sol[SelFac]) + tempBpos, RetryJob);
				FactoryEC[SelFac] = EC1;
			}
			else if (BestPos == (Sol[SelFac].size() - 1))
			{
				int RetryJob = Sol[SelFac][BestPos - 1];
				Sol[SelFac].erase(Sol[SelFac].begin() + BestPos - 1);
				int EC1 = FindBestInsertionPositonInOneFactory(tempBpos, RetryJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
				Sol[SelFac].insert(begin(Sol[SelFac]) + tempBpos, RetryJob);
				FactoryEC[SelFac] = EC1;
			}
			else
			{
				int RandNum = rand() % 2;
				if (RandNum == 0)
				{
					int RetryJob = Sol[SelFac][BestPos + 1];
					Sol[SelFac].erase(Sol[SelFac].begin() + BestPos + 1);
					int EC1 = FindBestInsertionPositonInOneFactory(tempBpos, RetryJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
					Sol[SelFac].insert(begin(Sol[SelFac]) + tempBpos, RetryJob);
					FactoryEC[SelFac] = EC1;
				}
				else
				{
					int RetryJob = Sol[SelFac][BestPos - 1];
					Sol[SelFac].erase(Sol[SelFac].begin() + BestPos - 1);
					int EC1 = FindBestInsertionPositonInOneFactory(tempBpos, RetryJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
					Sol[SelFac].insert(begin(Sol[SelFac]) + tempBpos, RetryJob);
					FactoryEC[SelFac] = EC1;
				}
			}
		}
	}
	return accumulate(FactoryEC.begin(), FactoryEC.end(), 0);
}


int NEH2_In(vector<vector<int>>& Sol, vector<int>& Aspan, int SpanUpperBound)
{
	Sol.clear();
	Sol.resize(gFactory);

	vector<int>FullPermutation;
	FullPermutation.resize(gJobs);
	for (int j = 0; j < gJobs; j++)
		FullPermutation[j] = j;
	NEHInit02(FullPermutation); //做完此操作后工件序列按加工时间升序排列

	int FinalEC = 0;
	for (int i = 0; i < FullPermutation.size(); i++)
	{
		int Job = FullPermutation[i];
		int SelFac = 0;
		int BestPos = 0;
		FinalEC = Job_To_BestFactory_Bestposition(Job, Sol, SelFac, BestPos, Aspan, SpanUpperBound);
		if (Sol[SelFac].size() > 1) //wy:感觉可以i使用swap代替insert试试
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
				int RandNum = rand() % 2;
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

int NEH2_en(vector<vector<int>>& Sol, vector<int>& Aspan, int SpanUpperBound)
{
	Sol.clear();
	Sol.resize(gFactory);

	vector<int>FullPermutation;
	FullPermutation.resize(gJobs);
	for (int j = 0; j < gJobs; j++)
		FullPermutation[j] = j;
	NEHInit(FullPermutation); //做完此操作后工件序列按加工时间降序排列

	int FinalEC = 0;
	for (int i = 0; i < FullPermutation.size(); i++)
	{
		int Job = FullPermutation[i];
		int SelFac = 0;
		int BestPos = 0;
		FinalEC = Job_To_BestFactory_Bestposition(Job, Sol, SelFac, BestPos, Aspan, SpanUpperBound);
		if (Sol[SelFac].size() > 1) //wy:感觉可以i使用swap代替insert试试
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
				int RandNum = rand() % 2;
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


void GetAscendingOrder(vector<int>& JobAscendingOrder)
{
	JobAscendingOrder.clear();
	JobAscendingOrder.resize(gJobs);
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
		JobAscendingOrder[j] = ch[j].dim;
	delete[]ch;
}

//Generate a job sequence in non-increasing order
void GetNonIncreasingOrder(vector<int>& NonIncreasingOrder)
{
	NonIncreasingOrder.clear();
	NonIncreasingOrder.resize(gJobs);
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
		NonIncreasingOrder[j] = ch[j].dim;
	delete[]ch;
}


int DSPT(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	Seq.resize(gFactory);
	for (int i = 0; i < Seq.size(); i++)
		Seq[i].clear();

	vector<int>JobAscendingOrder;
	GetAscendingOrder(JobAscendingOrder);

	int Counter = 0;
	for (int f = 0; f < Seq.size(); f++)
	{
		Seq[f].push_back(JobAscendingOrder[f]);
		Counter = f;
	}

	int TEC;
	while (Counter < gJobs - 1)
	{
		Counter = Counter + 1;

		TEC = Job_To_BestFactory_Bestposition_DPSO(JobAscendingOrder[Counter], Seq, Aspan, SpanUpperBound);

	}

	//while (Counter < gJobs - 1)
	//{
	//	Counter = Counter + 1;
	//	vector<vector<int>>TempSeq = Seq;
	//	int FacPt = -1;
	//	int LocalSpan = INT_MAX;
	//	for (int f = 0; f < TempSeq.size(); f++)
	//	{
	//		TempSeq[f].push_back(JobAscendingOrder[Counter]);
	//		int TempSpan = Get_AllFac_EC(TempSeq, Aspan);
	//		if (TempSpan < LocalSpan && Aspan[f]<=SpanUpperBound)
	//		{
	//			LocalSpan = TempSpan;
	//			FacPt = f;
	//		}
	//		TempSeq = Seq;
	//	}
	//	Seq[FacPt].push_back(JobAscendingOrder[Counter]);
	//}
	//int TEC = Get_AllFac_EC(Seq, Aspan);
	return TEC;
}

int DNEH(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	Seq.resize(gFactory);
	for (int i = 0; i < Seq.size(); i++)
		Seq[i].clear();

	vector<int>NonIncreasingOrder;
	GetNonIncreasingOrder(NonIncreasingOrder);

	int Counter = 0;
	for (int f = 0; f < Seq.size(); f++)
	{
		Seq[f].push_back(NonIncreasingOrder[f]);
		Counter = f;
	}

	int TEC = Get_AllFac_EC(Seq, Aspan);
	while (Counter < gJobs - 1)
	{
		Counter = Counter + 1;

		TEC = Job_To_BestFactory_Bestposition_DPSO(NonIncreasingOrder[Counter], Seq, Aspan, SpanUpperBound);

	}
	return TEC;
}