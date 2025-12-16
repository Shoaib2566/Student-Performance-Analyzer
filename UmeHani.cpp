#include<iostream>
#include<map>
#include<string>
#include<algorithm>

using namespace std;

//Assuming these are already declared
struct Score{
	int score;
	Score* next;
};

class Subject{
	public:
		Score* head;
};

class Student{
	public:
		map<string, Subject> subjects;
		string getName();
};

//the calculation part 
class Calculation{
	public:
		
		//using static to avoid extra parameters and independency 
		//linked list for avg of sub
		static double Average(Subject &sub){
			if (sub.head == NULL) return 0;
			
			int sum = 0;
			int count = 0;
			Score* temp = sub.head;
			
			while(temp){
				sum += temp -> score;
				count++;
				temp = temp -> next;
			}
			return (double)sum/count;
		}
		
		//hash map for avg of students
		static double StudentAverage(Student & stud){
			double sum = 0;
			int count = 0;
			
			for(auto &p : stud.subjects){
				sum += Average(p.second);
				count++;
			}
			if (count == 0) return 0;
			
			return sum / count;
		}
		
		//func for class avg
		static double ClassAverage(Student arr[], int count){
			if (count == 0) return 0;
			
			double sum = 0;
			
			for(int i = 0; i < count; i++){
				sum += StudentAverage(arr[i]);
			}
			
			double avg = sum/count;
			
			return avg;
		}
		
		//func for highest and lowest performer using linear search
		static void Performance(Student arr[], int count){
			int perf = 0;
			
			//highest
			for(int i = 1; i <count; i++){
				if(StudentAverage(arr[i]) > StudentAverage(arr[perf]))
				perf = i;
			}
			cout <<"Highest Performer: "<< perf <<endl;
			
			//lowest
			for(int i = 1; i <count; i++){
				if(StudentAverage(arr[i]) < StudentAverage(arr[perf]))
				perf = i;
			}
			cout <<"Lowest Performer: "<< perf <<endl;	
		}
		
		//ranking using selection sort
		static void StudentRanks(Student arr[], int count){
			for(int i = 0; i < count -1; i++){
				int num = i;
				for(int j = i+1; j< count; j++){
					if(StudentAverage(arr[j])>StudentAverage(arr[num])){
						num = j;
					}
				}
				
				if(num != i) swap(arr[i], arr[num]);
				
			}
		}
		
		static void Display(Student arr[], int count){
			StudentRanks(arr, count);
			
			cout<<"\t\t\tCLASS RANK\n";
			for(int i = 0; i< count; i++){
				cout<< i+1 <<". "<<arr[i].getName()<<" Average: "<<StudentAverage(arr[i]) <<endl;
			}
		}
		
};