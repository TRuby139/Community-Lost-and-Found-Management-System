#ifndef COMMUNITY_LOST_&_FOUND_MANAGEMENT_SYSTEM_H_INCLUDED
#define COMMUNITY_LOST_&_FOUND_MANAGEMENT_SYSTEM_H_INCLUDED

#include <iostream>
#include <string>

using namespace std;

struct Item {
    int id;
    string Item_Name;
    string category;
    string Location_Found;
    string date;
    string status; // Lost, Found, or Claimed
};

struct Node {
    Item details;
    // Pointers for the Doubly Linked List (Main Storage)
    Node *next;
    Node *prev;

    // Pointer for the Hash Table to resolve collision, which used SEPERATE CHAINING
    // If two IDs have the same hash, they form a mini-list using this pointer.
    Node *Next_Bucket;
};

class Lost_and_Found {
private:
    //1. Doubly Linked List, use this because we need to connect the list after deletion
    Node *head;
    Node *tail;

    // 2. Hash Table, use this because this is fast
    // Note: An array of *pointers* is usually allowed for Hash Tables.
    static const int TABLE_SIZE = 30;
    Node* Hash_Table[TABLE_SIZE];

    // Hash Function,using Item_Name to calculate index,which means can search using Item_Name
    int Hash_Function(string Name) {
        int sum = 0;

        // sum the value of string
        for (int i = 0; i < Name.length(); i++){
            sum = sum + Name[i]; // Name[i] is char, in c++ char is store in ASCII
        }
        return sum % TABLE_SIZE;
    }

    bool Is_Empty(){
        if (head == NULL){
            return true;
        }
        else{
            return false;
        }
    }

public:
    // Constructor, initialize the pointer as NULL to prevent error
    Lost_and_Found() {
        head = NULL;
        tail = NULL;

        // Initialize all Hash Table buckets to NULL
        for(int i = 0; i < TABLE_SIZE; i++) {
            Hash_Table[i] = NULL;
        }
    }

    // Function 1 - Add Item, Use Hashing and Linked list
    void Add_Item(int id, string name, string category, string location, string date, string status) {
        // Step 1: Create the new Node
        Node* New_Node = new Node();
        New_Node->details.id = id;
        New_Node->details.Item_Name = name;
        New_Node->details.category = category;
        New_Node->details.Location_Found = location;
        New_Node->details.date = date;
        New_Node->details.status = status;

        // Initialize pointers in the new Node
        New_Node->prev = NULL;
        New_Node->next = NULL;
        New_Node->Next_Bucket = NULL;

        // Step 2: Add to Doubly Linked List, using Stack style
        // This makes "Display" show the newest items first.
        if (Is_Empty()) {    //special case for the first items
            tail = New_Node;
            head = New_Node;
        } else {
            New_Node->next = head; // Link the next of the New_Node with the previous created node
            head->prev = New_Node; // Link the prev of the previous created node to New_Node
            head = New_Node;       // Update head for next storing, the New_Node is now "previous created node"
        }

        // Step 3: Add to Hash Table
        int Bucket_Index = Hash_Function(name);

        // Separate Chaining
        // Point the new node to a bucket
        New_Node->Next_Bucket = Hash_Table[Bucket_Index];

        // Update the certain bucket to point to this new node
        Hash_Table[Bucket_Index] = New_Node;

        cout << "Item Added ! \n";
    }

    // Function 2 - Search, Uses Hashing, very fast
    // Time complexity - O(1), much faster than looping through all the list.
    void Search_Name(string NAME) {
        if (Is_Empty()){
            cout << "There is nothing in the list !\n";
            return;
        }
        else{
            bool found = false;
            int index = Hash_Function(NAME);
            Node *current = Hash_Table[index];

            // Only search the list in this bucket, with sequential search
            while (current != NULL) {
                if (current->details.Item_Name == NAME) {
                    cout << "Match Found: " << current->details.Item_Name << endl;
                    found = true;
                }
                current = current->Next_Bucket; // Move using the Hash pointer
            }
            if (!found){
                    cout << "Item Name " << NAME << " not found." << endl;
            }
        }
    }

    // Function 3 - Display All, Use Doubly Linked List
    void Display_All() {
        if (Is_Empty()){
            cout << "There is nothing in the list !\n";
            return;
        }
        else{
            Node* current = head;
            while (current != NULL) {
                cout << "ID: " << current->details.id
                     << " | Name: " << current->details.Item_Name << endl;
                current = current->next; // Move using the LIST pointer
            }
        }
    }

    // Function 4 - Sort, Use Doubly Linked List, swap the pointer, using merge sort
    void Sort(){
        if (Is_Empty()){
            cout << "There is nothing in the list !\n";
            return;
        }
        else{

        }
    }
    // Function 5 - Edit, Use Doubly Linked List and Hash Table
    void Edit(){
        if (Is_Empty()){
            cout << "There is nothing in the list !\n";
            return;
        }
        else{

        }
    }
    // Function 6 - Delete, Use Doubly linked list, reconnect the prev and next
    void Delete(int Delete_ID){
        if (Is_Empty()){
            cout << "There is nothing in the list !\n";
            return;
        }
        else{
            Node *Delete_Target = NULL;
            Node *current = head;

            //Step 1 Find the delete target, Linear Search
            while(current != NULL){
                if(current->details.id == Delete_ID){
                    Delete_Target = current;
                    break; //terminate entire loop
                }
                current = current->next;
            }
            //if nothing was found
            if (Delete_Target == NULL){
                cout << "ID: " << Delete_ID << " does not occur.\n";
                return;
            }

            //Step 2 Disconnect the pointer in the list

            // Case A: Deleting the Head, the latest Node added
            if (Delete_Target == head) {
                head = head->next;
                if (head != NULL) {
                    head->prev = NULL; // Point the prev from the delete target to null
                } else {
                    tail = NULL; // If head is null, that means the list is empty
                }
            }
            // Case B: Deleting the Tail, the oldest data in the list
            else if (Delete_Target == tail) {
                tail = tail->prev; // point the tail to the second oldest data
                if (tail != NULL) {
                    tail->next = NULL; // Change the pointer from the delete target to null
                }
            }
            // Case C: Deleting from the Middle
            else {
                Delete_Target->prev->next = Delete_Target->next; // Point the prev data to the next data
                Delete_Target->next->prev = Delete_Target->prev; // Point the next data to prev data
            }

            //Step 3: Disconnect in hash table pointer

            int index = Hash_Function(Delete_Target->details.Item_Name);

            // Case A: It's the latest item in the bucket
            if (Hash_Table[index] == Delete_Target) {
                Hash_Table[index] = Delete_Target->Next_Bucket;
            }
            // Case B: It's deep inside the bucket chain
            else {
                Node* temp = Hash_Table[index];
                while (temp != NULL && temp->Next_Bucket != Delete_Target) {
                    temp = temp->Next_Bucket; //check through the bucket
                }
                // If we found the previous data of the target in the hash table
                if (temp != NULL) {
                    temp->Next_Bucket = Delete_Target->Next_Bucket; //Link the previous data to the next data of the target
                }
            }

            //Step 4: Delete Memory
            delete Delete_Target;
            cout << "Item ID: " << Delete_ID << " deleted.\n";
        }
    }

};

#endif // COMMUNITY_LOST_&_FOUND_MANAGEMENT_SYSTEM_H_INCLUDED
