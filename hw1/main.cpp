/* @Author
Student Name: Ahmet Gökçe
Student ID: 150180076
Date: 19.10.2019 */

#include <iostream>
#include <stdio.h>


using namespace std;

struct node {
	int size;
	int quant;
	node *next;
};

struct stock {
	node* head;

	void create();
	void add_stock(int);
	void sell(int);
	void current_stock();
	void clear();
};

void stock::create() {
	head = NULL;
}

void stock::add_stock(int shoe_num) {
	node* add_shoe = new node;

	node* tail= new node;
	node* traver= new node;

	add_shoe->size = shoe_num;
	add_shoe->quant = 1;
	add_shoe->next = NULL;

	traver = head;

	if (head == NULL) {      //first node being added
		head = add_shoe;
		head->next = NULL;
		head->quant = 1;
	}
	else {

		if (add_shoe->size < head->size) {         //Insert to head of list
			add_shoe->next = head;
			head = add_shoe;
			return;
		}
		else if (add_shoe->size == head->size) {   //Add quantity to head of list
			head->quant += 1;
			return;
		}

		while (traver) {
			if (traver->size < add_shoe->size) {
				tail = traver;
				traver = traver->next;
			}
			else if (traver->size == add_shoe->size) {  //Add quantity to any position in list
				traver->quant += 1;
				return;
			}
			else {
				break;
			}
		}

		if (traver) {  //Insert into a position
			add_shoe->next = traver;
			tail->next = add_shoe;
		}
		else {        //Insert into the end
			tail->next = add_shoe;
		}
	}
}

void stock::sell(int shoe_num) {

	node* tail = new node;
	node* traver;

	bool isEqualAnynum = false;

	traver = head;
	
	if (head == NULL){ //if there is no stock and user tries to sell shoe
		cout << "NO_STOCK" << endl;
		return;
	}
	
	if (head->size == shoe_num) { //if first node is sold
		isEqualAnynum = true;
		head->quant -= 1;
		if (head->quant == 0) {
			head = head->next;
			delete traver;
			return;
		}		
	}
	 
	while(traver)
	{
		if ((traver->size == shoe_num)&&(traver!=head)) {  
			isEqualAnynum = true;
			traver->quant -= 1;
			if (traver->quant == 0) {
				tail->next = traver->next;
				delete traver;
				return;
			}
		}
		tail = traver;
		traver = traver->next;
	}
	if (!(isEqualAnynum))
		cout << "NO_STOCK" << endl;

}

void stock::current_stock() {
	node *temp;
	temp = head;
	while (temp != NULL)
	{
		cout << temp->size << ":" << temp->quant << endl;
		temp = temp->next;
	}
	delete temp;
}

void stock::clear() {
	node* temp;
	while (head) {
		temp = head;
		head = head->next;
		delete temp;
	}
}

int main(int argc,char* argv[]) {

	char* fileName = argv[1];
	
	stock* shoe_stock = NULL;
	shoe_stock = new stock;

	shoe_stock->create();

	FILE* input_file;
	input_file = fopen(fileName, "r");

	int x;
	while (!feof(input_file)) {
		fscanf(input_file, "%d", &x);

		if (x > 0) 
			shoe_stock->add_stock(x);
		
		else if (x == 0) 
			shoe_stock->current_stock();
		
		else 
			shoe_stock->sell(x*-1);
		

	}

	fclose(input_file);
	shoe_stock->clear();
	return 0;
}
