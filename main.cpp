#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

using namespace std;

// =========================
// Book Class
// =========================
class Book {
private:
    int id;
    string title;
    string author;
    bool available;

public:
    Book(int id, string title, string author, bool available = true) {
        this->id = id;
        this->title = title;
        this->author = author;
        this->available = available;
    }

    int getId() const {
        return id;
    }

    string getTitle() const {
        return title;
    }

    string getAuthor() const {
        return author;
    }

    bool isAvailable() const {
        return available;
    }

    void setAvailability(bool status) {
        available = status;
    }

    void display() const {
        cout << "ID: " << id
             << " | Title: " << title
             << " | Author: " << author
             << " | Status: "
             << (available ? "Available" : "Issued")
             << endl;
    }
};

// =========================
// Member Class
// =========================
class Member {
private:
    int id;
    string name;

public:
    Member(int id, string name) {
        this->id = id;
        this->name = name;
    }

    int getId() const {
        return id;
    }

    string getName() const {
        return name;
    }

    void display() const {
        cout << "ID: " << id
             << " | Name: " << name << endl;
    }
};

// =========================
// Loan Class
// =========================
class Loan {
private:
    int bookId;
    int memberId;

public:
    Loan(int bookId, int memberId) {
        this->bookId = bookId;
        this->memberId = memberId;
    }

    int getBookId() const {
        return bookId;
    }

    int getMemberId() const {
        return memberId;
    }
};

// =========================
// Library Manager Class
// =========================
class LibraryManager {
private:
    vector<Book> books;
    vector<Member> members;
    vector<Loan> loans;

    string fileName = "library_records.txt";

public:

    // Check if book ID already exists
    bool bookExists(int id) {
        for (const Book& book : books) {
            if (book.getId() == id) {
                return true;
            }
        }
        return false;
    }

    // Check if member ID already exists
    bool memberExists(int id) {
        for (const Member& member : members) {
            if (member.getId() == id) {
                return true;
            }
        }
        return false;
    }

    // Find book
    Book* findBook(int id) {
        for (Book& book : books) {
            if (book.getId() == id) {
                return &book;
            }
        }
        return nullptr;
    }

    // Find member
    Member* findMember(int id) {
        for (Member& member : members) {
            if (member.getId() == id) {
                return &member;
            }
        }
        return nullptr;
    }

    // Check if a loan exists
    int findLoan(int bookId) {
        for (int i = 0; i < loans.size(); i++) {
            if (loans[i].getBookId() == bookId) {
                return i;
            }
        }
        return -1;
    }

    // Add book
    void addBook() {
        int id;
        string title, author;

        cout << "\nEnter Book ID: ";
        cin >> id;

        if (bookExists(id)) {
            cout << "Error: Book ID already exists.\n";
            return;
        }

        cin.ignore();

        cout << "Enter Book Title: ";
        getline(cin, title);

        cout << "Enter Author Name: ";
        getline(cin, author);

        books.push_back(Book(id, title, author));

        saveData();

        cout << "Book added successfully.\n";
    }

    // Add member
    void addMember() {
        int id;
        string name;

        cout << "\nEnter Member ID: ";
        cin >> id;

        if (memberExists(id)) {
            cout << "Error: Member ID already exists.\n";
            return;
        }

        cin.ignore();

        cout << "Enter Member Name: ";
        getline(cin, name);

        members.push_back(Member(id, name));

        saveData();

        cout << "Member added successfully.\n";
    }

    // Display all books
    void displayBooks() {
        if (books.empty()) {
            cout << "\nNo books found.\n";
            return;
        }

        cout << "\n========== BOOKS ==========\n";

        for (const Book& book : books) {
            book.display();
        }
    }

    // Display all members
    void displayMembers() {
        if (members.empty()) {
            cout << "\nNo members found.\n";
            return;
        }

        cout << "\n========== MEMBERS ==========\n";

        for (const Member& member : members) {
            member.display();
        }
    }

    // Search book
    void searchBook() {
        string keyword;

        cin.ignore();

        cout << "\nEnter title or author to search: ";
        getline(cin, keyword);

        bool found = false;

        for (const Book& book : books) {
            if (book.getTitle().find(keyword) != string::npos ||
                book.getAuthor().find(keyword) != string::npos) {

                book.display();
                found = true;
            }
        }

        if (!found) {
            cout << "No matching book found.\n";
        }
    }

    // Check availability
    void checkAvailability() {
        int id;

        cout << "\nEnter Book ID: ";
        cin >> id;

        Book* book = findBook(id);

        if (book == nullptr) {
            cout << "Book not found.\n";
            return;
        }

        if (book->isAvailable()) {
            cout << "Book is AVAILABLE.\n";
        } else {
            cout << "Book is currently ISSUED.\n";
        }
    }

    // Issue book
    void issueBook() {
        int bookId, memberId;

        cout << "\nEnter Book ID: ";
        cin >> bookId;

        Book* book = findBook(bookId);

        if (book == nullptr) {
            cout << "Book not found.\n";
            return;
        }

        if (!book->isAvailable()) {
            cout << "Book is already issued.\n";
            return;
        }

        cout << "Enter Member ID: ";
        cin >> memberId;

        Member* member = findMember(memberId);

        if (member == nullptr) {
            cout << "Member not found.\n";
            return;
        }

        book->setAvailability(false);

        loans.push_back(Loan(bookId, memberId));

        saveData();

        cout << "Book issued successfully to "
             << member->getName() << ".\n";
    }

    // Return book
    void returnBook() {
        int bookId;

        cout << "\nEnter Book ID: ";
        cin >> bookId;

        Book* book = findBook(bookId);

        if (book == nullptr) {
            cout << "Book not found.\n";
            return;
        }

        int loanIndex = findLoan(bookId);

        if (loanIndex == -1) {
            cout << "This book is not currently issued.\n";
            return;
        }

        book->setAvailability(true);

        loans.erase(loans.begin() + loanIndex);

        saveData();

        cout << "Book returned successfully.\n";
    }

    // Save data
    void saveData() {
        ofstream file(fileName);

        if (!file) {
            cout << "Error: Unable to save data.\n";
            return;
        }

        // Save books
        file << "[BOOKS]\n";

        for (const Book& book : books) {
            file << book.getId() << "|"
                 << book.getTitle() << "|"
                 << book.getAuthor() << "|"
                 << book.isAvailable() << "\n";
        }

        // Save members
        file << "[MEMBERS]\n";

        for (const Member& member : members) {
            file << member.getId() << "|"
                 << member.getName() << "\n";
        }

        // Save loans
        file << "[LOANS]\n";

        for (const Loan& loan : loans) {
            file << loan.getBookId() << "|"
                 << loan.getMemberId() << "\n";
        }

        file.close();
    }

    // Load data
    void loadData() {
        ifstream file(fileName);

        if (!file) {
            cout << "No previous records found. Starting with empty library.\n";
            return;
        }

        string line;
        string section;

        while (getline(file, line)) {

            if (line == "[BOOKS]" ||
                line == "[MEMBERS]" ||
                line == "[LOANS]") {

                section = line;
                continue;
            }

            if (line.empty()) {
                continue;
            }

            stringstream ss(line);
            string part;

            if (section == "[BOOKS]") {

                int id;
                string title;
                string author;
                bool available;

                getline(ss, part, '|');
                id = stoi(part);

                getline(ss, title, '|');
                getline(ss, author, '|');

                getline(ss, part, '|');
                available = stoi(part);

                books.push_back(
                    Book(id, title, author, available)
                );
            }

            else if (section == "[MEMBERS]") {

                int id;
                string name;

                getline(ss, part, '|');
                id = stoi(part);

                getline(ss, name, '|');

                members.push_back(
                    Member(id, name)
                );
            }

            else if (section == "[LOANS]") {

                int bookId;
                int memberId;

                getline(ss, part, '|');
                bookId = stoi(part);

                getline(ss, part, '|');
                memberId = stoi(part);

                loans.push_back(
                    Loan(bookId, memberId)
                );
            }
        }

        file.close();

        cout << "Previous records loaded successfully.\n";
    }

    // Main menu
    void run() {

        loadData();

        int choice;

        do {
            cout << "\n====================================\n";
            cout << "     OBJECT-ORIENTED LIBRARY\n";
            cout << "          MANAGEMENT SYSTEM\n";
            cout << "====================================\n";

            cout << "1. Add Book\n";
            cout << "2. Add Member\n";
            cout << "3. Display Books\n";
            cout << "4. Display Members\n";
            cout << "5. Search Book\n";
            cout << "6. Check Availability\n";
            cout << "7. Issue Book\n";
            cout << "8. Return Book\n";
            cout << "9. Exit\n";

            cout << "------------------------------------\n";
            cout << "Enter your choice: ";
            cin >> choice;

            switch (choice) {

                case 1:
                    addBook();
                    break;

                case 2:
                    addMember();
                    break;

                case 3:
                    displayBooks();
                    break;

                case 4:
                    displayMembers();
                    break;

                case 5:
                    searchBook();
                    break;

                case 6:
                    checkAvailability();
                    break;

                case 7:
                    issueBook();
                    break;

                case 8:
                    returnBook();
                    break;

                case 9:
                    saveData();
                    cout << "\nThank you for using the Library Manager!\n";
                    break;

                default:
                    cout << "Invalid choice. Please try again.\n";
            }

        } while (choice != 9);
    }
};

// =========================
// Main Function
// =========================

int main() {

    LibraryManager library;

    library.run();

    return 0;
}
