/*
PLEASE, DO NOT CHANGE void display(bool verbose, bool testing), int getUsableDay() and int getUsableTime() FUNCTIONS.
YOU HAVE TO WRITE THE REQUIRED  FUNCTIONS THAT IS MENTIONED ABOVE. YOU CAN ADD NEW FUNCTIONS IF YOU NEED.
*/

/* @Author
Student Name: Ahmet Gökçe
Student ID : 150180076
Date: 15.11.2019 */

#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <iomanip>

#include "task_management_tool.h"

using namespace std; 


void WorkPlan::display(bool verbose,bool testing)
{
	string inone="***";
	if (head!=NULL)
	{
		Task *pivot =new Task;
		Task *compeer =new Task;
			
		pivot=head;
		do
		{
			if(testing)
				inone+=" ";
			else
				cout << pivot->day <<". DAY"<< endl;
			compeer=pivot;
			while(compeer!=NULL)
			{
				string PREV= compeer->previous!=NULL?compeer->previous->name:"NULL";
				string NEXT=compeer->next!=NULL?compeer->next->name:"NULL";
				string CONT=compeer->counterpart!=NULL?compeer->counterpart->name:"NULL";
				if (testing)
					inone+=compeer->name;
				else
					if(verbose)
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< PREV<<"\t<- "<< compeer->name <<"("<< compeer->priority <<")->\t"<<NEXT <<"\t |_"<<CONT<<endl;
					else
						cout<<"\t"<<setw(2)<< compeer->time <<":00\t"<< compeer->name <<"("<< compeer->priority <<")"<<endl;				
				compeer=compeer->counterpart;
			}
			pivot=pivot->next;
		}while(pivot!=head);
		if(testing)
		{
			cout<<inone<<endl;
			cout<<"(checking cycled list:";
			if (checkCycledList())
				cout<<" PASS)"<<endl;
			else
				cout<<" FAIL)"<<endl;
		}
	}
	else
		cout<<"There is no task yet!"<<endl;
}

int WorkPlan::getUsableDay()
{
	return usable_day;
}

int WorkPlan::getUsableTime()
{
	return usable_time;
}


void WorkPlan::create()
{		
	head = NULL;
}

void WorkPlan::close()
{
	Task* temp1;
	Task* temp2;
	(head->previous)->next = NULL;
	while(head){
		temp1=head;
		head=head->next;
		temp2=temp1;
		while(temp2){
			temp1=temp1->counterpart;
			delete temp2;
			temp2 = temp1;
		}
	}
	return;
}

void WorkPlan::add(Task *task)
{
	Task* traverse, *behind, *addTask;
	traverse = head;
	
	addTask = new Task;
	addTask->name = new char[strlen(task->name)];
	strcpy(addTask->name,task->name);
	addTask->day = task->day;
	addTask->time = task->time;
	addTask->priority = task->priority;
	addTask->counterpart = NULL;
	
	bool isSameDay = false;
	
	if(head==NULL){ //if first task is being added
		head=addTask;
		head->previous=head;
		head->next=head;
	}
	
	Task* checker = getTask(addTask->day, addTask->time); //check whether the day and the time are available
	if(!checker){ //if yes
		
		if(addTask->day==head->day && addTask->time < head->time){ //add to head of list in same day
			addTask->counterpart = head;
			addTask->next = head->next;
			addTask->previous = head->previous;
			(head->next)->previous = addTask;
			head = addTask;
			(head->previous)->next =addTask;
			return;
		}		
		if(addTask->day < head->day){// add to head of list as a new day
			addTask->next = head;
			addTask->previous = head->previous;
			head->previous = addTask;
			(addTask->previous)->next = addTask;
			head = addTask;
			return;
		}
		
		traverse=head;
		do{
			if(addTask->day == traverse->day){
				isSameDay = true;
				break;
			}
			else if(addTask->day > traverse->day){
				traverse = traverse->next;
				continue;
			}
			else{
				break;
			}
		}while(traverse!=head);
		
		if(isSameDay){
			if(addTask->time < traverse->time){ // insert in between as a first task of day existed
				addTask->counterpart = traverse;
			
				addTask->next = traverse->next;
				addTask->previous = traverse->previous;
			
				(traverse->previous)->next = addTask;
				(traverse->next)->previous = addTask;
			
				traverse->next=NULL;
				traverse->previous=NULL;
				return;
			}
			else{
				while(traverse && addTask->time > traverse->time){
					behind = traverse;
					traverse = traverse->counterpart;
				}
				if(traverse){ //insert in between as a counterpart
					addTask->counterpart = traverse;
					behind->counterpart = addTask;
					addTask->next = NULL;
					addTask->previous = NULL;
					return;
				}
				else{ //insert to end of day
					behind->counterpart = addTask;
					addTask->counterpart = NULL;
					addTask->next = NULL;
					addTask->previous = NULL;
					return;
				}
			}
			
		}
		else{ //insert in 
			traverse = traverse->previous;
			addTask->next = traverse->next;
			addTask->previous = traverse;
			traverse->next = addTask;
			(addTask->next)->previous =addTask;
			addTask->counterpart=NULL;
			return;
		}
			
	}
	else{ // if there is a task where we want to add
		if(addTask->priority > checker->priority){
			checkAvailableNextTimesFor(checker);
			Task* temp = new Task;
			temp->name = new char[3];
			strcpy(temp->name,checker->name);
			temp->time=usable_time;
			temp->day=usable_day;
			temp->priority = checker->priority;
			add(temp);
			remove(checker);
			add(addTask);
			return;
		}
		else if(addTask->priority < checker->priority){
			checkAvailableNextTimesFor(addTask);
			addTask->day = usable_day;
			addTask->time = usable_time;
			add(addTask);
			return;
		}
	}

}

Task * WorkPlan::getTask(int day, int time)
{
	Task* traverse;
	traverse=head;
	
	do{
		if(traverse->day==day){
			if(traverse->time == time){
				return traverse;
			}
			else{
				traverse = traverse->counterpart;
			}
		}	
		else
			traverse= traverse->next;
	}while(traverse!=head && traverse!=NULL);
	return NULL;
}


void WorkPlan::checkAvailableNextTimesFor(Task *delayed)	
{	
	usable_day=0;
	usable_time=0;
	
	int day=delayed->day;
	int time=delayed->time;
	
	Task* toDelay = getTask(day,time);
	Task* traverse,*traverse2;
	
	if(!toDelay){
		return;
	}
	
	if(head->next == head){ //if there is one day
		if(toDelay==head){ // if we delay head
			if(head->counterpart){ //if head is not only task
				traverse2=head->counterpart;
				while(traverse2){
					if((time+1)<traverse2->time){
						usable_day = day;
						usable_time = time+1;
						return;
					}
					if((time+1)==traverse2->time){
						traverse2 = traverse2->counterpart;
						time+=1;
						continue;
					}
				}
			}
			else{ // if head is only task but nothing will be done about it
				return;
			}
		}
	}
	
	
	traverse = head;
	while(traverse->next!=head){ // we find the day of delayed and point to first task of it
		if(traverse->day == day){
			break;
		}
		else if(traverse->day < day){
			traverse = traverse->next;
		}
		else{ // if day does not exist
			return;
		}
	}
	traverse2 = toDelay->counterpart;
	
	while(traverse2){ // if we can delay the task in the same day
		if((time+1) < traverse2->time){
			usable_day = day;
			usable_time = time+1;
			return;
		}
		if((time+1)==traverse2->time){
			traverse2 = traverse2->counterpart;
			time+=1;
			continue;
		}	
	}
	
	if(traverse->next != head){ // if we delay the task in next days
		traverse = traverse->next;
		while(traverse != head){ 
			traverse2 = traverse->counterpart;
			time = 8;
			if(traverse->time != 8){ //
				usable_day = traverse->day;
				usable_time = 8;
				return;
			}
			while(traverse2){
				if((time+1) < traverse2->time){
					usable_day = traverse->day;
					usable_time = time+1;
					return;
				}
				if((time+1)==traverse2->time){
					traverse2 = traverse2->counterpart;
					time+=1;
					continue;
				}
			}
			traverse = traverse->next;
		}
	}
	
}

void WorkPlan::delayAllTasksOfDay(int day)
{
	Task* traverse,*traverse2,*lastTaskofDay,*toDelay;
	traverse=head;
	while(traverse->day!=day){
		traverse = traverse->next;
	}
	lastTaskofDay = traverse;
	while(lastTaskofDay->counterpart!=NULL){
		lastTaskofDay = lastTaskofDay->counterpart;
	}
	toDelay = new Task;
	traverse2 = traverse;
	
	while(traverse!=lastTaskofDay){ //delay tasks until one task remains or there ARE tasks
		traverse2 = traverse->counterpart;
		checkAvailableNextTimesFor(lastTaskofDay);
		toDelay->name = new char(strlen(traverse->name));
		strcpy(toDelay->name,traverse->name);
		toDelay->priority = traverse->priority;
		toDelay->time = usable_time;
		toDelay->day = usable_day;
		add(toDelay);
		remove(traverse);
		traverse = traverse2;
	}
	
	if(traverse==lastTaskofDay){
		checkAvailableNextTimesFor(lastTaskofDay);
		strcpy(toDelay->name,traverse->name);;
		toDelay->priority = traverse->priority;
		toDelay->time = usable_time;
		toDelay->day = usable_day;
		add(toDelay);
		remove(traverse);
	}
	
}

void WorkPlan::remove(Task *target) 
{
	Task* behind,*rmTarget,*traverse;
	rmTarget = getTask(target->day, target->time);
	traverse = head;
	if(rmTarget->day==head->day){ // if we delete task from first day
		if(rmTarget==head){ //delete head
			if(head->counterpart){ // if head is not only task at the day
				rmTarget->next = head->next;
				(head->next)->previous = head->counterpart;
				(head->previous)->next = head->counterpart;
				(head->counterpart)->previous = head->previous;
				head = head->counterpart;
				delete rmTarget;
				return;
			}
			else{ // if it is
				(head->next)->previous = head->previous;
				(head->previous)->next = head->next;
				head = head->next;
				if(head == head->next){ // head is only task at all
					delete rmTarget;
					head=NULL;
					return;
				}
				delete rmTarget;
				return;
			}
		}
		else{
			behind = traverse;
			while(rmTarget->time > traverse->time){
				traverse = traverse->counterpart;
			}
			if(!traverse){
				return;
			}
			if(rmTarget->time == traverse->time){
				while(behind->counterpart->time != rmTarget->time){
					behind = behind->counterpart;
				}
				if(traverse->counterpart==NULL){
					behind->counterpart = NULL;
					delete rmTarget;
					return;
				}
				else{
					behind->counterpart = rmTarget->counterpart;
					delete rmTarget;
					return;
				}
				behind->counterpart = rmTarget->counterpart;
				delete rmTarget;
				return;		
			}
			else{
				return;
			}
		}
	}
	
	if(rmTarget->next){//remove first task of day
		
		behind = rmTarget->previous;
		if(rmTarget->counterpart){ // if rmTarget is not only task at the day
			behind->next = rmTarget->counterpart;
			(rmTarget->next)->previous = rmTarget->counterpart;
			rmTarget->counterpart->next = rmTarget->next;
			rmTarget->counterpart->previous = rmTarget->previous;
			delete rmTarget;
			return;
		}
		else{ // if it is
			behind->next = rmTarget->next;
			(rmTarget->next)->previous = behind;
			delete rmTarget;
			return;
		}
	}
	else{ //remove from inside
		behind = head;
		while(behind->day != rmTarget->day){
			behind = behind->next;
		}
		while(behind->counterpart->time != rmTarget->time){
			behind = behind->counterpart;
		}
		behind->counterpart = rmTarget->counterpart;
		delete rmTarget;
		return;
	}
}

bool WorkPlan::checkCycledList()
{				
	Task *pivot=new Task();
	pivot=head;
	int patient=100;
	bool r=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->previous;
		if(pivot==head)
		{
			r=true;
			break;
		}
	}
	cout<<"("<<100-patient<<")";
	patient=100;
	bool l=false;
	while (pivot!=NULL&&patient>0)
	{		
		patient--;
		pivot=pivot->next;
		if(pivot==head)
		{
			l=true;
			break;
		}
	}
	return r&l;
}
