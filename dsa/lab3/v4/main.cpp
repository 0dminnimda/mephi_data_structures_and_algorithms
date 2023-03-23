// To create a console application to work with the library table interactively with MVC, we can follow the Model-View-Controller design pattern. 

// First, we need to implement the `Table` class and its derived classes to provide the necessary functionality for manipulating the key spaces. We can create a derived class for each type of table implementation (in-memory table, external memory table, etc.), and use polymorphism to allow the application to work with any type of table.

// Next, we need to implement the user interface, which will consist of a console-based menu system that allows the user to interact with the table. We can create a `View` class to handle the input and output of the user interface, and use the `Table` classes to perform the necessary operations on the key spaces.

// Finally, we need to create a `Controller` class that serves as the intermediary between the `View` and `Table` classes. The `Controller` will receive input from the `View`, perform the necessary operations on the `Table`, and then pass the results back to the `View` for display.

// Here's a possible implementation of the `View` class:

// #include "table.hpp"
#include "table_mem_vec_iter.hpp"
#include <iostream>

class View {
public:
    void show_menu() const {
        std::cout << "Menu:" << std::endl;
        std::cout << "1. Insert key space" << std::endl;
        std::cout << "2. Remove key space" << std::endl;
        std::cout << "3. Search for key space" << std::endl;
        std::cout << "4. Output table" << std::endl;
        std::cout << "5. Import table from file" << std::endl;
        std::cout << "6. Remove key spaces by parent key" << std::endl;
        std::cout << "7. Search key spaces by parent key" << std::endl;
        std::cout << "0. Exit" << std::endl;
    }

    void show_error(const std::string& message) const {
        std::cerr << "Error: " << message << std::endl;
    }

    void show_success(const std::string& message) const {
        std::cout << "Success: " << message << std::endl;
    }

    KeyType get_key() const {
        KeyType key;
        std::cout << "Enter key: ";
        std::cin >> key;
        return key;
    }

    InfoType* get_info() const {
        InfoType* info = new InfoType;
        std::cout << "Enter info: ";
        std::cin >> *info;
        return info;
    }

    KeyType get_parent_key() const {
        KeyType parent_key;
        std::cout << "Enter parent key [enter for absent parent]: ";
        std::cin >> parent_key;
        return parent_key;
    }

    void output_table(const Table& table) const {
        std::cout << "Table:" << std::endl;
        table.output(std::cout);
    }

    std::string get_filename() const {
        std::string filename;
        std::cout << "Enter filename: ";
        std::cin >> filename;
        return filename;
    }

    void show_search_result(SearchIterator& it) const {
        if (it != it.end()) {
            std::cout << "Found key space with key " << (*it).key << " and info " << *((*it).info) << std::endl;
        } else {
            std::cout << "Key space not found" << std::endl;
        }
    }

    void show_table_result(Table* table) const {
        if (table != nullptr) {
            output_table(*table);
        } else {
            std::cout << "No key spaces found with specified parent key" << std::endl;
        }
    }
};

// The `View` class provides methods for displaying the menu, showing error and success messages, getting input from the user (key, info, parent key, filename), outputting the table, and displaying search and table results.

// Here's a possible implementation of the `Controller` class:

class Controller {
public:
    Controller(Table& table, View& view) : table_(table), view_(view) {}

    void run() {
        int choice;
        do {
            view_.show_menu();
            std::cout << "Enter choice: ";
            std::cin >> choice;
            switch (choice) {
                case 1:
                    insert_key_space();
                    break;
                case 2:
                    remove_key_space();
                    break;
                case 3:
                    search_for_key_space();
                    break;
                case 4:
                    output_table();
                    break;
                case 5:
                    import_table();
                    break;
                case 6:
                    remove_key_spaces_by_parent_key();
                    break;
                case 7:
                    search_key_spaces_by_parent_key();
                    break;
                case 0:
                    std::cout << "Exiting...";
                    break;
                default:
                    view_.show_error("Invalid choice");
                    break;
            }
        } while (choice != 0);
    }

private:
    Table& table_;
    View& view_;

    void insert_key_space() {
        KeyType key = view_.get_key();
        InfoType* info = view_.get_info();
        if (table_.insert(key, info)) {
            view_.show_success("Key space inserted");
        } else {
            view_.show_error("Failed to insert key space");
        }
    }

    void remove_key_space() {
        KeyType key = view_.get_key();
        if (table_.remove(key)) {
            view_.show_success("Key space removed");
        } else {
            view_.show_error("Failed to remove key space");
        }
    }

    void search_for_key_space() {
        KeyType key = view_.get_key();
        SearchIterator it = table_.search(key);
        view_.show_search_result(it);
    }

    void output_table() {
        view_.output_table(table_);
    }

    void import_table() {
        std::string filename = view_.get_filename();
        if (table_.import(filename)) {
            view_.show_success("Table imported from file");
        } else {
            view_.show_error("Failed to import table from file");
        }
    }

    void remove_key_spaces_by_parent_key() {
        KeyType parent_key = view_.get_parent_key();
        if (table_.removeByParentKey(parent_key)) {
            view_.show_success("Key spaces removed by parent key");
        } else {
            view_.show_error("Failed to remove key spaces by parent key");
        }
    }

    void search_key_spaces_by_parent_key() {
        KeyType parent_key = view_.get_parent_key();
        Table* table = table_.searchByParentKey(parent_key);
        view_.show_table_result(table);
        delete table;
    }
};

// The `Controller` class provides methods for handling each menu option: inserting a key space, removing a key space, searching for a key space, outputting the table, importing the table from a file, removing key spaces by parent key, and searching for key spaces by parent key. Each method calls the appropriate method in the `Table` class and passes the results to the `View` for display.

// Finally, we can create the `main` function to instantiate the necessary objects and start the application:

int main() {
    const IndexType MAX_SIZE = 1000;
    Table* table = new InMemoryTable(MAX_SIZE);  // instantiate the table object
    View view;
    Controller controller(*table, view);  // instantiate the controller object
    controller.run();  // start the application
    delete table;
    return 0;
}

// In this example, we've instantiated an `InMemoryTable` object as the `Table` implementation, but we could easily substitute this with any other implementation derived from the `Table` class.
