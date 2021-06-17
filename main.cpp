#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>
#include<ctime>

using namespace std;

//SOME GLOBAL VARIABLES
int *pre_occupancies; //array holding the previous occupancies
int return_flight_index=0; // the size of return array for CHECKNADDRETURNROUTES function



/*  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A
 *************************************************************************************************************
    PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A  PART A
 *************************************************************************************************************/

class Graph {
    public:
	int numVertex, numEdge;
	int **matrix;
	string *city;
	public:
	Graph();
	~Graph();
	void ADDGCITY(int v, string city_name);
	void REMOVEGCITY(string city_name);
	int SEARCHGCITY(string city_name);
	void ADDGROUTE(string city1, string city2);
	void REMOVEGROUTE(string city1, string city2);
	int SEARCHGROUTE(string city1, string city2);
	void LISTGROUTES() ;
	int* all_routes();
};

// Constructor
Graph::Graph(){
	int i;
	numVertex = 20;
	numEdge = 0;
	city = new string[numVertex]; // Initialize city array
	for (i=0; i < numVertex; i++)
	city[i] = "";
	//Size is numVertex*numVertex
	matrix = new int*[numVertex];
	for(i=0; i < numVertex; i++)
	matrix[i] = new int[numVertex];
	for(i=0; i < numVertex; i++) // Initialize to 0
	for (int j=0; j < numVertex; j++)
	matrix[i][j] = 0;
}

// Destructor, Return dynamically allocated memory
Graph::~Graph(){
	delete[] city;
	for (int i=0; i < numVertex; i++)
	delete[] matrix[i];
	delete [] matrix;
}

void Graph::ADDGCITY(int v, string city_name){
    if(city[v]==""){
        city[v]=city_name;
    }
    else
        cerr<<"Corresponding location is not empty!"<<endl;
}

void Graph::REMOVEGCITY(string city_name){
    int v1=SEARCHGCITY(city_name);
    for (int i=0;i<numVertex;i++){
        if (city[i]==city_name){
            city[i]=""; //make it null i.e. remove
        }
    }
    //clear corresponding column and row.
    for (int i=0;i<numVertex;i++){
        matrix[v1][i] = 0;
        matrix[i][v1] = 0;
    }
}

int Graph::SEARCHGCITY(string city_name){
    int number=999; //default value, if not found
    for (int i=0;i<numVertex;i++){
        if (city[i]==city_name){
            number=i;
        }
    }
    return number;
}

void Graph::ADDGROUTE(string city1, string city2){
    int v1= SEARCHGCITY(city1);
    int v2= SEARCHGCITY(city2);
    int route_fare=rand() % 300 + 1; //random number between 1-300
    if (matrix[v1][v2]==0)
        numEdge++;
    if(v1!=999 && v2!=999) //check if cities are valid
        matrix[v1][v2] = route_fare;
}

void Graph::REMOVEGROUTE(string city1, string city2){
    int v1= SEARCHGCITY(city1);
    int v2= SEARCHGCITY(city2);
    if(matrix[v1][v2] != 0)
        numEdge--;
    matrix[v1][v2]=0;
}

int Graph::SEARCHGROUTE(string city1, string city2){
    int v1= SEARCHGCITY(city1);
    int v2= SEARCHGCITY(city2);
    if (v1==999 || v2==999 || matrix[v1][v2]==0){
        return 999; //meaning that no such route
    }
    return matrix[v1][v2];
}

void Graph::LISTGROUTES() {
    cout<<"Routes:"<<endl;
    for (int i=0;i<numVertex;i++){
        for (int j=0; j<numVertex;j++){
                if(matrix[i][j]>0) {
                    cout<<"("<<city[i]<<"---> "<<city[j]<<", "<<SEARCHGROUTE(city[i],city[j])<<')'<<endl;
                }
        }
    }
}

//special method return all flight numbers as an array
int* Graph::all_routes() {
    int* routes;
    routes = new int[numEdge];
    int index=0;
    for (int i=0;i<numVertex;i++){
        for (int j=0; j<numVertex;j++){
                if(matrix[i][j]>0) {
                    int f_number = 100*(i+1) + (j+1);
                    routes[index]= f_number;
                    index++;
                }
        }
    }
    return routes;
}

//function to initialize as expected, reading from .txt file
//and using ADDGCITY method to generate city array and
//ADDGROUTE method to initialize routes.
void INITROUTEMAP (Graph &obj){
    string line;
    ifstream myfile ("citylist.txt");
    if (myfile.is_open())
    {
    int i=0;
    while (getline(myfile,line))
    {
    obj.ADDGCITY(i,line);
    i++;
    }
    myfile.close();
    }
    else cout << "Unable to open file";

    obj.ADDGROUTE("Probia","Las Datas");
    obj.ADDGROUTE("Signale","Probia");
    obj.ADDGROUTE("Probia","Gulf of Archi");
    obj.ADDGROUTE("Gulf of Archi","Logicheim");
    obj.ADDGROUTE("Logicheim","Probia");
    obj.ADDGROUTE("Probia","Logicheim");
}


/*  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B
 *************************************************************************************************************
    PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B  PART B
 *************************************************************************************************************/
class NodeBST
{
private:
    int OCCUPANCYRATE;
public:
    int data;
    NodeBST* left;
    NodeBST* right;
public:
    NodeBST();
    NodeBST* ADDTFLIGHT(NodeBST* node, int flight);
    NodeBST* REMOVETFLIGHT(NodeBST* node, int flight);
    NodeBST* SEARCHTFLIGHT(NodeBST* node, int flight) ;
    void     SETOCCUPANCYRATE();
    int      GETOCCUPANCYRATE();

};

NodeBST::NodeBST():data(0),left(NULL),right(NULL){}

NodeBST* NodeBST::ADDTFLIGHT(NodeBST* node, int flight){
    if(node == NULL){
        node = new NodeBST;
        node->data=flight;
        node->left= NULL;
        node->right=NULL;
        node->SETOCCUPANCYRATE(); // SET OCC_RATE FOR EACH ADDED flight
    }
    else if(node->data < flight){
        node->right = ADDTFLIGHT(node->right, flight);
    }
    else{
        node->left= ADDTFLIGHT(node->left, flight);
    }
    return node;
}

NodeBST* NodeBST::REMOVETFLIGHT(NodeBST* node, int flight){
    if (node==NULL)
        return node;
    if (flight < node->data)
        node->left=REMOVETFLIGHT(node->left,flight);
    else if(flight > node->data)
        node->right=REMOVETFLIGHT(node->right, flight);

    else {
        if (node->left==NULL){
            NodeBST* temp= node ->right;
            free(node);
            return temp;
        }
        else if(node->right==NULL) {
            NodeBST* temp= node->left;
            free(node);
            return temp;
        }

    NodeBST* minVal = node->right;
    while(minVal && minVal->left !=NULL)
        minVal = minVal->left;

    node->data =minVal->data;

    node->right = REMOVETFLIGHT(node->right, minVal->data);
    }
    return node;
}

NodeBST* NodeBST::SEARCHTFLIGHT(NodeBST* node, int flight) {
    if (node==NULL || node->data == flight)
        return node;
    if (node->data <flight)
        return SEARCHTFLIGHT(node->right, flight);

    return SEARCHTFLIGHT(node->left, flight);
}

//a function to initialize BST accordingly,
//and I also initially set occupancies in this function
//just for once
NodeBST* INITFLIGHTPLAN (NodeBST &obj, Graph &obj1) {
    NodeBST* root = NULL; //set root initially as NULL.
    for (int i=0;i<20;i++){
        for (int j=0; j<20;j++){
            if (obj1.matrix[i][j]>0){
                int temp = 100*(i+1) + (j+1);
                root = obj.ADDTFLIGHT(root,temp);
                //obj.SEARCHTFLIGHT(root,temp)->SETOCCUPANCYRATE(); // I used this before now I implement it in ADDTFLIGHT
                //cout<<obj.SEARCHTFLIGHT(root,temp)->GETOCCUPANCYRATE()<<endl; //DEBUG, SEE OCCUPANCIES
            }
        }
    }
    return root; //return root so that later we can use it in int main
}

//special function to print flight number by doing inorder traversal on BST
void display_flights(NodeBST* root){
    if (root != NULL) {
        display_flights(root->left);
        if(root->data <1000)
            cout<< "EE0"<<root->data<<endl;
        else
            cout<< "EE"<<root->data<<endl;
        display_flights(root->right);
    }
}


/*  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C
 *************************************************************************************************************
    PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C  PART C
 *************************************************************************************************************/

int* ADDMONTHLYNEWROUTES (int N, Graph &obj){
    cout<<"Monthly New Route Offers"<<endl;
    int *flight_numbers;
    flight_numbers = new int[N]; //create a dynamic array for new flights
    while(N>0){
        int i=rand() % 19 ; //random number between 0-19
        int j=rand() % 19 ; //random number between 0-19
        string city1=obj.city[i]; //find corresponding city
        string city2=obj.city[j]; //find corresponding city
        if (city1 != "" && city2 != "") {
            if(obj.matrix[i][j]==0){
                obj.ADDGROUTE(city1,city2); //if conditions satisfied add new route
                cout<<"("<<city1<<"---> "<<city2<<", "<<obj.SEARCHGROUTE(city1,city2)<<')'<<" Route Added"<<endl;
                flight_numbers[N-1] = 100*(i+1) + (j+1);
                //cout<<flight_numbers[N-1]<<endl; //DEBUG, OBSERVE THE ARRAY
                N--;
            }
        }
    }
    return flight_numbers;
}

void ADDMONTHLYNEWFLIGHTS (NodeBST &obj, int *p,int N, NodeBST* root) {
    cout<<"Monthly New Flight Offers"<<endl;
    //NodeBST* root = NULL;
    for (int i=0;i<N;i++){
            obj.ADDTFLIGHT(root,p[i]);
            //cout<<p[i]<<endl;
            if(p[i]<1000)
                cout<< "Flight EE0"<<p[i]<<" Added"<<endl;
            else
                cout<< "Flight EE"<<p[i]<<" Added"<<endl;
    }
}

/*  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D
 *************************************************************************************************************
    PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D  PART D
 *************************************************************************************************************/

void NodeBST::SETOCCUPANCYRATE() {
    OCCUPANCYRATE= rand() % 100 + 1;
}

int NodeBST::GETOCCUPANCYRATE() {
    return OCCUPANCYRATE;
}

//special function to add previous occupancies of the flights
//to global array in-order so that we can later compare them with
//new occupancies.
void add_to_preocc(NodeBST* root, NodeBST &obj, Graph &obj1){
    pre_occupancies= new int[obj1.numEdge]; //allocate memory for global array
    for (int i =0; i<obj1.numEdge; i++){
        pre_occupancies[i]=obj.SEARCHTFLIGHT(root,(obj1.all_routes())[i])->GETOCCUPANCYRATE();
    }
}

//special function to update all occupancies
void update_occs(NodeBST* root){
    if (root != NULL) {
        update_occs(root->left);
            root->SETOCCUPANCYRATE();
        update_occs(root->right);
    }
}

//I use array whose size is 50 and divide it into two part where first 25 index
//is for canceled flight numbers and remaining part is for unpopular ones.
int* CANCELUNPOPULARFLIGHTS(NodeBST &obj, NodeBST* root, int OT, Graph obj1){
    int* unpopandcanceled= new int[50]{0};
    int* all = obj1.all_routes();
    update_occs(root); //update occupancies since you have previous ones in global array.
    int new_occ;
    int index_canceled=0;
    int index_unpop=25;
    for (int i=0; i<obj1.numEdge; i++){
        if (pre_occupancies[i]< OT){
            new_occ= obj.SEARCHTFLIGHT(root,all[i])->GETOCCUPANCYRATE();
            if (new_occ< OT){
                unpopandcanceled[index_canceled]=all[i];
                index_canceled++;
            }
            else{
                unpopandcanceled[index_unpop]=all[i];
                index_unpop++;
            }
        }
    }
    add_to_preocc(root,obj,obj1);
    return unpopandcanceled;
}

void DISCARDUNPOPULARROUTESANDUPDATEPRICES(int* unpopandcanceled, Graph &obj1){
    int temp;
    int first;
    int last;
    int price;
    string city1;
    string city2;

    cout<<"Cancelled Flights"<<endl;

    for (int i=0;i<25;i++){
        if (unpopandcanceled[i] != 0){
            temp=unpopandcanceled[i];
            first = temp/100;
            last= temp- (first*100);
            city1=obj1.city[first-1];
            city2=obj1.city[last-1];
            if(unpopandcanceled[i]<1000)
                cout<< "Flight EE0"<<unpopandcanceled[i]<<" Cancelled"<<endl;
            else
                cout<< "Flight EE"<<unpopandcanceled[i]<<" Cancelled"<<endl;
            obj1.REMOVEGROUTE(city1,city2);
        }
    }
    cout<<endl;
    cout<<"Discarded Routes & Routes on Discount"<<endl;
    for (int i=25;i<50;i++){
        if (unpopandcanceled[i] != 0){
            temp=unpopandcanceled[i];
            first = temp/100;
            last= temp- (first*100);
            price=obj1.matrix[first-1][last-1];
            price=price/2;
            obj1.matrix[first-1][last-1] = price;
            cout<<"("<<obj1.city[first-1]<<"---> "<<obj1.city[last-1]<<", "<<obj1.SEARCHGROUTE(obj1.city[first-1],obj1.city[last-1])<<')'<<" is 50% Off"<<endl;
        }
    }
}




/*  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E
 *************************************************************************************************************
    PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E  PART E
 *************************************************************************************************************/

 //initially array is created with size of edge numbers,
 //fill the dynamic array with flight numbers and if there
 //is remaining empty index fill it with 0. If one want to see
 //this returning array --> note that initially there are 6 routes
 // then we add 5 routes to it before we use CHECKNADDRETURNROUTES
 //function. So initially numEdge=11, goto main part and uncomment
 // "DEBUG, SEE ADDED RETURN FLIGHT NUMBERS" part and change
 // return_flight_index with 11 (you may observe 0s if there are).

int* CHECKNADDRETURNROUTES(Graph &obj1){
    cout<<"Missing Return Routes"<<endl;
    int* new_flights;
    new_flights = new int[obj1.numEdge]{0};//return flights must be smaller than edge number.
    int index=0;
    for (int i=0;i<obj1.numVertex;i++){
        for (int j=0; j<obj1.numVertex;j++){
            if(obj1.matrix[i][j]!=0 && obj1.matrix[j][i]==0) {
                new_flights[index] = 100*(j+1) + (i+1);
                index=index + 1;
                string city_1=obj1.city[j];
                string city_2=obj1.city[i];
                obj1.ADDGROUTE(city_1,city_2);
                cout<<"("<<city_1<<"---> "<<city_2<<", "<<obj1.SEARCHGROUTE(city_1,city_2)<<')'<<" Return Route Added"<<endl;
            }
        }
    }
    return_flight_index=index; //update global variable
    return new_flights;
}

void ADDRETURNFLIGHTS(NodeBST &obj, NodeBST* root, int *return_flights){
    cout<<"Missing Return Flights"<<endl;
    for (int i=0; i<return_flight_index; i++){
        obj.ADDTFLIGHT(root,return_flights[i]);
        if(return_flights[i]<1000)
            cout<< "Flight EE0"<<return_flights[i]<<" Added"<<endl;
        else
            cout<< "Flight EE"<<return_flights[i]<<" Added"<<endl;
    }
}



/*  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F
 *************************************************************************************************************
    PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F  PART F
 *************************************************************************************************************/

void DISPLAYROUTEOPTIONS(Graph &obj1, NodeBST* root, string dep_city, string arr_city){
    int v1= obj1.SEARCHGCITY(dep_city);
    int v2= obj1.SEARCHGCITY(arr_city);
    string stop_city1;
    string stop_city2;
    int direct=0;
    int onestop=0;
    int twostop=0;
    int temp;

    //check direct trip
    if (obj1.SEARCHGROUTE(dep_city, arr_city) != 999) {
        direct++;
        cout<<"Direct Trip:";
        temp=100*(v1+1) + (v2+1);
        if(temp<1000)
            cout<<dep_city<<" -> EE0"<<temp<<"-> "<<arr_city<<", Fee: "<<obj1.SEARCHGROUTE(dep_city, arr_city)<<endl;
        else
            cout<<dep_city<<" -> EE"<<temp<<"-> "<<arr_city<<", Fee: "<<obj1.SEARCHGROUTE(dep_city, arr_city)<<endl;
    }

    //check 1-stop trip
    for (int j=0;j<obj1.numVertex;j++){
        if (obj1.matrix[v1][j] != 0){
            stop_city1= obj1.city[j];
            if(obj1.matrix[j][v2] != 0){
                onestop++;
                cout<<"1-stop Trip:";
                temp=100*(v1+1) + (j+1);
                if(temp<1000)
                    cout<<dep_city<<" -> EE0"<<temp<<"-> "<<stop_city1;
                else
                    cout<<dep_city<<" -> EE"<<temp<<"-> "<<stop_city1;

                temp=100*(j+1) + (v2+1);
                if(temp<1000)
                    cout<<" -> EE0"<<temp<<"-> "<<arr_city<<", Fee: "<<obj1.SEARCHGROUTE(dep_city, stop_city1)<<"+"<<obj1.SEARCHGROUTE(stop_city1, arr_city)<<endl;
                else
                    cout<<" -> EE"<<temp<<"-> "<<arr_city<<", Fee: "<<obj1.SEARCHGROUTE(dep_city, stop_city1)<<"+"<<obj1.SEARCHGROUTE(stop_city1, arr_city)<<endl;
            }
        }
    }

    //check 2-stop trip
    for (int j=0;j<obj1.numVertex;j++){
        if(obj1.matrix[v1][j] != 0){
            stop_city1= obj1.city[j];
            for (int jnew=0; jnew<obj1.numVertex; jnew++){
                if(obj1.matrix[j][jnew] !=0){
                    stop_city2= obj1.city[jnew];
                    if(obj1.matrix[jnew][v2] != 0){
                        twostop++;
                        cout<<"2-stop Trip:";
                        temp = 100*(v1+1) + (j+1);
                        if(temp<1000)
                            cout<<dep_city<<" -> EE0"<<temp<<"-> "<<stop_city1;
                        else
                            cout<<dep_city<<" -> EE"<<temp<<"-> "<<stop_city1;

                        temp = 100*(j+1) + (jnew+1);
                        if(temp<1000)
                            cout<<" -> EE0"<<temp<<"-> "<<stop_city2;
                        else
                            cout<<" -> EE"<<temp<<"-> "<<stop_city2;

                        temp = 100*(jnew+1) + (v2+1);
                        if(temp<1000){
                            cout<<" -> EE0"<<temp<<"-> "<<arr_city<<", Fee: "<<obj1.SEARCHGROUTE(dep_city, stop_city1)<<"+"<<obj1.SEARCHGROUTE(stop_city1, stop_city2)<<"+";
                            cout<<obj1.SEARCHGROUTE(stop_city2, arr_city)<<endl;
                        }
                        else{
                            cout<<" -> EE"<<temp<<"-> "<<arr_city<<", Fee: "<<obj1.SEARCHGROUTE(dep_city, stop_city1)<<"+"<<obj1.SEARCHGROUTE(stop_city1, stop_city2)<<"+";
                            cout<<obj1.SEARCHGROUTE(stop_city2, arr_city)<<endl;
                        }
                    }
                }
            }
        }
    }

    if (direct==0 && onestop==0 && twostop==0){
        cout<<"No Trip Options"<<endl;
    }
}






/*  MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN
 *************************************************************************************************************
    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN    MAIN
 *************************************************************************************************************/

int main()
{
    srand(time(0));
    NodeBST objNode;
    Graph objGraph;
    INITROUTEMAP(objGraph);
    int N=5 ; //USER INPUT FOR NEW ROUTES
    int OT=70;

    cout<<"----------------Month 1------------------:"<<endl;

    objGraph.LISTGROUTES();
    cout<<endl;
    NodeBST* root = INITFLIGHTPLAN(objNode, objGraph);
    cout<<"Flights: "<<endl;
    display_flights(root);

    cout<<"----------------Month 2------------------:"<<endl;

    //add_to_preocc(root,objNode,objGraph); //update global array
    cout<<endl;

    int *p = ADDMONTHLYNEWROUTES(N, objGraph);
    cout<<endl;
    ADDMONTHLYNEWFLIGHTS(objNode,p,N,root);
    cout<<endl;
    objGraph.LISTGROUTES();
    cout<<endl;
    cout<<"Flights: "<<endl;
    display_flights(root);
    cout<<endl;

    cout<<"----------------Month 3------------------:"<<endl;

   // int *canc_and_unpop=CANCELUNPOPULARFLIGHTS(objNode,root,OT,objGraph);
    cout<<endl;
    /*for (int i=0;i<50;i++){
        cout<<canc_and_unpop[i]<<endl;
    }*/ // DEBUG, SEE THE CORRESPONDING ARRAY
    //DISCARDUNPOPULARROUTESANDUPDATEPRICES(canc_and_unpop,objGraph);
    cout<<endl;
    p = ADDMONTHLYNEWROUTES(N, objGraph);
    cout<<endl;
    ADDMONTHLYNEWFLIGHTS(objNode,p,N,root);
    cout<<endl;

    objGraph.LISTGROUTES();
    cout<<endl;
    cout<<"Flights: "<<endl;
    display_flights(root);
    cout<<endl;

    cout<<"----------------Month 4------------------:"<<endl;
    //canc_and_unpop=CANCELUNPOPULARFLIGHTS(objNode,root,OT,objGraph);
    cout<<endl;
    //DISCARDUNPOPULARROUTESANDUPDATEPRICES(canc_and_unpop,objGraph);
    cout<<endl;
    p = ADDMONTHLYNEWROUTES(N, objGraph);
    cout<<endl;
    ADDMONTHLYNEWFLIGHTS(objNode,p,N,root);
    cout<<endl;


    objGraph.LISTGROUTES();
    cout<<endl;
    cout<<"Flights: "<<endl;
    display_flights(root);
    cout<<endl;

    //add_to_currocc(root,objNode, objGraph);

    cout<<"----------------Month 5------------------:"<<endl;
    //canc_and_unpop=CANCELUNPOPULARFLIGHTS(objNode,root,OT,objGraph);
    cout<<endl;
    /*for (int i=0;i<50;i++){
        cout<<canc_and_unpop[i]<<endl;
    }*/ // DEBUG, SEE THE CORRESPONDING ARRAY
    //DISCARDUNPOPULARROUTESANDUPDATEPRICES(canc_and_unpop,objGraph);
    cout<<endl;
    p = ADDMONTHLYNEWROUTES(N, objGraph);
    cout<<endl;
    ADDMONTHLYNEWFLIGHTS(objNode,p,N,root);
    cout<<endl;
    objGraph.LISTGROUTES();
    cout<<endl;
    cout<<"Flights: "<<endl;
    display_flights(root);
    cout<<endl;

    cout<<"----------------Month 6------------------:"<<endl;
    //canc_and_unpop=CANCELUNPOPULARFLIGHTS(objNode,root,OT,objGraph);
    cout<<endl;
    /*for (int i=0;i<50;i++){
        cout<<canc_and_unpop[i]<<endl;
    }*/ // DEBUG, SEE THE CORRESPONDING ARRAY
    //DISCARDUNPOPULARROUTESANDUPDATEPRICES(canc_and_unpop,objGraph);
    cout<<endl;
    p = ADDMONTHLYNEWROUTES(N, objGraph);
    cout<<endl;
    ADDMONTHLYNEWFLIGHTS(objNode,p,N,root);
    cout<<endl;
    int *new_flights= CHECKNADDRETURNROUTES(objGraph);
    cout<<endl;
    ADDRETURNFLIGHTS(objNode,root,new_flights);
    cout<<endl;
    /* DEBUG, SEE ADDED RETURN FLIGHT NUMBERS
    for (int i=0;i<return_flight_index; i++){
        cout<<new_flights[i]<<endl;
    }
    */
    objGraph.LISTGROUTES();
    cout<<endl;
    cout<<"Flights: "<<endl;
    display_flights(root);
    cout<<endl;

    cout<<"-----------------------------------------------"<<endl;
    cout<<"One-way Trip Options for Probia ---> Biomedberg :"<<endl;
    DISPLAYROUTEOPTIONS(objGraph,root,"Probia","Biomedberg");
    cout<<endl;

    cout<<"One-way Trip Options for Magneta ---> Machineland :"<<endl;
    DISPLAYROUTEOPTIONS(objGraph,root,"Magneta","Machineland");
    cout<<endl;

    cout<<"One-way Trip Options for West Circuitta ---> East Circuitta :"<<endl;
    DISPLAYROUTEOPTIONS(objGraph,root,"West Circuitta","East Circuitta");
    cout<<endl;

    cout<<"One-way Trip Options for North Kirchoff ---> Telecommunicastan :"<<endl;
    DISPLAYROUTEOPTIONS(objGraph,root,"North Kirchoff","Telecommunicastan");

    return 0;
}
