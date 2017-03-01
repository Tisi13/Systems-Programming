#include <stdio.h>
#include <stdlib.h>
#define MAX_ALLOC 1024
#define BUFFER_SIZE (MAX_ALLOC - (sizeof(int) + 2 * sizeof(void *)))


struct data 
{
    struct data *previous;
    struct data *next;
    int size;
    char data[BUFFER_SIZE];
};


/* reverses a buffer */
void reverse(char *s, int length)
{
    int i, j;
    char tmp;
    
    i = 0;
    j = length - 1;
    
    while(i < j)
    {
        tmp = s[i];
        s[i] = s[j];
        s[j] = tmp;
        
        i++;
        j--;
    }
}

/* allocates and initialises a data buffer */
struct data *alloc_data()
{
    struct data *result;
        
    result = malloc(sizeof(struct data));
    
    if (result == NULL)
    {
        perror("Error ocurred while allocating memory");
        exit(-1);
    }
    
    result->previous = result->next = NULL;
    result->size = 0;
    
    
    return result;
}

/* frees a data linked list */

void free_data(struct data *begin)
{
    struct data *next;
    
    while (begin != NULL)
    {
        next = begin->next;
        free(begin);
        begin = next;
    }
}
/* reads a file into data structure*/

struct data *readfile(FILE *file_handle)
{
    struct data *begin, *previous, *current;
    int actually_read;
    
    begin = current = alloc_data();
    previous        = NULL;
    
    while (1)
    {
        actually_read = (int) fread(current->data, 1, BUFFER_SIZE, file_handle);
        current->size = actually_read;
        
        if (previous != NULL)
            previous->next = current;
        
        current->previous = previous;
        
        if (actually_read < 0)
        {
            perror("Error while reading file\n");
            free_data(begin);
            exit(-1);
        }
        
        if (actually_read < BUFFER_SIZE)
            return begin;
        
        previous = current;
        current = alloc_data();
    }
}


/*writes buffers in reverse order*/
void write_contents_backwards(struct data *first)
{
    //seek end
    while (first->next != NULL)
        first = first->next;
    
    while (first != NULL)
    {
        fwrite(first->data, (size_t) first->size, 1, stdout);
        first = first->previous;
    }
}

/* reverse every buffer of linked list data structure*/
void reverse_innerbuffers(struct data *first)
{
    while (first != NULL)
    {
        reverse(first->data, first->size);
        first = first->next;
    }
}


int main(int argc, char *argv[])
{
    FILE *file_handle;
    struct data *file_contents;
    
    
    if (argc < 2)
    {
        printf("Not enough arguments given");
        return 1;
    }

    
    file_handle = fopen(argv[1], "rb");
    if (file_handle == 0)
    {
        printf("Could not fopen file\n");
        return 1;
    }
    
    /* reads in file into linked list, starting with file_contents */
    file_contents = readfile(file_handle);
    
    /* reverse buffers in linked list */
    reverse_innerbuffers(file_contents);
    
    /*goes through linked list in reverse order, writing contents to sdout */
    write_contents_backwards(file_contents);
    
    /* clean up */
    free_data(file_contents);
    fclose(file_handle);
    
    
    return 0;
}
