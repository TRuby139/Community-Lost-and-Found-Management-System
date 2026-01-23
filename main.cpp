#include <iostream>
#include <cstdlib> // Required for system("cls")
#include <string>
#include <iomanip>
#include "Community Lost & Found Management System.h"

using namespace std;

int main() {
    if (!Login()) {
        cout << "\n" << RED << "Too many failed attempts. System Locking Down." << RESET << "\n";
        return 0;
    }

    Lost_and_Found system;
    int choice;
    int Item_Count = 0;

    do {
        Clear_Screen();
        cout << "\n" << CYAN << "============================================" << endl;
        cout << "   COMMUNITY LOST & FOUND MANAGEMENT SYSTEM  " << endl;
        cout << "============================================" << RESET << endl;

        cout << YELLOW << " [1]" << RESET << " Add New Item\n";
        cout << YELLOW << " [2]" << RESET << " Search Item (by Name)\n";
        cout << YELLOW << " [3]" << RESET << " Display All Items\n";
        cout << YELLOW << " [4]" << RESET << " Edit Item Details\n";
        cout << YELLOW << " [5]" << RESET << " Sort Items\n";
        cout << YELLOW << " [6]" << RESET << " Delete Item\n";
        cout << YELLOW << " [0]" << RESET << " Exit\n";
        cout << CYAN << "============================================" << RESET << endl;
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << RED << "Invalid input! Please enter a number." << RESET << "\n";
            cin.clear();
            string trash; getline(cin, trash);
            Pause();
            continue; // Skip this loop
        }
        cin.ignore(); // Clear the newline character from the buffer

        switch (choice) {
            case 1: {
                Clear_Screen();
                cout << CYAN << "--- ADD NEW ITEM ---" << RESET << "\n";
                string name, category, location, date, status;
                ++Item_Count;
                status = "Lost";

                cout << "Enter Item Name: ";
                getline(cin, name);
                cout << "Enter Category: ";
                getline(cin, category);
                cout << "Enter Location: ";
                getline(cin, location);
                cout << "Enter Date (YYYY-MM-DD): ";
                getline(cin, date);

                system.Add_Item(Item_Count, name, category, location, date, status);
                Pause();
                break;
            }
            case 2: {
                Clear_Screen();
                string searchName;
                cout << CYAN << "--- SEARCH ITEM ---" << RESET << "\n";
                cout << "Enter Name to Search: ";
                getline(cin, searchName);
                system.Search_Name(searchName);
                Pause();
                break;
            }
            case 3:
                Clear_Screen();
                cout << CYAN << "--- DISPLAY ALL ITEMS ---" << RESET << "\n";
                system.Display_All();
                Pause();
                break;
            case 4: {
                Clear_Screen();
                cout << CYAN << "--- EDIT ITEM ---" << RESET << "\n";
                int edit_Id;
                cout << "Enter ID of item to Edit: ";
                if (cin >> edit_Id) {
                    cin.ignore();
                    system.Edit(edit_Id);
                } else {
                    cout << RED << "Invalid ID." << RESET << "\n";
                    cin.clear(); string trash; getline(cin, trash);
                }
                Pause();
                break;
            }
            case 5:
                Clear_Screen();
                cout << CYAN << "--- SORT ITEMS ---" << RESET << "\n";
                system.Sort();
                Pause();
                break;
            case 6: {
                Clear_Screen();
                cout << CYAN << "--- DELETE ITEM ---" << RESET << "\n";
                int deleteId;
                cout << "Enter ID of item to Delete: ";
                if (cin >> deleteId) {
                      system.Delete(deleteId);
                } else {
                    cout << RED << "Invalid ID." << RESET << "\n";
                    cin.clear(); string trash; getline(cin, trash);
                }
                cin.ignore();
                Pause();
                break;
            }
            case 7:{
                ++Item_Count;
                system.Add_Item(Item_Count,"Blue Bag","School Bag", "MMU Canteen", "2026-01-27", "Lost");
                ++Item_Count;
                system.Add_Item(Item_Count, "A Red Wallet", "Personal", "Main Library", "2026-01-10", "Lost");
                ++Item_Count;
                system.Add_Item(Item_Count, "iPhone 15", "Electronics", "FCM Hall", "2026-01-12", "Found");
                ++Item_Count;
                system.Add_Item(Item_Count, "Water Bottle", "Personal", "Gymnasium", "2026-01-15", "Claimed");
                ++Item_Count;
                system.Add_Item(Item_Count, "Calculus Notebook", "Stationery", "CLC Class 2", "2026-01-18", "Lost");
                ++Item_Count;
                system.Add_Item(Item_Count, "Honda Car Keys", "Personal", "Parking Lot A", "2024-07-20", "Found");
                ++Item_Count;
                system.Add_Item(Item_Count, "Grey Hoodie", "Clothing", "Male Toilet", "2025-01-22", "Lost");
                ++Item_Count;
                system.Add_Item(Item_Count, "Laptop Charger", "Electronics", "Starbucks", "2026-01-25", "Lost");
                ++Item_Count;
                system.Add_Item(Item_Count, "Black Umbrella", "Personal", "Main Gate", "2026-01-26", "Found");
                ++Item_Count;
                system.Add_Item(Item_Count, "Scientific Calculator", "Stationery", "Exam Hall", "2026-03-27", "Claimed");
                break;
            }
            case 0:{
                cout << "\n" << GREEN << "Exiting system... Goodbye!" << RESET << "\n";
                break;
            }
            default:{
                cout << RED << "Invalid choice! Please try again." << RESET << "\n";
                Pause();
            }
        }
    } while (choice != 0);

    return 0;
}
