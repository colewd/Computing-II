#include<stdlib.h>
#include<stdio.h>
#include "generic.h"

typedef void* GENERIC_VECTOR;
typedef void* MY_STRING;

typedef struct node Node;
struct node
{
	int key;
	GENERIC_VECTOR stringHandles;
	MY_STRING display_key;
	Node* left;
	Node* right;
	int count;
};

enum status {FAILURE, SUCCESS};
typedef enum status Status;

enum boolean {FALSE, TRUE};
typedef enum boolean Boolean;


//change item assign to return status
GENERIC_VECTOR generic_vector_init_default(void(*item_assign)(Item_ptr,Item),void(*item_destroy)(Item_ptr));

int generic_vector_get_size(GENERIC_VECTOR hVector);

int generic_vector_get_capacity(GENERIC_VECTOR hVector);

void generic_vector_destroy(GENERIC_VECTOR* phGeneric_vector);

Status generic_vector_push_back(GENERIC_VECTOR hVector, Item item);

//Precondition: None
//Postcondition: Allocate space for a string object that represents the empty
//string. The string will have capacity 7 and size 0 by default. A copy of
//the address of the opaque object will be returned on success and NULL on 
//failure.
MY_STRING my_string_init_default(void);

//Precondition: phMy_string holds the address of a valid handle to a MY_STRING
//object.
//Postcondition: The memory used for the MY_STRING object has been reclaimed by
//the system and the handle referred to by the pointer phMy_string has been 
//set to NULL.
//void my_string_destroy(MY_STRING* phMy_string);
void my_string_destroy(Item* pItem);

//Precondition: c_string is a valid null terminated c-string.
//Postcondition: Allocate space for a string object that represents a string
//with the same value given as the given c-string. The capacity of the string
//object will be set to be one greater than is required to hold the string.
MY_STRING my_string_init_c_string(const char* c_string);

//Precondition: hMy_string is the handle of a valid My_string object.
//Postcondition: Returns a copy of the integer value of the object's capacity.
int my_string_get_capacity(MY_STRING hMy_string);

//Precondition: hMy_string is the handle of a valid My_string object.
//Postcondition: Returns a copy of the integer value of the object's size.
int my_string_get_size(MY_STRING hMy_string);

//Precondition: hLeft_string and hRight_string are valid My_string objects.
//Postcondition: returns an integer less than zero if the string represented by
//hLeft_string is lexicographically smaller than hRight_string. If one string is a 
//prefix of the other string then the shorter string is considered to be the shorter
//one. (app is less than apple). Returns 0 if the strings are the same and returns a
//number greater than zero if the string represented by hLeft_string is bigger than
//hRight_string.
int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string);

//Precondition: hMy_string is the handle to a valid My_string object.
//Postcondition: hMy_string will be the handle of a string object that contains the
//next string from the file stream fp according to the following rules:
//1)Leading whitespace will be ignored.
//2)All characters (after the first non whitespace character is obtained and included)
//will be added to the string until a stopping condition is met. The capacity of the 
//string will continue to grow as needed until all characters are stored.
//3)A stopping condition is met if we read a whitespace character after we have read at
//least one non whitespace character or if we reach the end of the file.
//Function will return SUCCESS if a non-empty string is read succesfully and FAILURE
//otherwise. Remember that the incoming string may already contain some data and this
//function should replace the data but not necessarily resize the array unless needed.
Status my_string_extraction(MY_STRING hMy_string, FILE* fp);

//Precondition: hMy_string is the handle to a valid MY_string object.
//Postcondition: If succesful, places the character item at the end of the string and 
//returns SUCCESS. If the string does not have enough room and cannot resize to 
//accomodate the new character then the operation fails and FAILURE is returned. The 
//resize operation will attempt to amortize the cost of a resize by making the string
//capacity somewhat larger than it was before (up to 2 times bigger).
Status my_string_push_back(MY_STRING hMy_string, char item);

//Precondition: hMy_string is the handle to a valid My_string object.
//Postcondition: Removes the last character of a string in constant time. Guaranteed to
//not cause a resize operation of the internal data. Returns SUCCESS on succes and FAILURE
//if the string is empty.
Status my_string_pop_back(MY_STRING hMy_string);

//Precondition: hMy_string is the handle to a valid My_string object.
//Postcondition: Returns the address of the character located at the given index if the
//index is in bounds but otherwise returns NULL. This address is not useable as a c-string
//since the data is not NULL terminated and is intended to just provide access to the 
//element at that index.
char* my_string_at(MY_STRING hMy_string, int index);

//Precondition:hMy_String is the handle to a valid My_string object.
//Postcondition:Returns the address of the first element of the string object for use as a 
//c-string. The resulting c-string is guarenteed to be NULL terminated and the memory is 
//still maintained by the string object though the NULL is not actually counted as part of 
//the string (in size).
char* my_string_c_str(MY_STRING hMy_string);

//Precondition:hResult and hAppend are handles to valid My_string objects.
//Postcondition:hResult if the handle of a string that conatins the original hResult object
//followed by the hAppend object concatenated together. This function should guarentee no 
//change to the hAppend object and return SUCCESS if the operation is succesful and FAILURE 
//if the hResult object is unable to accomodate the characters in the hAppend string perhaps
//because of a failed resize operation. On FAILURE, no change to either string should be
//made.
Status my_string_concat(MY_STRING hResult, MY_STRING hAppend);

//check that size works in extraction even when not resized


//Precondition: hMy_string is the handle to a valid My_string object.
//Postcondition: Returns an enumerated type with value TRUE if the string is empty an FALSE
//otherwise.
Boolean my_string_empty(MY_STRING hMy_string);

//Precondition: hMy_string is the handle to a valid My_string object.
//Postcondition: Writes the characters contained in the string object indicated by the 
//handle hMY_string to the file stream fp. Function will return SUCCESS if it successfully
//writes the string and FAILURE otherwise.
Status my_string_insertion(MY_STRING hMy_string, FILE* fp);

//Precondition: pLeft is the address of a MY_STRING handle containing a valid MY_STRING 
//object address or NULL, The value of Right must be the handle of a valid MY_STRING object.
//Postcondition: On Success pLeft will contain the address of a handle to a valid MY_STRING
//object that is a deep copy of the object indicated by Right. If the value of the handle 
//indicated by pLeft is originally NULL then the function will attempt to initialize a new 
//object that is a deep copy of the object indicated by Right, otherwise the object
//indicated by the handle at the address pLeft will attempt to resize to hold the data in 
//Right. On failure pLeft will be left as NULL and any memory that may have been used by a 
//potential object indicated by pLeft will be returned to the freestore.
void my_string_assignment(Item* pLeft, Item Right);

void play_hangman(GENERIC_VECTOR set_of_words[]);

int get_num_letters(GENERIC_VECTOR set_of_words[]);

void generate_working_words(GENERIC_VECTOR set_of_words, GENERIC_VECTOR working_words);

int get_num_guesses(void);

char get_guess(int guessed_letters[]);

Node* make_tree(GENERIC_VECTOR working_words, char guess, int num_letters, MY_STRING current_display_string);

int get_key(Item working_word, char guess, int num_letters);

void tree_insert(Node**pRoot, Item working_word, int key, MY_STRING display_key, int num_letters);

void in_order(Node* pRoot);

void find_largest_bin(Node* pRoot, int* max, Node** maxNode);

void tree_destroy(Node** pRoot);

Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word, char guess);

void clear_keyboard_buffer(void);

//copies hRightString to hLeftString
void my_string_copy(MY_STRING hLeftString, MY_STRING hRightString);

void my_string_print(MY_STRING hString);

void make_first_display_string(MY_STRING hString, int num_letters);

void get_random_string(MY_STRING hWord, Node* largest_bin);

Boolean display_is_full(MY_STRING display_string);
