#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>		//random
#include <cstdlib>
#include <ctime>
using namespace std;

int pop_size = 6;

unordered_map<string, vector<string>> umap, e_students;
unordered_map<string, int> exam_cap;
unordered_map<string, int> exam_map;
unordered_map<string, vector<int>> ex_pMap;
unordered_map<string, int> ex_rMap;
int max_percap = 500;
const int num_periods = 10;
const int num_rooms = 1;
const int num_exams = 88;
vector<string> arr[num_rooms + 1][num_periods + 1][2];
int size_arr[num_rooms+1][num_periods+1][2];
int room_arr[num_rooms + 1] = {0};




int myrandom (int i) { return std::rand()%i;}



int print_tt(int timetable){

	int num_exam = 0;
	int unsched = 0;
	int pc = 0;				//period constraints;


	// cout<<"num_periods:"<<num_periods<<endl;
	for(int i = 0; i < num_periods; i++){
		cout<<"Period "<<i<<":\n";
		int sump = 0;
		for(int j = 0; j < num_rooms+1; j++){
			int sum = 0;
			cout<<"Room No. "<<j<<" :";//<<"( "<<room_arr[j]<<" ) :";
			for(vector<string>::iterator it = arr[j][i][timetable].begin(); it != arr[j][i][timetable].end(); it++){
				cout<<*it<<";";
				unordered_map<string, int>::const_iterator cons_it = exam_cap.find(*it);
				//cout<<"( "<<cons_it->second <<" )*";				
				sum += cons_it->second;
				num_exam++;
				if(j == num_rooms){
					unsched++;
					if(ex_rMap.find(*it) == ex_rMap.end() && ex_pMap.find(*it) != ex_pMap.end()){
						pc++;
					}				
				}
			}
			//cout<<"("<<sum<<")"; 		
			if(sum > room_arr[j] && j != num_rooms)
				cout<<"ERRORr";	
			sump += sum;
			cout<<endl;
		}
		if(sump > max_percap)
			cout<<"Errorp";	
	}

	cout<<"NUM_EXAM : "<<num_exam<<endl;
	cout<<"BIN_PACKING_UNSCHED : "<<unsched<<endl;
	cout<<"UNSCHED : "<<arr[0][num_periods][timetable].size()<<endl;
	cout<<"perios constraint : "<<pc<<endl;
	return unsched + arr[0][num_periods][timetable].size();
	cout<<"\n\n\n\n\n\n\n";
}




/*

int print_tt(int timetable){

	int num_exam = 0;
	int unsched = 0;
	int pc = 0;				//period constraints;


	// cout<<"num_periods:"<<num_periods<<endl;
	for(int i = 0; i < num_periods; i++){
		// cout<<"Period :"<<i<<"\n";
		int sump = 0;
		for(int j = 0; j < num_rooms+1; j++){
			int sum = 0;
			// cout<<"Room No. "<<j<<" :";//<<"( "<<room_arr[j]<<" ) :";
			for(vector<string>::iterator it = arr[j][i][timetable].begin(); it != arr[j][i][timetable].end(); it++){
				// cout<<*it<<";";
				unordered_map<string, int>::const_iterator cons_it = exam_cap.find(*it);
				//cout<<"( "<<cons_it->second <<" )*";				
				sum += cons_it->second;
				num_exam++;
				if(j == num_rooms){
					unsched++;
					if(ex_rMap.find(*it) == ex_rMap.end() && ex_pMap.find(*it) != ex_pMap.end()){
						pc++;
					}				
				}
			}
			// cout<<"("<<sum<<")"; 		
			if(sum > room_arr[j] && j != num_rooms)
				cout<<"ERRORr";	
			sump += sum;
			// cout<<endl;
		}
		if(sump > max_percap)
			cout<<"Errorp";	
	}


	cout<<"\n\n\n\n\n\n\n";
	cout<<"NUM_EXAM : "<<num_exam<<endl;
	cout<<"BIN_PACKING_UNSCHED : "<<unsched<<endl;
	cout<<"UNSCHED : "<<arr[0][num_periods][timetable].size()<<endl;
	cout<<"perios constraint : "<<pc<<endl;
	return unsched + arr[0][num_periods][timetable].size();
}


*/


bool intersect(string e1, string e2){

	unordered_map<string, vector<string>>::const_iterator it1 = e_students.find(e1);
	unordered_map<string, vector<string>>::const_iterator it2 = e_students.find(e2);
	if(it1 == e_students.end() || it2 == e_students.end())
		cout<<"haha "<<e1<<" "<<e2<<endl;
	if(e1 =="" or e2 == "")
		cout<<"YESS"<<endl;
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

		if(checker != 2 && i %16 != 15){
			vector<string> pen;
			for(int j = 0; j < num_rooms+1; j++){
				
				for(vector<string>::iterator it = arr[j][i][tt].begin(); it != arr[j][i][tt].end(); it++){
					string e1 = *it;
					for(int j1 = 0 ; j1 < num_rooms+1; j1++){
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
		
		for(int i = 0; i < num_rooms+1; i++){
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
		
		for(int i = 0; i < num_rooms+1; i++){
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

/*	unordered_map<string, int>::const_iterator it1 = exam_map.find(event);
	int time = it1->second;
	
	int periods_time;
	if(period < num_rooms && period % 3 == 0)
		periods_time = 3;
	else if(period > 15 && period % 3 == 1)
		periods_time = 3;	else periods_time = 2;
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

	//cout<<"out of legal period"<<endl;

	return room;
	//arr[room][period][tt].push_back(event)
}





int new_legal_period(string event, int tt, int period){

	// cout<<"in new_legal_period: ";
	
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
	int per_cap = 0;
	for(int i = 0; i < num_rooms+1; i++){
		
		for(vector<string>::iterator it = arr[i][period][tt].begin(); it != arr[i][period][tt].end(); ++it){
			string exam = *it;
			// cout<<exam<<" "<<event<<"* "<<endl;
			if(intersect(exam, event))
				return -1;
			
			unordered_map<string, int>::const_iterator cons_it = exam_cap.find(exam);
			per_cap += cons_it->second;
		}		
	}


	unordered_map<string, int>::const_iterator cons_it = exam_cap.find(event);
	per_cap += cons_it->second;

	// if(period == 0)
	// 	cout<<"ERROR:"<<event<<" "<<cons_it->second<<" "<<per_cap<<" \n";

	if(per_cap <= max_percap){
		room = num_rooms;
		return room;
	}
	return -1;
}


int legal_room(string event, int tt, int room){

	int period = -1;										//room -> room in which exam is placed 

/*	unordered_map<string, int>::const_iterator it1 = exam_map.find(event);
	int time = it1->second;
*/
	int room_cap = room_arr[room];
	

	//num_of sitting <= rooms available
	for(int i = 0; i < num_periods; i++){

/*		int periods_time;
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






void penalty_exams(string event, int tt, int period){

		
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
		
		for(int i = 0; i < num_rooms+1; i++){
			for(vector<string>::iterator it = arr[i][period-1][tt].begin(); it != arr[i][period-1][tt].end(); it++){

				string e = *it;
				if(e == event)
					continue;

				if(intersect(e, event))
					cout<<e<<" ";

		
			}
		}
	}
	
	if(next){		//checking next
		
		for(int i = 0; i < num_rooms+1; i++){
			for(vector<string>::iterator it = arr[i][period+1][tt].begin(); it != arr[i][period+1][tt].end(); it++){
				
				string e = *it;				
				if(e == event)
					continue;

				if(intersect(e, event))
					cout<<e<<" ";
			}
		}
	}

	cout<<endl;

}





vector<string> conflicting_exams(string event, int tt, int period){

	bool legal = false;

	vector<string> ret_exam;										//room -> room in which exam is placed 

	for(int i = 0; i < num_rooms + 1; i++){
		int sum_of_rcap = 0;
		for(vector<string>::iterator it = arr[i][period][tt].begin(); it != arr[i][period][tt].end(); ++it){
			string exam = *it;
			
			if(intersect(exam, event)){
				ret_exam.push_back(exam);
			}
			
		}
	}

	return ret_exam;
}




void swap(vector<string> con_exam, string e, int e_p, int p, int i){

	int checker1 = con_exam.size();
	// cout<<" SWAP "<<e<<" "<<checker1<<endl;
	
	for(vector<string>::iterator it = con_exam.begin(); it != con_exam.end() && checker1 > 0; it++){
		// cout<<" IN "<<endl;
		checker1--;
		string conf = *it;

		int room = -1;

		bool removed = false;
		for(int r = 0; r < num_rooms+1 && !removed; r++){
			for(vector<string>::iterator eit = arr[r][p][i].begin(); eit != arr[r][p][i].end() && !removed; ){
				string find_confe = *eit;
				if(conf == find_confe){
					arr[r][p][i].erase(eit);
					room = r;
					removed = true;
				}
				else eit++;
			}
		}

		// cout<<" good till here "<<endl;

		bool swap = false;

		for(int j = 0; j < num_periods && !swap; j++){
			if(j != p){
				for(int k = 0; k < num_rooms && !swap; k++){
					// cout<<" in1 "<<endl;
					int checker2 = arr[k][j][i].size();

					for(vector<string>::iterator eit = arr[k][j][i].begin(); eit != arr[k][j][i].end() && !swap && checker2 > 0; ){
						checker2--;					
						string swape = *eit;
						arr[k][j][i].erase(eit);
						bool pushed_in = false;
						// cout<<" again good "<<endl;
						if(!intersect(swape, e)){
							
							int new_r1 = legal_period(swape, i, p);
							int new_r2 = legal_period(conf, i, j);
							// cout<<"noi "<<new_r1<<" "<<new_r2;
							if(new_r1 != -1 && new_r2 != -1){
								//cout<<"yep";
								arr[new_r1][p][i].push_back(swape);
								arr[new_r2][j][i].push_back(conf);								
								pushed_in = true;
								swap = true;
							}
						}
						// cout<<" noi out"<<endl;
						if(!pushed_in)
							arr[k][j][i].push_back(swape);
					}
					// cout<<" out1 "<<endl;
				}
			}
		}	

		// cout<<"room :"<<room<<endl;

		if(room == -1){
			cout<<conf<<" ROOM ERROR "<<p<<endl;
		}
		if(!swap)
			arr[room][p][i].push_back(conf);
		//else cout<<conf<<" ";
		// cout<<" OUT "<<endl;
	}
	//cout<<endl;
	// cout<<" out of swap "<<endl;
}





bool gen_swap(string e1, string e2, int p1, int p2, int i, bool red_penalty){			//deosnt include unscheduled exams

	bool swap = true;
	int room1 = legal_period(e1, i, p2);
	int room2 = legal_period(e2, i, p1);
	if(room1 != -1 && room2 != -1){
		
		if(red_penalty){
			int penalty1 = penalty(e1, i, p1);
			int penalty2 = penalty(e2, i, p2);
			int new_penalty1 = penalty(e1, i, p2);
			int new_penalty2 = penalty(e2, i, p1);
			if(penalty1 + penalty2 < new_penalty1 + new_penalty2){
				swap = false;
			}
		}

		if(swap){
			arr[room1][p2][i].push_back(e1);
			arr[room2][p1][i].push_back(e2);
		}
	}
	else swap = false;
	return swap;
}




bool swap2_consec(string e1, string e2, int p1i, int p2i, int i, bool red_penalty, int init_penalty = 100000){

	cout<<" in "<<endl;

	int final_p1 = -1;
	int final_p2 = -1;
	int final_r1 = -1;
	int final_r2 = -1;

	for(int p1 = 0; p1 < num_periods; p1++){
		for(int p2 = 0; p2 < num_periods; p2++){
			int r1 = legal_period(e1, i, p1);
			int r2 = legal_period(e2, i, p2);
			if(r1 != -1 && r2 != -1){

				bool assign = true;
				
				if(red_penalty){
					int penalty1 = penalty(e1, i, p1);
					int penalty2 = penalty(e2, i, p2);
					if(penalty1 + penalty2 > init_penalty){
						assign = false;
					}
				}
				if(assign && p1 != p1i && p2 != p2i){
					final_p1 = p1;
					final_p2 = p2;
					final_r1 = r1;
					final_r2 = r2;
				}
			}
		}
	}

	if(final_p1 != -1){
		arr[final_r1][final_p1][i].push_back(e1);
		arr[final_r2][final_p2][i].push_back(e2);
		cout<<" out "<<endl;
		return true;
	}
	else{
		return false;
		cout<<" out "<<endl;
	}
}




int main(int argc, char* argv[]){


	long long int start_s=clock();

	int best_unsched = 10000;
	int best_conflicts = 100000;
	// int iter = atoi(argv[3]);
	int iter = 1; 

	

	std::srand ( unsigned ( std::time(0) ) );


	ifstream file_enrol, file_exam, file_roomCap, ex_period, ex_room;			//constraints -> ex_period, ex_room
	file_enrol.open(argv[1]);

	string student_code, exam_code;

	while(!file_enrol.eof()){ 
		file_enrol >> student_code >> exam_code;								//1st input enrolements
		umap[student_code].push_back(exam_code);
		e_students[exam_code].push_back(student_code);
		exam_cap[exam_code]++;
	}
	
	file_enrol.close();

	


	file_exam.open(argv[2]);
	
	string ex_code;
	int ex_time;
	string ex_arr[num_exams];
	int counter = 0; 


	while(!file_exam.eof() && counter < num_exams){
		file_exam >> ex_code >> ex_time;							//2nd input exams
		exam_map[ex_code] = ex_time;
		ex_arr[counter] = ex_code;
		counter++;
		
	}

	file_exam.close();



	// file_roomCap.open(argv[3]);

	// int room, cap;
	room_arr[0] = max_percap;

	// while(!file_roomCap.eof()){

	// 	file_roomCap >> room >> cap;
	// 	room_arr[room] = cap;
	// }
	// cout<<endl;
	// file_roomCap.close();

	//sort(room_arr, room_arr + num_rooms);



/*
	ex_period.open(argv[4]);

	string ex_p_code;
	int ex_p_per;			//ex_p_code -> in case(exam_period file) -> code of exam
	while(!ex_period.eof()){
		ex_period >> ex_p_code >> ex_p_per;
		ex_pMap[ex_p_code].push_back(ex_p_per);
	}
	cout<<endl;
	ex_period.close();



	ex_room.open(argv[5]);

	string ex_r_code;
	int ex_r_room;
	while(!ex_room.eof()){
		ex_room >> ex_r_code >> ex_r_room;
		ex_rMap[ex_r_code] = ex_r_room;
		cout<<ex_r_code<<"--"<<endl;
	}
	cout<<endl;
	ex_room.close();*/

	//	------------------------INITIAL POPULATION GENERATION----------------------

	while(iter--){

		vector<string> arr_empty[num_rooms + 1][num_periods + 1][2];
		// arr = arr_empty;
		for(int i = 0; i < num_rooms+1; i++){
			for(int j = 0; j < num_periods+1; j++){
				for(int k = 0 ; k < 2; k++){
					arr[i][j][k] = arr_empty[i][j][k];
				}
			}
		}

		cout<<"starting pop inititalization\n";

		int i = 0;

		for (int q = 0; q < num_exams; ++q){				//all exams put in unscheduled category
			arr[0][num_periods][i].push_back(ex_arr[q]);	
		}

		random_shuffle ( arr[0][num_periods][i].begin(), arr[0][num_periods][i].end(), myrandom);
		string event = arr[0][num_periods][i].back();	
		cout<<"str_event:"<<event<<" ";

		// cout<<"np: "<<num_periods<<endl;
		// cout<<arr[0][num_periods][i].size();
		 //print_tt(0);
				

		arr[0][num_periods][1] = arr[0][num_periods][0];

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
	      			cout<<"IN HERE"<<endl;
					int r = it1 -> second;
					cout<<r<<endl;
					arr[r][cur_period][i].push_back(e);
					arr[r][cur_period][1].push_back(e);
	  				//arr[r][cur_period][i + pop_size].push_back(e);
	  				found = true;
	  				cons1++;
	  				if(i == 0)
	  					cout << local_it -> first << "scheduled in p: "<< cur_period <<" r: "<< r <<endl;
	      		}
				else{

					
					int r = new_legal_period(e, i, cur_period);
	      		
	      			if(r != -1){
	      				arr[r][cur_period][i].push_back(e);      			
	      				arr[r][cur_period][1].push_back(e);
	      				if(e == "")
	      					cout<<"EMPTY"<<endl;
	      				//arr[r][cur_period][i + pop_size].push_back(e);
	      				found = true;
	      				cons1++;
	      				if(i == 0)
	      					cout << local_it -> first << "scheduled in p: "<< cur_period <<" r: "<< r<<endl;
	      			}	      		
		      	}
	      	}
		}

		cout<<endl<<endl;

			cout<<"done0\n";

		int pika = 0;

		for ( auto local_it = ex_rMap.begin(); local_it!= ex_rMap.end(); ++local_it ){
	  		int r = local_it -> second;
	  		string e  = local_it ->first;
	  		if(ex_pMap.find(e) == ex_pMap.end()){

	  			int p = legal_room(e, i, r);

	  		 	if(p != -1){
	  		 		arr[r][p][i].push_back(e);  		 	
	  		 		arr[r][p][1].push_back(e);
	  		 		//arr[r][p][i + pop_size].push_back(e);
	  		 		cons2++;
	  		 		if(i == 0)
	  		 			cout << local_it -> first << "scheduled in p: "<< p <<" r: "<< r <<endl;
	  		 	}      		
	  		}
		}


		int sum = cons1 + cons2;
		cout << cons1 << " " << cons2 <<" "<<sum<< endl;

		int for_size = arr[0][num_periods][i].size();

		int cc = 5;


		for(int it = 0; it != for_size; ++it){	

			string event = arr[0][num_periods][i].back();
			
			if(cc > 0){
				cout<<"event:"<<event<<" ";
				cc--;
			}		

			arr[0][num_periods][i].pop_back();
			arr[0][num_periods][1].pop_back();	



			if (ex_pMap.find(event) == ex_pMap.end() && ex_rMap.find(event) == ex_rMap.end()){	//if no constraints the exam scheduled then
				
				bool inserted = false;
				
				for (int j = 0; j < num_periods; ++j){

					// cout<<"ev: "<<event<<" i: "<<i<<" j: "<<j<<endl;
					
					int rom = new_legal_period(event , i, j);
					
					if(rom != -1){				
						arr[num_rooms][j][i].push_back(event);	
						arr[num_rooms][j][1].push_back(event);
						inserted = true;
						break;					
					}
				}

				if(!inserted){
					vector<string>::iterator myit = arr[0][num_periods][i].begin();
					myit = arr[0][num_periods][i].insert(myit, event);	
					vector<string>::iterator myit1 = arr[0][num_periods][1].begin();
					myit1 = arr[0][num_periods][1].insert(myit1, event);
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
				
		cout<<arr[0][num_periods][i].size()<<endl;

		cout<<"-----------------------------TIMETABLE AFTER POPULATION INITIALIZATION---------------------------------------\n";
		//int unscheduledexams = print_tt(0);


		//------------------------STARTING LOCAL SEARCH------------------------------
		int local_search = 20;
		int final_tt = -1;
		int final_penalty = 0;

		while(local_search--){

			if(local_search % 2 == 0)
				i = 1;
			else i = 0;

			cout<<"i: "<<i<<endl;

			//---------------------------------------------------------
			//Hill climbing operator

			int new_rear = 0;
			for(int j = 0 ; j < num_periods ;j++){
				
				for(int k = 0 ; k < num_rooms+1; k++){
					
					for(vector<string>::iterator it = arr[k][j][i].begin(); it != arr[k][j][i].end(); ){
						
						string e = *it;
						if (ex_pMap.find(e) == ex_pMap.end() && ex_rMap.find(e) == ex_rMap.end()){
							int p = penalty(e, i, j);
							int new_period = -1;
							for (int l = 0; l < num_periods; ++l){

								if(l != j){

									int new_r = new_legal_period(e, i, l);	//new_legal_per just tells whether its possible to place exam in that period and specify the room
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

								//int new_r = legal_period(e, new_period, i);
														
								arr[num_rooms][new_period][i].push_back(e);							
								it = arr[k][j][i].erase(it);	
								new_rear++;		
												
							}
							else it++;
						}
					}									
				}
			}




			int j = num_periods;
			int k = 0;

			//cout<<"done till here"<<endl;
			
			for(vector<string>::iterator it = arr[k][j][i].begin(); it != arr[k][j][i].end(); ){

				
				string e = *it;
			
				if (ex_pMap.find(e) == ex_pMap.end() && ex_rMap.find(e) == ex_rMap.end()){

					int p = 10000;			//assuming it to be largest number
					int new_period = -1;
					for (int l = 0; l < num_periods; ++l){

						if(l != j){

							int new_r = new_legal_period(e, i, l);	//new_legal_per just tells whether its possible to place exam in that period and specify the room
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

					//cout<<"done till here"<<endl;

					
					if(new_period != -1){		
						int new_r = new_legal_period(e, i, new_period);						
						new_rear++;						
						arr[new_r][new_period][i].push_back(e);							
						it = arr[k][j][i].erase(it);					
					}
					else it++;
				}
			}




			cout<<"done hill climbing "<<new_rear<<endl;
			//---------------- Evaluation function-----------------finding whether the solution has improved or not

			int new_tt = i;
			final_tt = new_tt;
			int old_tt;
			if(i == 0)
				old_tt = 1;
			else old_tt = 0;

			float old_fitness;
			float new_fitness;


			int unsched = arr[0][num_periods][old_tt].size();
			int num_conflicts = tot_penalty(old_tt) ;
			cout<<"-----------------------------\n"<<"unsched: "<<unsched<<" "<<"num_conflicts: "<<num_conflicts<<endl;	
			old_fitness = (10 * num_exams) / ((2000 * unsched) + num_conflicts);						//num_events = num_exams		
			int old_unsch = unsched;
			int old_numc = num_conflicts;


			unsched = arr[0][num_periods][new_tt].size();
			num_conflicts = tot_penalty(new_tt);
			cout<<"-----------------------------\n"<<"unsched: "<<unsched<<" "<<"num_conflicts: "<<num_conflicts<<endl;
			int new_unsch = unsched;
			int new_numc = num_conflicts;
			

			// if(local_search == 0)
			// 	unscheduledexams = print_tt(new_tt);
			
			if(unsched == 0 && num_conflicts == 0){
				// unscheduledexams = print_tt(new_tt);
				final_penalty = 0;
				break;
			}
			new_fitness = (10 * num_exams) / ((2000 * unsched) + num_conflicts);						//num_events = num_exams


			cout<<"old_fitness: "<<old_fitness<<"\n new fitness: "<<new_fitness<<endl;
			if(new_fitness > old_fitness)
				cout<<"IMPROVEMENT IN LOCAL AREA\n";
			if(new_unsch == old_unsch && new_numc == old_numc)
				break;

			for(int i = 0; i < num_rooms+1; i++){
				for(int j = 0; j < num_periods+1; j++){
					arr[i][j][old_tt] = arr[i][j][new_tt];
				}
			}
		} 	

		int stop_si=clock();
		cout << "BEF_BIN_time: " << (stop_si-start_s)/double(CLOCKS_PER_SEC)<< endl;

		//--------------------------BIN PACKING STARTS(next fit decreasing)---------------------------------------

		for(int i = 0; i < num_periods; i++){

		
			sort(arr[num_rooms][i][final_tt].begin(), arr[num_rooms][i][final_tt].end(), [](string e1, string e2){
				unordered_map<string, int>::const_iterator cons_it1 = exam_cap.find(e1);
				int size1 = cons_it1 -> second;

				unordered_map<string, int>::const_iterator cons_it2 = exam_cap.find(e2);
				int size2 = cons_it2 -> second;

				return (size1 > size2);
			});


			for(vector<string>::iterator it = arr[num_rooms][i][final_tt].begin(); it != arr[num_rooms][i][final_tt].end(); ){

				string e = *it;
				bool removed = false;

				for(int j = 0 ; j < num_rooms; j++){
					int sum = 0;
					for(vector<string>::iterator it2 = arr[j][i][final_tt].begin(); it2 != arr[j][i][final_tt].end(); ++it2){
						unordered_map<string, int>::const_iterator cons_it = exam_cap.find(*it2);				
						sum += cons_it->second;
					}
					unordered_map<string, int>::const_iterator cons_it = exam_cap.find(e);				
					sum += cons_it->second;

					if(sum <= room_arr[j]){
						arr[j][i][final_tt].push_back(e);
						arr[num_rooms][i][final_tt].erase(it);
						removed = true;
						break;
					}
					
				}	

				if(!removed)
					it++;		
			}		
		}

		// int runsched = print_tt(final_tt);
		cout<<"tot_penalty: "<<tot_penalty(final_tt)<<endl;


		//----------------------FINSHED BIN PACKING------------------------------------------------------------
		//------------putting all the unscheduled one int the last period--------------------
	/*

		for(int i = 0; i < num_periods; i++){

			for(vector<string>::iterator it = arr[num_rooms][i][final_tt].begin(); it != arr[num_rooms][i][final_tt].end(); ){
				arr[0][num_periods][final_tt].push_back(*it);
				arr[num_rooms][i][final_tt].erase(it);
			}		
		}
	*/

		//--------------applying hill climbing on these uncheduled ones now------------------

		//cout<<"done till here"<<endl;

		local_search = 20;

		i = final_tt;
		//int p = 0;												//keep this penalty the same


		//int final_unsched = print_tt(i);


		while(local_search--){

			int crazy = 0;
			int forced = 0;
			int awesome = 0;

			int k = num_rooms;
			//int j = num_periods;
			int new_rear = 0;

			cout<<"---------------------------------------------LOCAL SEARCH "<<local_search<<endl;

			for(int j = 0 ; j <= num_periods ;j++){

				if(j == num_periods)
					k = 0;
				
				for(vector<string>::iterator it = arr[k][j][i].begin(); it != arr[k][j][i].end(); ){

					//cout<<"------------------------------"<<j<<"-----------------------------------\n";
					// cout<<*it<<endl;

		
					int p = 100000;
					int conflict_penalty = p;
					bool best = false;
					
					string e = *it;
					if (ex_pMap.find(e) == ex_pMap.end() && ex_rMap.find(e) == ex_rMap.end()){
						//int p = penalty(e, i, j);

			
						int new_period = -1;

						vector<string> exam_in_hand;
						string single_exam_in_hand;
						int period_in_hand = -1; 
						int single_period_in_hand = -1; 
						int least_conflicting_exams = 100000;


			
						for (int l = 0; l < num_periods; ++l){

							if(l != j){

								int new_r = legal_period(e, i, l);	//new_legal_per just tells whether its possible to place exam in that period and specify the room
								
								if(new_r != -1){
									
									int new_p = penalty(e, i, l);
									
									if(new_p < p){
										p = new_p;
										new_period = l;		
										new_rear++;			
									}
									
								}
								else{
									
									vector<string> con_exam = conflicting_exams(e, i, l);				//check the room thingie also
									int con_exam_size = con_exam.size();
									if(con_exam_size != 0 && con_exam_size < least_conflicting_exams){
										least_conflicting_exams = con_exam_size;
								 		exam_in_hand = con_exam;
								 		period_in_hand = l;	
								 		//best = true;								
									}
								}

								

								string con_exam;				//check the room thingie also

								vector<string> con_exams = conflicting_exams(e, i, l);				//check the room thingie also
								int con_exam_size = con_exams.size();									
								if(con_exam_size == 1){
									con_exam = con_exams[0];
									int cur_penalty = penalty(con_exam, i, l);
									int new_penalty = penalty(e, i, l);
								 	if(cur_penalty > new_penalty && new_penalty < conflict_penalty){
						
								 		conflict_penalty = new_penalty;
								 		single_exam_in_hand = con_exam;
								 		single_period_in_hand = l;
									}
								}
							


							}
						}

						bool e_deleted = false;
						bool best = false;


						// cout<<" in "<<endl;

						
						if(new_period != -1){	

							int new_r = legal_period(e, i, new_period);
							if(new_r != -1){
								arr[new_r][new_period][i].push_back(e);							
								arr[k][j][i].erase(it);	
								forced++;
								e_deleted = true;
								cout<<e <<" forced "<<new_period;
								// print_tt(i);						
							}
												
						}
						else if(period_in_hand != -1){

							// cout<<e<<" in swap "<<period_in_hand<<endl;
							swap(exam_in_hand, e, j, period_in_hand, i);

							int new_r = legal_period(e, i, period_in_hand);
							if(new_r != -1){
								arr[new_r][period_in_hand][i].push_back(e);
								arr[k][j][i].erase(it);
								e_deleted = true;	
								best = true;							
								cout<<"\n\n"<<e<<" unsched placed "<<period_in_hand;
								// print_tt(i);
								awesome++;
							}

							// if(! e_deleted)
							// 	it++;
						}


						if(!e_deleted && single_period_in_hand != -1){
														//bool e_deleted = false;						
							if(single_exam_in_hand != e){

								
								for(int r = 0; r < num_rooms+1; r++){
									for(vector<string>::iterator it_hand = arr[r][single_period_in_hand][i].begin(); it_hand != arr[r][single_period_in_hand][i].end(); ){
										string e_in_hand = *it_hand;
										if(e_in_hand == single_exam_in_hand)
											arr[r][single_period_in_hand][i].erase(it_hand);
										else it_hand++;
									}	
								}

								
								int new_r = legal_period(e, i, single_period_in_hand);
								if(new_r != -1){
									arr[new_r][single_period_in_hand][i].push_back(e);
									arr[0][num_periods][i].push_back(single_exam_in_hand);
									arr[k][j][i].erase(it);
									e_deleted = true;
									crazy++;
									cout<<"\n\n"<<e<<" placed in "<<single_period_in_hand<<" and "<<single_exam_in_hand<<" put in unscheduled\n";
									// print_tt(i);  
								}
							}

						}

						if(! e_deleted)
							it++;

						// cout<<" out "<<endl;


					}
				}				
			}	

			//cout<<"now what"<<endl;

			// int count_swaps = 0;
			// int normal_swap = 0;

			
			// print_tt(i);
			
			if(true){
							
				for(int p1 = 0; p1 < num_periods+1; p1++){
					for(int r1 = 0; r1 < num_rooms+1; r1++){
						size_arr[r1][p1][i] = arr[r1][p1][i].size();
					}
				}

				for(int p1 = 0; p1 < num_periods; p1++){
					for(int r1 = 0; r1 < num_rooms; r1++){
						int size1 = size_arr[r1][p1][i];
						for(vector<string>::iterator v1 = arr[r1][p1][i].begin(); v1 != arr[r1][p1][i].end() && size1--; ){	
							string e1 = *v1;
							arr[r1][p1][i].erase(v1);
							bool swap1 = false;

							for(int p2 = 0; p2 < num_periods && !swap1; p2++){
								if(p1 != p2){
									for(int r2 = 0; r2 < num_rooms && !swap1; r2++){
										int size2 = size_arr[r2][p2][i];
										for(vector<string>::iterator v2 = arr[r2][p2][i].begin(); v2 != arr[r2][p2][i].end() && size2-- && !swap1; ){
											string e2 = *v2;
											if(e1 == e2)
												continue;
											
											arr[r2][p2][i].erase(v2);
											bool swap2 = gen_swap(e1, e2, p1, p2, i, true);

											swap1 = swap1 || swap2;
											
											if(!swap2){
												arr[r2][p2][i].push_back(e2);
											}

										}
									}
								}
							}

							if(!swap1){
								arr[r1][p1][i].push_back(e1);
							}
						}
					}
				}
			}


			/*
			for(int p1 = 0; p1 < num_periods+1; p1++){
				for(int r1 = 0; r1 < num_rooms+1; r1++){	
					int size1 = arr[r1][p1][i].size();
					for(vector<string>::iterator v1 = arr[r1][p1][i].begin(); v1 != arr[r1][p1][i].end() && size1--; ){
						
						string e1 = *v1;
						arr[r1][p1][i].erase(v1);
						bool swap1 = false;
						for(int p2 = 0; p2 < num_periods+1; p2++){
							for(int r2 = 0; r2 < num_rooms+1; r2++){
								int size2 = arr[r2][p2][i].size();
								for(vector<string>::iterator v2 = arr[r2][p2][i].begin(); v2 != arr[r2][p2][i].end() && size2--; ){
	

									cout<<"in again"<<r2<<" "<<p2<<" "<<p1<<endl;
									string e2 = *v2;
									if(e1 == e2)
										continue;
									
									
									arr[r2][p2][i].erase(v2);
									
									

									bool swap2 = swap2_consec(e1, e2, p1, p2, i, false);
									swap1 = (swap1 || swap2);
									if(!swap2)
										arr[r2][p2][i].push_back(e2);
									else if(p1 == num_periods || p2 == num_periods) count_swaps++;
									else normal_swap++;
								
									
								}
							}
						}

						if(!swap1)
							arr[r1][p1][i].push_back(e1);
					}
				}
			}

			*/

			// cout<<"\ncount swaps: "<<count_swaps<<endl; 
			// cout<<"\nnormal swaps: "<<normal_swap<<endl; 
			//int changed_unsched = print_tt(i);

			// cout<<"\nforced: "<<forced;
			// cout<<"\ncrazy ulta pulta: "<<crazy;
			// cout<<"\nawesome:"<<awesome;
			// cout<<"\nmy awesome strategy unsched: "<<(final_unsched - changed_unsched)<<endl;

			int fin_unsched = arr[0][num_periods][i].size();
			int new_conflicts = tot_penalty(i) ;
			if(fin_unsched == 0 && new_conflicts == 0){															//perfect timetable found
				print_tt(i);
				long long int stop_si=clock();
				cout<<"conflicts: "<<new_conflicts<<endl;
				cout << "time: " << (stop_si-start_s)/double(CLOCKS_PER_SEC) << endl;
				exit(0);
			}
			// int pconflicts = tot_penalty(i);
			cout<<"conflicts: "<<new_conflicts<<endl;
		}

	
		int pconflicts = tot_penalty(final_tt);

		int punsched = print_tt(final_tt);
		if(best_unsched > punsched){
			best_unsched = punsched;
			best_conflicts = pconflicts;
		}
		if(best_unsched == punsched && best_conflicts > pconflicts){
			best_conflicts = pconflicts;
		}

		cout<<"tot_penalty: "<<pconflicts;
		
	}



	cout<<"\n\n\nbest unsched: "<<best_unsched;
	cout<<"\nbest conflicts: "<<best_conflicts; 
	

	long long int stop_s=clock();
	cout << "time: " << (stop_s-start_s)/double(CLOCKS_PER_SEC)<< endl;

}
