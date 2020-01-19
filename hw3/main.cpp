/* @Author
Student Name: Ahmet Gökçe
Student ID: 150180076
Date: 18.12.2019 */

#include <iostream>
#include <istream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <sstream>
using namespace std;

struct MobileHost{
    int MH_ID;
    void create(int);
    void addMH(int);
    MobileHost* mhNext; // each mh has a pointer to other mobile hosts which have same parent bs id
};

struct BaseStation{
    int BS_ID;

    void create(int);
    void addChildBS(int);
    void addLevelBS(int);
    void addMH(int);

    BaseStation* subNext; // it holds children basestations and can be assumed as left leaf
    BaseStation* levelNext; //it holds basestations which have same parent bs id and can be assumed as right leaf
    MobileHost* mhNext; // mobile hosts connected to basestation
};

struct CentralController{
    int CC_ID;
    BaseStation* root; //root is a bs added first and keeps other bs connecting to Central Controller by levelNext

    void create();
    void addBS(int);
    void close(BaseStation*);
};

void MobileHost::create(int mh_id) {
    MH_ID = mh_id;
    mhNext = NULL;
}

void MobileHost::addMH(int mh_id) {
    MobileHost* MH = new MobileHost;
    MH->create(mh_id);
    mhNext = MH;
}

void BaseStation::create(int bs_id) {
    BS_ID = bs_id;
    subNext = NULL;
    levelNext = NULL;
    mhNext = NULL;
}

void BaseStation::addChildBS(int bs_id) {
    BaseStation* BS = new BaseStation; //allocate memory for new bs
    BS->create(bs_id); // creating it
    if(!subNext){ // if "this" bs doesnt have any child bs
        subNext = BS;
    }
    else{ // if it has
        BaseStation* traveler = subNext;
        while(traveler->subNext){ //find the end of child basestations
            traveler = traveler->subNext;
        }
        traveler->subNext = BS;
    }
}

void BaseStation::addLevelBS(int bs_id) {
    BaseStation* BS = new BaseStation; //allocate memory for new bs
    BS->create(bs_id); // creating it
    if(!levelNext){ // if "this" bs doesnt have any level bs
        levelNext = BS;
    }
    else{ // if it has
        BaseStation* traveler = levelNext;
        while(traveler->levelNext){ //find the end of level basestations
            traveler = traveler->levelNext;
        }
        traveler->levelNext = BS;
    }
}

void BaseStation::addMH(int mh_id) {
    MobileHost* MH = new MobileHost; //allocate memory for new mh
    MH->create(mh_id);
    if(!mhNext){ // if "this" bs doesnt have mh
        mhNext = MH;
    }
    else{// if it has
        MobileHost* traveler = mhNext;
        while(traveler->mhNext){ //find the end of the mobilehosts
            traveler = traveler->mhNext;
        }
        traveler->mhNext = MH;
    }
}

void CentralController::create() { //creating Central Conroller
    CC_ID = 0;
    root = NULL;
}

void CentralController::addBS(int bs_id) {
    BaseStation* BS = new BaseStation;
    BS->create(bs_id);
    BaseStation* traveler;
    if(!root){ // if central controller does not have any basestation
        root = BS;
        return;
    }
    else{ //if it has
        traveler = root;
        while (traveler->levelNext){ //find the end of level basestations
            traveler = traveler->levelNext;
        }
        traveler->levelNext = BS;
    }
}

void CentralController::close(BaseStation* bs) { //delete network with POSTORDER travel
    if (bs) {
        if (bs->subNext != NULL){
            close(bs->subNext);
            bs->subNext = NULL;
        }
        if (bs->levelNext != NULL){
            close(bs->levelNext);
            bs->levelNext = NULL;
        }
        delete bs;
    }
}

BaseStation* searchBS(BaseStation* root, int id){ //search to find base station with INORDER travel
    if(root == NULL){ //if basestation is empty
        return NULL;
    }
    if(root->BS_ID == id){ // if basestation which has id is found
        return root;
    }
    BaseStation* bs;
    bs = searchBS(root->subNext,id); //go into child base stations and search here
    if(bs){ //if bs is found
        return bs;
    }//if no, continue
    bs = searchBS(root->levelNext,id); //go into level base stations and search here
    if(bs){ //if bs is found
        return bs;
    }//if no, base station with id does not exist
    return NULL;
}

MobileHost* searchMHinBS(MobileHost* mh, int id) { //search to find mobile host with INORDER travel
    if(mh == NULL){ //if mh is empty
        return NULL;
    }
    if(mh->MH_ID == id){ // if MobileHost which has id is found
        return mh;
    }
    MobileHost* mh_temp = searchMHinBS(mh->mhNext, id); // travel till the end of mobile hosts
    if(mh_temp){ //if mh is found
        return mh_temp;
    }
    return NULL; //if no
}

bool searchMH(BaseStation* root, int id, int* parent_id){
    if(root == NULL){ //if root is empty
        return false;
    }
    MobileHost* mh; //temp mobile host
    bool isExist;
    cout<<root->BS_ID<<" "; //printing BS_ID to show where we are
    mh = searchMHinBS(root->mhNext,id); // search in mobile host of root
    if(mh){ //if mh founded
        *parent_id = root->BS_ID; //write parent id of root to parent_id to use it
        return true;
    }
    isExist = searchMH(root->subNext, id, parent_id); // search in child base stations
    if(isExist){//if is found, no need to check level base stations and return true
        return true;
    }
    isExist = searchMH(root->levelNext, id, parent_id); // search in level base stations
    return isExist;
}

void pathFinder(BaseStation* bs, BaseStation* traveler){
    if(traveler == bs){ //if travel equals bs at first
        cout<<traveler->BS_ID<<" ";
    }
    while(traveler!=bs){
        if(searchBS(traveler->subNext,bs->BS_ID)){ //if bs is found in children of traveler print traveler id
            cout<<traveler->BS_ID<<" ";
            traveler = traveler->subNext;
        }
        if(searchBS(traveler->levelNext,bs->BS_ID)){
            traveler = traveler->levelNext;
        }
        if(traveler==bs){ // at the end we found parent of traveler and print it
            cout<<traveler->BS_ID<<" ";
        }
    }
}

struct MessageNode{
    string Message;
    int Des;

    void create(string,int);
    MessageNode* next;
};

struct MessageQueue{
    MessageNode* first;
    MessageNode* last;

    void create();
    void Enqueue(string, int); //add new node
    MessageNode Dequeue();//return first element to use
    bool isEmpty();
    void close();
};

void MessageNode::create(string message, int des) {
    Message = message;
    Des = des;
    next = NULL;

}

void MessageQueue::create() {
    first= NULL;
    last = NULL;
}

void MessageQueue::Enqueue(string message, int id) {
    MessageNode* MD = new MessageNode;
    MD->create(message,id);
    if(isEmpty()){ // if adding first element
        last = MD;
        first= last;
    }
    else{ //else add to end which is *last
        last->next = MD;
        last = MD;
    }
}

void MessageQueue::close(){
    MessageNode* msg;
    while (first){
        msg = first;
        first = first->next;
        delete msg;
    }
}

MessageNode MessageQueue::Dequeue() {
    MessageNode* topnode;
    MessageNode temp;
    topnode = first;
    if(first){
        first = first->next;
    }
    temp.Des = topnode->Des;
    temp.Message = topnode->Message;
    delete topnode;
    return temp;
}

bool MessageQueue::isEmpty() {
    if(!first){ //if first is not null, queue is not empty
        return true;
    }
    return false;
}

int main(int argc, char* argv[]) {
    CentralController CC;
    MessageQueue MQ;
    //string NetworkFilePath = argv[1];
    //string MessagesFilePath = argv[2];
    string NetworkFilePath = "Network.txt";
    string MessagesFilePath = "Messages.txt";
    ifstream NetworkFile;
    NetworkFile.open(NetworkFilePath);
    if(!NetworkFile){
        cout<<"Network file cannot be found";
    }

    CC.create();
    MQ.create();
    while(!NetworkFile.eof()){
        string line;
        string type, self_id, parent_id;
        getline(NetworkFile,line); // read line from text
        if(line == ""){
            break;
        }
        stringstream X(line);                    // seperate it to
        getline(X, type, ' ');    // type
        getline(X, self_id, ' '); // self id
        getline(X, parent_id, '\n'); // parent id
        int self_ID = stoi(self_id); //string to int
        int parent_ID = stoi(parent_id); //string to int
        if(type == "BS" && parent_ID == 0){ // add basestation to centralcontroller
            CC.addBS(self_ID);
            continue;
        }
        if(type == "BS" && parent_ID != 0){ // add basestation to another basestation
            BaseStation* parent = searchBS(CC.root,parent_ID);
            if(!parent->subNext){ //if parent does not have any child, add to parent as a child bs
                parent->addChildBS(self_ID);
            }
            else{ //else add to first child as a level bs
                parent->subNext->addLevelBS(self_ID);
            }
            continue;
        }
        if(type == "MH" && parent_ID != 0){ // add mobilehost to basestation
            BaseStation* parent = searchBS(CC.root,parent_ID); //find base station which has parent id
            if(parent){
                parent->addMH(self_ID);
            }
            continue;
        }
        if (type == "MH" && parent_ID == 0) // prevent adding mobilehost to centralcontroller
            continue;
    }
    NetworkFile.close();

    ifstream MessagesFile;
    MessagesFile.open(MessagesFilePath);
    if(!MessagesFile){
        cout<<"Messages file cannot be found";
    }
    while(!MessagesFile.eof()){
        string line;
        getline(MessagesFile,line); // read line from text
        if(line == ""){
            break;
        }
        string message,mh_id;
        stringstream X(line);                     // seperate it to
        getline(X,message,'>');     // message to deliver
        getline(X,mh_id,'\n');       //mobile host to deliver message
        int MH_ID = stoi(mh_id); //string to int
        MQ.Enqueue(message,MH_ID); //add message to queue
    }
    MessagesFile.close();

    while (!MQ.isEmpty()){ // read until queue is being empty
        MessageNode  md = MQ.Dequeue();
        int parent_id = 0;
        cout<<"Traversing:0 ";bool isExist = searchMH(CC.root,md.Des, &parent_id);cout<<endl; //print base stations travelled
        BaseStation* bs = searchBS(CC.root, parent_id);
        BaseStation* traveler = CC.root;

        if(isExist){ // if destination mobile host is found
            cout<<"Message:"<<md.Message<<" To:0 ";pathFinder(bs,traveler);cout<<"mh_"<<md.Des<<endl;
        }

        else{ //if no
            cout<<"Can not be reached the mobile host mh_"<<md.Des<<" at the moment"<<endl;
        }
    }
    MQ.close();
    CC.close(CC.root);
    return  0;
}