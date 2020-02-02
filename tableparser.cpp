#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;



class List {
public:
	List() { head = NULL; };
	~List();
	struct node {//our pair of values
		char* key;	 //key	
		char* value; //value
		node *next;  //pointer to the next
	};
	node* getHead() const;  //returns us the head of the list
	int empty() const;	//check if empty
	void addHead(char* key, char* value);	//adding the head
	//void addHead(const char* key, const char* value);
	node* insert(node* n, const char* key, const char* value); // add elem to list
	void print(); //printing the reslut
private:
	node* head; 
};




class parser {
public:
	parser(); //parser
	void printTable(); //table output
private:
	char *query();
	char *get();
	char *post();
	List list; //our list
	static const int URL_LIMIT = 500;
};



char* parser::post() { ///func returning our url if using POST method
	char* str = getenv("CONTENT_LENGTH");
	int size = atoi(str);
	char* url = new char[size + 1];
	fread(url, size, 1, stdin);
	url[size] = '\0';
	return url;
}


char* parser::get() { //func returning our url if using GET method
	char* str = getenv("QUERY_STRING");
	int size = strlen(str);
	if(size > URL_LIMIT)//limiting the mem allocation up to URL_LIMIT symb
		return NULL; 
	char* url = new char[size + 1];
	strcpy(url, str);
	return url;
}


char* parser::query() { //method checking query
	char * method = getenv("REQUEST_METHOD");
	if (strcmp(method, "GET") == 0) //calls get func if value GET
		return get();
	if (strcmp(method, "POST") == 0) //calls post func if value POST
		return post();
}



char decode(const char* str) {	//decoding
	char a = *str;	//1st sym after %
	char b = *(str + 1);	//2nd sym after%
	char c; 
	if (isalpha(a)) {  //if 1st symbol NOT num
		c = (a - 'A' + 10) << 4;
	}
	else { //if 1st symbol num
		c = (a - '0') << 4;
	}
	if (isalpha(b)) {	//if 2nd symbol NOT num
		c += (b - 'A' + 10);
	}
	else {
		c += (b - '0');	//if 2nd symbol num
	}
	return c;
}


int main() {
	cout << "Content-type:text/html \n\n";
	parser p;

	cout << "<html>\n";
	cout << "<head>\n";
	cout << "<title> Personal wine card</title>\n";
	cout<<"<body background='https://wallpaperstream.com/wallpapers/full/white/Wine-Close-Up-White-Background.jpg'>";
	cout << "</head>\n";
	cout << "<body>\n";
	p.printTable();
	cout << "</body>\n";
	cout << "</html>\n";
	//system("pause");
	return 0;
}


parser::parser() {
	int i = 0;
	char value[100]; //value buffer
	char key[100];  //key buffer
	char* ptr = key; //pointer to  value/key buffer for non checking which buffer when write in switch
	char * str = query();
	
	
	if(str == NULL){
		cout << "Sorry, we cannot handle the request :( <br>\n";
		return;
	}
	
	char c;

	while (str[i] != '\0' && list.empty()) {
		switch (str[i]) {
		case '=':
			*ptr = '\0';
			ptr = value;
			break;
		case '&':
			*ptr = '\0';
			ptr = key;
			list.addHead(key, value);
			break;
		case '+':
			*ptr = ' ';
			ptr++;
			break;
		case '%':
			c = decode(str + i + 1);
			*ptr = c;
			ptr++;
			i += 2;
			break;
		default:
			*ptr = str[i];
			ptr++;
		}
		i++;
	}
	
	if(list.empty()){
		*ptr = '\0';
		list.addHead(key, value);
	}else{
		
		List::node* pointer =list.getHead();
	
		while (str[i] != '\0') {
			switch (str[i]) {
			case '=':
				*ptr = '\0';
				ptr = value;
				break;
			case '&':
				*ptr = '\0';
				ptr = key;
				pointer=list.insert(pointer,key, value);
				break;
			case '+':
				*ptr = ' ';
				ptr++;
				break;
			case '%':
				c = decode(str + i + 1);
				*ptr = c;
				ptr++;
				i += 2;
				break;
			default:
				*ptr = str[i];
				ptr++;
			}
			i++;
		}
		*ptr = '\0';
		pointer = list.insert(pointer, key, value);
	}
	delete str;
}

List::~List()
{
	if(head != NULL){
		node* prev = head;
		node* current = prev->next;
		if(current == NULL){
			delete prev->key;
			delete prev->value;
			delete prev;
		}else{
			while (current != NULL) {
				delete prev->key;
				delete prev->value;
				delete prev;
				prev = current;
				current=current->next;
			}	
		}
	}
	
}


List::node * List::getHead() const
{
	return head;
}


int List::empty() const
{
	return head==NULL;
}


void List::addHead(char * key, char * value)//adding the head
{
	head = new node;
	head->key = new char(strlen(head->key) + 1);
	head->value = new char(strlen(head->value) + 1);
	head->next = NULL;
	head->key = strcpy(head->key, key);
	head->value = strcpy(head->value, value);
}



List::node* List::insert(node* n, const char* key, const char* value) {//elem add to list ,returns ptrNext to know the inserting index
	n->next = new node;
	n = n->next;
	n->key = new char[strlen(key) + 1];
	n->value = new char[strlen(value) + 1];
	n->next = NULL;
	strcpy(n->key, key);
	strcpy(n->value, value);
	return n;
}



void List::print() {
	if (head == NULL)
		cout << "Is empty" << endl;
	else {
		node* current = head;
		while (current != NULL) {
			cout << current->key << "  " << current->value << endl;
			current = current->next;
		}
	}
}



void parser::printTable()
{
	List::node* current=list.getHead();
	cout << "<center><h2>This is your Wine card:</h2></center> \n";
	cout << "<center><table border=\"1\"></center>\n";
	while(current!=NULL){
		cout << "<center><tr></center> \n";
		cout << "<center><td></center> \n";
		cout << current->key;
		cout << "<center></td></center> \n";
		cout << "<center><td> </center>\n";
		cout << current->value;
		cout << "<center></td> </center>\n";
		cout << "<center></tr> </center>\n";
		current=current->next;
	}
	cout << "<center></table> </center>\n";
}
