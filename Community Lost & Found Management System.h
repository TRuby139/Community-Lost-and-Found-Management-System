#ifndef COMMUNITY_LOST_&_FOUND_MANAGEMENT_SYSTEM_H_INCLUDED
#define COMMUNITY_LOST_&_FOUND_MANAGEMENT_SYSTEM_H_INCLUDED

#include <iostream>
#include <string>
#include <iomanip>

using namespace std;

// ANSI Color Codes for UI Design
#define RESET   "\033[0m"
#define RED     "\033[31m"      // For Errors
#define GREEN   "\033[32m"      // For Success
#define YELLOW  "\033[33m"      // For Prompts
#define CYAN    "\033[36m"      // For Titles/Borders

struct Item {
    int id;
    string Item_Name;
    string category;
    string Location_Found;
    string date; // YYYY-MM-DD
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

void Clear_Screen() {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void Pause() {
    cout << "\n" << YELLOW << "Press Enter to continue..." << RESET;
    char temp;
    cin.get(temp);
}

bool Login() {
    string username, password;
    int attempts = 0;

    while (attempts < 3) {
        Clear_Screen();
        // Color: Cyan for Border
        cout << CYAN << "============================================\n";
        cout << "      SYSTEM SECURITY CHECK      \n";
        cout << "============================================" << RESET << "\n";
        cout << "Default credentials (admin / 1234)\n\n";

        cout << "Enter Username: "; cin >> username;
        cout << "Enter Password: "; cin >> password;

        if (username == "admin" && password == "1234") {
            // Color: Green for Success
            cout << "\n" << GREEN << "Login Successful! Access Granted." << RESET << "\n";
            cin.ignore();
            Pause();
            return true;
        } else {
            // Color: Red for Error
            cout << "\n" << RED << "Access Denied! Incorrect credentials." << RESET << "\n";
            attempts++;
            cout << YELLOW << "Attempts remaining: " << (3 - attempts) << RESET << endl;
            cin.ignore();
            Pause();
        }
    }
    return false;
}

class Lost_and_Found {
private:
    //1. Doubly Linked List, use this because we can connect the list after deletion faster O(1)
    Node *head;
    Node *tail;

    // 2. Hash Table, use this because this is fast
    static const int TABLE_SIZE = 30;
    Node** Hash_Table; // Dynamic allocation, Pointer to a pointer

    // --- MERGE SORT ---

    // Step 1 - Split the List ,Uses 2 pointer to find the middle
    Node* split(Node* List_Ptr) {
        Node* fast = List_Ptr;
        Node* slow = List_Ptr;

        // the fast pointer run faster then slow pointer twice
        // which means when the fast pointer end loop
        // the slow pointer will point at the middle of the list
        while (fast->next != nullptr && fast->next->next != nullptr) {
            fast = fast->next->next;
            slow = slow->next;
        }

        // Cut the list into two
        Node* temp = slow->next;
        slow->next = nullptr;
        return temp;
    }

    // Step 2 - Merge Lists
    Node* Merge_Alpha(Node *first, Node *second) {

        // If either list is empty, return the other list
        if (first == nullptr) return second;
        if (second == nullptr) return first;

        // Sorting by NAME (A-Z)
        if (To_Upper_Case(first->details.Item_Name) > To_Upper_Case(second->details.Item_Name)) {
            // Recursively merge the rest of the lists and
            // link the result to the current node
            first->next = Merge_Alpha(first->next, second);
            if (first->next != NULL) {
                first->next->prev = first;
            }
            first->prev = nullptr;
            return first;
        }
        else {
            second->next = Merge_Alpha(first, second->next);
            if (second->next != NULL) {
                second->next->prev = second;
            }
            second->prev = nullptr;
            return second;
        }
    }

    // Step 3 - The main function, use recursive, sort alphabet
    Node* Merge_Sort_Alpha(Node *head) {

        // Base case, return the list when it only have 1 or nothing
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        // Start splitting the list
        Node* second = split(head);

        // Recursion happens here
        head = Merge_Sort_Alpha(head); // Sort the left part
        second = Merge_Sort_Alpha(second); // Sort the right part

        return Merge_Alpha(head, second);
    }

    // This one merge and sort the date
        Node* Merge_Date(Node *first, Node *second) {

        // If either list is empty, return the other list
        if (first == nullptr) return second;
        if (second == nullptr) return first;

        // Sorting by Date
            if (first->details.date > second->details.date) {
                // Recursively merge the rest of the lists and
                // link the result to the current node
                first->next = Merge_Date(first->next, second);
                if (first->next != NULL) {
                    first->next->prev = first;
                }
                first->prev = nullptr;
                return first;
            }
            else {
                second->next = Merge_Date(first, second->next);
                if (second->next != NULL) {
                    second->next->prev = second;
                }
                second->prev = nullptr;
                return second;
            }
    }

        Node* Merge_Sort_Date(Node *head) {

        // Base case, return the list when it only have 1 or nothing
        if (head == nullptr || head->next == nullptr) {
            return head;
        }

        // Start splitting the list
        Node* second = split(head);

        // Recursion happens here
        head = Merge_Sort_Date(head); // Sort the left part
        second = Merge_Sort_Date(second); // Sort the right part

        return Merge_Date(head, second);
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
        if (head == nullptr){
            return true;
        }
        else{
            return false;
        }
    }

    // Helper function - Converts string to UPPERCASE
    string To_Upper_Case(string str) {
        string result = "";
        for (char c : str) {
            result += toupper(c);
        }
        return result;
    }

public:
    // Constructor, initialize the pointer as nullptr to prevent error
    Lost_and_Found() {
        head = nullptr;
        tail = nullptr;

        // Dynamically allocate the array
        Hash_Table = new Node*[TABLE_SIZE];

        // Initialize all Hash Table buckets to nullptr
        for(int i = 0; i < TABLE_SIZE; i++) {
            Hash_Table[i] = nullptr;
        }
    }

        // Destructor: Cleans up memory when program closes
    ~Lost_and_Found() {
        // Delete the Hash Table Array
        delete[] Hash_Table;
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
        New_Node->prev = nullptr;
        New_Node->next = nullptr;
        New_Node->Next_Bucket = nullptr;

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
            cout << "\n----------------------------------------------------------------------------------------------------------------------------\n";
            while (current != nullptr) {
                if (current->details.Item_Name == NAME) {
                    cout << left << setw(4) << "ID: " << setw(5) << current->details.id
                        << setw(7) << "|Name: " << setw(22) << current->details.Item_Name
                        << setw(11) << "|Category: " << setw(14) << current->details.category
                        << setw(17) << "|Location Found: " << setw(14) << current->details.Location_Found
                        << setw(7) << "|Date: " << setw(10) << current->details.date
                        << setw(8) << "|Status: " << setw(7) << current->details.status << "\n";
                    found = true;
                }
                current = current->Next_Bucket; // Move using the Hash pointer
            }
            cout << "----------------------------------------------------------------------------------------------------------------------------\n";
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
            Node* current = nullptr;
            cout << "Display in \n 1.Ascending \n 2.Descending \n";
            cout << "Enter your choice (1 or 2): ";
            cin >> choice;
            cin.ignore(); // Clear buffer to prevent getline errors
            cout << endl;

            switch(choice){
                case 1:
                    current = tail;
                    cout << "\n----------------------------------------------------------------------------------------------------------------------------\n";
                    while (current != nullptr) {
                        cout << left << setw(4) << "ID: " << setw(5) << current->details.id
                            << setw(7) << "|Name: " << setw(22) << current->details.Item_Name
                            << setw(11) << "|Category: " << setw(14) << current->details.category
                            << setw(17) << "|Location Found: " << setw(14) << current->details.Location_Found
                            << setw(7) << "|Date: " << setw(10) << current->details.date
                            << setw(8) << "|Status: " << setw(7) << current->details.status << "\n";
                        current = current->prev; // Move using the LIST pointer
                    }
                    cout << "----------------------------------------------------------------------------------------------------------------------------\n";

                    break;
                case 2:
                    current = head;
                    cout << "\n----------------------------------------------------------------------------------------------------------------------------\n";
                    while (current != nullptr) {
                        cout << left << setw(4) << "ID: " << setw(5) << current->details.id
                            << setw(7) << "|Name: " << setw(22) << current->details.Item_Name
                            << setw(11) << "|Category: " << setw(14) << current->details.category
                            << setw(17) << "|Location Found: " << setw(14) << current->details.Location_Found
                            << setw(7) << "|Date: " << setw(10) << current->details.date
                            << setw(8) << "|Status: " << setw(7) << current->details.status << "\n";
                        current = current->next;// Move using the LIST pointer
                    }
                    cout << "----------------------------------------------------------------------------------------------------------------------------\n";
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
            int choose;
            cout << "Sort: \n1.Sort by alphabet \n2.Sort by date\n";
            cout << "Enter your choice: ";
            cin >> choose;
            cin.ignore();

            // 1. Run the Sort
            if (choose == 1) {
                head = Merge_Sort_Alpha(head);
                cout << "List sorted by Name.\n";
            }
            else if (choose == 2) {
                head = Merge_Sort_Date(head);
                cout << "List sorted by Date.\n";
            }
            else {
                cout << "Invalid choice.\n";
                return;
            }

            // 2. Fix the Tail Pointer
            // (Merge sort rearranges links but forgets who is 'tail', so we find it again)
            Node* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            tail = temp;

            cout << "List sorted.\n";
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
            while (current != nullptr) {
                if (current->details.id == ID) {
                    break;
                }
                current = current->next;
            }

            if (current == nullptr) {
                cout << "ID: " << ID << " not found.\n";
                return;
            }

            // Step 2: Show Edit Item Details
            cout << "\n--- Editing Item: " << current->details.Item_Name << " ---\n";
            cout << left <<setw(15) << "1. Category: " << current->details.category << "\n";
            cout << setw(15) << "2. Location: " << current->details.Location_Found << "\n";
            cout << setw(15) << "3. Date: " << current->details.date << "\n";
            cout << setw(15) << "4. Status: " << current->details.status << "\n";
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
            Node *Delete_Target = nullptr;
            Node *current = head;

            //Step 1 Find the delete target, Linear Search
            while(current != nullptr){
                if(current->details.id == Delete_ID){
                    Delete_Target = current;
                    break; //terminate entire loop
                }
                current = current->next;
            }
            //if nothing was found
            if (Delete_Target == nullptr){
                cout << "ID: " << Delete_ID << " does not occur.\n";
                return;
            }

            //Step 2 Disconnect the pointer in the list

            // Case A: Deleting the Head, the latest Node added
            if (Delete_Target == head) {
                head = head->next;
                if (head != nullptr) {
                    head->prev = nullptr; // Point the prev from the delete target to nullptr
                } else {
                    tail = nullptr; // If head is nullptr, that means the list is empty
                }
            }
            // Case B: Deleting the Tail, the oldest data in the list
            else if (Delete_Target == tail) {
                tail = tail->prev; // point the tail to the second oldest data
                if (tail != nullptr) {
                    tail->next = nullptr; // Change the pointer from the delete target to nullptr
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
                while (temp != nullptr && temp->Next_Bucket != Delete_Target) {
                    temp = temp->Next_Bucket; //check through the bucket
                }
                // If we found the previous data of the target in the hash table
                if (temp != nullptr) {
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