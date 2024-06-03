#include<iostream>
#include<cstdlib>
#include<fstream>
#include<iomanip>
#include<cmath>

using namespace std;

int main()
{
	ifstream Data_file;
	Data_file.open("ruspini.txt");

	int i, j;
	double data[75][2] = { 0 };
	for (i = 0; i < 75; i++)			//데이터 불러오기
		for (j = 0; j < 2; j++)
			Data_file >> data[i][j];

	// cout << "20240603_02";
	/*
	cout << "데이터 값" << endl;
	for (i = 0; i < 75; i++)
	{
		for (j = 0; j < 2; j++)
			cout << data[i][j]<<setw(5);
		cout << endl;
	}
	cout << endl;
	*/
	
	int cluster[75][4] = { 0 };
	/*
	for (i = 0; i < 75; i++)
		cluster[i][rand() % 4] == 1;

	cout << "초기해 클러스터 결과" << endl;
	for (i = 0; i < 75; i++)
	{
		for (j = 0; j < 4; j++)
			cout << cluster[i][j];
		cout << endl;
	}
	cout << endl;
	*/

	// --------------------------------------룰렛휠-----------------------------------------

	double DATA_distance[75][75] = { 0 };				// 각 데이터 간의 거리 저장
	double SUM_DATA_distance[75][1] = { 0 };
	for (i = 0; i < 75; i++)
	{
		for (j = 0; j < 75; j++)
		{
			DATA_distance[i][j] = sqrt((data[i][0] - data[j][0]) * (data[i][0] - data[j][0]) + (data[i][1] - data[j][1]) * (data[i][1] - data[j][1]));
			SUM_DATA_distance[i][0] += DATA_distance[i][j];
		}
	}

	double SUM_of_DATA_distance_percentage[1][75] = { 0 };		// 거리 비율의 합 계산
	for (j = 0; j < 75; j++)
	{
		for (i = 0; i < 75; i++)
			SUM_of_DATA_distance_percentage[0][j] += (DATA_distance[i][j] / SUM_DATA_distance[i][0]);
	}

	double TRANSPOSE_SUM_of_DATA[1][75] = { 0 };				// 거리 비율의 합의 역수값 계산
	double Check_TRANSPOSE_DATA = 0;							// 비율의 합이 1인지 확인
	for (j = 0; j < 75; j++)
	{
		TRANSPOSE_SUM_of_DATA[0][j] = (1 / SUM_of_DATA_distance_percentage[0][j]);
		Check_TRANSPOSE_DATA += TRANSPOSE_SUM_of_DATA[0][j];
	}

	double range_of_data = 0;									// 선택확률 범위 계산
	double Range_of_Percentage[1][75] = { 0 };					// 선택확률 누적범위 계산
	for (j = 0; j < 75; j++)
	{
		range_of_data += (TRANSPOSE_SUM_of_DATA[0][j] / Check_TRANSPOSE_DATA);
		Range_of_Percentage[0][j] = range_of_data;
	}

	double Cluster[75][4] = { 0 };

	for (i = 0; i < 75; i++)
	{
		for (j = 0; j < 4; j++)
		{
			Cluster[i][rand()%4] = 1;
		}
	}


		
	int n = 0;
	int Best_Cluster[75][4] = { 0 };
	double Best_Evaluation_value = 10000000;
	while (1)
	{
		n++;
		cout << "-----------" << n << "번째" << "--------------" << endl<<endl;

	
		double Parent_1[4] = { 0 };
		for (i = 0; i < 4; i++)
		{
			Parent_1[i] = rand() / (double)RAND_MAX;
		}

		double Parent_1_Center[4][2] = { 0 };
		for (j = 0; j < 4; j++)
		{
			for (i = 0; i < 75; i++)
			{
				if (i > 0)
				{
					if (Parent_1[i] <= Range_of_Percentage[0][j] && Parent_1[i] > Range_of_Percentage[0][j - 1])
					{
						Parent_1_Center[j][0] = data[i][0];
						Parent_1_Center[j][1] = data[i][1];
					}
				}
				else if (i == 0)
				{
					if (Parent_1[i] <= Range_of_Percentage[0][j])
					{
						Parent_1_Center[j][0] = data[i][0];
						Parent_1_Center[j][1] = data[i][1];
					}
				}
			}
		}

		//------------------------Selection 시작---------------------------------

		double data_distanse_1[75][4] = { 0 };

		for (i = 0; i < 75; i++)
		{
			for (j = 0; j < 4; j++)
				data_distanse_1[i][j] = sqrt((Parent_1_Center[j][0] - data[i][0]) * (Parent_1_Center[j][0] - data[i][0]) + (Parent_1_Center[j][1] - data[i][1]) * (Parent_1_Center[j][1] - data[i][1]));
		}

		double distanse_point_1[75] = { 0 };
		for (i = 0; i < 75; i++)
			distanse_point_1[i] = 10000000;
		
		for (i = 0; i < 75; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (distanse_point_1[i] > data_distanse_1[i][j])
					distanse_point_1[i] = data_distanse_1[i][j];
			}
		}

		int Parent_1_Cluster[75][4] = { 0 }, Parent_2_Cluster[75][4] = { 0 };
		for (i = 0; i < 75; i++)
		{
			for (j = 0; j < 4; j++)
			{
				if (distanse_point_1[i] == data_distanse_1[i][j])
					Parent_1_Cluster[i][j] = 1;
				Parent_2_Cluster[i][j] = Cluster[i][j];
			}
		}




		//--------------------------crossover 시작-------------------------------------

		int Child_Cluster[75][4] = { 0 };
		double Data_percentage[75] = { 0 };
		double Mu_rate = 0.1;


		for (i = 0; i < 75; i++)
			Data_percentage[i] = rand() / (double)RAND_MAX;

		/*
		for (i = 0; i < 75; i++)
		{
			if (distanse_point_1[i] <= distanse_point_2[i])
			{
				if (Data_percentage[i] <= Mu_rate)
					Child_Cluster[i][j] = Parent_2_Cluster[i][j];
				else
					Child_Cluster[i][j] = Parent_1_Cluster[i][j];
			}
			else
			{
				if (Data_percentage[i] <= Mu_rate)
					Child_Cluster[i][j] = Parent_1_Cluster[i][j];
				else
					Child_Cluster[i][j] = Parent_2_Cluster[i][j];
			}
		}
		*/

		int Parent_1_count, Parent_2_count;
		double Parent_1_sum_x, Parent_1_sum_y, Parent_1_avg[4][2] = { 0 }, Parent_2_sum_x, Parent_2_sum_y, Parent_2_avg[4][2] = { 0 };
		for (j = 0; j < 4; j++)					// 각 클러스터의 평균 계산
		{
			Parent_1_count = 0;
			Parent_2_count = 0;
			Parent_1_sum_x = 0;
			Parent_1_sum_y = 0;
			Parent_2_sum_x = 0;
			Parent_2_sum_y = 0;
			for (i = 0; i < 75; i++)
			{
				if (Parent_1_Cluster[i][j] == 1)
				{
					Parent_1_sum_x += data[i][0];
					Parent_1_sum_y += data[i][1];
					Parent_1_count++;
				}if (Parent_2_Cluster[i][j] == 1)
				{
					Parent_2_sum_x += data[i][0];
					Parent_2_sum_y += data[i][1];
					Parent_2_count++;
				}
			}
			Parent_1_avg[j][0] = Parent_1_sum_x / double(Parent_1_count);
			Parent_1_avg[j][1] = Parent_1_sum_y / double(Parent_1_count);
			Parent_2_avg[j][0] = Parent_2_sum_x / double(Parent_2_count);
			Parent_2_avg[j][1] = Parent_2_sum_y / double(Parent_2_count);
		}

		double Parent_1_distance[75] = { 0 }, Parent_2_distance[75] = { 0 };
		for (j = 0; j < 4; j++)			//데이터와 평균까지의 거리 계산 및 평가값 계산
		{
			for (i = 0; i < 75; i++)
			{
				if (Parent_1_Cluster[i][j] == 1)
					Parent_1_distance[i] = sqrt((Parent_1_avg[j][0] - data[i][0]) * (Parent_1_avg[j][0] - data[i][0]) + (Parent_1_avg[j][1] - data[i][1]) * (Parent_1_avg[j][1] - data[i][1]));
				if (Parent_2_Cluster[i][j] == 1)
					Parent_2_distance[i] = sqrt((Parent_2_avg[j][0] - data[i][0]) * (Parent_2_avg[j][0] - data[i][0]) + (Parent_2_avg[j][1] - data[i][1]) * (Parent_2_avg[j][1] - data[i][1]));
			}
		}


		// Case 2 of Mutation

		int Crossover_point = 0;
		Crossover_point = rand() % 75;

		for (i = 0; i < 75; i++)
		{
			if (Parent_1_distance[i] <= Parent_2_distance[i])
			{
				for (j = 0; j < 4; j++)
				{
					Child_Cluster[i][j] = Parent_1_Cluster[i][j];
				}
			}
			else
			{
				for (j = 0; j < 4; j++)
				{
					Child_Cluster[i][j] = Parent_2_Cluster[i][j];
				}
			}

		}

		int Mu_number=0;
		for (i = 0; i < 75; i++)
		{
			if (Data_percentage[i] <= Mu_rate)
			{
				for (j = 0; j < 4; j++)
				{
					Child_Cluster[i][j] = 0;
				}
				Mu_number = rand() % 4;
				/*
				while (1)
				{
					if (Parent_1_Cluster[i][Mu_number] == 1 || Parent_2_Cluster[i][Mu_number] == 1)
						Mu_number = rand() % 4;
					else
						break;
				}
				*/
				Child_Cluster[i][Mu_number] = 1;
			}
		}
		


		int count;
		double sum_x, sum_y, avg[4][2] = { 0 };
		for (j = 0; j < 4; j++)					// 각 클러스터의 평균 계산
		{
			count = 0;
			sum_x = 0;
			sum_y = 0;
			for (i = 0; i < 75; i++)
			{
				if (Child_Cluster[i][j] == 1)
				{
					sum_x += data[i][0];
					sum_y += data[i][1];
					count++;
				}
			}
			avg[j][0] = sum_x / double(count);
			avg[j][1] = sum_y / double(count);
		}

		
		cout << "클러스터 평균 결과" << endl<<"x의 평균"<<setw(10)<<"y의 평균"<<endl;
		for (j = 0; j < 2; j++)
		{
			for (i = 0; i < 4; i++)
			{
				cout << setw(10)<<avg[i][j];
			}
			cout << endl;
		}
		cout << endl;

		double Child_Evaluation_value = 0;
		double distance[75][4] = { 0 };
		for (j = 0; j < 4; j++)			//데이터와 평균까지의 거리 계산 및 평가값 계산
		{
			for (i = 0; i < 75; i++)
			{
				distance[i][j] = sqrt((avg[j][0] - data[i][0]) * (avg[j][0] - data[i][0]) + (avg[j][1] - data[i][1]) * (avg[j][1] - data[i][1]));
				if (Child_Cluster[i][j] == 1)
					Child_Evaluation_value += distance[i][j];
			}
		}

		cout << n << "번째 해의 평가값 : " << Child_Evaluation_value << endl<<endl;

		
		if (Child_Evaluation_value < Best_Evaluation_value)
		{
			Best_Evaluation_value = Child_Evaluation_value;
			for (i = 0; i < 75; i++)
			{
				for (j = 0; j < 4; j++)
					Best_Cluster[i][j] = Child_Cluster[i][j];
			}
		}

		if (n == 200000)
			break;
		else
			for (i = 0; i < 75; i++)
			{
				for (j = 0; j < 4; j++)
					Cluster[i][j] = Child_Cluster[i][j];
			}

	}

	cout << "최종 평가값 : " << endl << Best_Evaluation_value << endl;

	return 0;
}
