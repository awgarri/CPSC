#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;


typedef struct node {
        string data;
        node* next;
        node* prev;
    }node;

void printList(node* head) {
    node* temp = head;

    while(temp != NULL) {
        cout << temp->data << '\n';
        temp = temp->next;
    }
}

int main(int arg, char** argv) {

    node* head;
    node* tail;
    node* n;

    int numberNodes = 0;

    cout << "how many nodes do you want?\n";
    cin >> numberNodes;

    n = new node;
    cin >> n->data;
    n->prev = NULL;
    head = n;
    tail = n; 

    for(int i = 1; i < numberNodes; ++i) {
        n = new node;
        cin >> n->data;
        n->prev = tail;
        tail->next = n;
        tail = n;  
    }

    n = head;
    cout << "LIST:\n";
    printList(head);

    tail->next = NULL;
        
}
