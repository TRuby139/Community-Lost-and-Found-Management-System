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
    //1. Doubly Linked List, use this because we can connect the list after deletion faster O(1)
    Node *head;
    Node *tail;

    // 2. Hash Table, use this because this is fast
    static const int TABLE_SIZE = 30;
    Node* Hash_Table[TABLE_SIZE];

    // --- MERGE SORT ---

    // Step 1. Split List: Uses "Fast & Slow" pointer trick to find the middle
    Node* split(Node* source) {
        Node* fast = source;
        Node* slow = source;

        while (fast->next != NULL && fast->next->next != NULL) {
            fast = fast->next->next;
            slow = slow->next;
        }
        Node* temp = slow->next;
        slow->next = NULL; // Cut the list into two
        return temp;
    }

    // Step 2. Merge Lists: Zips two sorted lists together
    Node* merge(Node* first, Node* second) {
        if (first == NULL) return second;
        if (second == NULL) return first;

        // SORTING CRITERIA: Currently sorting by NAME (A-Z)
        // If you want to sort by DATE, change to:
        // if (first->details.date < second->details.date)
        if (first->details.Item_Name < second->details.Item_Name) {
            first->next = merge(first->next, second);
            first->next->prev = first;
            first->prev = NULL;
            return first;
        } else {
            second->next = merge(first, second->next);
            second->next->prev = second;
            second->prev = NULL;
            return second;
        }
    }

    // Step 3. Recursive Sort: The engine that keeps splitting
    Node* mergeSort(Node* node) {
        if (node == NULL || node->next == NULL) {
            return node; // Base case
        }
        Node* secondHalf = split(node);

        node = mergeSort(node);
        secondHalf = mergeSort(secondHalf);

        return merge(node, secondHalf);
    }

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
        if (Is_Empty()) {    //special case for the first items
            tail = New_Node;
            head = New_Node;
        } else {
            New_Node->next = head; // Link the next of the New_Node with the previous created node
            head->prev = New_Node; // Link the prev of the previous created node to New_Node
            head = New_Node;       // Update head for next storing, the New_Node is now "previous created node"
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
                    cout << "ID: " << current->details.id
                     << " | Name: " << current->details.Item_Name
                     << " | Category: " << current->details.category
                     << " | Location Found: " << current->details.Location_Found
                     << " | Date: " << current->details.date
                     << " | Status: " << current->details.status << endl;
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
            int choice;
            Node* current = NULL;
            cout << "Display in \n 1. Ascending \n 2. Descending \n";
            cout << "Enter your choice (1 or 2): ";
            cin >> choice;
            cin.ignore(); // Clear buffer to prevent getline errors
            cout << endl;

            switch(choice){
                case 1:
                    current = tail;
                    while (current != NULL) {
                        cout << "ID: " << current->details.id
                             << " | Name: " << current->details.Item_Name
                             << " | Category: " << current->details.category
                             << " | Location Found: " << current->details.Location_Found
                             << " | Date: " << current->details.date
                             << " | Status: " << current->details.status << "\n";
                        current = current->prev; // Move using the LIST pointer
                    }
                    break;
                case 2:
                    current = head;
                    while (current != NULL) {
                        cout << "ID: " << current->details.id
                             << " | Name: " << current->details.Item_Name
                             << " | Category: " << current->details.category
                             << " | Location Found: " << current->details.Location_Found
                             << " | Date: " << current->details.date
                             << " | Status: " << current->details.status << "\n";
                        current = current->next; // Move using the LIST pointer
                    }
                    break;
                default:
                    cout << "Invalid input !\n";
                    break;
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
            // 1. Run the Sort
            head = mergeSort(head);

            // 2. Fix the Tail Pointer
            // (Merge sort rearranges links but forgets who is 'tail', so we find it again)
            Node* temp = head;
            while (temp->next != NULL) {
                temp = temp->next;
            }
            tail = temp;

            cout << "List sorted by Name (A-Z).\n";
            }
    }

    // Function 5 - Edit, Use Doubly Linked List and Hash Table
    void Edit(int ID){
        if (Is_Empty()){
            cout << "There is nothing in the list !\n";
            return;
        }
        else{
            Node* current = head;
            // Step 1: Find the item by ID
            while (current != NULL) {
                if (current->details.id == ID) {
                    break;
                }
                current = current->next;
            }

            if (current == NULL) {
                cout << "ID: " << ID << " not found.\n";
                return;
            }

            // Step 2: Show Edit Item Details
            cout << "\nEditing Item: " << current->details.Item_Name << " ---\n";
            cout << "1. Category: " << current->details.category << ")\n";
            cout << "2. Location: " << current->details.Location_Found << ")\n";
            cout << "3. Date: " << current->details.date << ")\n";
            cout << "4. Status: " << current->details.status << ")\n";
            cout << "0. Cancel\n";
            cout << "Select field to edit: ";

            int choice;
            cin >> choice;
            cin.ignore(); // Clear buffer to prevent getline errors

            string input;
            switch(choice) {
                case 1:
                    cout << "Enter new Category: ";
                    getline(cin, input);
                    current->details.category = input;
                    cout << "Category updated.\n";
                    break;
                case 2:
                    cout << "Enter new Location: ";
                    getline(cin, input);
                    current->details.Location_Found = input;
                    cout << "Location updated.\n";
                    break;
                case 3:
                    cout << "Enter new Date: ";
                    getline(cin, input);
                    current->details.date = input;
                    cout << "Date updated.\n";
                    break;
                case 4:
                    cout << "Enter new Status (Lost, Found, or Claimed): ";
                    getline(cin, input);
                    current->details.status = input;
                    cout << "Status updated.\n";
                    break;
                case 0:
                    cout << "Edit cancelled.\n";
                    break;
                default:
                    cout << "Invalid option.\n";
            }
        }
    }

    // Function 6 - Delete, Use Doubly linked list and Hash Table, reconnect the prev and next
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

can you check is there any problem with my header file?
