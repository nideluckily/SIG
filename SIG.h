#pragma once

int NEHF_MaxPTJobDisturbance(vector<vector<int>>& Sol, vector<int>& FactoryEC, vector<int>& Aspan, const int SpanUpperBound)
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
				int LeftJob = Sol[SelFac][BestPos - 1];
				int RightJob = Sol[SelFac][BestPos + 1];
				int LeftJobPT = 0, RightJobPT = 0;
				for (int s = 0; s < gStage; s++)
				{
					LeftJobPT += PTime[LeftJob][s];
					RightJobPT += PTime[RightJob][s];
				}
				if (LeftJobPT > RightJobPT)
				{
					Sol[SelFac].erase(Sol[SelFac].begin() + BestPos - 1);
					int EC1 = FindBestInsertionPositonInOneFactory(tempBpos, LeftJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
					Sol[SelFac].insert(begin(Sol[SelFac]) + tempBpos, LeftJob);
					FactoryEC[SelFac] = EC1;
				}
				else
				{
					Sol[SelFac].erase(Sol[SelFac].begin() + BestPos + 1);
					int EC1 = FindBestInsertionPositonInOneFactory(tempBpos, RightJob, Sol[SelFac], Aspan[SelFac], SpanUpperBound);
					Sol[SelFac].insert(begin(Sol[SelFac]) + tempBpos, RightJob);
					FactoryEC[SelFac] = EC1;
				}
			}
		}
	}
	return accumulate(FactoryEC.begin(), FactoryEC.end(), 0);
}


void DestructionInOneFac(vector<int>& Seq, vector<int>& DesJobSeq,  int&d)
{
	DesJobSeq.clear();
	if (d >= Seq.size())
	{
		d = 3;
	}
	while (DesJobSeq.size() < d)
	{
		int pt = rand() % Seq.size();
		DesJobSeq.push_back(Seq[pt]);
		Seq.erase(Seq.begin() + pt);
	}
}

int ReconstructionInOneFac(vector<int>& Seq, vector<int>& DesJobSeq, int& tempSpan, const int SpanUpperBound, int& Beoverstep)
{
	int minEC, cnt = 0;
	vector<int>tempSeq = Seq;

	int i = 0;
	while (i < DesJobSeq.size())
	{
		int DesJob = DesJobSeq[i];
		int BestPos = -1;

		minEC = FindBestInsertionPositonInOneFactory(BestPos, DesJob, Seq, tempSpan, SpanUpperBound);
		if (BestPos != -1)
		{
			Seq.insert(Seq.begin() + BestPos, DesJob);
			i++;
		}
		else
		{
			if (cnt == 0)
			{
				Seq = tempSeq;
				shuffle(DesJobSeq.begin(), DesJobSeq.end(), rand_generator());
				i = 0;
				cnt++;
			}
			else
			{
				Beoverstep = 1;
				break;
			}
		}
	}
	return minEC;
}

int DestructionAndReconstruction(vector< vector<int>>& Sol, vector<int>& FactoryEC, vector<int>& Aspan,
	int& d, int SpanUpperBound)
{
	int tempEC;
	int c = 0;
	vector<int> DesJobSeq, tempSeq;

	//while (c < 2)
	//{
	//	if (c == 0)   //对关键工厂操作
	//	{
	//		int maxValue = FactoryEC[0];
	//		int maxFac = 0;
	//		for (int i = 1; i < Sol.size(); i++)
	//		{
	//			if (maxValue < FactoryEC[i])
	//			{
	//				maxValue = FactoryEC[i];
	//				maxFac = i;
	//			}
	//		}
	//		tempSeq = Sol[maxFac];
	//		int temspan = Aspan[maxFac];
	//		int Beoverstep = 0;   //判断span是否越界

	//		DestructionInOneFac(tempSeq, DesJobSeq, d);
	//		tempEC = ReconstructionInOneFac(tempSeq, DesJobSeq, Aspan[maxFac], SpanUpperBound, Beoverstep);
	//		if (tempEC < maxValue && Beoverstep == 0)
	//		{
	//			Sol[maxFac] = tempSeq;
	//			FactoryEC[maxFac] = tempEC;
	//		}
	//		else
	//		{
	//			Aspan[maxFac] = temspan;
	//		}
	//	}
	//	else
	//	{
	//		int ranFac = rand() % Sol.size();
	//		int ranValue = FactoryEC[ranFac];
	//		tempSeq = Sol[ranFac];
	//		int temspan = Aspan[ranFac];
	//		int Beoverstep = 0;   //判断span是否越界

	//		DestructionInOneFac(tempSeq, DesJobSeq, d);
	//		tempEC = ReconstructionInOneFac(tempSeq, DesJobSeq, Aspan[ranFac], SpanUpperBound, Beoverstep);
	//		if (tempEC < ranValue && Beoverstep == 0)
	//		{
	//			Sol[ranFac] = tempSeq;
	//			FactoryEC[ranFac] = tempEC;
	//		}
	//		else
	//		{
	//			Aspan[ranFac] = temspan;
	//		}
	//	}
	//	c++;
	//}
	int maxValue = FactoryEC[0];
	int maxFac = 0;
	for (int i = 1; i < Sol.size(); i++)
	{
		if (maxValue < FactoryEC[i])
		{
			maxValue = FactoryEC[i];
			maxFac = i;
		}
	}
	tempSeq = Sol[maxFac];
	int temspan = Aspan[maxFac];
	int Beoverstep = 0;   //判断span是否越界

	DestructionInOneFac(tempSeq, DesJobSeq, d);
	tempEC = ReconstructionInOneFac(tempSeq, DesJobSeq, Aspan[maxFac], SpanUpperBound, Beoverstep);
	if (tempEC < maxValue && Beoverstep == 0)
	{
		Sol[maxFac] = tempSeq;
		FactoryEC[maxFac] = tempEC;
	}
	else
	{
		Aspan[maxFac] = temspan;
	}
	int TEC = accumulate(FactoryEC.begin(), FactoryEC.end(), 0);
	return TEC;
}


int FacInsert(vector< vector<int>>& Sol, vector<int>& FactoryEC, vector<int>& Aspan, int SpanUpperBound)
{
	int maxValue = FactoryEC[0];
	int maxFac = 0;
	int minValue = FactoryEC[0];
	int minFac = 0;
	for (int i = 1; i < Sol.size(); i++)
	{
		if (maxValue < FactoryEC[i])
		{
			maxValue = FactoryEC[i];
			maxFac = i;
		}
		if (minValue > FactoryEC[i])
		{
			minValue = FactoryEC[i];
			minFac = i;
		}
	}
	int minspan = Aspan[minFac];

	vector<int> JobPTime(Sol[maxFac].size(), 0);
	int maxJob, maxpt, maxJobPTime = 0;

	for (int i = 0; i < Sol[maxFac].size(); i++)  //寻找处理时间最大的工件
	{
		for (int s = 0; s < gStage; s++)
		{
			JobPTime[i] += PTime[Sol[maxFac][i]][s];
		}
		if (JobPTime[i] > maxJobPTime)
		{
			maxJobPTime = JobPTime[i];
			maxJob = Sol[maxFac][i];
			maxpt = i;
		}
	}

	int bestPos = -1;
	int tempEC = FindBestInsertionPositonInOneFactory(bestPos, maxJob, Sol[minFac], Aspan[minFac], SpanUpperBound);
	if (tempEC < maxValue)
	{
		Sol[maxFac].erase(Sol[maxFac].begin() + maxpt);
		Sol[minFac].insert(Sol[minFac].begin() + bestPos, maxJob);
		FactoryEC[maxFac] = Get_OneFac_EC(Sol[maxFac], Aspan[maxFac]);
		FactoryEC[minFac] = tempEC;
	}
	else
	{
		Aspan[minFac] = minspan;
	}
	return accumulate(FactoryEC.begin(), FactoryEC.end(), 0);
}

int FacSwap(vector< vector<int>>& Sol, vector<int>& FactoryEC, vector<int>& Aspan, int SpanUpperBound)
{
	int maxValue = FactoryEC[0];
	int maxFac = 0;

	for (int i = 1; i < Sol.size(); i++)
	{
		if (maxValue < FactoryEC[i])
		{
			maxValue = FactoryEC[i];
			maxFac = i;
		}
	}
	int n = Sol[maxFac].size();

	for (int i = 0; i < n; i++)
	{
		int maxspan = Aspan[maxFac];
		int pt1 = rand() % Sol[maxFac].size();
		int ranFac, ranspan;

		do {
			ranFac = rand() % gFactory;
		} while (ranFac == maxFac);
		ranspan = Aspan[ranFac];

		int pt2 = rand() % Sol[ranFac].size();
		swap(Sol[maxFac][pt1], Sol[ranFac][pt2]);

		int EC1 = Get_OneFac_EC(Sol[maxFac], Aspan[maxFac]);
		int EC2 = Get_OneFac_EC(Sol[ranFac], Aspan[ranFac]);

		if (EC1 + EC2 < FactoryEC[maxFac] + FactoryEC[ranFac] && max(Aspan[maxFac], Aspan[ranFac]) <= SpanUpperBound)
		{
			FactoryEC[maxFac] = EC1;
			FactoryEC[ranFac] = EC2;

			maxValue = FactoryEC[0];
			for (int j = 1; j < Sol.size(); j++)
			{
				if (maxValue < FactoryEC[j])
				{
					maxValue = FactoryEC[j];
					maxFac = j;
				}
			}
		}
		else
		{
			swap(Sol[maxFac][pt1], Sol[ranFac][pt2]);
			Aspan[maxFac] = maxspan;
			Aspan[ranFac] = ranspan;
		}
	}
	return accumulate(FactoryEC.begin(), FactoryEC.end(), 0);
}

int VND_Fac(vector< vector<int>>& Sol, vector<int>& FactoryEC, vector<int>& Aspan, int BestValue, int SpanUpperBound, float& R0)
{
	vector<vector<int>>tempSol;
	vector<int>tempAspan;
	vector<int>tempFactoryEC;
	int EC, count = 1;

	while (count <= 2)
	{
		tempSol = Sol;
		tempAspan = Aspan;
		tempFactoryEC = FactoryEC;

		switch (count)
		{
		case 1: EC = FacInsert(Sol, FactoryEC, Aspan, SpanUpperBound);
			break;
		case 2: EC = FacSwap(Sol, FactoryEC, Aspan, SpanUpperBound);
			break;
		}
		if (EC < BestValue)
		{

			BestValue = EC;
			count = 1;
			R0++;
		}
		else
		{
			Sol = tempSol;
			Aspan = tempAspan;
			FactoryEC = tempFactoryEC;
			count++;
		}
	}
	return BestValue;
}


int JobInsert(vector<int>& Seq, int EC, int& Span, int SpanUpperBound)
{
	int ranspan = Span;
	int tempEC;

	vector<int> JobPTime(Seq.size(), 0);
	int maxJob, maxpt, maxJobPTime = 0;

	for (int i = 0; i < Seq.size(); i++)  //寻找处理时间最大的工件
	{
		for (int s = 0; s < gStage; s++)
		{
			JobPTime[i] += PTime[Seq[i]][s];
		}
		if (JobPTime[i] > maxJobPTime)
		{
			maxJobPTime = JobPTime[i];
			maxJob = Seq[i];
			maxpt = i;
		}
	}

	Seq.erase(Seq.begin() + maxpt);

	int BestPos = -1;
	tempEC = FindBestInsertionPositonInOneFactory(BestPos, maxJob, Seq, Span, SpanUpperBound);
	if (tempEC < EC)
	{
		Seq.insert(Seq.begin() + BestPos, maxJob);
		EC = tempEC;
	}

	return EC;
}

int JobSwap(vector<int>& Seq, int EC, int& Span, int SpanUpperBound)
{
	int ranspan = Span;
	int tempEC;

	//vector<int> JobPTime(Seq.size(), 0);
	int  maxpt, maxJobPTime = INT_MAX;

	//for (int i = 0; i < Seq.size(); i++)  //寻找处理时间最大的工件
	//{
	//	for (int s = 0; s < gStage; s++)
	//	{
	//		JobPTime[i] += PTime[Seq[i]][s];
	//	}
	//	if (JobPTime[i] < maxJobPTime)
	//	{
	//		maxJobPTime = JobPTime[i];
	//		maxpt = i;
	//	}
	//}
	maxpt = rand() % Seq.size();

	for (int i = 0; i < Seq.size(); i++)
	{
		if (Seq[maxpt] != Seq[i])
		{
			swap(Seq[maxpt], Seq[i]);
			tempEC = Get_OneFac_EC(Seq, Span);
			if (tempEC < EC && Span <= SpanUpperBound)
			{
				EC = tempEC;
			}
			else
			{
				swap(Seq[i], Seq[maxpt]);
				Span = ranspan;
			}
		}
	}

	return EC;
}

int VND_Job(vector< vector<int>>& Sol, vector<int>& FactoryEC, vector<int>& Aspan, int SpanUpperBound, float& R1)
{
	vector<int>tempSeq;
	int tempspan, tempEC;
	int count = 1;
	int EC;

	int ranFac = rand() % Sol.size();

	while (count <= 2)
	{
		tempSeq = Sol[ranFac];
		tempspan = Aspan[ranFac];
		tempEC = FactoryEC[ranFac];

		switch (count)
		{
		case 1: EC = JobInsert(Sol[ranFac], FactoryEC[ranFac], Aspan[ranFac], SpanUpperBound);
			break;
		case 2: EC = JobSwap(Sol[ranFac], FactoryEC[ranFac], Aspan[ranFac], SpanUpperBound);
			break;
		}
		if (EC < tempEC)
		{
			FactoryEC[ranFac] = EC;
			count = 1;
			R1++;
		}
		else
		{
			Sol[ranFac] = tempSeq;
			Aspan[ranFac] = tempspan;
			count++;
		}
	}
	return accumulate(FactoryEC.begin(), FactoryEC.end(), 0);
}



int SIG(long TimeLimit, int CPU)
{
	long InitTime = GetElapsedProcessTime();
	double Temperature = GetTemperature();

	int SpanUpperBound = GenerateSpanUpperBound();  //生成makespan上界

	vector<int> Aspan(gFactory, 0);		//记录每个工厂的span，防止越界
	vector<int> FactoryEC(gFactory, 0);		//记录每个工厂的能耗
	vector<vector<int>> Sol;      //二维数组，（工厂，队列）
	vector<int> Dlist = { 3, 4, 5, 6, 7 };

	int SeqEC;
	int iterNum = 0;
	float R0 = 0, R1 = 0;

	//初始化
	//BestValue = NEH22(Sol, FactoryEC, Aspan, SpanUpperBound);
	SeqEC = NEHF_MaxPTJobDisturbance(Sol, FactoryEC, Aspan, SpanUpperBound);

	vector<vector<int>> BestSol = Sol;
	vector<int> BestAspan = Aspan;
	int BestValue = SeqEC;


	while ((GetElapsedProcessTime() - InitTime) < TimeLimit)
	{
		srand(time(0));
		vector<vector<int>> TempSol = Sol;
		vector<int> TempFactoryEC = FactoryEC;
		vector<int> TempAspan = Aspan;
		int TempEC = SeqEC;

		int d = Dlist[rand() % Dlist.size()];

		//破坏重构
		TempEC = DestructionAndReconstruction(TempSol, TempFactoryEC, TempAspan, d, SpanUpperBound);
		if (TempEC < SeqEC)
		{
			SeqEC = TempEC;
			Sol = TempSol;
			FactoryEC = TempFactoryEC;
			Aspan = TempAspan;
			if (TempEC < BestValue)
			{
				BestSol = TempSol;
				BestAspan = TempAspan;
				BestValue = TempEC;
				Dlist.push_back(d);
			}
		}

		//VNS  +  反馈学习

		if ((GetElapsedProcessTime() - InitTime) < TimeLimit * 0.5)
		{
			int R = rand() % 2;
			if (R == 0)
			{
				TempEC = VND_Fac(TempSol, TempFactoryEC, TempAspan, TempEC, SpanUpperBound, R0);
			}
			else
			{
				TempEC = VND_Job(TempSol, TempFactoryEC, TempAspan, SpanUpperBound, R1);
			}
		}
		else
		{
			if (R0 == 0 && R1 == 0)
			{
				TempEC = VND_Fac(TempSol, TempFactoryEC, TempAspan, TempEC, SpanUpperBound, R0);
				TempEC = VND_Job(TempSol, TempFactoryEC, TempAspan, SpanUpperBound, R1);
			}
			else
			{
				float r = rand() % 1000 / (float)1000;
				if (r <= ((float)R0 / (R0 + R1)))
				{
					TempEC = VND_Fac(TempSol, TempFactoryEC, TempAspan, TempEC, SpanUpperBound, R0);
				}
				else
				{
					TempEC = VND_Job(TempSol, TempFactoryEC, TempAspan, SpanUpperBound, R1);
				}
			}
		}


		//接受准则

		if (TempEC < SeqEC)
		{
			SeqEC = TempEC;
			Sol = TempSol;
			FactoryEC = TempFactoryEC;
			Aspan = TempAspan;
			if (TempEC < BestValue)
			{
				BestSol = TempSol;
				BestAspan = TempAspan;
				BestValue = TempEC;
			}
		}
		else if (((double)rand() / RAND_MAX) <= exp(-(TempEC - SeqEC) / Temperature))
		{
			SeqEC = TempEC;
			Sol = TempSol;
			FactoryEC = TempFactoryEC;
			Aspan = TempAspan;
		}
		iterNum++;
	}

	CheckChrom1(BestSol, BestAspan, BestValue);

	//cout << BestValue << "(" << iterNum << ")" << "  ";

	return BestValue;
}