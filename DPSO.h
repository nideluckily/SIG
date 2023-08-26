#pragma once

void GetIncreasingOrder(vector<float>Position, vector<int>& Order)   //按升序排列
{
	Order.clear();

	vector<vector<float>>Order_List;
	Order_List.resize(Position.size());
	for (int i = 0; i < Order_List.size(); i++)
		Order_List[i].resize(2);

	for (int i = 0; i < Order_List.size(); i++)
	{
		Order_List[i][0] = i;
		Order_List[i][1] = Position[i];
	}

	sort(Order_List.begin(), Order_List.end(), [](vector<float> t1, vector<float> t2)
		{
			return (t1[1] < t2[1]);
		});

	for (int i = 0; i < Order_List.size(); i++)
		Order.push_back(Order_List[i][0]);
}

int SPV(vector<float>Position, vector<vector<int>>& Seq, vector<int>& Aspan, int SpanUpperBound)
{
	Seq.resize(gFactory);
	for (int f = 0; f < Seq.size(); f++)
		Seq[f].clear();

	vector<int>JobSeq;
	JobSeq.clear();
	for (int i = 0; i < gJobs; i++)
		JobSeq.push_back(i);

	vector<int>Order;
	GetIncreasingOrder(Position, Order);

	for (int f = 0; f < Seq.size(); f++)
		Seq[f].push_back(JobSeq[Order[f]]);

	vector<int>ECSeq;
	ECSeq.resize(Seq.size());

	for (int f = 0; f < ECSeq.size(); f++)
		ECSeq[f] = Get_OneFac_EC(Seq[f], Aspan[f]);

	for (int i = Seq.size(); i < Order.size(); i++)
	{
		int TempEC = INT_MAX;
		int FacwithMinSpan = -1;
		for (int f = 0; f < ECSeq.size(); f++)
		{
			if (ECSeq[f] < TempEC)
			{
				TempEC = ECSeq[f];
				FacwithMinSpan = f;
			}
		}
		int bestpos = -1;
		//ECSeq[FacwithMinSpan] = Job_To_SomeFactory_Bestposition_DPSO(JobSeq[Order[i]], Seq[FacwithMinSpan],Aspan[FacwithMinSpan], SpanUpperBound);

		ECSeq[FacwithMinSpan] = FindBestInsertionPositon(bestpos, JobSeq[Order[i]], Seq[FacwithMinSpan], Aspan[FacwithMinSpan], SpanUpperBound);
		if (bestpos != -1)
		{
			Seq[FacwithMinSpan].insert(Seq[FacwithMinSpan].begin() + bestpos, JobSeq[Order[i]]);
		}
	}
	int EC = accumulate(ECSeq.begin(), ECSeq.end(), 0);
	return EC;
}

void UpdateVelocity(float c1, float c2, vector<float>& Velocity, vector<float>Position, vector<float>PersonalBestPos, vector<float>GlobalBestPos)
{
	default_random_engine e(rand());
	uniform_real_distribution<float>u(0, 1.0);
	float randnum1 = u(e);
	float randnum2 = u(e);

	for (int i = 0; i < Velocity.size(); i++)
		Velocity[i] = Velocity[i] + c1 * randnum1 * (PersonalBestPos[i] - Position[i]) + c2 * randnum2 * (GlobalBestPos[i] - Position[i]);
}

void UpdatePosition(vector<float>& Position, vector<float>Velocity)
{
	for (int i = 0; i < Position.size(); i++)
		Position[i] = Position[i] + Velocity[i];
}

void Solution_Improvement(long TimeLimit, long InitialTime, int Iter, int& Span, vector<vector<int>>& Seq, vector<float>& Position, 
	vector<float>& Velocity, vector<int>& Aspan, int SpanUpperBound)
{
	vector<vector<int>>BestSeq = Seq;
	int BestSpan = Span;
	vector<float>BestPosition = Position;
	vector<float>BestVelocity = Velocity;

	int Counter = 0;
	while (Counter < Iter)
	{
		if (GetElapsedProcessTime() - InitialTime > TimeLimit)
		{
			Counter = Iter;
			break;
		}
		//adjacent pairwise interchange scheme
		int RandFac = rand() % Seq.size();
		int StartPoint = rand() % Seq[RandFac].size();
		if (StartPoint == Seq[RandFac].size() - 1)
		{
			int FrontJob = Seq[RandFac][StartPoint - 1];
			int AfterJob = Seq[RandFac][StartPoint];

			Seq[RandFac][StartPoint - 1] = AfterJob;
			Seq[RandFac][StartPoint] = FrontJob;

			float TempPos = Position[AfterJob];
			Position[AfterJob] = Position[FrontJob];
			Position[FrontJob] = TempPos;

			float TempVelocity = Velocity[AfterJob];
			Velocity[AfterJob] = Velocity[FrontJob];
			Velocity[FrontJob] = TempVelocity;
		}
		else
		{
			int FrontJob = Seq[RandFac][StartPoint];
			int AfterJob = Seq[RandFac][StartPoint + 1];

			Seq[RandFac][StartPoint] = AfterJob;
			Seq[RandFac][StartPoint + 1] = FrontJob;

			float TempPos = Position[AfterJob];
			Position[AfterJob] = Position[FrontJob];
			Position[FrontJob] = TempPos;

			float TempVelocity = Velocity[AfterJob];
			Velocity[AfterJob] = Velocity[FrontJob];
			Velocity[FrontJob] = TempVelocity;
		}

		//find two distinct elements randomly;
/*		int Job1, Job2, JobPt1, JobPt2, FacPt1, FacPt2;
		do {
			FacPt1 = rand() % Seq.size();
			FacPt2 = rand() % Seq.size();
			JobPt1 = rand() % Seq[FacPt1].size();
			JobPt2 = rand() % Seq[FacPt2].size();
			Job1 = Seq[FacPt1][JobPt1];
			Job2 = Seq[FacPt2][JobPt2];
		} while (Job1 == Job2);

		//swap
		Seq[FacPt1][JobPt1] = Job2;
		Seq[FacPt2][JobPt2] = Job1;

		float TempPos = Position[Job1];
		Position[Job1] = Position[Job2];
		Position[Job2] = TempPos;

		float TempVelocity = Velocity[Job1];
		Velocity[Job1] = Velocity[Job2];
		Velocity[Job2] = TempVelocity;*/

		Span = Get_AllFac_EC(Seq, Aspan);
		if (Span < BestSpan && *max_element(Aspan.begin(), Aspan.end())<= SpanUpperBound)
		{
			BestSpan = Span;
			BestSeq = Seq;
			BestPosition = Position;
			BestVelocity = Velocity;
			Counter = 0;
		}
		else
		{
			Span = BestSpan;
			Seq = BestSeq;
			Position = BestPosition;
			Velocity = BestVelocity;
			Counter++;
		}
	}
	Span = BestSpan;
	Seq = BestSeq;
	Position = BestPosition;
	Velocity = BestVelocity;
}

int DPSO(long TimeLimit, int Psize, int Iter, float c1, float c2)
{

	long InitTime = GetElapsedProcessTime();
	int SpanUpperBound = GenerateSpanUpperBound();  //生成makespan上界

	//初始化
	vector<vector<vector<int>>>Population;
	Population.resize(Psize);

	vector<int>PopEC;
	PopEC.resize(Psize, 0);

	vector<vector<int>>AllSpan(Psize, vector<int>(gFactory, 0));

	PopEC[0] = DSPT(Population[0], AllSpan[0], SpanUpperBound);
	PopEC[1] = DNEH(Population[1], AllSpan[1], SpanUpperBound);

	vector<vector<float>>PopPosition;
	PopPosition.resize(Psize);
	for (int i = 0; i < PopPosition.size(); i++)
	{
		for (int j = 0; j < gJobs; j++)
		{
			default_random_engine e(rand());
			uniform_real_distribution<float>u(0, 1.0);
			float randnum = u(e);
			PopPosition[i].push_back(randnum);
		}
	}

	for (int i = 2; i < Psize; i++)
		PopEC[i] = SPV(PopPosition[i], Population[i], AllSpan[i], SpanUpperBound);

	vector<vector<float>>PopVelocity;
	PopVelocity.resize(Psize);
	for (int i = 0; i < PopVelocity.size(); i++)
	{
		for (int j = 0; j < gJobs; j++)
		{
			default_random_engine e(rand());
			uniform_real_distribution<float>u(0, 1.0);
			float randnum = u(e);
			PopVelocity[i].push_back(randnum);
		}
	}

	vector<vector<vector<int>>>PersonalBestPop = Population;
	vector<int>PersonalBestPopEC = PopEC;
	vector<vector<float>>PersonalBestPopPosition = PopPosition;
	vector<vector<float>>PersonalBestPopVelocity = PopVelocity;

	while (GetElapsedProcessTime() - InitTime < TimeLimit)
	{
		int BestIndiPt = -1;
		int SpanofBestIndi = INT_MAX;
		for (int i = 0; i < PopEC.size(); i++)
		{
			if (PopEC[i] < SpanofBestIndi)
			{
				SpanofBestIndi = PopEC[i];
				BestIndiPt = i;
			}
		}
		vector<vector<int>>GlobalBestIndi = Population[BestIndiPt];
		vector<float>GlobalBestPosition = PopPosition[BestIndiPt];
		vector<float>GlobalBestVelocity = PopVelocity[BestIndiPt];

		for (int i = 0; i < Population.size(); i++)
		{
			if (GetElapsedProcessTime() - InitTime > TimeLimit)
				break;

			UpdateVelocity(c1, c2, PopVelocity[i], PopPosition[i], PersonalBestPopPosition[i], GlobalBestPosition);
			UpdatePosition(PopPosition[i], PopVelocity[i]);
			PopEC[i] = SPV(PopPosition[i], Population[i], AllSpan[i], SpanUpperBound);
			Solution_Improvement(TimeLimit, InitTime, Iter, PopEC[i], Population[i], PopPosition[i], PopVelocity[i], AllSpan[i], SpanUpperBound);

			if (PopEC[i] < PersonalBestPopEC[i])
			{
				PersonalBestPopEC[i] = PopEC[i];
				PersonalBestPop[i] = Population[i];
				PersonalBestPopPosition[i] = PopPosition[i];
				PersonalBestPopVelocity[i] = PopVelocity[i];
			}
		}
	}
	//for (int i = 0; i < PersonalBestPop.size(); i++)
	//	CheckChrom(PersonalBestPop[i], PersonalBestPopEC[i]);
	int total = INT_MAX;
	for (int i = 0; i < PersonalBestPop.size(); i++)
	{
		if (Get_AllFac_EC(PersonalBestPop[i], AllSpan[i]) < total)
		{
			total = Get_AllFac_EC(PersonalBestPop[i], AllSpan[i]);
		}
	}

	//cout << total << "   ";
	return total;/**min_element(PersonalBestPopEC.begin(), PersonalBestPopEC.end());*/
}


//int DPSO_Curve(long TimeLimit, int Psize, int Iter, float c1, float c2, int LocalValue, long LocalTime, int No)
//{
//	long InitTime = GetElapsedProcessTime();
//
//	//初始化
//	vector<vector<vector<int>>>Population;
//	Population.resize(Psize);
//
//	vector<int>PopEC;
//	PopEC.resize(Psize, 0);
//
//	PopEC[0] = DSPT(Population[0]);
//	PopEC[1] = DNEH(Population[1]);
//
//	vector<vector<float>>PopPosition;
//	PopPosition.resize(Psize);
//	for (int i = 0; i < PopPosition.size(); i++)
//	{
//		for (int j = 0; j < gJobs; j++)
//		{
//			default_random_engine e(rand());
//			uniform_real_distribution<float>u(0, 1.0);
//			float randnum = u(e);
//			PopPosition[i].push_back(randnum);
//		}
//	}
//
//	for (int i = 2; i < Psize; i++)
//		PopEC[i] = SPV(PopPosition[i], Population[i]);
//
//	vector<vector<float>>PopVelocity;
//	PopVelocity.resize(Psize);
//	for (int i = 0; i < PopVelocity.size(); i++)
//	{
//		for (int j = 0; j < gJobs; j++)
//		{
//			default_random_engine e(rand());
//			uniform_real_distribution<float>u(0, 1.0);
//			float randnum = u(e);
//			PopVelocity[i].push_back(randnum);
//		}
//	}
//
//	vector<vector<vector<int>>>PersonalBestPop = Population;
//	vector<int>PersonalBestPopEC = PopEC;
//	vector<vector<float>>PersonalBestPopPosition = PopPosition;
//	vector<vector<float>>PersonalBestPopVelocity = PopVelocity;
//
//	//建立一个txt文件
//	ostringstream str;
//	str << "Test_PSO_Curve_Ins=" << No << ".txt";                //
//	ofstream ofile;
//	ofile.open(str.str());
//
//	while (GetElapsedProcessTime() - InitTime < TimeLimit)
//	{
//		int BestIndiPt = -1;
//		int SpanofBestIndi = INT_MAX;
//		for (int i = 0; i < PopEC.size(); i++)
//		{
//			if (PopEC[i] < SpanofBestIndi)
//			{
//				SpanofBestIndi = PopEC[i];
//				BestIndiPt = i;
//			}
//		}
//		vector<vector<int>>GlobalBestIndi = Population[BestIndiPt];
//		vector<float>GlobalBestPosition = PopPosition[BestIndiPt];
//		vector<float>GlobalBestVelocity = PopVelocity[BestIndiPt];
//
//		for (int i = 0; i < Population.size(); i++)
//		{
//			if (GetElapsedProcessTime() - InitTime > TimeLimit)
//				break;
//
//			UpdateVelocity(c1, c2, PopVelocity[i], PopPosition[i], PersonalBestPopPosition[i], GlobalBestPosition);
//			UpdatePosition(PopPosition[i], PopVelocity[i]);
//			PopEC[i] = SPV(PopPosition[i], Population[i]);
//			Solution_Improvement(TimeLimit, InitTime, Iter, PopEC[i], Population[i], PopPosition[i], PopVelocity[i]);
//
//			if (PopEC[i] < PersonalBestPopEC[i])
//			{
//				PersonalBestPopEC[i] = PopEC[i];
//				PersonalBestPop[i] = Population[i];
//				PersonalBestPopPosition[i] = PopPosition[i];
//				PersonalBestPopVelocity[i] = PopVelocity[i];
//			}
//		}
//
//		int total = INT_MAX;
//		for (int i = 0; i < PersonalBestPop.size(); i++)
//		{
//			if (GetFspan(PersonalBestPop[i]) < total)
//			{
//				total = GetFspan(PersonalBestPop[i]);
//			}
//		}
//		LocalValue = total;                             //
//		LocalTime = GetElapsedProcessTime() - InitTime;         //
//		ofile << LocalTime << "\t" << LocalValue << "\t" << endl;
//	}
//	ofile.close();
//	//for (int i = 0; i < PersonalBestPop.size(); i++)
//	//	CheckChrom(PersonalBestPop[i], PersonalBestPopEC[i]);
//	int total = INT_MAX;
//	for (int i = 0; i < PersonalBestPop.size(); i++)
//	{
//		if (GetFspan(PersonalBestPop[i]) < total)
//		{
//			total = GetFspan(PersonalBestPop[i]);
//		}
//	}
//	return total;/**min_element(PersonalBestPopEC.begin(), PersonalBestPopEC.end());*/
//}


