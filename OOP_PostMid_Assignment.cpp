#include <iostream>
using namespace std;

const int MAX_RES = 50;
const int MAX_MEM = 20;
const int MAX_BORROW = 10;

// ================= BASE CLASS =================
class LibraryResource {
protected:
    int resourceID;
    string title;
    string author;
    bool isAvailable;

public:
    LibraryResource() {}

    LibraryResource(int id, string t, string a, bool avail = true) {
        this->resourceID = id;   // this pointer used to distinguish between parameter and member variable
        this->title = t;
        this->author = a;
        this->isAvailable = avail;
    }

    void setID(int id){
        resourceID = id;
    }

    void setTitle(string title){
        this->title = title;
    }

    void setAuthor(string author){
        this->author = author;
    }

    void setAvailable(bool status) {
        this->isAvailable = status; // this pointer used to distinguish between parameter and member variable
    }

    int getID() { // getter for ID
        return resourceID; 
    }
    string getTitle(){
        return title;
    }
    string getAuthor(){
        return author;
    }
    bool getStatus() { // getter for Status
        return isAvailable; 
    }

    void displayDetails() {
        cout << "ID: " << resourceID << ", Title: " << title << ", Author: " << author << ", Available: " << (isAvailable ? "Yes" : "No") << endl;
    }

    int calculateLateFee(int daysLate) {
        return 0;
    }
};

// ================= DERIVED CLASSES =================
class Book : public LibraryResource {
    string ISBN;
    int pageCount;
public:
    Book(int id, string t, string a, string isbn, int pages) : LibraryResource(id, t, a) {
        ISBN = isbn;
        pageCount = pages;
    }
    void displayDetails(){
        LibraryResource :: displayDetails();
        cout << "ISBN: " << ISBN << ", Pages: " << pageCount << endl;
    }
    int calculateLateFee(int daysLate) {
        return daysLate * 5;
    }
};

class Magazine : public LibraryResource {
    int issueNumber;
public:
    Magazine(int id, string t, string a, int issue) : LibraryResource(id, t, a) {
        issueNumber = issue;
    }
    void displayDetails(){
        LibraryResource :: displayDetails();
        cout << "Issue Number: " << issueNumber << endl;
    }
    int calculateLateFee(int daysLate) {
        return daysLate * 3;
    }
};

class DVD : public LibraryResource {
    int duration;
public:
    DVD(int id, string t, string a, int d) : LibraryResource(id, t, a) {
        duration = d;
    }

    void displayDetails(){
        LibraryResource :: displayDetails();
        cout << "Duration: " << duration << " minutes" << endl;
    }
    int calculateLateFee(int daysLate) {
        return daysLate * 10;
    }
};

// ================= MEMBER CLASS =================
class LibraryMember {
    int memberID;
    string name;

    LibraryResource borrowedItems[MAX_BORROW];
    int count;

public:
    LibraryMember() { 
        count = 0; 
    }

    LibraryMember(int id, string n) {
        memberID = id;
        name = n;
        count = 0;
    }

    int getID() {
         return memberID; 
    }

    void borrowResource(LibraryResource res) {
        if (count < MAX_BORROW) {
            borrowedItems[count++] = res;
            cout << "Borrowed successfully\n";
        } else {
            cout << "Limit reached\n";
        }
    }

    void returnResource(int rid) { // resource id
        for (int i = 0; i < count; i++) {
            if (borrowedItems[i].getID() == rid) {
                for (int j = i; j < count - 1; j++) {
                    borrowedItems[j] = borrowedItems[j + 1];
                }
                count--;
                cout << "Returned successfully\n";
                return;
            }
        }
        cout << "Resource not found\n";
    }

    void displayBorrowedItems() {
        if (count == 0) {
            cout << "No borrowed items\n";
            return;
        }
        for (int i = 0; i < count; i++) {
            borrowedItems[i].displayDetails();
        }
    }

    int calculateTotalLateFee(int daysLate) {
        int total = 0;
        for (int i = 0; i < count; i++) {
            total += borrowedItems[i].calculateLateFee(daysLate);
        }
        return total;
    }

    friend void adminView(LibraryResource[], int, LibraryMember[], int);
};

// ================= FRIEND FUNCTION =================
void adminView(LibraryResource resources[], int rCount, LibraryMember members[], int mCount) {

    cout << "\n--- All Resources ---\n";
    for (int i = 0; i < rCount; i++) {
        resources[i].displayDetails();
    }

    cout << "\n--- Members ---\n";
    for (int i = 0; i < mCount; i++) {
        cout << "Member ID: " << members[i].getID() << endl;
        members[i].displayBorrowedItems();
    }
}

// ================= MAIN =================
int main() {
    LibraryResource resources[MAX_RES];
    LibraryMember members[MAX_MEM];

    int rCount = 0, mCount = 0;
    int choice;

    do {
        cout << "\n1.Add Resource\n2.Add Member\n3.Borrow\n4.Return\n5.Show Resources\n6.Show Member Items\n7.Calculate Fee\n8.Admin View\n9.Exit\n";
        cin >> choice;

        switch (choice) {

        case 1: {
            int id, type;
            string t, a;

            cout << "1.Book, 2.Magazine, 3.DVD: ";
            cin >> type;
            cout << "Enter ID Title Author: ";
            cin >> id >> t >> a;

            if (type == 1){
                string isbn;
                int pages;
                cout << "Enter ISBN and pages: ";
                cin >> isbn >> pages;
                resources[rCount++] = Book(id, t, a, isbn, pages);
            } 
            else if (type == 2){
                int issue;
                cout << "Enter Issue Number: ";
                cin >> issue;
            
                resources[rCount++] = Magazine(id, t, a, issue);
            } 
            else if (type == 3){
                int duration;
                cout << "Enter Duration: ";
                cin >> duration;
                resources[rCount++] = DVD(id, t, a, duration);
            }    
            break;
        }

        case 2: {
            int id;
            string name;
            cout << "Enter Member ID and Name: ";
            cin >> id >> name;

            members[mCount++] = LibraryMember(id, name);
            break;
        }

        case 3: {
            int mid, rid;
            cout << "Enter Member ID and Resource ID: ";
            cin >> mid >> rid;

            for (int i = 0; i < mCount; i++) {
                if (members[i].getID() == mid) {
                    for (int j = 0; j < rCount; j++) {
                        if (resources[j].getID() == rid && resources[j].getStatus()) {
                            members[i].borrowResource(resources[j]);
                            resources[j].setAvailable(false);
                        }
                    }
                }
            }
            break;
        }

        case 4: {
            int mid, rid;
            cout << "Enter Member ID and Resource ID: ";
            cin >> mid >> rid;

            for (int i = 0; i < mCount; i++) {
                if (members[i].getID() == mid) {
                    members[i].returnResource(rid);
                    for (int j = 0; j < rCount; j++) {
                        if (resources[j].getID() == rid) {
                            resources[j].setAvailable(true);
                        }
                    }
                }
            }
            break;
        }

        case 5: {
            for (int i = 0; i < rCount; i++) {
                resources[i].displayDetails();
            }
            break;
        }

        case 6: {
            int mid;
            cout << "Enter Member ID: ";
            cin >> mid;

            for (int i = 0; i < mCount; i++) {
                if (members[i].getID() == mid) {
                    members[i].displayBorrowedItems();
                }
            }
            break;
        }

        case 7: {
            int mid, days;
            cout << "Enter Member ID and days late: ";
            cin >> mid >> days;

            for (int i = 0; i < mCount; i++) {
                if (members[i].getID() == mid) {
                    cout << "Total Fee: "
                         << members[i].calculateTotalLateFee(days) << endl;
                }
            }
            break;
        }

        case 8:
            adminView(resources, rCount, members, mCount);
            break;

        case 9:
            cout << "Exiting...\n";
            break;

        default:
            cout << "Invalid choice\n";
        }

    } while (choice != 9);

    return 0;
}