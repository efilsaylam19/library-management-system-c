#include "library.h"
#include <ctype.h>

// Helper function to clear input buffer
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Get string input with validation
int get_string_input(char *buffer, int max_len, const char *prompt) {
    printf("%s", prompt);
    if (fgets(buffer, max_len, stdin) == NULL) {
        return 0;
    }
    
    // Remove newline if present
    size_t len = strlen(buffer);
    if (len > 0 && buffer[len - 1] == '\n') {
        buffer[len - 1] = '\0';
        len--;
    }
    
    if (len == 0) {
        return 0;
    }
    
    return 1;
}

// Get integer input with validation
int get_int_input(int *value, const char *prompt) {
    char buffer[20];
    printf("%s", prompt);
    
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        return 0;
    }
    
    if (sscanf(buffer, "%d", value) != 1) {
        return 0;
    }
    
    return 1;
}

// Add book menu
void menu_add_book(Library *lib) {
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
    int year;
    
    printf("\n=== ADD NEW BOOK ===\n");
    
    if (!get_string_input(title, MAX_TITLE_LEN, "Enter title: ")) {
        printf("Error: Invalid title.\n");
        return;
    }
    
    if (!get_string_input(author, MAX_AUTHOR_LEN, "Enter author: ")) {
        printf("Error: Invalid author.\n");
        return;
    }
    
    if (!get_string_input(isbn, MAX_ISBN_LEN, "Enter ISBN: ")) {
        printf("Error: Invalid ISBN.\n");
        return;
    }
    
    if (!get_int_input(&year, "Enter publication year: ")) {
        printf("Error: Invalid year.\n");
        return;
    }
    
    int result = library_add_book(lib, title, author, isbn, year);
    if (result > 0) {
        printf("Book added successfully with ID: %d\n", result);
        library_save_to_file(lib, FILENAME);
    } else if (result == -1) {
        printf("Error: ISBN already exists.\n");
    } else {
        printf("Error: Failed to add book. Please check your input.\n");
    }
}

// Search book menu
void menu_search_book(Library *lib) {
    int choice;
    char search_term[MAX_TITLE_LEN];
    Book *book = NULL;
    
    printf("\n=== SEARCH BOOK ===\n");
    printf("1. Search by ID\n");
    printf("2. Search by Title\n");
    printf("3. Search by Author\n");
    printf("4. Search by ISBN\n");
    printf("Enter choice: ");
    
    if (!get_int_input(&choice, "")) {
        printf("Invalid input.\n");
        return;
    }
    
    switch (choice) {
        case 1: {
            int id;
            if (!get_int_input(&id, "Enter book ID: ")) {
                printf("Invalid ID.\n");
                return;
            }
            book = library_find_book_by_id(lib, id);
            break;
        }
        case 2:
            if (!get_string_input(search_term, MAX_TITLE_LEN, "Enter title: ")) {
                printf("Invalid title.\n");
                return;
            }
            book = library_find_book_by_title(lib, search_term);
            break;
        case 3:
            if (!get_string_input(search_term, MAX_AUTHOR_LEN, "Enter author: ")) {
                printf("Invalid author.\n");
                return;
            }
            book = library_find_book_by_author(lib, search_term);
            break;
        case 4:
            if (!get_string_input(search_term, MAX_ISBN_LEN, "Enter ISBN: ")) {
                printf("Invalid ISBN.\n");
                return;
            }
            book = library_find_book_by_isbn(lib, search_term);
            break;
        default:
            printf("Invalid choice.\n");
            return;
    }
    
    if (book != NULL) {
        printf("\nBook found:\n");
        printf("----------------------------------------\n");
        library_display_book(book);
        printf("----------------------------------------\n");
    } else {
        printf("Book not found.\n");
    }
}

// Update book menu
void menu_update_book(Library *lib) {
    int id;
    char title[MAX_TITLE_LEN];
    char author[MAX_AUTHOR_LEN];
    char isbn[MAX_ISBN_LEN];
    int year;
    
    printf("\n=== UPDATE BOOK ===\n");
    
    if (!get_int_input(&id, "Enter book ID to update: ")) {
        printf("Invalid ID.\n");
        return;
    }
    
    Book *book = library_find_book_by_id(lib, id);
    if (book == NULL) {
        printf("Book not found.\n");
        return;
    }
    
    printf("Current book information:\n");
    library_display_book(book);
    printf("\nEnter new information (press Enter to keep current value):\n");
    
    printf("Enter new title: ");
    if (fgets(title, MAX_TITLE_LEN, stdin) != NULL) {
        size_t len = strlen(title);
        if (len > 0 && title[len - 1] == '\n') {
            title[len - 1] = '\0';
            len--;
        }
        if (len == 0) {
            strcpy(title, book->title);
        }
    } else {
        strcpy(title, book->title);
    }
    
    printf("Enter new author: ");
    if (fgets(author, MAX_AUTHOR_LEN, stdin) != NULL) {
        size_t len = strlen(author);
        if (len > 0 && author[len - 1] == '\n') {
            author[len - 1] = '\0';
            len--;
        }
        if (len == 0) {
            strcpy(author, book->author);
        }
    } else {
        strcpy(author, book->author);
    }
    
    printf("Enter new ISBN: ");
    if (fgets(isbn, MAX_ISBN_LEN, stdin) != NULL) {
        size_t len = strlen(isbn);
        if (len > 0 && isbn[len - 1] == '\n') {
            isbn[len - 1] = '\0';
            len--;
        }
        if (len == 0) {
            strcpy(isbn, book->isbn);
        }
    } else {
        strcpy(isbn, book->isbn);
    }
    
    printf("Enter new year: ");
    char year_str[20];
    if (fgets(year_str, sizeof(year_str), stdin) != NULL) {
        if (sscanf(year_str, "%d", &year) != 1) {
            year = book->year;
        }
    } else {
        year = book->year;
    }
    
    int result = book_update(lib, id, title, author, isbn, year);
    if (result > 0) {
        printf("Book updated successfully.\n");
        library_save_to_file(lib, FILENAME);
    } else if (result == -1) {
        printf("Error: Book not found.\n");
    } else if (result == -2) {
        printf("Error: ISBN already exists for another book.\n");
    } else {
        printf("Error: Failed to update book. Please check your input.\n");
    }
}

// Delete book menu
void menu_delete_book(Library *lib) {
    int id;
    
    printf("\n=== DELETE BOOK ===\n");
    
    if (!get_int_input(&id, "Enter book ID to delete: ")) {
        printf("Invalid ID.\n");
        return;
    }
    
    Book *book = library_find_book_by_id(lib, id);
    if (book == NULL) {
        printf("Book not found.\n");
        return;
    }
    
    printf("Book to delete:\n");
    library_display_book(book);
    
    printf("Are you sure you want to delete this book? (y/n): ");
    char confirm;
    if (scanf(" %c", &confirm) != 1) {
        clear_input_buffer();
        return;
    }
    clear_input_buffer();
    
    if (tolower(confirm) == 'y') {
        if (library_remove_book(lib, id)) {
            printf("Book deleted successfully.\n");
            library_save_to_file(lib, FILENAME);
        } else {
            printf("Error: Failed to delete book.\n");
        }
    } else {
        printf("Deletion cancelled.\n");
    }
}

// Borrow book menu
void menu_borrow_book(Library *lib, const User *logged_user) {
    int book_id;
    
    printf("\n=== BORROW BOOK ===\n");
    
    // First, display all available books
    printf("\n--- Available Books ---\n");
    library_display_available(lib);
    
    if (!get_int_input(&book_id, "Enter book ID to borrow: ")) {
        printf("Invalid book ID.\n");
        return;
    }
    
    int result = book_borrow(lib, book_id, logged_user->id);
    if (result > 0) {
        printf("Book borrowed successfully by %s (ID: %d).\n", logged_user->name, logged_user->id);
        library_save_to_file(lib, FILENAME);
    } else if (result == -1) {
        printf("Error: Book not found.\n");
    } else if (result == -2) {
        printf("Error: Book is already borrowed by another user.\n");
    } else if (result == -3) {
        printf("Error: Failed to update user borrowing record.\n");
    } else {
        printf("Error: Failed to borrow book.\n");
    }
}

// Return book menu
void menu_return_book(Library *lib, const User *logged_user) {
    int book_id;
    
    printf("\n=== RETURN BOOK ===\n");
    
    // First, display user's borrowed books
    printf("\n--- Your Borrowed Books ---\n");
    user_borrow_record_display(logged_user->id);
    
    if (!get_int_input(&book_id, "Enter book ID to return: ")) {
        printf("Invalid book ID.\n");
        return;
    }
    
    int result = book_return(lib, book_id, logged_user->id);
    if (result > 0) {
        printf("Book returned successfully by %s (ID: %d).\n", logged_user->name, logged_user->id);
        library_save_to_file(lib, FILENAME);
    } else if (result == -1) {
        printf("Error: Book not found.\n");
    } else if (result == -2) {
        printf("Error: Book is already available.\n");
    } else if (result == -3) {
        printf("Error: This book is not borrowed by you.\n");
    } else if (result == -4) {
        printf("Error: Failed to update user borrowing record.\n");
    } else {
        printf("Error: Failed to return book.\n");
    }
}

// Add user menu (admin only)
void menu_add_user(UserManager *um) {
    char username[MAX_USERNAME_LEN];
    char name[MAX_NAME_LEN];
    char password[MAX_PASSWORD_LEN];
    
    printf("\n=== ADD NEW USER ===\n");
    
    if (!get_string_input(username, MAX_USERNAME_LEN, "Enter username: ")) {
        printf("Error: Invalid username.\n");
        return;
    }
    
    if (!get_string_input(name, MAX_NAME_LEN, "Enter full name: ")) {
        printf("Error: Invalid name.\n");
        return;
    }
    
    if (!get_string_input(password, MAX_PASSWORD_LEN, "Enter password: ")) {
        printf("Error: Invalid password.\n");
        return;
    }
    
    int result = user_register(um, username, name, password);
    if (result > 0) {
        printf("User added successfully with ID: %d\n", result);
    } else if (result == -1) {
        printf("Error: Username already exists.\n");
    } else if (result == -2) {
        printf("Error: Cannot register as admin.\n");
    } else {
        printf("Error: Failed to add user. Please check your input.\n");
    }
}

// Display all users menu
void menu_display_users(UserManager *um) {
    if (um == NULL) {
        printf("User manager is NULL.\n");
        return;
    }
    
    if (um->count == 0) {
        printf("No users registered.\n");
        return;
    }
    
    printf("\n=== ALL USERS ===\n");
    printf("%-5s %-20s %-30s\n", "ID", "Username", "Name");
    printf("------------------------------------------------------------\n");
    
    for (int i = 0; i < um->count; i++) {
        printf("%-5d %-20s %-30s\n",
               um->users[i].id,
               um->users[i].username,
               um->users[i].name);
    }
    printf("\n");
}

// Display user's borrowed books menu
void menu_display_user_books(const User *logged_user, bool is_admin_user) {
    int user_id;
    
    printf("\n=== VIEW BORROWED BOOKS ===\n");
    
    if (is_admin_user) {
        if (!get_int_input(&user_id, "Enter user ID (0 for your own): ")) {
            printf("Invalid user ID.\n");
            return;
        }
        if (user_id == 0) {
            user_id = logged_user->id;
        }
    } else {
        user_id = logged_user->id;
    }
    
    printf("\nUser ID: %d\n", user_id);
    user_borrow_record_display(user_id);
}

// Admin menu
void display_admin_menu(const User *user) {
    printf("\n========================================\n");
    printf("    LIBRARY MANAGEMENT SYSTEM\n");
    printf("    ADMIN PANEL - Welcome %s\n", user->name);
    printf("========================================\n");
    printf("BOOK MANAGEMENT:\n");
    printf("  1. Add Book\n");
    printf("  2. Search Book\n");
    printf("  3. Update Book\n");
    printf("  4. Delete Book\n");
    printf("  5. Display All Books\n");
    printf("  6. Display Available Books\n");
    printf("  7. Display Borrowed Books\n");
    printf("\nUSER MANAGEMENT:\n");
    printf("  8. Add User\n");
    printf("  9. Display All Users\n");
    printf("\nBORROWING:\n");
    printf("  10. View User's Borrowed Books\n");
    printf("\n  11. Logout\n");
    printf("  12. Exit\n");
    printf("========================================\n");
}

// User menu
void display_user_menu(const User *user) {
    printf("\n========================================\n");
    printf("    LIBRARY MANAGEMENT SYSTEM\n");
    printf("    Welcome %s (ID: %d)\n", user->name, user->id);
    printf("========================================\n");
    printf("BOOK OPERATIONS:\n");
    printf("  1. Search Book\n");
    printf("  2. Display All Books\n");
    printf("  3. Display Available Books\n");
    printf("\nBORROWING:\n");
    printf("  4. Borrow Book\n");
    printf("  5. Return Book\n");
    printf("  6. View My Borrowed Books\n");
    printf("\n  7. Logout\n");
    printf("  8. Exit\n");
    printf("========================================\n");
}

// Login screen
int login_screen(UserManager *um, User **logged_user) {
    int choice;
    char username[MAX_USERNAME_LEN];
    char password[MAX_PASSWORD_LEN];
    char name[MAX_NAME_LEN];
    
    while (1) {
        printf("\n========================================\n");
        printf("    LIBRARY MANAGEMENT SYSTEM\n");
        printf("    LOGIN / REGISTER\n");
        printf("========================================\n");
        printf("1. Login\n");
        printf("2. Register\n");
        printf("3. Admin Login\n");
        printf("4. Exit\n");
        printf("========================================\n");
        printf("Enter your choice: ");
        
        if (!get_int_input(&choice, "")) {
            printf("Invalid input. Please enter a number.\n");
            clear_input_buffer();
            continue;
        }
        
        switch (choice) {
            case 1: // Login
                printf("\n=== LOGIN ===\n");
                if (!get_string_input(username, MAX_USERNAME_LEN, "Username: ")) {
                    printf("Invalid username.\n");
                    break;
                }
                if (!get_string_input(password, MAX_PASSWORD_LEN, "Password: ")) {
                    printf("Invalid password.\n");
                    break;
                }
                
                if (user_login(um, username, password, logged_user)) {
                    printf("\nLogin successful! Welcome %s.\n", (*logged_user)->name);
                    return 1;
                } else {
                    printf("Invalid username or password.\n");
                }
                break;
                
            case 2: // Register
                printf("\n=== REGISTER ===\n");
                if (!get_string_input(username, MAX_USERNAME_LEN, "Username: ")) {
                    printf("Invalid username.\n");
                    break;
                }
                if (!get_string_input(name, MAX_NAME_LEN, "Full Name: ")) {
                    printf("Invalid name.\n");
                    break;
                }
                if (!get_string_input(password, MAX_PASSWORD_LEN, "Password: ")) {
                    printf("Invalid password.\n");
                    break;
                }
                
                int result = user_register(um, username, name, password);
                if (result > 0) {
                    printf("Registration successful! Your user ID is: %d\n", result);
                    printf("Please login to continue.\n");
                } else if (result == -1) {
                    printf("Error: Username already exists.\n");
                } else if (result == -2) {
                    printf("Error: Cannot register as admin.\n");
                } else {
                    printf("Error: Registration failed.\n");
                }
                break;
                
            case 3: // Admin Login
                printf("\n=== ADMIN LOGIN ===\n");
                if (!get_string_input(username, MAX_USERNAME_LEN, "Admin Username: ")) {
                    printf("Invalid username.\n");
                    break;
                }
                if (!get_string_input(password, MAX_PASSWORD_LEN, "Admin Password: ")) {
                    printf("Invalid password.\n");
                    break;
                }
                
                if (user_login(um, username, password, logged_user)) {
                    if (is_admin(*logged_user)) {
                        printf("\nAdmin login successful! Welcome %s.\n", (*logged_user)->name);
                        return 1;
                    } else {
                        printf("Access denied. Not an admin account.\n");
                    }
                } else {
                    printf("Invalid admin credentials.\n");
                }
                break;
                
            case 4: // Exit
                return 0;
                
            default:
                printf("Invalid choice. Please try again.\n");
                break;
        }
    }
}

int main(void) {
    Library *lib = library_create();
    if (lib == NULL) {
        fprintf(stderr, "Error: Failed to create library.\n");
        return 1;
    }
    
    UserManager *um = user_manager_create();
    if (um == NULL) {
        fprintf(stderr, "Error: Failed to create user manager.\n");
        library_destroy(lib);
        return 1;
    }
    
    // Load existing data from files
    if (library_load_from_file(lib, FILENAME)) {
        printf("Library data loaded successfully.\n");
    } else {
        printf("Starting with empty library.\n");
        // Try to load from books.txt if library.dat doesn't exist
        int loaded = library_load_from_txt(lib, "books.txt");
        if (loaded > 0) {
            printf("Loaded %d books from books.txt\n", loaded);
            library_save_to_file(lib, FILENAME); // Save to binary format
        }
    }
    
    if (user_load_from_file(um, USERS_FILENAME)) {
        printf("User data loaded successfully.\n");
    } else {
        printf("Starting with no users.\n");
    }
    
    User *logged_user = NULL;
    bool system_running = true;
    
    while (system_running) {
        // Login screen
        if (logged_user == NULL) {
            if (!login_screen(um, &logged_user)) {
                // User chose to exit
                system_running = false;
                break;
            }
        }
        
        // Main application loop
        bool session_running = true;
        bool is_admin_user = is_admin(logged_user);
        
        while (session_running && system_running) {
            int choice;
            
            if (is_admin_user) {
                display_admin_menu(logged_user);
            } else {
                display_user_menu(logged_user);
            }
            
            printf("Enter your choice: ");
            
            if (!get_int_input(&choice, "")) {
                printf("Invalid input. Please enter a number.\n");
                clear_input_buffer();
                continue;
            }
            
            if (is_admin_user) {
                // Admin menu
                switch (choice) {
                    case 1:
                        menu_add_book(lib);
                        break;
                    case 2:
                        menu_search_book(lib);
                        break;
                    case 3:
                        menu_update_book(lib);
                        break;
                    case 4:
                        menu_delete_book(lib);
                        break;
                    case 5:
                        library_display_all(lib);
                        break;
                    case 6:
                        library_display_available(lib);
                        break;
                    case 7:
                        library_display_borrowed(lib);
                        break;
                    case 8:
                        menu_add_user(um);
                        break;
                    case 9:
                        menu_display_users(um);
                        break;
                    case 10:
                        menu_display_user_books(logged_user, true);
                        break;
                    case 11:
                        printf("Logging out...\n");
                        logged_user = NULL;
                        session_running = false;
                        break;
                    case 12:
                        printf("Saving data and exiting...\n");
                        library_save_to_file(lib, FILENAME);
                        user_save_to_file(um, USERS_FILENAME);
                        session_running = false;
                        system_running = false;
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                        break;
                }
            } else {
                // Regular user menu
                switch (choice) {
                    case 1:
                        menu_search_book(lib);
                        break;
                    case 2:
                        library_display_all(lib);
                        break;
                    case 3:
                        library_display_available(lib);
                        break;
                    case 4:
                        menu_borrow_book(lib, logged_user);
                        break;
                    case 5:
                        menu_return_book(lib, logged_user);
                        break;
                    case 6:
                        menu_display_user_books(logged_user, false);
                        break;
                    case 7:
                        printf("Logging out...\n");
                        logged_user = NULL;
                        session_running = false;
                        break;
                    case 8:
                        printf("Saving data and exiting...\n");
                        library_save_to_file(lib, FILENAME);
                        user_save_to_file(um, USERS_FILENAME);
                        session_running = false;
                        system_running = false;
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                        break;
                }
            }
            
            if (session_running) {
                printf("\nPress Enter to continue...");
                clear_input_buffer();
            }
        }
    }
    
    library_destroy(lib);
    user_manager_destroy(um);
    printf("Thank you for using Library Management System!\n");
    return 0;
}

