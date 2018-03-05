#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>		//random
#include <cstdlib>
#include <bitset>
#include <ctime>
using namespace std;

int pop_size = 6;

unordered_map<string, vector<string>> umap, e_students;
unordered_map<string, int> exam_cap;
unordered_map<string, int> exam_map;
unordered_map<string, vector<int>> ex_pMap;
unordered_map<string, int> ex_rMap;
int max_percap = 200;
const int num_periods = 30;
const int num_rooms = 1;
const int num_exams = 289;
vector<string> arr[num_rooms][num_periods + 1][12];
int room_arr[num_rooms + 1] = {0};



int myrandom (int i) { return std::rand()%i;}


void print_tt(int timetable){

	int num_exam = 0;
	int unsched = 0;

	cout <<num_exams<<" "<<num_periods<<"*********"<<endl;


	for(int i = 0; i < num_periods; i++){
		cout<<"--Period :"<<i<<"--\n";
		int sump = 0;
		for(int j = 0; j < num_rooms; j++){
			int sum = 0;
			cout<<"Room "<<j<<"( "<<room_arr[j]<<" ) :";
			for(vector<string>::iterator it = arr[j][i][timetable].begin(); it != arr[j][i][timetable].end(); it++){
				cout<<*it;
				unordered_map<string, int>::const_iterator cons_it = exam_cap.find(*it);
				cout<<"( "<<cons_it->second <<" )*";				
				sum += cons_it->second;
				num_exam++;
			}
			cout<<sum; 		
			if(sum > room_arr[j])
				cout<<"ERRORr";	
			sump += sum;
			cout<<endl;
		}
		if(sump > max_percap)
			cout<<"Errorp";
		cout<<endl;
	}

	cout<<"NUM_EXAM : "<<num_exam<<endl;
	cout<<"UNSCHED : "<<arr[0][num_periods][timetable].size()<<endl;
	int c = 0;
	for(vector<string>::iterator it = arr[0][num_periods][timetable].begin(); it != arr[0][num_periods][timetable].end(); it++){
		cout<<*it<<" ";
		unordered_map<string, int>::const_iterator cons_it = exam_cap.find(*it);
		c += cons_it->second;
	}
	//cout<<"NUM CONFLICTS : "<<c;*/
	cout<<endl;
}






bool intersect(string e1, string e2){

	unordered_map<string, vector<string>>::const_iterator it1 = e_students.find(e1);
	unordered_map<string, vector<string>>::const_iterator it2 = e_students.find(e2);
	vector<string> st1 = it1->second;
	vector<string> st2 = it2->second;

	sort(st1.begin(), st1.end());
	sort(st2.begin(), st2.end());
	bool first = false;
	int length;
	vector<string> *lower, *higher;

	if(st1.size() < st2.size()){
		length = st1.size();
		lower = &st2;
		higher = &st1;
	}
	else{
		length = st2.size();
		lower = &st1;
		higher = &st2;
	}
	for (int i = 0; i < length; ++i)
	{
		bool found = binary_search(lower->begin(), lower->end(), higher->at(i));
		if(found)
			return true;
	}
	return false;
}





int tot_penalty(int tt){

	

	int p = 0;
	for(int i = 0; i < num_periods-1; i++ ){

		int checker = ((i % 16) % 3);

		if(checker != 2 && i % 16 != 15){
			vector<string> pen;
			for(int j = 0; j < num_rooms; j++){
				
				for(vector<string>::iterator it = arr[j][i][tt].begin(); it != arr[j][i][tt].end(); it++){
					string e1 = *it;
					for(int j1 = 0 ; j1 < num_rooms; j1++){
						for(vector<string>::iterator it2 = arr[j1][i+1][tt].begin(); it2 != arr[j1][i+1][tt].end(); it2++){
		
							string e2 = *it2;
							
							unordered_map<string, vector<string>>::const_iterator it1 = e_students.find(e1);
							unordered_map<string, vector<string>>::const_iterator it3 = e_students.find(e2);
							vector<string> st1 = it1->second;
							vector<string> st2 = it3->second;							
							
							sort(st1.begin(), st1.end());
							sort(st2.begin(), st2.end());
							
							set_intersection(st1.begin(), st1.end(), st2.begin(), st2.end(), back_inserter(pen));
							
						}
					}
				}
			}
			p += pen.size();
		}
		
	}

	
	return p;
}



int Siz(int tt, int p){
	int size = 0;
	for(int i = 0; i < num_rooms; i++){
		size += arr[i][p][tt].size();
	}
	return size;
}



int penalty(string event, int tt, int period){

	vector<string> p;
	bool previous = false, next = false;

	int checker = ((period % 16) % 3);

	if(checker != 0)
		previous = true;

	if(checker != 2)
		next = true;

	if(period%16 == 15){
		previous = false;
		next = false;
	}

	if(period + 1 == num_periods)
		next = false;


	if(previous){			//checking previous
		
		for(int i = 0; i < num_rooms; i++){
			for(vector<string>::iterator it = arr[i][period-1][tt].begin(); it != arr[i][period-1][tt].end(); it++){

				string e = *it;
				if(e == event)
					continue;

				unordered_map<string, vector<string>>::const_iterator it1 = e_students.find(e);
				unordered_map<string, vector<string>>::const_iterator it2 = e_students.find(event);
				vector<string> st1 = it1->second;
				vector<string> st2 = it2->second;

				sort(st1.begin(), st1.end());
				sort(st2.begin(), st2.end());
				set_intersection(st1.begin(), st1.end(), st2.begin(), st2.end(), back_inserter(p));
			}
		}
	}
	int n = num_periods;



	if(next){		//checking next

		
		for(int i = 0; i < num_rooms; i++){
			
			for(vector<string>::iterator it = arr[i][period+1][tt].begin(); it != arr[i][period+1][tt].end(); it++){
				
				string e = *it;				
				
				if(e == event)
					continue;

				unordered_map<string, vector<string>>::const_iterator it1 = e_students.find(e);
				unordered_map<string, vector<string>>::const_iterator it2 = e_students.find(event);
				vector<string> st1 = it1->second;
				vector<string> st2 = it2->second;

				sort(st1.begin(), st1.end());
				sort(st2.begin(), st2.end());
				set_intersection(st1.begin(), st1.end(), st2.begin(), st2.end(), back_inserter(p));
			}
		}
	}
	return p.size();
}



int legal_period(string event, int tt, int period){
	
	bool legal = false;

	int room = -1;										//room -> room in which exam is placed 
/*
	unordered_map<string, int>::const_iterator it1 = exam_map.find(event);
	int time = it1->second;
	
	int periods_time;
	if(period < num_rooms && period % 3 == 0)
		periods_time = 3;
	else if(period > 15 && period % 3 == 1)
		periods_time = 3;
	else periods_time = 2;
	if(time > periods_time)
		return -1;					//i.e. cant be placed in that period 
	
*/

	//num_of sitting <= rooms available
	for(int i = 0; i < num_rooms; i++){
		int sum_of_rcap = 0;
		for(vector<string>::iterator it = arr[i][period][tt].begin(); it != arr[i][period][tt].end(); ++it){
			string exam = *it;
			
			if(intersect(exam, event))
				return -1;
			
			unordered_map<string, int>::const_iterator cons_it = exam_cap.find(exam);
			sum_of_rcap += cons_it->second;
			
		}
		unordered_map<string, int>::const_iterator cons_it = exam_cap.find(event);
		sum_of_rcap += cons_it->second;
		
		if(sum_of_rcap <= room_arr[i]){
			room = i;
			return room;
		}
		//else return false;
	}

	return room;
	//arr[room][period][tt].push_back(event)
}

int legal_room(string event, int tt, int room){

	int period = -1;										//room -> room in which exam is placed 

/*	unordered_map<string, int>::const_iterator it1 = exam_map.find(event);
	int time = it1->second;
*/
	int room_cap = room_arr[room];
	

	//num_of sitting <= rooms available
	for(int i = 0; i < num_periods; i++){
/*
		int periods_time;
		if(period < num_rooms && period % 3 == 0)
			periods_time = 3;
		else if(period > 15 && period % 3 == 1)
			periods_time = 3;
		else periods_time = 2;
		if(time > periods_time)
			continue;			
*/
		
		int sum_of_rcap = 0;
		bool breakp = false;
		for(vector<string>::iterator it = arr[room][i][tt].begin(); it != arr[room][i][tt].end() && breakp == false; ++it){
			string exam = *it;
			
			if(intersect(exam, event))
				breakp = true;
			
			unordered_map<string, int>::const_iterator cons_it = exam_cap.find(exam);
			sum_of_rcap += cons_it->second;
			
		}

		if(breakp)
			continue;

		unordered_map<string, int>::const_iterator cons_it = exam_cap.find(event);
		sum_of_rcap += cons_it->second;
		
		if(sum_of_rcap <= room_cap){
			period = i;
			return period;
		}	
	}
	return period;
}




int main(int argc, char *argv[]){

	std::srand ( unsigned ( std::time(0) ) );

	cout<<"start"<<endl;

	//-----------------------------------------------------------------------
	//READING DATA FROM FILE
	//	unordered_map<string, vector<string>> umap, e_students;
	//	unordered_map<string, int> exam_cap;

	int start_s=clock();


	ifstream file_enrol, file_exam, file_roomCap, ex_period, ex_room;			//constraints -> ex_period, ex_room
	file_enrol.open(argv[1]);

	string student_code, exam_code;

	while(!file_enrol.eof()){ 

		file_enrol >> student_code >> exam_code;								//1st input enrolements
		umap[student_code].push_back(exam_code);
		e_students[exam_code].push_back(student_code);
		exam_cap[exam_code]++;
	}
	cout<<endl;

	file_enrol.close();


	//	unordered_map<string, int> exam_map;

	file_exam.open(argv[2]);
	
	string ex_code;//, ex_time;
	int ex_time;
	string ex_arr[num_exams];
	int counter = 0; 
	
	while(!file_exam.eof() && counter < num_exams){
		file_exam >> ex_code >> ex_time;							//2nd input exams
		exam_map[ex_code] = ex_time;
		ex_arr[counter] = ex_code;
		counter++;
	}
	cout<<endl;
	file_exam.close();


	room_arr[0] = max_percap;

/*
	ex_period.open(argv[4]);
	unordered_map<string, vector<int>> ex_pMap;
	string ex_p_code;
	int ex_p_per;			//ex_p_code -> in case(exam_period file) -> code of exam
	while(!ex_period.eof()){
		ex_period >> ex_p_code >> ex_p_per;
		ex_pMap[ex_p_code].push_back(ex_p_per);
	}
	cout<<endl;
	ex_period.close();



	ex_room.open(argv[5]);
	unordered_map<string, int> ex_rMap;
	string ex_r_code;
	int ex_r_room;
	while(!ex_room.eof()){
		ex_room >> ex_r_code >> ex_r_room;
		ex_rMap[ex_r_code] = ex_r_room;
	}
	cout<<endl;
	ex_room.close();
*/

	//--------------------------------------------------------------------------
	//INITIAL POPULATION


	for (int i = 0; i < pop_size; ++i){

		cout<<"------inititalizing "<< i << endl;

		for (int q = 0; q < num_exams; ++q){				//all exams put in unscheduled category
			arr[0][num_periods][i].push_back(ex_arr[q]);	
		}

		cout << "SIZE:"<< arr[0][num_periods][i].size();


		random_shuffle ( arr[0][num_periods][i].begin(), arr[0][num_periods][i].end(), myrandom);
		
		arr[0][num_periods][i + pop_size] = arr[0][num_periods][i];

		int cons1 = 0, cons2 = 0;

		for ( auto local_it = ex_pMap.begin(); local_it!= ex_pMap.end(); ++local_it ){
      		vector<int> p = local_it -> second;
      		string e = local_it -> first;
      		bool found = false;
	      	for(vector<int>::iterator it = p.begin(); it != p.end() && found == false; it++){	

	      		int cur_period = *it;
	      		unordered_map<string, int>::const_iterator it1 = ex_rMap.find(e);
	      		if(it1 != ex_rMap.end()){
	      			//if(i == 0)cout<<e<<" found in room & per\n";
					int r = it1 -> second;
					arr[r][cur_period][i].push_back(e);
      				arr[r][cur_period][i + pop_size].push_back(e);
      				found = true;
      				cons1++;
      				if(i == 0)
      					cout << local_it -> first << "scheduled in p: "<< cur_period <<" r: "<< r <<endl;

	      		}
				else{
					//if(i == 0)cout<<e<<" found in per\n";
					int r = legal_period(e, i, cur_period);
	      		
	      			if(r != -1){
	      				arr[r][cur_period][i].push_back(e);
	      				arr[r][cur_period][i + pop_size].push_back(e);
	      				found = true;
	      				cons1++;
	      				if(i == 0)
	      					cout << local_it -> first << "scheduled in p: "<< cur_period <<" r: "<< r<<endl;
	      			}
		      		
		      	}
	      	}
		}

		cout<<endl<<endl;

		//	cout<<"done0\n";

		int pika = 0;

		for ( auto local_it = ex_rMap.begin(); local_it!= ex_rMap.end(); ++local_it ){
      		int r = local_it -> second;
      		string e  = local_it ->first;
      		if(ex_pMap.find(e) == ex_pMap.end()){

      			int p = legal_room(e, i, r);

      		 	if(p != -1){
      		 		arr[r][p][i].push_back(e);
      		 		arr[r][p][i + pop_size].push_back(e);
      		 		cons2++;
      		 		if(i == 0)
      		 			cout << local_it -> first << "scheduled in p: "<< p <<" r: "<< r <<endl;
      		 		//break;
      		 	}	      		
      		}
		}
/*
		if(i == 0){
			cout<<endl<<endl<<"after constraints print my timetable"<<endl;
			print_tt(0);
		}
*/
		//cout<<"done constaraints\n";
		int sum = cons1 + cons2;
		cout << cons1 << " " << cons2 <<" "<<sum<< endl;


		int for_size = arr[0][num_periods][i].size();
		cout<<"FOR-SIZE:"<<for_size<<" "<<endl;

		for(int it = 0; it != for_size; ++it){
	
			//cout<<it<<" ";			

			string event = arr[0][num_periods][i].back();
			
			//cout<<event<<" ";	
			if(event == "")
				cout<<"empty\n";
			
			
			arr[0][num_periods][i].pop_back();	

			arr[0][num_periods][i + pop_size].pop_back();

			if (ex_pMap.find(event) == ex_pMap.end() && ex_rMap.find(event) == ex_rMap.end()){	//if no constraints the exam scheduled then
				
				int goodness = 0;					//16 rooms num_periods periods + 1 unscheduled			
								
				for (int j = 0; j < num_periods; ++j){
					
					int rom = legal_period(event , i, j);
					
					if(rom != -1){
						
						goodness += (arr[rom][j][i].size() + Siz(i, j) + 1) / (penalty(event, i, j) + 1);
						
					}
				}
				
		
			

				int good_arr[goodness], counter = 0;

				for (int j = 0; j < num_periods; ++j){
					int rom = legal_period(event , i, j);
					
					if(rom != -1){

						int good_level = (arr[rom][j][i].size() + Siz(i, j) + 1) / (penalty(event, i, j) + 1);
						int num_pos = good_level;
						
						while(num_pos){
							good_arr[counter] = j;
							counter++;
							num_pos--;
						}
					}
				}

				

				if(goodness != 0){
					int random_per = rand() % goodness;
					int assign_period = good_arr[ random_per ];
					int room = legal_period(event, i, assign_period);
					arr[room][assign_period][i].push_back(event);
					arr[room][assign_period][i + pop_size].push_back(event);
				}
				else {
					vector<string>::iterator myit = arr[0][num_periods][i].begin();
					myit = arr[0][num_periods][i].insert(myit, event);	
					vector<string>::iterator myit1 = arr[0][num_periods][i + pop_size].begin();
					myit1 = arr[0][num_periods][i + pop_size].insert(myit1, event);	
				}
			}

			else {
				pika++;
				//cout<<"in else\n";
			}
			//cout<<"  haha  \n";
		}

		if(i == 0){
		 cout<<"pika: "<<pika<<endl;			
		}
				
		cout<<arr[0][num_periods][i].size();cout<<endl;
	}

	//cout<<"pika: "<<pika;

	cout<<num_exams;

	cout<<"\npopulation inititalized\n"<<endl;
	int sced = 0;
	for(int i = 0 ; i < num_periods; i++){
		for(int j = 0 ; j < num_rooms ; j++ ){
			cout<<arr[j][i][0].size()<<" ";
			sced += arr[j][i][0].size(); 	
		}
		cout<<endl;
	}
	cout<<endl<<endl;
	print_tt(0);
	cout<<sced<<" "<<arr[0][num_periods][0].size()<<" "<< arr[0][num_periods][0].size() + sced <<endl;

	//-------------------------------------------------------------------------------
	//Mutation + hybrid local search

	int num_of_generations = atoi(argv[3]);
	
	while(num_of_generations--){

		int roulette_arr[10000];

		int type_mut = rand() % 2;
		//int type_mut = 1;
		pop_size = 6;

		cout << "gen: "<< num_of_generations <<" "<<type_mut<< endl;

		for (int i = 0; i < pop_size; ++i)
		{

			i += pop_size;
			cout<<i<<" in mut"<<endl;


			if(type_mut == 0){						//light mutation
				//cout<<"lightmutation"<<endl;

				int random_events = rand() % num_exams; 
				
				for(int rev = 0; rev < random_events; rev++ ){
					int rp = rand() % num_periods;		//random period
					int rr = rand() % num_rooms;		//random room
					int num_of_exam = arr[rr][rp][i].size();
					
					//cout<<"before if"<<endl;
					if(num_of_exam != 0){
						
						int re = rand() % num_of_exam;
						string e = arr[rr][rp][i].at(re);
						if (ex_pMap.find(e) == ex_pMap.end() && ex_rMap.find(e) == ex_rMap.end()){

							bool replaced = false;
							
							for(int lp = 0; lp < num_periods && replaced == false; lp++){
								if(lp != rp){
									
									int new_room = legal_period(e, i, lp);
									if(new_room != -1){
										
										for(vector<string>::iterator nit = arr[rr][rp][i].begin(); nit != arr[rr][rp][i].end(); ){
											if(*nit == e){
												nit = arr[rr][rp][i].erase(nit);
												replaced = true;
												break;
											}
											else nit++;
											
										}
										arr[new_room][lp][i].push_back(e);//cout<<lp<<" "<<new_room<<" "<<i<<"done"<<endl;
									}
								}
							}
						}
					}
				}				
				cout<<"done in mutation"<<endl;
			}
			else if(type_mut == 1){			//heavy mutation
				cout<<"heavy mutation"<<endl;
				bitset<num_periods> disrupt;
				int average = tot_penalty(i)/num_periods;
				for (int q = 0; q < num_periods - 1; ++q) 		//for every period(except the last) in the timetable this loop calc whether to disrupt or not the period
				{
					int per_penalty = 0;
					for(int z = 0 ; z < num_rooms ; z++ ){		//for every room in that period
						
						for(vector<string>::iterator it = arr[z][q][i].begin(); it != arr[z][q][i].end(); ++it){		//for every exam
							
							string e = *it;
							int mut_penalty = penalty(e, i, q);		//e -> exam check its intersection with exams in priod q+1
							per_penalty += mut_penalty;

						}						
					}

					if(per_penalty < average){
						disrupt[q] = 0;
					}
					else {
						if(q >= 1 && disrupt[q-1] == 0){
							disrupt[q] = 0;
							disrupt[q+1] = 1;
						}
						else{
							disrupt[q] = 1;
						}
					}
				}
				cout<<"disrupt calculated "<<disrupt<<endl;

				vector<string> rearrange = {};		//rearrange -> vector of all exams grouped together to be rescheduled
				for (int q = 0; q < num_periods; ++q){
					
					if(disrupt[q] == 1){
						for (int z = 0; z < num_rooms; ++z)
						{
							for(vector<string>::iterator rit = arr[z][q][i].begin(); rit != arr[z][q][i].end(); ){
								string re_exam = *rit;
								if (ex_pMap.find(re_exam) == ex_pMap.end() && ex_rMap.find(re_exam) == ex_rMap.end()){

									rearrange.push_back(re_exam);
									rit = arr[z][q][i].erase(rit);	
								}
								else rit++;
							}
						}
					}	
					random_shuffle ( rearrange.begin(), rearrange.end(), myrandom );
				}


				for(vector<string>::iterator it = rearrange.begin(); it != rearrange.end(); ++it){		//rearranging done here
					
					bool found_p = false;
					string exam = *it;

					for(int q = 0 ; q < num_periods; q++){
						
						int room = legal_period(exam, i, q); 
						if(room != -1){
							arr[room][q][i].push_back(exam);
							found_p = true;
							break;
						}
					}
					if(found_p == false){
						arr[0][num_periods][i].push_back(exam);
					}
				}

				cout<<"rearranged\n"<<endl;				
			}

			cout<<"done hill climbing"<<endl;



			//---------------------------------------------------------
			//Hill climbing operator on already scheduled exams

			
			int new_rear = 0;
			for(int j = 0 ; j < num_periods ;j++){
				for(int k = 0 ; k < num_rooms; k++){
					
					for(vector<string>::iterator it = arr[k][j][i].begin(); it != arr[k][j][i].end(); ){
						string e = *it;
						
						if (ex_pMap.find(e) == ex_pMap.end() && ex_rMap.find(e) == ex_rMap.end()){
							int p = penalty(e, i, j);
							
							int new_period = -1;
							for (int l = 0; l < num_periods; ++l){

								if(l != j){

									int new_r = legal_period(e, i, l);	//new_legal_per just tells whether its possible to place exam in that period and specify the room
									//cout<<new_r<<endl;
									if(new_r != -1){
										int new_p = penalty(e, i, l);
										if(p > new_p){
											//cout<<e<<" from "<<j<<"( "<<p<<" ) to "<<l<<"( "<<new_p<<" ) ";
											p = new_p;
											new_period = l;				

											//break;
										}
									}
								}
							}
							
							if(new_period != -1){		

								int new_r = legal_period(e, i, new_period);
								//cout<<av_penalty(i)<<" ";		
								new_rear++;						
								arr[new_r][new_period][i].push_back(e);							
								it = arr[k][j][i].erase(it);	
								//cout<<av_penalty(i)<<endl;						
							}
							else it++;
/*
							sced = 0;
							for(int u = 0 ; u < num_periods; u++){
								for(int j = 0 ; j < num_rooms ; j++ ){
									cout<<arr[j][u][i].size()<<" ";
									sced += arr[j][u][i].size(); 	
								}
								cout<<endl;
							}
							//cout<<endl;
							//print_tt(0);
							cout<<sced<<" "<<arr[0][num_periods][i].size()<<" "<< arr[0][num_periods][i].size() + sced <<" 0"<<endl;*/
						}
						else if (ex_pMap.find(e) != ex_pMap.end() && ex_rMap.find(e) != ex_rMap.end()){
							

							int p = penalty(e, i, j);
							int new_period = -1;

							unordered_map<string, vector<int>>::const_iterator cons_itp = ex_pMap.find(e);
							vector<int> temp = cons_itp->second;											

							unordered_map<string, int>::const_iterator cons_itr = ex_rMap.find(e);
							int temp_r = cons_itr->second;	

							for(vector<int>::iterator it3 = temp.begin(); it3 != temp.end(); it3++){

								int l = *it3;
								if(l >= num_periods)
									break;
								
								if(l != j){

									int new_r = legal_period(e, i, l);
									if(new_r != -1){
										int new_p = penalty(e, i, l);

										if(p > new_p){

											int cons_size = 0;

											for(vector<string>::iterator it4 = arr[temp_r][l][i].begin(); it4 != arr[temp_r][l][i].end(); it4++){								
												if( ex_rMap.find(*it4) != ex_rMap.end()){
													unordered_map<string, int>::const_iterator cons_it2 = exam_cap.find(*it4);
													cons_size += cons_it2->second;	
												}
											}

											unordered_map<string, int>::const_iterator cons_it2 = exam_cap.find(e);
											cons_size += cons_it2->second;						

											if(cons_size <= room_arr[temp_r]){
												p = new_p;
												new_period = l;									
											}

										}
									}

								} 								
							}

							if(new_period != -1){		
								arr[temp_r][new_period][i].push_back(e);							
								it = arr[k][j][i].erase(it);
								//cout<<temp_r<<" "<<new_period<<" "<<j<<endl;
								new_rear++;									
							}

							else it++;
/*
							sced = 0;
							for(int u = 0 ; u < num_periods; u++){
								for(int j = 0 ; j < num_rooms ; j++ ){
									cout<<arr[j][u][i].size()<<" ";
									sced += arr[j][u][i].size(); 	
								}
								cout<<endl;
							}
							//cout<<endl;
							//print_tt(0);
							cout<<sced<<" "<<arr[0][num_periods][i].size()<<" "<< arr[0][num_periods][i].size() + sced <<" 1"<<endl;*/					
						}

						else if(ex_pMap.find(e) != ex_pMap.end()){

							int p = penalty(e, i, j);
							int new_period = -1;

							unordered_map<string, vector<int>>::const_iterator cons_it = ex_pMap.find(e);
							vector<int> temp = cons_it->second;	

							for(vector<int>::iterator it3 = temp.begin(); it3 != temp.end(); it3++){
								int current_period = *it3;

								if(current_period >= num_periods)
									break;

								int new_r = legal_period(e, i, current_period);
								if(current_period != j && new_r != -1){
									int new_p = penalty(e, i, current_period);
									if(p > new_p){
										p = new_p;
										new_period = current_period;		
											
									}
								} 
							}

							if(new_period != -1){	
								int new_r = legal_period(e, i, new_period);							
								arr[new_r][new_period][i].push_back(e);							
								it = arr[k][j][i].erase(it);
								new_rear++;			
							}
							else it++;
/*
							sced = 0;
							for(int u = 0 ; u < num_periods; u++){
								for(int j = 0 ; j < num_rooms ; j++ ){
									//cout<<arr[j][u][i].size()<<" ";
									sced += arr[j][u][i].size(); 	
								}
								//cout<<endl;
							}*/
							//cout<<endl<<endl;
							//print_tt(0);
							//cout<<sced<<" "<<arr[0][num_periods][i].size()<<" "<< arr[0][num_periods][i].size() + sced <<" 2"<<endl;
						}
						else{

							int p = penalty(e, i, j);
							int new_period = -1;

							unordered_map<string, int>::const_iterator cons_it = ex_rMap.find(e);
							int temp = cons_it->second;	

							for(int l = 0; l < num_periods; l++){

								int new_r = legal_period(e, i, l);
								
								if(l != j && new_r != -1){
									int new_p = penalty(e, i, l);

									if(p > new_p){

										int cons_size = 0;

										for(vector<string>::iterator it = arr[temp][l][i].begin(); it != arr[temp][l][i].end(); it++){								
											if( ex_rMap.find(*it) != ex_rMap.end()){
												unordered_map<string, int>::const_iterator cons_it2 = exam_cap.find(*it);
												cons_size += cons_it2->second;	
											}
										}

										unordered_map<string, int>::const_iterator cons_it2 = exam_cap.find(e);
										cons_size += cons_it2->second;						

										if(cons_size <= room_arr[temp]){
											p = new_p;
											new_period = l;		
											
										}
									}
								} 								
							}

							if(new_period != -1){		
								arr[temp][new_period][i].push_back(e);							
								it = arr[k][j][i].erase(it);
								new_rear++;									
							}

							else it++;					
/*
							sced = 0;
							for(int u = 0 ; u < num_periods; u++){
								for(int j = 0 ; j < num_rooms ; j++ ){
									//cout<<arr[j][u][i].size()<<" ";
									sced += arr[j][u][i].size(); 	
								}
								//cout<<endl;
							}*/
							//cout<<endl<<endl;
							//print_tt(0);
							//cout<<sced<<" "<<arr[0][num_periods][i].size()<<" "<< arr[0][num_periods][i].size() + sced <<" 3"<<endl;
						}
					}				
				}
			}




			//hill climbing on unscheduled exams


			new_rear = 0;			

			int j = num_periods;
			int k = 0;

			
			for(vector<string>::iterator it = arr[k][j][i].begin(); it != arr[k][j][i].end(); ){

				
				string e = *it;
			
				if (ex_pMap.find(e) == ex_pMap.end() && ex_rMap.find(e) == ex_rMap.end()){

					int p = 10000;			//assuming it to be largest number
					int new_period = -1;
					for (int l = 0; l < num_periods; ++l){

						if(l != j){

							int new_r = legal_period(e, i, l);	//new_legal_per just tells whether its possible to place exam in that period and specify the room
							//cout<<new_r<<endl;
							if(new_r != -1){
								int new_p = penalty(e, i, l);
								if(p > new_p){
									//cout<<e<<" from "<<j<<"( "<<p<<" ) to "<<l<<"( "<<new_p<<" ) ";
									p = new_p;
									new_period = l;				

									//break;
								}
							}
						}
					}

					
					if(new_period != -1){		

						int new_r = legal_period(e, i, new_period);
						//cout<<av_penalty(i)<<" ";		
						new_rear++;						
						arr[new_r][new_period][i].push_back(e);							
						it = arr[k][j][i].erase(it);	
						//cout<<av_penalty(i)<<endl;						
					}
					else it++;
					
				
					/*
					sced = 0;
					for(int u = 0 ; u < num_periods; u++){
						for(int j = 0 ; j < num_rooms ; j++ ){
							cout<<arr[j][u][i].size()<<" ";
							sced += arr[j][u][i].size(); 	
						}
						cout<<endl;
					}
					//cout<<endl;
					//print_tt(0);
					cout<<sced<<" "<<arr[0][num_periods][i].size()<<" "<< arr[0][num_periods][i].size() + sced <<" 0"<<endl;*/
				}
				else if (ex_pMap.find(e) != ex_pMap.end() && ex_rMap.find(e) != ex_rMap.end()){
					
					
					int p = 10000;
					int new_period = -1;

					unordered_map<string, vector<int>>::const_iterator cons_itp = ex_pMap.find(e);
					vector<int> temp = cons_itp->second;											

					unordered_map<string, int>::const_iterator cons_itr = ex_rMap.find(e);
					int temp_r = cons_itr->second;	

					for(vector<int>::iterator it3 = temp.begin(); it3 != temp.end(); it3++){

						int l = *it3;
						if(l >= num_periods)
							break;
						
						if(l != j){

							int new_r = legal_period(e, i, l);
							if(new_r != -1){
								int new_p = penalty(e, i, l);

								if(p > new_p){

									int cons_size = 0;

									for(vector<string>::iterator it4 = arr[temp_r][l][i].begin(); it4 != arr[temp_r][l][i].end(); it4++){								
										if( ex_rMap.find(*it4) != ex_rMap.end()){
											unordered_map<string, int>::const_iterator cons_it2 = exam_cap.find(*it4);
											cons_size += cons_it2->second;	
										}
									}

									unordered_map<string, int>::const_iterator cons_it2 = exam_cap.find(e);
									cons_size += cons_it2->second;						

									if(cons_size <= room_arr[temp_r]){
										p = new_p;
										new_period = l;									
									}

								}
							}

						} 								
					}

					if(new_period != -1){		
						arr[temp_r][new_period][i].push_back(e);							
						it = arr[k][j][i].erase(it);
						//cout<<temp_r<<" "<<new_period<<" "<<j<<endl;
						new_rear++;									
					}

					else it++;
/*
					sced = 0;
					for(int u = 0 ; u < num_periods; u++){
						for(int j = 0 ; j < num_rooms ; j++ ){
							cout<<arr[j][u][i].size()<<" ";
							sced += arr[j][u][i].size(); 	
						}
						cout<<endl;
					}
					//cout<<endl;
					//print_tt(0);
					cout<<sced<<" "<<arr[0][num_periods][i].size()<<" "<< arr[0][num_periods][i].size() + sced <<" 1"<<endl;*/					
				}

				else if(ex_pMap.find(e) != ex_pMap.end()){
					

					int p = 10000;
					int new_period = -1;

					unordered_map<string, vector<int>>::const_iterator cons_it = ex_pMap.find(e);
					vector<int> temp = cons_it->second;	

					for(vector<int>::iterator it3 = temp.begin(); it3 != temp.end(); it3++){
						int current_period = *it3;

						if(current_period >= num_periods)
							break;

						int new_r = legal_period(e, i, current_period);
						if(current_period != j && new_r != -1){
							int new_p = penalty(e, i, current_period);
							if(p > new_p){
								p = new_p;
								new_period = current_period;		
									
							}
						} 
					}

					if(new_period != -1){	
						int new_r = legal_period(e, i, new_period);							
						arr[new_r][new_period][i].push_back(e);							
						it = arr[k][j][i].erase(it);
						new_rear++;			
					}
					else it++;
/*
					sced = 0;
					for(int u = 0 ; u < num_periods; u++){
						for(int j = 0 ; j < num_rooms ; j++ ){
							//cout<<arr[j][u][i].size()<<" ";
							sced += arr[j][u][i].size(); 	
						}
						//cout<<endl;
					}*/
					//cout<<endl<<endl;
					//print_tt(0);
					//cout<<sced<<" "<<arr[0][num_periods][i].size()<<" "<< arr[0][num_periods][i].size() + sced <<" 2"<<endl;
				}
				else{
					
					int p = 10000;
					int new_period = -1;

					unordered_map<string, int>::const_iterator cons_it = ex_rMap.find(e);
					int temp = cons_it->second;	

					for(int l = 0; l < num_periods; l++){

						int new_r = legal_period(e, i, l);
						
						if(l != j && new_r != -1){
							int new_p = penalty(e, i, l);

							if(p > new_p){

								int cons_size = 0;

								for(vector<string>::iterator it = arr[temp][l][i].begin(); it != arr[temp][l][i].end(); it++){								
									if( ex_rMap.find(*it) != ex_rMap.end()){
										unordered_map<string, int>::const_iterator cons_it2 = exam_cap.find(*it);
										cons_size += cons_it2->second;	
									}
								}

								unordered_map<string, int>::const_iterator cons_it2 = exam_cap.find(e);
								cons_size += cons_it2->second;						

								if(cons_size <= room_arr[temp]){
									p = new_p;
									new_period = l;		
									
								}
							}
						} 								
					}

					if(new_period != -1){		
						arr[temp][new_period][i].push_back(e);							
						it = arr[k][j][i].erase(it);
						new_rear++;									
					}

					else it++;					
					/*
					sced = 0;
					for(int u = 0 ; u < num_periods; u++){
						for(int j = 0 ; j < num_rooms ; j++ ){
							//cout<<arr[j][u][i].size()<<" ";
							sced += arr[j][u][i].size(); 	
						}
						//cout<<endl;
					}*/
					//cout<<endl<<endl;
					//print_tt(0);
					//cout<<sced<<" "<<arr[0][num_periods][i].size()<<" "<< arr[0][num_periods][i].size() + sced <<" 3"<<endl;
				}

				
			}				
	
			
			cout<<"after hill climbing\n"<<endl;
			/*
			int sced1 = 0;
			
			for(int u = 0 ; u < num_periods; u++){
				for(int j = 0 ; j < num_rooms ; j++ ){
					//cout<<arr[j][u][i].size()<<" ";
					sced1 += arr[j][u][i].size(); 	
				}
				cout<<endl;
			}
			//cout<<endl<<endl;
			//print_tt(0);
			cout<<sced1<<" "<<arr[0][num_periods][i].size()<<" "<< arr[0][num_periods][i].size() + sced1 <<endl;
			cout<<"done after hill climbing\n";*/




			i -= pop_size;
		}

		cout<<"done hill climbing**\n\n"<<endl;
	
		//now we have twice the population size
		//----------------------------------------------------------
		//SELECTION

		float tot_fitness = 0;
		int max_fitness = -1;
		int timetable = 0;
		int least_conflict = 0;


		int unsched;// = arr[0][num_periods][i].size();
		cout<<"num_conflicts: ";
		for (int i = 0; i < 2*pop_size; ++i)
		{
			unsched = arr[0][num_periods][i].size();

			cout<<"beep beep"<<endl;
			int num_conflicts = tot_penalty(i) ;	
			cout<<num_conflicts<<" "<<unsched<<"******"<<endl;		
			float loc_fitness = (100 * num_exams) / (2000.0 * unsched + num_conflicts);						//num_events = 805
			cout<<loc_fitness<<" "<<num_conflicts<<" "<<unsched<<" "<<num_exams<<" "<<unsched<<" "<<num_conflicts<<endl;
			tot_fitness += loc_fitness;
			//max_fitness = (max_fitness > loc_fitness)?max_fitness:loc_fitness;
			if(max_fitness < loc_fitness){
				max_fitness = loc_fitness;
				timetable = i;
				least_conflict = num_conflicts;
			}
		}

		cout<<"tot_fit: "<<tot_fitness<<endl;


		//--------------------PRINTING LAST BEST TIMETABLE----------------------------

		if(num_of_generations == 0)		
			print_tt(timetable);

		//--------------------DONE PRINTING TIMETABLE-----------------------

		cout<<"max_fitness: "<<max_fitness<<endl;
		cout<<"conflict: "<<least_conflict<<endl;

		int roulette_counter = 0;

		for (int i = 0; i < 2*pop_size; ++i)
		{
			unsched = arr[0][num_periods][i].size();
			int num_conflicts = tot_penalty(i) ;
			if(unsched == 0 && num_conflicts == 0){															//perfect timetable found
				print_tt(i);
				int stop_si=clock();
				cout << "time: " << (stop_si-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
				exit(0);
			}
			float loc_fitness = (100 * num_exams) / (2000.0 * unsched + num_conflicts);						//num_events = 805
			int area_in_roulette = (loc_fitness / tot_fitness) * 10000;
			cout<<area_in_roulette<<" ";
			
			for (int j = 0; j < area_in_roulette; ++j){
				roulette_arr[roulette_counter] = i;
				roulette_counter++;
			} 	
		}

		cout<<roulette_counter<<endl;

		cout<<"dene 1"<<endl;

		int copy_val = roulette_arr[roulette_counter];
		cout<<copy_val<<endl;

		while(roulette_counter < 9999){
			roulette_counter++;
			roulette_arr[roulette_counter] = copy_val;
		}

		cout<<roulette_counter<<endl;

		cout<<"dene 1"<<endl;
		//Again make the new array

		vector<string> temp_arr[num_rooms][num_periods + 1][pop_size];

		for (int i = 0; i <  pop_size; ++i)
		{
			int random_num = rand() % 10000;
			int rand_tt = roulette_arr[random_num]; 
			cout<<"done "<<rand_tt<<" "<<random_num<<endl;
			for(int j = 0; j < num_periods + 1; j++){
				for(int k = 0; k < num_rooms; k++){
					temp_arr[k][j][i] = arr[k][j][rand_tt];
					//cout<<j<<" "<<k<<endl;					
				}
			}
		}
		cout<<"dene 2"<<endl;
		
		for(int k = 0; k < num_rooms; k++){
			for(int j = 0; j < num_periods + 1; j++){
				for(int i = 0; i < pop_size; i++){
					arr[k][j][i] = temp_arr[k][j][i];
				}
			}
		}

		cout<<"dene 3"<<endl;

	}

	int stop_s=clock();
	cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)*1000 << endl;
}
