#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include"my_string.h"
#include"generic.h"


int main(int argc, char* argv[])
{
	//create an array of 30 void* to point to 30 generic vectors
	GENERIC_VECTOR word_list[30];
	MY_STRING hString;
	MY_STRING hWord=NULL;
	FILE* fp;
	int i, j, num_of_conversions;
	char ch;


	hString = my_string_init_default();
	if (hString == NULL)
	{
		printf("Unable to create space\n");
	}

	fp = fopen("dictionary.txt", "r");

	if (fp == NULL)
	{
		printf("unable to open file\n");
	}

	//set each handle to null to begin with
	for (i = 0; i<30; i++)
	{
		word_list[i] = NULL;
	}

	//init 30 generic vectors
	for (i = 0; i<30; i++)
	{
		word_list[i] = generic_vector_init_default(my_string_assignment, my_string_destroy);
		if (word_list[i] == NULL)
		{
			printf("Unable to create a generic vector\n");
			exit(1);
		}
	}

	//extract from dictionary and put in correct position
	//size is # of words of i length
	while (my_string_extraction(hString, fp))
	{
		j = my_string_get_size(hString);
		my_string_assignment(&hWord, hString);
		generic_vector_push_back(word_list[j], hWord);
		my_string_destroy(&hWord);
	}

	for (i = 0; i < 30; i++)
	{
		printf("%d: %d\n", i, generic_vector_get_size(word_list[i]));
	}
	do
	{
		play_hangman(word_list);
		printf("Would you like to play again? y or n: ");
		num_of_conversions = scanf(" %c", &ch);
		ch = tolower(ch);

		while (num_of_conversions != 1&& ch!='y' && ch!= 'n')
		{
			clear_keyboard_buffer();
			printf("That answer is not valid, y or n: ");
			num_of_conversions = scanf(" %c", &ch);
			ch = tolower(ch);
		}
	} while (ch == 'y');

	for (i = 0; i < 30; i++)
	{
		generic_vector_destroy(&word_list[i]);
	}

	my_string_destroy(&hString);
	fclose(fp);
	
  return 0;
}
