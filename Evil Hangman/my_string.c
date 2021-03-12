#include<stdlib.h>
#include<stdio.h>
#include<ctype.h>
#include "my_string.h"

struct my_string
{
  int size;
  int capacity;
  char* data;
};
typedef struct my_string My_string;

struct generic_vector
{
	int size;
	int capacity;
	Item* data;
	void(*item_assign)(Item_ptr, Item);
	void(*item_destroy)(Item_ptr);
};
typedef struct generic_vector Generic_vector;

GENERIC_VECTOR generic_vector_init_default(
	void(*item_assign)(Item_ptr, Item),
	void(*item_destroy)(Item_ptr))
{
	Generic_vector* pVector;
	int i;
	pVector = (Generic_vector*)malloc(sizeof(Generic_vector));
	if (pVector != NULL)
	{
		pVector->size = 0;
		pVector->capacity = 1;
		pVector->item_destroy = item_destroy;
		pVector->item_assign = item_assign;
		pVector->data = (void**)malloc(sizeof(void*)* pVector->capacity);
		if (pVector->data == NULL)
		{
			free(pVector);
			pVector = NULL;
		}
		else
		{
			for (i = 0; i < pVector->capacity; i++)
			{
				pVector->data[i] = NULL;
			}
		}
	}
	return pVector;
}

Status generic_vector_push_back(GENERIC_VECTOR hVector, Item item)
{
	Generic_vector* pVector = (Generic_vector*)hVector;
	Item* temp;
	int i;

	if (pVector->size >= pVector->capacity)
	{
		temp = (Item*)malloc(sizeof(Item)* 2 * pVector->capacity);
		if (temp == NULL)
		{
			return FAILURE;
		}
		pVector->capacity *= 2;
		for (i = 0; i < pVector->size; i++)
		{
			temp[i] = pVector->data[i]; 
		}
		for (i = pVector->size; i < pVector->capacity; i++)
		{
			temp[i] = NULL;
		}
		free(pVector->data);
		pVector->data = temp;
	}

	pVector->item_assign(&pVector->data[pVector->size], item);
	pVector->size++;

	return SUCCESS;
}

int generic_vector_get_size(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;

	return pVector->size;
}
int generic_vector_get_capacity(GENERIC_VECTOR hVector)
{
	Generic_vector* pVector = (Generic_vector*)hVector;

	return pVector->capacity;
}

void generic_vector_destroy(GENERIC_VECTOR* phGeneric_vector)
{
	Generic_vector* pVector = (Generic_vector*)*phGeneric_vector;
	int i;

	for (i = 0; i < pVector->capacity; i++)
	{
		if (pVector->data[i] != NULL)
		{
			pVector->item_destroy(&pVector->data[i]);
		}
	}

	free(pVector->data);
	free(pVector);
	*phGeneric_vector = NULL;
}

MY_STRING my_string_init_default(void)
{
  My_string* pMy_string;
  pMy_string = (My_string*)malloc(sizeof(My_string));
  if(pMy_string != NULL)
    {
      pMy_string->size = 0;
      pMy_string->capacity = 7;
      pMy_string->data = (char*)malloc(sizeof(char)*(pMy_string->capacity));
      if (pMy_string->data == NULL)
	{
	  free(pMy_string);
	  pMy_string = NULL;
	}
    }
  return pMy_string;
}

MY_STRING my_string_init_c_string(const char* c_string)
{
  int length=0;
  char ch;
  My_string* pMy_string;
  
  ch = c_string[length];
  while(ch!='\0')//determine string length for memory allocation
    {
      length++;
      ch = c_string[length];
    }
  length++;

  pMy_string = (My_string*)malloc(sizeof(My_string));
  if (pMy_string != NULL)
    {
      pMy_string->size = length-1;
      pMy_string->capacity = length;
      pMy_string->data = (char*)malloc(sizeof(char)*pMy_string->capacity);
      if(pMy_string == NULL)
	{
	  free(pMy_string);
          pMy_string = NULL;
        }
      length = 0;
      while(length<pMy_string->capacity)//copy each char from c_string to data array
	{
	  pMy_string->data[length]= c_string[length];
	  length++;
	}
    }
  return pMy_string;
}

int my_string_get_capacity(MY_STRING hMy_string)
{
  My_string* pMy_string = (My_string*)hMy_string;
  return pMy_string->capacity;
}       

int my_string_get_size(MY_STRING hMy_string)
{
  My_string* pMy_string = (My_string*)hMy_string;
  return pMy_string->size;
}

int my_string_compare(MY_STRING hLeft_string, MY_STRING hRight_string)
{
  My_string* pLeft_string = (My_string*)hLeft_string;
  My_string* pRight_string =(My_string*)hRight_string;
  int i;
  char chLeft, chRight;

  i = 0;
  chLeft = pLeft_string->data[i];
  chRight = pRight_string->data[i];

  if (my_string_get_size(hLeft_string) == 0 && my_string_get_size(hRight_string) == 0)//the strings are the same
  {
	  return 0;
  }

  while(i<my_string_get_size(hLeft_string) && i<my_string_get_size(hRight_string))
    {
      if(chLeft>chRight)
	{
          return 1;
	}
      if(chRight>chLeft)
	{
	  return -1;
        }
      if (chLeft == chRight)//if each char is the same, compare the next two chars
	{
	  i++;
	  if(i<my_string_get_size(hLeft_string))
	    {
	      chLeft = pLeft_string->data[i];
	      chRight = pRight_string->data[i];
	    }	
}
    }
  if (my_string_get_size(hLeft_string) == my_string_get_size(hRight_string))
  {
	  return 0;
  }
  else if (my_string_get_size(hLeft_string) < my_string_get_size(hRight_string))//the left string is shorter than the right string
    {
      return -1;
    }
  return 1; // Right string size is 0
}


Status my_string_extraction(MY_STRING hMy_string, FILE* fp)
{
  My_string* pMy_string = (My_string*)hMy_string;
  char ch;
  int j, numOfConversions, i=0;
  char* temp;

  numOfConversions = fscanf(fp, "%c", &ch);
  if (numOfConversions == EOF)
    {
      return FAILURE;
    }

  while(numOfConversions != EOF)
    {
      if ((ch==' '||ch=='\n'||ch=='\t')&&(i==0))//ignore leading whitespace
	{ 
	  numOfConversions = fscanf(fp,"%c", &ch);
	}
      else
	{
	  if ((ch == ' '||ch=='\n'||ch=='\t')&& (i!=0))//put whitespace back
	    {
	      ungetc(ch, fp);
	      break;
	    }
	  if(pMy_string->size >= pMy_string->capacity)
	    {
	      temp = (char*) malloc(sizeof(char)*2*pMy_string->capacity);
	      if (temp == NULL)
		{
		  return FAILURE;
		}
	      for (j=0; j< pMy_string->size; j++)
		{
		  temp[j] = pMy_string->data[j];
	        }
	      free(pMy_string->data);
	      pMy_string->data = temp;
	      pMy_string->size = (j+1);
	      pMy_string->capacity *=2;
	    }
	  pMy_string->data[i]= ch;
	  pMy_string->size = i+1;
	  numOfConversions = fscanf(fp,"%c", &ch);
	  i++;
	}
    }
  if(i==0)//if only spaces are read
    {
      return FAILURE;
    }  
return SUCCESS;
}

Status my_string_insertion(MY_STRING hMy_string, FILE* fp)
{
  My_string* pMy_string = (My_string*)hMy_string;
  int i = 0;

  if (pMy_string->size==0)
    {
      return FAILURE;
    }
  while(i < pMy_string->size)
    {
      fprintf(fp,"%c",pMy_string->data[i]);
      i++;
    }
  return SUCCESS;
}

Status my_string_push_back(MY_STRING hMy_string, char item)
{
  My_string* pMy_string = (My_string*)hMy_string;
  char* temp;
  int i;

  if(pMy_string->size>=pMy_string->capacity)
    {
      temp = (char*)malloc(sizeof(char)*2*pMy_string->capacity);
      if (temp==NULL)
	{
	  return FAILURE;//unable to allocate memory and item was not added 
	}
      for(i=0; i<pMy_string->size; i++)
	{
	  temp[i]=pMy_string->data[i];
	}
      free(pMy_string->data);
      pMy_string->data = temp;
      pMy_string->capacity *=2;
    }
  pMy_string->data[pMy_string->size]= item;
  pMy_string->size++;

return SUCCESS;
}

Status my_string_pop_back(MY_STRING hMy_string)
{
  My_string* pMy_string = (My_string*) hMy_string;

  if(my_string_empty(hMy_string))
    {
      return FAILURE;
    }
  pMy_string->size--;
  return SUCCESS;
}

char* my_string_at(MY_STRING hMy_string, int index)
{
  My_string* pMy_string = (My_string*)hMy_string;

  if(index>=(pMy_string->size))//return null when index is OOB
    {
      return NULL;
    }

  return pMy_string->data+index;
}

char* my_string_c_str(MY_STRING hMy_string)
{
  My_string* pMy_string = (My_string*)hMy_string;
  char* temp;
  int i;

  if((pMy_string->size+1)>= pMy_string->capacity)
    {
      temp = (char*)malloc(sizeof(char)*(pMy_string->capacity)+1);
      if(temp==NULL)
	{
	  return NULL;
	}
      for(i=0; i<pMy_string->size; i++)
	{
	  temp[i]= pMy_string->data[i];
	}
      free(pMy_string->data);
      pMy_string->data=temp;
      pMy_string->capacity++;
    }
  pMy_string->data[pMy_string->size]='\0';
  return pMy_string->data;
}

Status my_string_concat(MY_STRING hResult, MY_STRING hAppend)
{
  My_string* pResult = (My_string*)hResult;
  My_string* pAppend = (My_string*)hAppend;
  char* temp;
  int i, j, newSize;

  if(pAppend->size >= (pResult->capacity)- (pResult->size))
    {
      temp = (char*)malloc(sizeof(char)*(pResult->capacity)+(pAppend->size));
      if(temp == NULL)
	{
	  return FAILURE;
	}
      for(i=0; i<pResult->size; i++)
	{
	  temp[i]= pResult->data[i];
	}
      free (pResult->data);
      pResult->data = temp;
      pResult->capacity = (pResult->capacity)+(pAppend->size);
    }
  j=0;
  newSize = pResult->size + pAppend->size; 
  for(i=pResult->size ; i <newSize; i++)
    {
      pResult->data[i]=pAppend->data[j];
      j++;
    }
  pResult->size = newSize;
  return SUCCESS;
}

Boolean my_string_empty(MY_STRING hMy_string)
{
  My_string* pMy_string = (My_string*)hMy_string;
  if(pMy_string->size <= 0)
    {
      return TRUE;
    }
  return FALSE;
}

void my_string_assignment(Item* pLeft, Item Right)
{
  My_string* pMy_string_left = (My_string*)*pLeft;
  My_string* pMy_string_right = (My_string*)Right;
  char* temp;
  int i;

  if (pMy_string_left == NULL)
  {
	  pMy_string_left = my_string_init_default();
	  if (pMy_string_left == NULL)
	  {
		  printf("FAILURE! Could not create space for pMY_string_left\n");
		  *pLeft = NULL;
		  return;
	  }
	 
  }
  if (pMy_string_right->size >= pMy_string_left->capacity)
  {
	temp = (char*)malloc(sizeof(char)* pMy_string_right->capacity);
	if (temp == NULL)
	{
		*pLeft = NULL;
		 return;//unable to allocate memory and item was not added 
	 }
	for (i = 0; i<pMy_string_right->size; i++)
	 {
		temp[i] = pMy_string_right->data[i];
	 }
	free(pMy_string_left->data);
	pMy_string_left->data = temp;
	pMy_string_left->capacity = pMy_string_right->capacity;
	pMy_string_left->size = pMy_string_right->size;
  }
else
{
	for (i = 0; i<pMy_string_right->size; i++)
	{
		pMy_string_left->data[i] = pMy_string_right->data[i];
	}
	pMy_string_left->size = pMy_string_right->size;
}
 *pLeft = pMy_string_left;
  return;
}


/*void my_string_destroy(MY_STRING* phMy_string)
{
  My_string* pMy_string = (My_string*)*phMy_string;
  free(pMy_string->data);
  free(pMy_string);
  *phMy_string = NULL;
}
*/

void my_string_destroy(Item* phItem)
{
  My_string* pMy_string = (My_string*)*phItem;
  free(pMy_string->data);
  free(pMy_string);
  *phItem = NULL;
}


void play_hangman(GENERIC_VECTOR set_of_words[])
{
	GENERIC_VECTOR working_words = NULL;
	Boolean game_over = FALSE;
	int num_letters, num_guesses, i, max=0;
	int guessed_letters[26];
	MY_STRING display_string = NULL;
	MY_STRING old_display_string = NULL;
	Node* current_tree;
	Node* largest_bin=NULL;
	char current_guess;
	
	//create an array of 26 ints and set each index to 0
	//when a letter is guessed, change its position (guess - a) to 1
	//in order to keep track or letters already guessed
	for (i = 0; i < 26; i++)
	{
		guessed_letters[i] = 0;
	}

	//create a generic vector that will hold MY_STRING working words
	working_words = generic_vector_init_default(my_string_assignment, my_string_destroy);
	if (working_words == NULL)
	{
		printf("Unable to create space for WW");//change this later
	}

	num_letters = get_num_letters(set_of_words);

	generate_working_words(set_of_words[num_letters], working_words);

	printf("There are %d working words left\n", generic_vector_get_size(working_words));

	num_guesses = get_num_guesses();
	display_string = my_string_init_default();
	if (display_string == NULL)
	{
		printf("Unable to create space for display_string\n");//change this later
	}
	old_display_string = my_string_init_default();
	if (old_display_string == NULL)
	{
		printf("Unable to create space for old_display_string\n");//change this later
	}
	make_first_display_string(display_string, num_letters);
	my_string_copy(old_display_string, display_string);

	while (!game_over)
	{
		printf("Your word so far:\n");
		my_string_print(display_string);
		printf("\n");
		current_guess = get_guess(guessed_letters);
		current_tree = make_tree(working_words, current_guess, num_letters, display_string);
		find_largest_bin(current_tree, &max, &largest_bin);
		//printf("Key: %d	   Count: %d\n", largest_bin->key, largest_bin->count);
		//my_string_print(largest_bin->display_key);
		//printf("\n");

		my_string_copy(display_string, largest_bin->display_key);
		if (my_string_compare(display_string, old_display_string) == 0)
		{
			printf("Sorry that letter was not in the word\n");
			num_guesses--;
		}

		if (my_string_compare(display_string, old_display_string) != 0)
		{
			printf("That letter is in the word!\n");
		}
		
		if (num_guesses == 0 && !display_is_full(display_string))
		{
			printf("GAME OVER\n");
			printf("The word I was thinking of was ");
			get_random_string(display_string, largest_bin);
			my_string_print(display_string);
			printf("\n");
			game_over = TRUE;
		}

		else if (display_is_full(display_string))
		{
			printf("YOU WON!\n");
			printf("The word was ");
			my_string_print(display_string);
			printf("\n");
			game_over = TRUE;
		}
		if(!game_over)
		  {

		    my_string_copy(old_display_string, display_string);
		    printf("You have %d guesses left\n", num_guesses);

		    generate_working_words(largest_bin->stringHandles, working_words);
		    //printf("%d\n", generic_vector_get_size(working_words));
		  }
		tree_destroy(&current_tree);
	}

	generic_vector_destroy(&working_words);
	my_string_destroy(&display_string);
	my_string_destroy(&old_display_string);
	return;
}


int get_num_letters(GENERIC_VECTOR set_of_words[])
{
  int num_of_conversions, num_letters;//add clear keyboard buffer

	printf("Please enter the number of letters you want in your word: ");
	num_of_conversions = scanf("%d", &num_letters);

	//while (no_words_of_this_length(num_letters, set_of_words))
	while (num_letters <1 || num_letters> 29 || generic_vector_get_size(set_of_words[num_letters]) == 0||num_of_conversions!=1)
	{
		clear_keyboard_buffer();
		printf("I'm sorry you have entered an invalid number.\n");
		printf("Please choose a different number: ");
		num_of_conversions = scanf("%d", &num_letters);
	}
	return num_letters;
}

void generate_working_words(GENERIC_VECTOR set_of_words, GENERIC_VECTOR working_words)
{
	MY_STRING hString=NULL;
	Generic_vector* pGeneric_vector = (Generic_vector*)set_of_words;
	Generic_vector* pWorking_words = (Generic_vector*)working_words;
	int i;

	pWorking_words->size = 0;
	//make a complete and independent copy of set of words
	for (i = 0; i < generic_vector_get_size(set_of_words); i++)
	{
		my_string_assignment(&hString, pGeneric_vector->data[i]);
		generic_vector_push_back(working_words, hString);
		my_string_destroy(&hString);
	}
}

//DONT FORGET TO CHECK THE ANDS AND ORS

int get_num_guesses(void)
{
	int num_guesses;
	int num_of_conversions;

	printf("Please enter the number of guesses you'd like: ");
	num_of_conversions = scanf("%d", &num_guesses);

	while (num_of_conversions != 1 || num_guesses <= 0 || num_guesses>26)
	{
		clear_keyboard_buffer();
		printf("This number of guesses is invalid. Number of guesses must be 1-26.\n");
		printf("Please enter a different number: ");
		num_of_conversions = scanf("%d", &num_guesses);
	}

	return num_guesses;
}

char get_guess(int guessed_letters[])
{
	char guess;
	int num_of_conversions;

	printf("Enter your guess? ");
	num_of_conversions= scanf(" %c", &guess);
	guess=tolower(guess);

	while (num_of_conversions != 1 || guessed_letters[guess - 'a'] == 1||!isalpha(guess))
	{
		clear_keyboard_buffer();
		printf("You have already guessed %c or that guess is invalid\n", guess);
		printf("Please guess again: ");
		num_of_conversions = scanf(" %c", &guess);
		guess=tolower(guess);
	}

	guessed_letters[guess - 'a'] = 1;
	return guess;
}

Node* make_tree(GENERIC_VECTOR working_words, char guess, int num_letters, MY_STRING current_display_string)
{
	Generic_vector* pWorking_words = (Generic_vector*)working_words;
	Node* pRoot = NULL;
	int i, key;
	MY_STRING new_display_string=NULL;
	//Item hString;

	new_display_string = my_string_init_default();
	if (new_display_string == NULL)
	{
		printf("problem creating new_display_string in make_tree\n");
	}

	for (i = 0; i < generic_vector_get_size(working_words); i++)
	{
		key = get_key(pWorking_words->data[i], guess, num_letters);
		get_word_key_value(current_display_string, new_display_string, pWorking_words->data[i], guess);
		tree_insert(&pRoot, pWorking_words->data[i], key, new_display_string, num_letters);
	}

	my_string_destroy(&new_display_string);
	return pRoot;
}

int get_key(Item working_word, char guess, int num_letters)
{
	My_string* pMy_string = (My_string*)working_word;
	int i, key = 0;
	for (i = 0; i < num_letters; i++)
	{
		if (pMy_string->data[i] == guess)
		{
			key = key + 1;
		}
		key = key << 1;
	}

	return key;
}

void tree_insert(Node**pRoot,Item working_word, int key, MY_STRING display_key, int num_letters)
{
	if (*pRoot == NULL)
	{
		*pRoot = (Node*)malloc(sizeof(Node));
		if (*pRoot == NULL)
		{
			printf("FAIL\n");//change this
		}
		(*pRoot)->key = key;
		(*pRoot)->display_key = my_string_init_default();
		my_string_copy((*pRoot)->display_key, display_key);
		(*pRoot)->stringHandles = generic_vector_init_default(my_string_assignment, my_string_destroy);
		generic_vector_push_back((*pRoot)->stringHandles, working_word);
		(*pRoot)->left = NULL;
		(*pRoot)->right = NULL;
		(*pRoot)->count = 1;
	}

	else
	{
		if (key <(*pRoot)->key)
		  //if (my_string_compare(display_key, (*pRoot)->display_key)<0)
		{
			tree_insert(&(*pRoot)->left, working_word, key, display_key, num_letters);
		}
		else if (key >(*pRoot)->key)
		  //else if (my_string_compare(display_key, (*pRoot)->display_key)>0)
		{
			tree_insert(&(*pRoot)->right, working_word, key, display_key, num_letters);
		}
		else
		{
			(*pRoot)->count++;
			generic_vector_push_back((*pRoot)->stringHandles, working_word);
		}
	}

}

void in_order(Node* pRoot)
{
	if (pRoot != NULL)
	{
		in_order(pRoot->left);
		printf("key: %d\n", pRoot->key);
		in_order(pRoot->right);
	}
}

void find_largest_bin(Node* pRoot, int* max, Node** maxNode)
{
	int maxLeft;
	int maxRight;
	Node* maxLeftNode = NULL;
	Node* maxRightNode = NULL;

	if (pRoot == NULL)
	{
		*max = 0;
		*maxNode = NULL;
		return;
	}

	find_largest_bin(pRoot->left, &maxLeft, &maxLeftNode);

	if (pRoot->count > maxLeft)
	{
		*max = pRoot->count;
		*maxNode = pRoot;
	}
	else
	{
		*max = maxLeft;
		*maxNode = maxLeftNode;
	}

	find_largest_bin(pRoot->right, &maxRight, &maxRightNode);

	if (maxRight>*max)
	{
		*max = maxRight;
		*maxNode = maxRightNode;
	}
}

void tree_destroy(Node** pRoot)
{
	if (*pRoot != NULL)
	{
		tree_destroy(&(*pRoot)->left);
		tree_destroy(&(*pRoot)->right);
		generic_vector_destroy(&(*pRoot)->stringHandles);//is this right
		my_string_destroy(&(*pRoot)->display_key);
		free(*pRoot);
		*pRoot = NULL;
	}
}

Status get_word_key_value(MY_STRING current_word_family, MY_STRING new_key, MY_STRING word, char guess)
{
	My_string* pWord = (My_string*)word;
	My_string* pNew_key = (My_string*)new_key;
	My_string* pCurrent_word_family = (My_string*)current_word_family;
	int i;

	pNew_key->size = 0;
	for (i = 0; i < my_string_get_size(current_word_family); i++)
	{
		if (guess == pWord->data[i])
		{
			my_string_push_back(new_key, pWord->data[i]);
		}
		else
		{
			my_string_push_back(new_key, pCurrent_word_family->data[i]);
		}
	}

	return SUCCESS;
}

void clear_keyboard_buffer(void)
{
	char ch;
	int numOfConversion = scanf("%c", &ch);

	while (ch != '\n'||numOfConversion!=1)
	{
		numOfConversion = scanf("%c", &ch);
	}
}

void my_string_copy(MY_STRING hLeftString, MY_STRING hRightString)
{
	My_string* pLeftString = (My_string*)hLeftString;
	My_string* pRightString = (My_string*)hRightString;
	int i;

	pLeftString->size = 0;
	
	for (i = 0; i < pRightString->size; i++)
	{
		my_string_push_back(hLeftString, pRightString->data[i]);
	}
}

void my_string_print(MY_STRING hString)
{
	My_string* pString = (My_string*)hString;
	int i;

	for (i = 0; i < pString->size; i++)
	{
		printf("%c", pString->data[i]);
	}
}

void make_first_display_string(MY_STRING hString, int num_letters)
{
	int i;
	for (i = 0; i < num_letters; i++)
	{
		my_string_push_back(hString, '-');
	}
}

void get_random_string(MY_STRING hWord, Node* largest_bin)
{
	Generic_vector* pVector = (Generic_vector*)largest_bin->stringHandles;
	my_string_copy(hWord, pVector->data[0]);//change this to random value
}

Boolean display_is_full(MY_STRING display_string)
{
	int i;

	for (i = 0; i < my_string_get_size(display_string); i++)
	{
		if ((*my_string_at(display_string, i)) == '-')
		{
			return FALSE;
		}
	}
	return TRUE;
}
