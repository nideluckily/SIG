#pragma once


void SwapJobs_random_betweenTwoFac(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound) // swap two neighbor jobs
{
	vector<vector<int>>tempSeq;
	tempSeq = Seq;

	int maxvalue = -1;
	int maxFac = -1;
	for (int i = 0; i < gFactory; i++) //wy：寻找关键工厂
	{
		if (maxvalue < Get_OneFac_EC(Seq[i], Aspan[i]))
		{
			maxvalue = Get_OneFac_EC(Seq[i], Aspan[i]);
			maxFac = i;
		}
	}

	int Fpt1;
	do
	{
		Fpt1 = rand() % gFactory;

	} while (Fpt1 == maxFac);

	int FinalEC;

	vector<int> tempAspan = Aspan;
	for (int i = 0; i < gJobs * gJobs; i++)
	{
		int r1 = rand() % Seq[maxFac].size();
		int r2 = rand() % Seq[Fpt1].size();
		swap(tempSeq[maxFac][r1], tempSeq[Fpt1][r2]);

		FinalEC = Get_AllFac_EC(tempSeq, tempAspan);
		if (FinalEC < Get_AllFac_EC(Seq, Aspan) && *max_element(tempAspan.begin(), tempAspan.end()) <= SpanUpperBound)
		{
			Seq = tempSeq;
			Aspan = tempAspan;
		}
		else {
			tempSeq = Seq;
			tempAspan = Aspan;
		}

	}


	if (Get_AllFac_EC(tempSeq, tempAspan) < Get_AllFac_EC(Seq, Aspan))
	{
		Seq = tempSeq;
	}

}

void SwapJobs_random_betweenTwoFac02(vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound) // swap two neighbor jobs
{
	vector<vector<int>>tempSeq;
	tempSeq = Seq;

	int Fpt1, Fpt2;
	do
	{
		Fpt1 = rand() % gFactory;
		Fpt2 = rand() % gFactory;
	} while (Fpt1 == Fpt2);

	int FinalEC;

	vector<int> tempAspan = Aspan;
	for (int i = 0; i < gJobs * gJobs; i++)
	{
		int r1 = rand() % Seq[Fpt1].size();
		int r2 = rand() % Seq[Fpt2].size();
		swap(tempSeq[Fpt2][r2], tempSeq[Fpt1][r1]);

		FinalEC = Get_AllFac_EC(tempSeq, tempAspan);
		if (FinalEC < Get_AllFac_EC(Seq, Aspan) && *max_element(tempAspan.begin(), tempAspan.end()) <= SpanUpperBound)
		{
			Seq = tempSeq;
			Aspan = tempAspan;
		}
		else {
			tempSeq = Seq;
			tempAspan = Aspan;

		}
	}


	if (Get_AllFac_EC(tempSeq, tempAspan) < Get_AllFac_EC(Seq, Aspan))
	{
		Seq = tempSeq;
	}

}


//随机选个工厂，然后交换里面的工件
void Swap_Inner(vector<int> Seq, int& tempspan, int SpanUpperBound)
{
	vector<int>tempSeq;
	vector<int> tempSeq02;
	tempSeq = Seq;
	int tempspan1 = tempspan;

	int FinalEC;
	for (int i = 0; i < tempSeq.size(); i++)
	{
		tempSeq02 = tempSeq;

		for (int j = 0; j < tempSeq02.size(); j++)
		{
			swap(tempSeq02[i], tempSeq02[j]);

			if (Get_OneFac_EC(tempSeq, tempspan1) > Get_OneFac_EC(tempSeq02, tempspan1) && tempspan1 <= SpanUpperBound)
			{
				tempSeq = tempSeq02;
				tempspan = tempspan1;
			}
		}

	}

	if (Get_OneFac_EC(tempSeq, tempspan1) < Get_OneFac_EC(Seq, tempspan))
	{
		Seq = tempSeq;
		tempspan = tempspan1;
	}
}

void NSwap(vector <int>& p, int& tempspan, int SpanUpperBound)
{
	//Find two distinct elements randomly;
	for (int i = 0; i < gJobs * gJobs; ++i)
	{
		vector <int> p1(p.size(), 0);
		p1 = p;
		int tempspan1 = tempspan;

		int  pt1, pt2;
		do
		{
			pt1 = rand() % p.size();
			pt2 = rand() % p.size();
		} while (pt1 == pt2);

		swap(p1[pt1], p1[pt2]);
		if (Get_OneFac_EC(p1, tempspan1) < Get_OneFac_EC(p, tempspan) && tempspan1 <= SpanUpperBound)
		{
			p = p1;
			tempspan = tempspan1;
		}
	}

}


void Destruction_To_someFactory(vector<int>& Seq, vector<int>& DesJobSeq, int PLen)//wy:工厂内破坏
{
	DesJobSeq.clear();
	while (DesJobSeq.size() < PLen)
	{
		if (Seq.size() > 1)
		{
			int pt = rand() % Seq.size();
			DesJobSeq.push_back(Seq[pt]);
			Seq.erase(Seq.begin() + pt);
		}
	}
}

void Construction_To_someFactory(vector<int>& Seq, vector<int>& DesJobSeq, int tempSpan, int SpanUpperBound, int& Beoverstep)//wy:工厂内重构                      
{
	vector<int>tempSeq = Seq;
	for (int i = 0; i < DesJobSeq.size(); i++)
	{
		int Job = DesJobSeq[i];
		int BestPos = -1;
		//Job_To_SomeFactory_Bestposition(Job, Seq, tempSpan, SpanUpperBound);
		FindBestInsertionPositon(BestPos, Job, Seq, tempSpan, SpanUpperBound);
		if (BestPos != -1)
		{
			Seq.insert(Seq.begin() + BestPos, Job);
		}
		else
		{
			Beoverstep = 1;
			break;
		}
	}
}


int CIG(long TimeLimit, int PLen, double Tem, double learnrate, double discountrate, double actionselectrate)
{

	long InitTime = GetElapsedProcessTime();
	int SpanUpperBound = GenerateSpanUpperBound();  //生成makespan上界

	vector<vector<int>>Seq;
	vector<vector<int>>Seq2;
	vector<int> Aspan(gFactory, 0);
	vector<int> Aspan2(gFactory, 0);

	int EC = NEH2_In(Seq, Aspan, SpanUpperBound);
	int EC2 = NEH2_en(Seq2, Aspan2, SpanUpperBound);


	vector<vector<int>>BestSeqSofar;
	vector<int>BestAspan;
	int BestValueSofar;

	if (Get_AllFac_EC(Seq, Aspan) < Get_AllFac_EC(Seq2, Aspan2))
	{
		BestSeqSofar = Seq;
		BestValueSofar = EC;
		BestAspan = Aspan;
	}
	else
	{
		BestSeqSofar = Seq2;
		BestValueSofar = EC2;
		BestAspan = Aspan2;
	}

	int iter = 0;

	while (GetElapsedProcessTime() - InitTime < TimeLimit)
	{
		srand(time(0));
		vector<vector<int>>TempSeq = Seq;
		vector<vector<int>>TempSeq2 = Seq2;
		vector<int>tempAspan1 = Aspan;
		vector<int>tempAspan2 = Aspan2;

		int TempEC = EC;
		int TempEC2 = EC2;

		SwapJobs_random_betweenTwoFac(TempSeq, tempAspan1, SpanUpperBound);
		SwapJobs_random_betweenTwoFac02(TempSeq2, tempAspan2, SpanUpperBound);


		for (int i = 0; i < gFactory; ++i)
		{
			vector<int>DesJobSeq1, tempJobSeq1;
			tempJobSeq1 = TempSeq[i];
			int Beoverstep1 = 0;

			Destruction_To_someFactory(tempJobSeq1, DesJobSeq1, 6);
			Construction_To_someFactory(tempJobSeq1, DesJobSeq1, tempAspan1[i], SpanUpperBound, Beoverstep1);

			if (Get_OneFac_EC(tempJobSeq1, tempAspan1[i]) < Get_OneFac_EC(Seq[i], Aspan[i]) && Beoverstep1 == 0)
			{
				TempSeq[i] = tempJobSeq1;
			}

			vector<int>DesJobSeq2, tempJobSeq2;
			tempJobSeq2 = TempSeq2[i];
			int Beoverstep2 = 0;

			Destruction_To_someFactory(tempJobSeq2, DesJobSeq2, 6);
			Construction_To_someFactory(tempJobSeq2, DesJobSeq2, tempAspan2[i], SpanUpperBound, Beoverstep2);

			if (Get_OneFac_EC(tempJobSeq2, tempAspan2[i]) < Get_OneFac_EC(Seq2[i], Aspan2[i]) && Beoverstep2 == 0)//wy:这里应该是Seq2吧
			{
				TempSeq2[i] = tempJobSeq2;
			}

			int r1 = rand() % 2;

			if (r1 == 0)
			{
				NSwap(TempSeq[i], tempAspan1[i], SpanUpperBound);    //wy：随机交换
			}
			else if (r1 == 1)
			{
				Swap_Inner(TempSeq[i], tempAspan1[i], SpanUpperBound);   //wy:顺序交换
			}

			int r2 = rand() % 2;

			if (r2 == 0)
			{
				NSwap(TempSeq2[i], tempAspan2[i], SpanUpperBound);
			}
			else if (r1 == 1)
			{
				Swap_Inner(TempSeq2[i], tempAspan2[i], SpanUpperBound);
			}


			TempEC = Get_OneFac_EC(TempSeq[i], tempAspan1[i]);
			TempEC2 = Get_OneFac_EC(TempSeq2[i], tempAspan2[i]);

			if (TempEC2 > TempEC)
			{
				TempSeq2 = TempSeq;
			}

			else {
				TempSeq = TempSeq2;
			}

		}


		TempEC = Get_AllFac_EC(TempSeq, tempAspan1);
		TempEC2 = Get_AllFac_EC(TempSeq2, tempAspan2);

		if (TempEC2 > TempEC)
		{
			if (TempEC < EC)
			{
				EC = TempEC;
				Seq = TempSeq;
				Aspan = tempAspan1;

				if (EC < BestValueSofar)
				{
					BestValueSofar = EC;
					BestSeqSofar = Seq;
					BestAspan = Aspan;
				}
			}
		}

		else {
			EC = TempEC2;
			Seq = TempSeq2;
			Aspan = tempAspan2;

			if (EC < BestValueSofar)
			{
				BestValueSofar = EC;
				BestSeqSofar = Seq;
				BestAspan = Aspan;
			}
		}
		iter++;
	}
	CheckChrom1(BestSeqSofar, BestAspan, BestValueSofar);

	//cout << BestValueSofar << "(" << iter << ")" << "   ";
	return BestValueSofar;
}

//int CIG_Curve(long TimeLimit, int LocalValue, long LocalTime, int No)
//{
//
//	long InitTime = GetElapsedProcessTime();
//
//	vector<vector<int>>Seq;
//	vector<vector<int>>Seq2;
//
//	int EC = NEH2_In(Seq);
//	int EC2 = NEH2_In(Seq2);
//
//
//	vector<vector<int>>BestSeqSofar;
//	int BestValueSofar;
//
//	if (GetFspan(Seq) < GetFspan(Seq2))
//	{
//		BestSeqSofar = Seq;
//		BestValueSofar = EC;
//	}
//	else
//	{
//		BestSeqSofar = Seq2;
//		BestValueSofar = EC2;
//	}
//
//	int iter = 1;
//
//	//建立一个txt文件
//	ostringstream str;
//	str << "Test_IG_BLOCK_Curve_Ins=" << No << ".txt";                //
//	ofstream ofile;
//	ofile.open(str.str());
//
//
//	//输出初始解以及初始时间
//	LocalValue = BestValueSofar;                             //
//	LocalTime = GetElapsedProcessTime() - InitTime;         //
//	ofile << LocalTime << "\t" << LocalValue << "\t" << endl;
//
//	while (GetElapsedProcessTime() - InitTime < TimeLimit)
//	{
//		vector<vector<int>>TempSeq = Seq;
//		vector<vector<int>>TempSeq2 = Seq2;
//
//		int TempEC = EC;
//		int TempEC2 = EC2;
//
//		SwapJobs_random_betweenTwoFac(TempSeq);
//		SwapJobs_random_betweenTwoFac02(TempSeq2);
//
//
//		for (int i = 0; i < gFactory; ++i)
//		{
//			vector<int>DesJobSeq1, tempJobSeq1;
//			tempJobSeq1 = TempSeq[i];
//
//			Destruction_To_someFactory(tempJobSeq1, DesJobSeq1, 2);
//			Construction_To_someFactory(tempJobSeq1, DesJobSeq1);
//
//			if (GetFspan(tempJobSeq1) < GetFspan(Seq))
//			{
//				TempSeq[i] = tempJobSeq1;
//			}
//
//			vector<int>DesJobSeq2, tempJobSeq2;
//			tempJobSeq2 = TempSeq2[i];
//
//			Destruction_To_someFactory(tempJobSeq2, DesJobSeq2, 2);
//			Construction_To_someFactory(tempJobSeq2, DesJobSeq2);
//
//			if (GetFspan(tempJobSeq2) < GetFspan(Seq))
//			{
//				TempSeq2[i] = tempJobSeq2;
//			}
//
//			int r1 = rand() % 2;
//
//			if (r1 == 0)
//			{
//				NSwap(TempSeq[i]);
//			}
//			else if (r1 == 1)
//			{
//				Swap_Inner(TempSeq[i]);
//			}
//
//			int r2 = rand() % 2;
//
//			if (r2 == 0)
//			{
//				NSwap(TempSeq2[i]);
//			}
//			else if (r1 == 1)
//			{
//				Swap_Inner(TempSeq2[i]);
//			}
//
//
//			TempEC = GetFspan(TempSeq[i]);
//			TempEC2 = GetFspan(TempSeq2[i]);
//
//			if (TempEC2 > TempEC)
//			{
//				TempSeq2 = TempSeq;
//			}
//
//			else {
//				TempSeq = TempSeq2;
//			}
//
//		}
//
//
//		TempEC = GetFspan(TempSeq);
//		TempEC2 = GetFspan(TempSeq2);
//
//		if (TempEC2 > TempEC)
//		{
//			if (TempEC < EC)
//			{
//				EC = TempEC;
//				Seq = TempSeq;
//
//				if (EC < BestValueSofar)
//				{
//					BestValueSofar = EC;
//					BestSeqSofar = Seq;
//				}
//			}
//		}
//
//		else {
//			EC = TempEC2;
//			Seq = TempSeq2;
//
//			if (EC < BestValueSofar)
//			{
//				BestValueSofar = EC;
//				BestSeqSofar = Seq;
//			}
//		}
//
//		LocalValue = BestValueSofar;                             //
//		LocalTime = GetElapsedProcessTime() - InitTime;         //
//		ofile << LocalTime << "\t" << LocalValue << "\t" << endl;
//	}
//	ofile.close();
//	CheckChrom(BestSeqSofar, BestValueSofar);
//	return BestValueSofar;
//}