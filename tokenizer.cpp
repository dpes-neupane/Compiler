
// Note that here we use std::stack 
// for Stack operations 
// and queue for queue operations
#include<iostream>
#include<string.h>
#include<stack>
#include<queue>
#include<algorithm>

using namespace std;




struct Node{
	char data;
	Node* left, *right;
	bool leaf;
	int firstPosArr[100];
	
	int lastPosArr[100];
	
	bool nullable;

	int followPos[100];
	
};

struct DTrans{
	int state[200];
	char input;
	int outputState[200];
	int mark;
};

DTrans* newDTrans(){
	DTrans* newState = (DTrans*) malloc(sizeof(DTrans));
	memset(newState->state, 0, 200);
	memset(newState->outputState, 0, 200);
	newState->mark = 0;
	return newState;
}



struct DStates{
	DTrans* dstates[200];
	int position;
	
};

DStates* newDStates(){
	DStates* newdstates = (DStates*) malloc(sizeof(DStates));
	newdstates->position = -1;
	return newdstates;
}





Node* newNode(char op){
	Node* node = (Node*) malloc(sizeof(Node));
	node->data = op;
	node->left = node->right = NULL;
	node->leaf = false;
	memset(node->firstPosArr, 0, 100);
	memset(node->lastPosArr, 0, 100);
	memset(node->followPos, 0, 100);
	return node;
}




Node* rootNode(){
	Node* node = (Node*) malloc(sizeof(Node));
	node->data = 'N';
	node->left = node->right = NULL;
	node->leaf=false;
	return node;
}

//global variables
queue<char> outputQ;
int resultArray[100];
int followPos[100];
Node* leafPosArray[100];
int noOfLeaf;
DStates* dStates = newDStates();




//Function to return precedence of operators 
int prec(char c) 
{ 
	 
	if(c == '*')
	return 3; 
	else if(c == '.')
	return 2; 
	else if(c == '+') 
	return 1; 
	else
	return -1; 
} 

// The main function to convert infix expression 
//to postfix expression 
string infixToPostfix(string s){
	stack<char> st; 
	st.push('N'); 
	int l = s.length(); 
	string ns; 
	for(int i = 0; i < l; i++) 
	{ 
		
		// If the scanned character is 
		// an operand, add it to output string. 
		if((s[i] >= 'a' && s[i] <= 'z') || 
		(s[i] >= 'A' && s[i] <= 'Z') || (s[i] == '#')){
			ns+=s[i]; 
			outputQ.push(s[i]);
			
		} 
		

		// If the scanned character is an 
		// ‘(‘, push it to the stack. 
		else if(s[i] == '(') 
		
		st.push('('); 
		
		// If the scanned character is an ‘)’, 
		// pop and to output string from the stack 
		// until an ‘(‘ is encountered. 
		else if(s[i] == ')') 
		{ 
			while(st.top() != 'N' && st.top() != '(') 
			{ 
				char c = st.top(); 
				st.pop(); 
				ns += c; 
				outputQ.push(c);
				
			} 
			if(st.top() == '(') 
			{ 
				char c = st.top(); 
				st.pop(); 
			} 
		} 
		
		//If an operator is scanned 
		else{ 
			while(st.top() != 'N' && prec(s[i]) <= 
								prec(st.top())) 
			{ 
				char c = st.top(); 
				st.pop(); 
				ns += c; 
				outputQ.push(c);
				
			} 
			st.push(s[i]); 
		} 

	} 
	
	// Pop all the remaining elements from the stack 
	while(st.top() != 'N') 
	{ 
		char c = st.top(); 
		st.pop(); 
		ns += c; 
		outputQ.push(c);
	} 
	
	return ns;

} 




void SetUnion(int firstArray[], int secondArray[]){
	memset(resultArray, 0, 100);
	int i=0; 
	while(firstArray[i] != 0){
		resultArray[i] = firstArray[i];
		
		i++;
	}
	// cout<<i<<" \n";
	int j=0, l=i;
	while(secondArray[j]!=0){
		int k=0; 
		while(secondArray[j] != resultArray[k] && resultArray[k] != 0){
			k++;
		}
		if(k >= i){
			resultArray[l++] = secondArray[j];
		}
		j++;
	}
	
}
//transfer function to put elements of one array to another --- for transferring the resulting array in the nodes' array
void transfer(int from[], int to[]){
	int i=0, j=0;
	while(from[i] !=0){
		to[i] = from[i];
		i++;
	}
}

//transfer if the array is not empty

void transfer2(int from[], int to[]){
	int i=0, j=0;
	while(to[i] != 0){
		i++;

	}
	while(from[j]!=0){
		to[i++] = from[j];
		j++;
	}
}

//to display the positions of the nodes---like the firstpos, the lastpos, followpos

void display (int arr[], string spacing ){
	int i=0;
	
	while(arr[i] != 0){
		cout<<arr[i]<<" ";
		i++;
	}cout<<spacing;
}







//preorder traversal for followpos calculation

void preOrder(Node* root, Node* arr[]){
	if(root == NULL){
		return;
	}
	
	
	
	if(root->data == '.'){
		int i=0;
		while(root->left->lastPosArr[i] != 0){
			Node* temp = arr[root->left->lastPosArr[i]];
			
			if(temp->followPos[0] == 0){
				transfer(root->right->firstPosArr, temp->followPos);
				
				
			}

			else{
				transfer2(root->right->firstPosArr, temp->followPos);
			}
			
			
			
			i++;
		}
		
	}


	
	if(root->data == '*'){
		int i=0; 
		while(root->left->lastPosArr[i] != 0){
			Node* temp = arr[root->left->lastPosArr[i]];
			
			if(temp->followPos[0] == 0){
				transfer(root->left->firstPosArr, temp->followPos);
				
			}
			else{
				transfer2(root->left->firstPosArr, temp->followPos);
			}
			
			i++;
		}
		

	}

	preOrder(root->left, arr);

	preOrder(root->right, arr);
}

//inOrder traversal


void inOrder(Node* root, char w){
	if(root == NULL){
		return;
	}
	inOrder(root->left, w);
	if (w == 'f'){
			cout<<root->data<<" ";
			display(root->firstPosArr, "\n");
	}
	if(w == 'l'){
		cout<<root->data<<" ";
		display(root->lastPosArr, "\n");
	}
	if(root->leaf){
		
		if(w == 'g'){
			cout<<root->firstPosArr[0]<<"      ";
			display(root->followPos, "\n");
		}
		
	}
	inOrder(root->right, w);
}

//the output was put in the queue so that the infix expression can be put together into a syntax tree


int syntaxTree(Node* mainRoot, int noOfLeaf, Node* leafPosArray[]){
	stack<Node*>intermediate_stack;
	
	
	while(!outputQ.empty()){
		char c = outputQ.front();
		
		//if it is not an operator it is a leaf node
		//the char are put in a stack 
		if(prec(c) < 0){
			
			Node* leaf = newNode(c);
			leaf->leaf = true;
			noOfLeaf++;
			
			leaf->firstPosArr[0] = noOfLeaf;
			leaf->lastPosArr[0] = noOfLeaf;
			leafPosArray[noOfLeaf] = leaf;
			intermediate_stack.push(leaf);		
		}

		//else we get an operator and if it is an '*' we will have only one leaf node
		//the stack is popped and the leaf nodes are added to the operator node

		else{
			Node* root = newNode(c);
		
			if(prec(c) == 3){

				root->left = intermediate_stack.top();
				
				int i=0;
				while(root->left->firstPosArr[i] != 0){
					root->firstPosArr[i] = root->left->firstPosArr[i];
					i++;
				}
				i=0;
				while(root->left->lastPosArr[i] != 0){
					root->lastPosArr[i] = root->left->lastPosArr[i];
					i++;
				}
				root->nullable = true; 
				intermediate_stack.pop();
			}
			else{
				root->right = intermediate_stack.top();

				intermediate_stack.pop();
				
				root->left = intermediate_stack.top();
				if(root->data == '+'){//calculating firstpos and lastpos for (+) in regular expression
					root->nullable = root->left->nullable || root->right->nullable;	
					SetUnion(root->right->firstPosArr, root->left->firstPosArr);
					transfer(resultArray, root->firstPosArr);

					SetUnion(root->right->lastPosArr, root->left->lastPosArr);
					transfer(resultArray, root->lastPosArr);

					
				}
				
				else{//the operator in "." which is handled according the firstpos and lastpos rules
					root->nullable = root->left->nullable && root->right->nullable;
					// cout<<root->left->data<<"  "<<root->right->data<<"\n";
					if(root->left->nullable){
						SetUnion(root->left->firstPosArr, root->right->firstPosArr);
						transfer(resultArray, root->firstPosArr);
					}
					else{
						transfer(root->left->firstPosArr, root->firstPosArr);
					}
					if(root->right->nullable){
						SetUnion(root->left->lastPosArr, root->right->lastPosArr);
						transfer(resultArray, root->lastPosArr);
					}
					else{
						transfer(root->right->lastPosArr, root->lastPosArr);
					}
				}
				
				intermediate_stack.pop();
			
			}
			intermediate_stack.push(root);
			//getting the main root of the tree
			mainRoot->data = c;
			mainRoot->left = root->left;
			mainRoot->right = root->right;
			memset(mainRoot->firstPosArr, 0, 100);
			memset(mainRoot->lastPosArr, 0, 100);
			transfer(root->firstPosArr, mainRoot->firstPosArr);
			transfer(root->lastPosArr, mainRoot->lastPosArr);
			
		}
		outputQ.pop();
	}
	return noOfLeaf;

}


//making the DFA




//function to find the unique alphabets (input symbols) of the regular expression

int findInputSymbols(char inputSymbols[]){
	int k=0;
	
	for(int i=1; i<noOfLeaf; i++){
		bool flag = 0;
		for(int j=0; j<k; j++){
			if(leafPosArray[i]->data == inputSymbols[j]){
					// cout<<leafPosArray[i]->data<<" "<<leafPosArray[j]->data<<"   ";
					flag = 1;
					break;
				}
				
		}
		if(!flag){
			inputSymbols[k++] = leafPosArray[i] -> data;
			
		}
	}
	return k;
}


// checking if there are any unmarked states in the DStates

struct State{
	bool mark;
	int state[100];
};


struct States{
	int position;
	State* statesArr[100];
};


State* newState(int state[]){
	State* nState = (State*) malloc(sizeof(State)); //dipesh
	nState->mark = 0;
	transfer(state, nState->state);
	return nState;
}


States* addState(State* state){
	States* newstate = (States*) malloc(sizeof(States));
	newstate->position = 0;
	newstate->statesArr[newstate->position] = state;
	return newstate;
}


int checkMarks(States* dstates){
	int i=0;
	while(i<=dstates->position){
		// cout<<dstates->statesArr[i]->mark;
		if(!(dstates->statesArr[i]->mark)){
		
			return i;
		}
		i++;
	}
	return -1;
}

//function to check whether the state has the given inputSymbol in the array
bool checkIndex(int state, int* stateArr){
	int i=0; 
	
	while(stateArr[i]!=0){
		if((stateArr[i] ^ state) == 0){
			return 1;
		}

		
		i++;
	}
	return 0;
}


//function to check whether the state is in the DStates or not
int checkState(States* dstates, int potNewState[]){
	int nStates = dstates->position; 
	
	int lenPotState = 0;//finding the length of the new
	while(potNewState[lenPotState] !=0 ){
		lenPotState++;
	}
	// cout<<lenPotState<<" ";
	sort(potNewState, potNewState + lenPotState);
	
	for(int i=0; i<=nStates; i++){
		
		int lenDState = 0;//finding the length of the current states present in the dstates array
		while(dstates->statesArr[i]->state[lenDState] != 0 ){
			lenDState++;
		}
		// cout<<lenDState<<" ";
		
		sort(dstates->statesArr[i]->state, (dstates->statesArr[i]->state) + lenDState);
		int j = 0;
		int count=0;
		while(dstates->statesArr[i]->state[j] != 0 && potNewState[j] != 0 ){
			if((dstates->statesArr[i]->state[j] ^ potNewState[j]) == 0){
				count++;
			}
			j++;
		}
		if (count == lenDState && lenDState == lenPotState){
			return i;
		}
		


				
	}
	
	


	return -1;
}	

//to display the transition table
void displayState(DStates* dstates){
	
	for(int i=0; i<=dstates->position; i++){
		display(dstates->dstates[i]->state, "     ");
		cout<<"|"<<dstates->dstates[i]->input<<"     |";
		display(dstates->dstates[i]->outputState, "\n");

	}
	
	
}



void DFA(Node* mainRoot, char inputSymbol[], int lenOfInput){
	State* state = newState(mainRoot->firstPosArr);//making the start state
	//push the startNode into an array
	States* states = addState(state);
	int unmarked = checkMarks(states);
	int index = checkState(states, state->state);
	
	
	while(unmarked != -1){//if there is no unmarked state in array then get out
		states->statesArr[unmarked]->mark = 1;

		
		for(int i=0; i<lenOfInput; i++){//for each input, we will check which state the current unmarked state will go

			int potState[100];
			memset(potState, 0, 100);
			bool flag = 0;
			for(int j=1; j<noOfLeaf; j++){//we will check which leaf has the data equal to the input and the state has to have that leaf as well
				if((inputSymbol[i] == leafPosArray[j]->data) && (checkIndex(leafPosArray[j] -> firstPosArr[0], states->statesArr[unmarked]->state))){
					
					
					if(!flag){//the potential array might be empty at first 
						transfer(leafPosArray[j] -> followPos, potState);
						flag = 1;
					}
					else{//we have to add more followpos if there are more than one state that correspond to the input symbol
						SetUnion(leafPosArray[j] -> followPos, potState);
						transfer(resultArray, potState);
					}
					
					
				}

			}
			//we have found the output state for the given state
			// now we have make sure that this is in the States array or not
			index = checkState(states, potState);// gives if there is the output state or not
			
			DTrans* transitionState = newDTrans();
			transfer( states->statesArr[unmarked]->state, transitionState->state);	
			transitionState->input = inputSymbol[i];
			transfer(potState, transitionState->outputState);
			dStates->dstates[++(dStates->position)] = transitionState; 

			if(index < 0){
				//we don't have to do anything, we just have to add the transition state in the array with the input symbol and the given output
				State* nwState = newState(potState);
				states->statesArr[++(states->position)] = nwState;	
				


			}
			


		}

		unmarked = checkMarks(states);
		// cout<<"\n"<<unmarked<<"\n";
	}	




}





//Driver program to test above functions 
int main(){ 
	// cout<<"Enter a valid regular expression with proper brackets:";
	string exp = "a.(a+b)*.b";
	// string exp;
	// cin>>exp;
	exp = '(' + exp +')' + ".#" ;
	string s= infixToPostfix(exp); 
	
	Node * mainRoot = rootNode();
	noOfLeaf = syntaxTree(mainRoot, noOfLeaf, leafPosArray);
	preOrder(mainRoot, leafPosArray); // it is actually calculating the followpos of the tree 
	cout<<"\nFirstPos:\n";
	inOrder(mainRoot, 'f');
	cout<<"\nLastpos:\n";
	inOrder(mainRoot, 'l');
	cout<<"\nFollowPos\n";
	inOrder(mainRoot, 'g');
	//find which one are the inputs
	char inputSymbols[100];
	int lenOfInput = findInputSymbols(inputSymbols);
	
	DFA(mainRoot, inputSymbols, lenOfInput);
	displayState(dStates);
	


	return 0; 
} 
