#pragma once

void Middle_insert(vector<int>& JobSeq)
{

	vector<int> tempSeq;
	tempSeq.resize(gJobs, 0);

	int count = 0;
	int i = 0;
	while (count < gJobs / 2)
	{
		tempSeq[i] = JobSeq[count];
		count++;
		i = i + 2;
	}

	i = 1;
	while (count < gJobs)
	{
		tempSeq[i] = JobSeq[count];
		count++;
		i = i + 2;
	}

	JobSeq = tempSeq;
}

int DNEH_SMR(vector<vector<int>>& Sol, vector<int>& Aspan, int SpanUpperBound)
{
	Sol.clear();
	Sol.resize(gFactory);

	vector<int>FullPermutation;
	FullPermutation.resize(gJobs);
	for (int j = 0; j < gJobs; j++)
		FullPermutation[j] = j;

	NEHInit02(FullPermutation);
	Middle_insert(FullPermutation);

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

void Swap_between(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound) // swap two neighbor jobs
{
	vector<vector<int>>tempSeq;
	tempSeq = Seq;
	bool flag = true;

	int maxvalue = -1;
	int maxFac = -1;
	for (int i = 0; i < Seq.size(); i++)
	{
		if (maxvalue < Get_OneFac_EC(Seq[i], Aspan[i]))
		{
			maxvalue = Get_OneFac_EC(Seq[i], Aspan[i]);
			maxFac = i;
		}
	}
	while (flag == true)
	{
		flag = false;

		int FinalEC;
		for (int i = 0; i < tempSeq[maxFac].size(); i++)
		{
			int Job = tempSeq[maxFac][i];
			int minEC = INT_MAX;
			int SelFac = 0;
			int BestPos = 0;

			FinalEC = Job_ToSwap_BestFactory_Bestposition(maxFac, i, Job, tempSeq, SelFac, BestPos, Aspan, SpanUpperBound);
			if (FinalEC < Get_AllFac_EC(Seq, Aspan))
			{
				flag = true;
				Seq = tempSeq;
			}
			else {
				tempSeq = Seq;
			}
		}

	}
	if (Get_AllFac_EC(tempSeq, Aspan) < Get_AllFac_EC(Seq, Aspan))
	{
		Seq = tempSeq;
	}
}

void Swap_Inner2(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	vector<vector<int>>tempSeq;
	tempSeq = Seq;
	bool flag = true;
	while (flag == true)
	{
		flag = false;
		int maxvalue = -1;
		int maxFac = -1;
		for (int i = 0; i < Seq.size(); i++)
		{
			if (maxvalue < Get_OneFac_EC(Seq[i], Aspan[i]))
			{
				maxvalue = Get_OneFac_EC(Seq[i], Aspan[i]);
				maxFac = i;
			}
		}
		int FinalEC;
		for (int i = 0; i < tempSeq[maxFac].size(); i++)
		{
			int temp;
			int pos;
			vector<vector<int>>tempSeq02;
			tempSeq02 = tempSeq;

			int minvalue = Get_AllFac_EC(tempSeq,Aspan);

			for (int j = 0; j < tempSeq02[maxFac].size(); j++)
			{
				temp = tempSeq02[maxFac][i];
				tempSeq02[maxFac][i] = tempSeq02[maxFac][j];
				tempSeq02[maxFac][j] = temp;
				if (Get_AllFac_EC(tempSeq02, Aspan) < minvalue)
				{
					minvalue = Get_AllFac_EC(tempSeq02, Aspan);
					pos = j;
				}
				tempSeq02 = tempSeq;
			}
			if (minvalue < Get_AllFac_EC(tempSeq, Aspan))
			{
				temp = tempSeq[maxFac][i];
				tempSeq[maxFac][i] = tempSeq[maxFac][pos];
				tempSeq[maxFac][pos] = temp;
				flag = true;
			}

		}

	}
	if (Get_AllFac_EC(tempSeq, Aspan) < Get_AllFac_EC(Seq, Aspan))
	{
		Seq = tempSeq;
	}
}

void Insertion_between(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	vector<vector<int>>tempSeq;
	tempSeq = Seq;
	int bestValuesofar = Get_AllFac_EC(Seq, Aspan);

	int maxvalue = -1;
	int maxFac = -1;
	for (int i = 0; i < Seq.size(); i++)
	{
		if (maxvalue < Get_OneFac_EC(Seq[i], Aspan[i]))
		{
			maxvalue = Get_OneFac_EC(Seq[i], Aspan[i]);
			maxFac = i;
		}
	}
	int FinalEC;
	for (int i = 0; i < tempSeq[maxFac].size(); i++)
	{
		int Job = tempSeq[maxFac][i];
		int SelFac = 0;
		int BestPos = 0;
		FinalEC = Job_To_BestFactory_Bestposition(Job, tempSeq, SelFac, BestPos, Aspan, SpanUpperBound);
		if (FinalEC < bestValuesofar)
		{
			continue;
		}
		else {
			tempSeq = Seq;
		}
	}

	if (Get_AllFac_EC(tempSeq, Aspan) < Get_AllFac_EC(Seq, Aspan))
	{
		Seq = tempSeq;
	}
}

void Insertion_Inner(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	vector<vector<int>>tempSeq;
	tempSeq = Seq;
	bool flag = true;
	int bestValuesofar = Get_AllFac_EC(Seq,Aspan);
	int maxvalue = -1;
	int maxFac = -1;
	for (int i = 0; i < Seq.size(); i++)
	{
		if (maxvalue < Get_OneFac_EC(Seq[i], Aspan[i]))
		{
			maxvalue = Get_OneFac_EC(Seq[i], Aspan[i]);
			maxFac = i;
		}
	}

	if (tempSeq[maxFac].size() > 1)
	{
		int Pos = 0;
		for (int count = 0; count < Seq[maxFac].size(); ++count)
		{
			int RetryJob = tempSeq[maxFac][Pos];
			tempSeq[maxFac].erase(tempSeq[maxFac].begin() + Pos);

			int BestPos = -1;
			FindBestInsertionPositon(BestPos, RetryJob, tempSeq[maxFac], Aspan[maxFac], SpanUpperBound);

			//Job_To_SomeFactory_Bestposition(RetryJob, tempSeq[maxFac], Aspan[maxFac], SpanUpperBound);
			if (BestPos != -1)
			{
				if (Get_AllFac_EC(tempSeq, Aspan) < Get_AllFac_EC(Seq, Aspan))
				{
					tempSeq[maxFac].insert(tempSeq[maxFac].begin() + BestPos, RetryJob);
					Seq = tempSeq;

				}
				else {
					tempSeq = Seq;
				}
			}
			else
			{
				tempSeq = Seq;
			}
		}
	}

	if (Get_AllFac_EC(tempSeq, Aspan) < Get_AllFac_EC(Seq, Aspan))
	{
		Seq = tempSeq;
	}

}


int Multi_neighborhood_LocalSearch(vector<vector<int>>& Seq, long TimeLimit, vector<int>& Aspan, int SpanUpperBound)
{
	int bestValuesoFar = Get_AllFac_EC(Seq, Aspan);
	int Lmax = 4;
	int L = 1;
	while (L <= Lmax)
	{
		if (GetElapsedProcessTime() == TimeLimit) break;

		switch (L)
		{
		case 1:Insertion_between(Seq, Aspan, SpanUpperBound); break;
		case 2:Swap_between(Seq, Aspan, SpanUpperBound); break;
		case 3:Insertion_Inner(Seq, Aspan, SpanUpperBound); break;
		case 4:Swap_Inner2(Seq, Aspan, SpanUpperBound); break;
		}
		if (bestValuesoFar > Get_AllFac_EC(Seq, Aspan))
		{
			bestValuesoFar = Get_AllFac_EC(Seq, Aspan);
			L = 1;
		}
		else {
			L = L + 1;
		}
	}
	return bestValuesoFar;
}

void Destruction02(vector<vector<int>>& Seq, vector<int>& DesJobSeq, int PLen)
{
	DesJobSeq.clear();

	int d = 4;

	while (DesJobSeq.size() < d)
	{
		int Fpt = rand() % Seq.size();
		//if ((Seq[Fpt].size() > 1))
		//{
		//	int pt = rand() % Seq[Fpt].size();
		//	DesJobSeq.push_back(Seq[Fpt][pt]);
		//	Seq[Fpt].erase(Seq[Fpt].begin() + pt);
		//}
		int pt = rand() % Seq[Fpt].size();
		DesJobSeq.push_back(Seq[Fpt][pt]);
		Seq[Fpt].erase(Seq[Fpt].begin() + pt);
	}
}

int Construction02(vector<vector<int>>& Seq, vector<int>DesJobSeq, vector<int>& Aspan, int SpanUpperBound)
{
	for (int i = 0; i < DesJobSeq.size(); i++)
	{
		int Job = DesJobSeq[i];
		int SelFac = 0;
		int BestPos = 0;
		Job_To_BestFactory_Bestposition(Job, Seq, SelFac, BestPos, Aspan, SpanUpperBound);
	}
		
	int FinalEC = Get_AllFac_EC(Seq,Aspan);
	return FinalEC;
}


int MNIG(long TimeLimit, int PLen, double Tem)
{
	long InitTime = GetElapsedProcessTime();
	int SpanUpperBound = GenerateSpanUpperBound();  //生成makespan上界

	vector<vector<int>>Seq;
	vector<int>Aspan(gFactory, 0);

	int EC = DNEH_SMR(Seq, Aspan, SpanUpperBound);

	vector<vector<int>>BestSeqSofar = Seq;
	int BestValueSofar = EC;

	double Temperature = GetTemperature02(Tem);

	while (GetElapsedProcessTime() - InitTime < TimeLimit)
	{
		vector<vector<int>>TempSeq = Seq;
		int TempSpan = EC;


		TempSpan = Multi_neighborhood_LocalSearch(TempSeq, TimeLimit, Aspan, SpanUpperBound);

		if (TempSpan < EC)
		{
			EC = TempSpan;
			Seq = TempSeq;

			if (EC < BestValueSofar)
			{
				BestValueSofar = EC;
				BestSeqSofar = Seq;
			}
		}

		else if (((double)rand() / RAND_MAX) <= exp(-(Get_AllFac_EC(TempSeq, Aspan) - Get_AllFac_EC(Seq, Aspan)) / Temperature)) {
			Seq = TempSeq;
		}

		vector<int>DesJobSeq;
		Destruction02(TempSeq, DesJobSeq, PLen);
		Construction02(TempSeq, DesJobSeq, Aspan, SpanUpperBound);
	}
	//GetFspan01(BestSeqSofar);
	CheckChrom1(BestSeqSofar, Aspan, BestValueSofar);
	//cout << BestValueSofar << "   ";
	return BestValueSofar;
}

//int MNIG_Curve(long TimeLimit, int PLen, double Tem, int LocalValue, long LocalTime, int No)
//{
//	long InitTime = GetElapsedProcessTime();
//	vector<vector<int>>Seq;
//	int Span = DNEH_SMR(Seq);
//
//	vector<vector<int>>BestSeqSofar = Seq;
//	int BestValueSofar = Span;
//	double Temperature = GetTemperature(Tem);
//
//	//建立一个txt文件
//	ostringstream str;
//	str << "Test_MN_IG_Curve_Ins=" << No << ".txt";                //
//	ofstream ofile;
//	ofile.open(str.str());
//
//	//输出初始解以及初始时间
//	LocalValue = BestValueSofar;                             //
//	LocalTime = GetElapsedProcessTime() - InitTime;         //
//	ofile << LocalTime << "\t" << LocalValue << "\t" << endl;
//
//	while (GetElapsedProcessTime() - InitTime < TimeLimit)
//	{
//		vector<vector<int>>TempSeq = Seq;
//		int TempSpan = Span;
//
//
//		TempSpan = Multi_neighborhood_LocalSearch(TempSeq, TimeLimit);
//
//		if (TempSpan < Span)
//		{
//			Span = TempSpan;
//			Seq = TempSeq;
//
//			if (Span < BestValueSofar)
//			{
//				BestValueSofar = Span;
//				BestSeqSofar = Seq;
//			}
//		}
//		else
//		{
//			double  t = exp((Span - TempSpan) / Temperature);            //改过的模拟退火
//			if (rand() < int(RAND_MAX * t))
//			{
//				Span = TempSpan;
//				Seq = TempSeq;
//			}
//		}
//		vector<int>DesJobSeq;
//		Destruction02(TempSeq, DesJobSeq, PLen);
//		Construction02(TempSeq, DesJobSeq);
//
//		LocalValue = BestValueSofar;                             //
//		LocalTime = GetElapsedProcessTime() - InitTime;         //
//		ofile << LocalTime << "\t" << LocalValue << "\t" << endl;
//	}
//	ofile.close();
//
//	CheckChrom(BestSeqSofar, BestValueSofar);
//	return BestValueSofar;
//}
