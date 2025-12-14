#include "library.h"

// Create a new library instance
Library* library_create(void) {
    Library *lib = (Library*)malloc(sizeof(Library));
    if (lib == NULL) {
        return NULL;
    }
    
    lib->capacity = 10;
    lib->count = 0;
    lib->books = (Book*)malloc(lib->capacity * sizeof(Book));
    
    if (lib->books == NULL) {
        free(lib);
        return NULL;
    }
    
    return lib;
}

// Destroy library and free all memory
void library_destroy(Library *lib) {
    if (lib != NULL) {
        if (lib->books != NULL) {
            free(lib->books);
        }
        free(lib);
    }
}

// Resize library capacity if needed
static int library_resize(Library *lib) {
    if (lib->count >= lib->capacity) {
        int new_capacity = lib->capacity * 2;
        Book *new_books = (Book*)realloc(lib->books, new_capacity * sizeof(Book));
        
        if (new_books == NULL) {
            return 0; // Failed to resize
        }
        
        lib->books = new_books;
        lib->capacity = new_capacity;
    }
    return 1;
}

// Add a new book to the library
int library_add_book(Library *lib, const char *title, const char *author,
                     const char *isbn, int year) {
    if (lib == NULL || title == NULL || author == NULL || isbn == NULL) {
        return 0; // Invalid parameters
    }
    
    // Validate inputs
    if (!validate_string(title, MAX_TITLE_LEN) || 
        !validate_string(author, MAX_AUTHOR_LEN) ||
        !validate_string(isbn, MAX_ISBN_LEN) ||
        !validate_year(year)) {
        return 0;
    }
    
    // Check if ISBN already exists
    if (library_find_book_by_isbn(lib, isbn) != NULL) {
        return -1; // ISBN already exists
    }
    
    // Resize if needed
    if (!library_resize(lib)) {
        return 0; // Memory allocation failed
    }
    
    // Create new book
    Book *new_book = &lib->books[lib->count];
    new_book->id = get_next_id(lib);
    strncpy(new_book->title, title, MAX_TITLE_LEN - 1);
    new_book->title[MAX_TITLE_LEN - 1] = '\0';
    strncpy(new_book->author, author, MAX_AUTHOR_LEN - 1);
    new_book->author[MAX_AUTHOR_LEN - 1] = '\0';
    strncpy(new_book->isbn, isbn, MAX_ISBN_LEN - 1);
    new_book->isbn[MAX_ISBN_LEN - 1] = '\0';
    new_book->year = year;
    new_book->is_available = true;
    new_book->borrower_id = 0;  // No borrower initially
    
    lib->count++;
    return new_book->id;
}

// Remove a book from the library
int library_remove_book(Library *lib, int id) {
    if (lib == NULL) {
        return 0;
    }
    
    for (int i = 0; i < lib->count; i++) {
        if (lib->books[i].id == id) {
            // Shift remaining books to fill the gap
            for (int j = i; j < lib->count - 1; j++) {
                lib->books[j] = lib->books[j + 1];
            }
            lib->count--;
            return 1; // Success
        }
    }
    
    return 0; // Book not found
}

// Find book by ID
Book* library_find_book_by_id(Library *lib, int id) {
    if (lib == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < lib->count; i++) {
        if (lib->books[i].id == id) {
            return &lib->books[i];
        }
    }
    
    return NULL;
}

// Find book by title
Book* library_find_book_by_title(Library *lib, const char *title) {
    if (lib == NULL || title == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->books[i].title, title) == 0) {
            return &lib->books[i];
        }
    }
    
    return NULL;
}

// Find book by author
Book* library_find_book_by_author(Library *lib, const char *author) {
    if (lib == NULL || author == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->books[i].author, author) == 0) {
            return &lib->books[i];
        }
    }
    
    return NULL;
}

// Find book by ISBN
Book* library_find_book_by_isbn(Library *lib, const char *isbn) {
    if (lib == NULL || isbn == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < lib->count; i++) {
        if (strcmp(lib->books[i].isbn, isbn) == 0) {
            return &lib->books[i];
        }
    }
    
    return NULL;
}

// Save library to binary file
int library_save_to_file(Library *lib, const char *filename) {
    if (lib == NULL || filename == NULL) {
        return 0;
    }
    
    FILE *file = fopen(filename, "wb");
    if (file == NULL) {
        return 0;
    }
    
    // Write count first
    if (fwrite(&lib->count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    // Write all books
    if (lib->count > 0) {
        size_t written = fwrite(lib->books, sizeof(Book), lib->count, file);
        if (written != (size_t)lib->count) {
            fclose(file);
            return 0;
        }
    }
    
    fclose(file);
    return 1;
}

// Load library from binary file
int library_load_from_file(Library *lib, const char *filename) {
    if (lib == NULL || filename == NULL) {
        return 0;
    }
    
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0; // File doesn't exist or can't be opened
    }
    
    // Read count
    int count;
    if (fread(&count, sizeof(int), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    if (count < 0 || count > 100000) { // Sanity check
        fclose(file);
        return 0;
    }
    
    // Resize library if needed
    while (lib->capacity < count) {
        if (!library_resize(lib)) {
            fclose(file);
            return 0;
        }
    }
    
    // Read all books
    if (count > 0) {
        size_t read = fread(lib->books, sizeof(Book), count, file);
        if (read != (size_t)count) {
            fclose(file);
            return 0;
        }
    }
    
    lib->count = count;
    fclose(file);
    return 1;
}

// Load library from text file (books.txt format: id;title;author)
int library_load_from_txt(Library *lib, const char *filename) {
    if (lib == NULL || filename == NULL) {
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0; // File doesn't exist or can't be opened
    }
    
    char line[500];
    int loaded_count = 0;
    
    // Read line by line
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        // Parse line: id;title;author
        int id;
        char title[MAX_TITLE_LEN];
        char author[MAX_AUTHOR_LEN];
        
        // Find first semicolon
        char *first_semi = strchr(line, ';');
        if (first_semi == NULL) continue;
        
        // Find second semicolon
        char *second_semi = strchr(first_semi + 1, ';');
        if (second_semi == NULL) continue;
        
        // Extract id
        *first_semi = '\0';
        if (sscanf(line, "%d", &id) != 1) {
            *first_semi = ';';
            continue;
        }
        
        // Extract title
        *second_semi = '\0';
        strncpy(title, first_semi + 1, MAX_TITLE_LEN - 1);
        title[MAX_TITLE_LEN - 1] = '\0';
        
        // Extract author
        strncpy(author, second_semi + 1, MAX_AUTHOR_LEN - 1);
        author[MAX_AUTHOR_LEN - 1] = '\0';
        
        // Check if book with this ID already exists
        Book *existing = library_find_book_by_id(lib, id);
        if (existing != NULL) {
            continue; // Skip if already exists
        }
        
        // Generate ISBN (simple format: ISBN-XXXX where XXXX is ID)
        char isbn[MAX_ISBN_LEN];
        snprintf(isbn, MAX_ISBN_LEN, "ISBN-%04d", id);
        
        // Resize if needed
        if (!library_resize(lib)) {
            continue; // Skip if resize fails
        }
        
        // Create new book directly with the ID from file
        Book *new_book = &lib->books[lib->count];
        new_book->id = id;
        strncpy(new_book->title, title, MAX_TITLE_LEN - 1);
        new_book->title[MAX_TITLE_LEN - 1] = '\0';
        strncpy(new_book->author, author, MAX_AUTHOR_LEN - 1);
        new_book->author[MAX_AUTHOR_LEN - 1] = '\0';
        strncpy(new_book->isbn, isbn, MAX_ISBN_LEN - 1);
        new_book->isbn[MAX_ISBN_LEN - 1] = '\0';
        new_book->year = 0; // Default year
        new_book->is_available = true;
        new_book->borrower_id = 0;
        
        lib->count++;
        loaded_count++;
    }
    
    fclose(file);
    return loaded_count;
}

// Display all books
void library_display_all(Library *lib) {
    if (lib == NULL) {
        printf("Library is NULL.\n");
        return;
    }
    
    if (lib->count == 0) {
        printf("No books in the library.\n");
        return;
    }
    
    printf("\n=== ALL BOOKS ===\n");
    printf("%-5s %-30s %-25s %-15s %-6s %-10s\n", 
           "ID", "Title", "Author", "ISBN", "Year", "Status");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < lib->count; i++) {
        library_display_book(&lib->books[i]);
    }
    printf("\n");
}

// Display a single book
void library_display_book(const Book *book) {
    if (book == NULL) {
        printf("Book is NULL.\n");
        return;
    }
    
    if (book->is_available) {
        printf("%-5d %-30s %-25s %-15s %-6d %-10s\n",
               book->id,
               book->title,
               book->author,
               book->isbn,
               book->year,
               "Available");
    } else {
        printf("%-5d %-30s %-25s %-15s %-6d %-10s (User ID: %d)\n",
               book->id,
               book->title,
               book->author,
               book->isbn,
               book->year,
               "Borrowed",
               book->borrower_id);
    }
}

// Display available books
void library_display_available(Library *lib) {
    if (lib == NULL) {
        printf("Library is NULL.\n");
        return;
    }
    
    int available_count = 0;
    printf("\n=== AVAILABLE BOOKS ===\n");
    printf("%-5s %-30s %-25s %-15s %-6s\n", 
           "ID", "Title", "Author", "ISBN", "Year");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < lib->count; i++) {
        if (lib->books[i].is_available) {
            printf("%-5d %-30s %-25s %-15s %-6d\n",
                   lib->books[i].id,
                   lib->books[i].title,
                   lib->books[i].author,
                   lib->books[i].isbn,
                   lib->books[i].year);
            available_count++;
        }
    }
    
    if (available_count == 0) {
        printf("No available books.\n");
    }
    printf("\n");
}

// Display borrowed books
void library_display_borrowed(Library *lib) {
    if (lib == NULL) {
        printf("Library is NULL.\n");
        return;
    }
    
    int borrowed_count = 0;
    printf("\n=== BORROWED BOOKS ===\n");
    printf("%-5s %-30s %-25s %-15s %-6s\n", 
           "ID", "Title", "Author", "ISBN", "Year");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < lib->count; i++) {
        if (!lib->books[i].is_available) {
            printf("%-5d %-30s %-25s %-15s %-6d\n",
                   lib->books[i].id,
                   lib->books[i].title,
                   lib->books[i].author,
                   lib->books[i].isbn,
                   lib->books[i].year);
            borrowed_count++;
        }
    }
    
    if (borrowed_count == 0) {
        printf("No borrowed books.\n");
    }
    printf("\n");
}

// Borrow a book
int book_borrow(Library *lib, int book_id, int user_id) {
    if (lib == NULL || user_id <= 0) {
        return 0;
    }
    
    Book *book = library_find_book_by_id(lib, book_id);
    if (book == NULL) {
        return -1; // Book not found
    }
    
    if (!book->is_available) {
        return -2; // Book already borrowed
    }
    
    // Update book status
    book->is_available = false;
    book->borrower_id = user_id;
    
    // Add to user's borrowing record file
    if (!user_borrow_record_add(user_id, book)) {
        // Rollback if file operation fails
        book->is_available = true;
        book->borrower_id = 0;
        return -3; // Failed to update user record
    }
    
    return 1; // Success
}

// Return a book
int book_return(Library *lib, int book_id, int user_id) {
    if (lib == NULL || user_id <= 0) {
        return 0;
    }
    
    Book *book = library_find_book_by_id(lib, book_id);
    if (book == NULL) {
        return -1; // Book not found
    }
    
    if (book->is_available) {
        return -2; // Book already available
    }
    
    // Verify the book is borrowed by this user
    if (book->borrower_id != user_id) {
        return -3; // Book is not borrowed by this user
    }
    
    // Remove from user's borrowing record file
    if (!user_borrow_record_remove(user_id, book_id)) {
        return -4; // Failed to update user record
    }
    
    // Update book status
    book->is_available = true;
    book->borrower_id = 0;
    
    return 1; // Success
}

// Update book information
int book_update(Library *lib, int id, const char *title, const char *author,
                const char *isbn, int year) {
    if (lib == NULL || title == NULL || author == NULL || isbn == NULL) {
        return 0;
    }
    
    Book *book = library_find_book_by_id(lib, id);
    if (book == NULL) {
        return -1; // Book not found
    }
    
    // Validate inputs
    if (!validate_string(title, MAX_TITLE_LEN) || 
        !validate_string(author, MAX_AUTHOR_LEN) ||
        !validate_string(isbn, MAX_ISBN_LEN) ||
        !validate_year(year)) {
        return 0;
    }
    
    // Check if ISBN already exists (and it's not the current book)
    Book *existing = library_find_book_by_isbn(lib, isbn);
    if (existing != NULL && existing->id != id) {
        return -2; // ISBN already exists for another book
    }
    
    // Update book information
    strncpy(book->title, title, MAX_TITLE_LEN - 1);
    book->title[MAX_TITLE_LEN - 1] = '\0';
    strncpy(book->author, author, MAX_AUTHOR_LEN - 1);
    book->author[MAX_AUTHOR_LEN - 1] = '\0';
    strncpy(book->isbn, isbn, MAX_ISBN_LEN - 1);
    book->isbn[MAX_ISBN_LEN - 1] = '\0';
    book->year = year;
    
    return 1; // Success
}

// Validate ISBN (basic validation - not empty and reasonable length)
bool validate_isbn(const char *isbn) {
    if (isbn == NULL || strlen(isbn) == 0 || strlen(isbn) >= MAX_ISBN_LEN) {
        return false;
    }
    return true;
}

// Validate year (reasonable range)
bool validate_year(int year) {
    return year >= 0 && year <= 9999;
}

// Validate string (not empty and within max length)
bool validate_string(const char *str, int max_len) {
    if (str == NULL) {
        return false;
    }
    size_t len = strlen(str);
    if (len == 0 || len >= (size_t)max_len) {
        return false;
    }
    return true;
}

// Get next available ID
int get_next_id(Library *lib) {
    if (lib == NULL || lib->count == 0) {
        return 1;
    }
    
    int max_id = 0;
    for (int i = 0; i < lib->count; i++) {
        if (lib->books[i].id > max_id) {
            max_id = lib->books[i].id;
        }
    }
    
    return max_id + 1;
}

// ==================== USER MANAGEMENT FUNCTIONS ====================

// Create a new user manager instance
UserManager* user_manager_create(void) {
    UserManager *um = (UserManager*)malloc(sizeof(UserManager));
    if (um == NULL) {
        return NULL;
    }
    
    um->capacity = 10;
    um->count = 0;
    um->users = (User*)malloc(um->capacity * sizeof(User));
    
    if (um->users == NULL) {
        free(um);
        return NULL;
    }
    
    return um;
}

// Destroy user manager and free all memory
void user_manager_destroy(UserManager *um) {
    if (um != NULL) {
        if (um->users != NULL) {
            free(um->users);
        }
        free(um);
    }
}

// Resize user manager capacity if needed
static int user_manager_resize(UserManager *um) {
    if (um->count >= um->capacity) {
        int new_capacity = um->capacity * 2;
        User *new_users = (User*)realloc(um->users, new_capacity * sizeof(User));
        
        if (new_users == NULL) {
            return 0; // Failed to resize
        }
        
        um->users = new_users;
        um->capacity = new_capacity;
    }
    return 1;
}

// Add a new user
int user_add(UserManager *um, const char *username, const char *name, const char *password, UserRole role) {
    if (um == NULL || username == NULL || name == NULL || password == NULL) {
        return 0; // Invalid parameters
    }
    
    // Validate inputs
    if (!validate_string(username, MAX_USERNAME_LEN) || 
        !validate_string(name, MAX_NAME_LEN) ||
        !validate_string(password, MAX_PASSWORD_LEN)) {
        return 0;
    }
    
    // Check if username already exists
    if (user_find_by_username(um, username) != NULL) {
        return -1; // Username already exists
    }
    
    // Resize if needed
    if (!user_manager_resize(um)) {
        return 0; // Memory allocation failed
    }
    
    // Create new user
    User *new_user = &um->users[um->count];
    new_user->id = get_next_user_id(um);
    strncpy(new_user->username, username, MAX_USERNAME_LEN - 1);
    new_user->username[MAX_USERNAME_LEN - 1] = '\0';
    strncpy(new_user->name, name, MAX_NAME_LEN - 1);
    new_user->name[MAX_NAME_LEN - 1] = '\0';
    strncpy(new_user->password, password, MAX_PASSWORD_LEN - 1);
    new_user->password[MAX_PASSWORD_LEN - 1] = '\0';
    new_user->role = role;
    
    um->count++;
    return new_user->id;
}

// Find user by ID
User* user_find_by_id(UserManager *um, int id) {
    if (um == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < um->count; i++) {
        if (um->users[i].id == id) {
            return &um->users[i];
        }
    }
    
    return NULL;
}

// Find user by username
User* user_find_by_username(UserManager *um, const char *username) {
    if (um == NULL || username == NULL) {
        return NULL;
    }
    
    for (int i = 0; i < um->count; i++) {
        if (strcmp(um->users[i].username, username) == 0) {
            return &um->users[i];
        }
    }
    
    return NULL;
}

// Save users to text file
int user_save_to_file(UserManager *um, const char *filename) {
    if (um == NULL || filename == NULL) {
        return 0;
    }
    
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        return 0;
    }
    
    // Write all users in format: id|username|name|password|role
    for (int i = 0; i < um->count; i++) {
        fprintf(file, "%d|%s|%s|%s|%d\n",
                um->users[i].id,
                um->users[i].username,
                um->users[i].name,
                um->users[i].password,
                um->users[i].role);
    }
    
    fclose(file);
    return 1;
}

// Load users from text file
int user_load_from_file(UserManager *um, const char *filename) {
    if (um == NULL || filename == NULL) {
        return 0;
    }
    
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        return 0; // File doesn't exist or can't be opened
    }
    
    char line[500];
    int count = 0;
    
    // Read line by line
    while (fgets(line, sizeof(line), file) != NULL) {
        // Remove newline
        size_t len = strlen(line);
        if (len > 0 && line[len - 1] == '\n') {
            line[len - 1] = '\0';
            len--;
        }
        
        if (len == 0) continue;
        
        // Parse line: id|username|name|password|role
        int id, role;
        char username[MAX_USERNAME_LEN];
        char name[MAX_NAME_LEN];
        char password[MAX_PASSWORD_LEN];
        
        if (sscanf(line, "%d|%49[^|]|%99[^|]|%49[^|]|%d",
                   &id, username, name, password, &role) != 5) {
            continue; // Skip invalid lines
        }
        
        // Resize if needed
        if (count >= um->capacity) {
            if (!user_manager_resize(um)) {
                fclose(file);
                return 0;
            }
        }
        
        // Add user
        User *new_user = &um->users[count];
        new_user->id = id;
        strncpy(new_user->username, username, MAX_USERNAME_LEN - 1);
        new_user->username[MAX_USERNAME_LEN - 1] = '\0';
        strncpy(new_user->name, name, MAX_NAME_LEN - 1);
        new_user->name[MAX_NAME_LEN - 1] = '\0';
        strncpy(new_user->password, password, MAX_PASSWORD_LEN - 1);
        new_user->password[MAX_PASSWORD_LEN - 1] = '\0';
        new_user->role = (UserRole)role;
        
        count++;
    }
    
    um->count = count;
    fclose(file);
    return 1;
}

// Get next available user ID
int get_next_user_id(UserManager *um) {
    if (um == NULL || um->count == 0) {
        return 1;
    }
    
    int max_id = 0;
    for (int i = 0; i < um->count; i++) {
        if (um->users[i].id > max_id) {
            max_id = um->users[i].id;
        }
    }
    
    return max_id + 1;
}

// ==================== AUTHENTICATION FUNCTIONS ====================

// Login function
int user_login(UserManager *um, const char *username, const char *password, User **logged_user) {
    if (um == NULL || username == NULL || password == NULL || logged_user == NULL) {
        return 0;
    }
    
    // Check for admin login
    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        // Create temporary admin user
        static User admin_user;
        admin_user.id = 0;
        strncpy(admin_user.username, ADMIN_USERNAME, MAX_USERNAME_LEN - 1);
        admin_user.username[MAX_USERNAME_LEN - 1] = '\0';
        strncpy(admin_user.name, "Administrator", MAX_NAME_LEN - 1);
        admin_user.name[MAX_NAME_LEN - 1] = '\0';
        admin_user.role = ROLE_ADMIN;
        *logged_user = &admin_user;
        return 1;
    }
    
    // Check regular users
    User *user = user_find_by_username(um, username);
    if (user != NULL && strcmp(user->password, password) == 0) {
        *logged_user = user;
        return 1;
    }
    
    return 0; // Invalid credentials
}

// Register new user
int user_register(UserManager *um, const char *username, const char *name, const char *password) {
    if (um == NULL || username == NULL || name == NULL || password == NULL) {
        return 0;
    }
    
    // Check if username is admin
    if (strcmp(username, ADMIN_USERNAME) == 0) {
        return -2; // Cannot register as admin
    }
    
    // Validate inputs
    if (!validate_string(username, MAX_USERNAME_LEN) || 
        !validate_string(name, MAX_NAME_LEN) ||
        !validate_string(password, MAX_PASSWORD_LEN)) {
        return 0;
    }
    
    // Check if username already exists
    if (user_find_by_username(um, username) != NULL) {
        return -1; // Username already exists
    }
    
    // Add user with ROLE_USER
    int result = user_add(um, username, name, password, ROLE_USER);
    if (result > 0) {
        user_save_to_file(um, USERS_FILENAME);
    }
    
    return result;
}

// Check if user is admin
bool is_admin(const User *user) {
    if (user == NULL) {
        return false;
    }
    return user->role == ROLE_ADMIN;
}

// ==================== USER BORROWING RECORDS ====================

// Get filename for user's borrowing record
char* get_user_filename(int user_id) {
    static char filename[50];
    snprintf(filename, sizeof(filename), "user_%d_books.dat", user_id);
    return filename;
}

// Add a book to user's borrowing record file
int user_borrow_record_add(int user_id, const Book *book) {
    if (book == NULL || user_id <= 0) {
        return 0;
    }
    
    char *filename = get_user_filename(user_id);
    FILE *file = fopen(filename, "ab"); // Append mode
    if (file == NULL) {
        return 0;
    }
    
    // Create borrow record
    BorrowRecord record;
    record.book_id = book->id;
    strncpy(record.title, book->title, MAX_TITLE_LEN - 1);
    record.title[MAX_TITLE_LEN - 1] = '\0';
    strncpy(record.author, book->author, MAX_AUTHOR_LEN - 1);
    record.author[MAX_AUTHOR_LEN - 1] = '\0';
    strncpy(record.isbn, book->isbn, MAX_ISBN_LEN - 1);
    record.isbn[MAX_ISBN_LEN - 1] = '\0';
    record.year = book->year;
    
    // Write record
    if (fwrite(&record, sizeof(BorrowRecord), 1, file) != 1) {
        fclose(file);
        return 0;
    }
    
    fclose(file);
    return 1;
}

// Remove a book from user's borrowing record file
int user_borrow_record_remove(int user_id, int book_id) {
    if (user_id <= 0 || book_id <= 0) {
        return 0;
    }
    
    char *filename = get_user_filename(user_id);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        return 0; // File doesn't exist
    }
    
    // Read all records
    BorrowRecord *records = NULL;
    int count = 0;
    int capacity = 10;
    records = (BorrowRecord*)malloc(capacity * sizeof(BorrowRecord));
    
    if (records == NULL) {
        fclose(file);
        return 0;
    }
    
    BorrowRecord record;
    while (fread(&record, sizeof(BorrowRecord), 1, file) == 1) {
        if (count >= capacity) {
            capacity *= 2;
            BorrowRecord *new_records = (BorrowRecord*)realloc(records, capacity * sizeof(BorrowRecord));
            if (new_records == NULL) {
                free(records);
                fclose(file);
                return 0;
            }
            records = new_records;
        }
        
        // Only keep records that don't match the book_id to remove
        if (record.book_id != book_id) {
            records[count++] = record;
        }
    }
    fclose(file);
    
    // Write back all records except the removed one
    file = fopen(filename, "wb");
    if (file == NULL) {
        free(records);
        return 0;
    }
    
    if (count > 0) {
        if (fwrite(records, sizeof(BorrowRecord), count, file) != (size_t)count) {
            free(records);
            fclose(file);
            return 0;
        }
    }
    
    free(records);
    fclose(file);
    return 1;
}

// Load user's borrowing records
int user_borrow_record_load(int user_id, BorrowRecord **records, int *count) {
    if (user_id <= 0 || records == NULL || count == NULL) {
        return 0;
    }
    
    char *filename = get_user_filename(user_id);
    FILE *file = fopen(filename, "rb");
    if (file == NULL) {
        *records = NULL;
        *count = 0;
        return 1; // File doesn't exist, but that's okay
    }
    
    // Count records
    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    fseek(file, 0, SEEK_SET);
    
    *count = (int)(file_size / sizeof(BorrowRecord));
    
    if (*count == 0) {
        *records = NULL;
        fclose(file);
        return 1;
    }
    
    // Allocate memory
    *records = (BorrowRecord*)malloc(*count * sizeof(BorrowRecord));
    if (*records == NULL) {
        fclose(file);
        return 0;
    }
    
    // Read all records
    size_t read = fread(*records, sizeof(BorrowRecord), *count, file);
    fclose(file);
    
    if (read != (size_t)*count) {
        free(*records);
        *records = NULL;
        *count = 0;
        return 0;
    }
    
    return 1;
}

// Display user's borrowing records
void user_borrow_record_display(int user_id) {
    BorrowRecord *records = NULL;
    int count = 0;
    
    if (!user_borrow_record_load(user_id, &records, &count)) {
        printf("Error loading borrowing records.\n");
        return;
    }
    
    if (count == 0 || records == NULL) {
        printf("No borrowed books for this user.\n");
        return;
    }
    
    printf("\n=== BORROWED BOOKS (User ID: %d) ===\n", user_id);
    printf("%-5s %-30s %-25s %-15s %-6s\n", 
           "ID", "Title", "Author", "ISBN", "Year");
    printf("--------------------------------------------------------------------------------\n");
    
    for (int i = 0; i < count; i++) {
        printf("%-5d %-30s %-25s %-15s %-6d\n",
               records[i].book_id,
               records[i].title,
               records[i].author,
               records[i].isbn,
               records[i].year);
    }
    
    printf("\n");
    free(records);
}

