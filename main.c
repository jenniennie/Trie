/*
COP 3502C Assignment 5
This program is written by: Jennifer Nguyen
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "leak_detector_c.h"

struct trie 
{    
    int word; // letter is there or not
    int frequency; // how  many  copies  the  string are used
    int sum; // sum of frequencies
    int max;
    struct trie* next[26];
    
};

void query(FILE *ofp, struct trie* text, char word[], char cur[], int iter)
{
    // if text is null, the word is unknown
    if (text == NULL)
    {
        printf("unknown word");
        fprintf(ofp, "unknown word");
        return; 
    }
    
    // if reached next least letter, search for the most likely next node
    if (iter == strlen(word))
    {
        int length = strlen(cur); 
        int i;    
        int found = -1;
        // iterate through alphabet
        for (i = 0; i < 26; i++) 
        {        
            cur[length] = (char)('a' + i);        
            cur[length + 1] = '\0'; 
            
            if (text->next[i] != NULL)
            {
                if (text->max == text->next[i]->sum)
                {
                    found++;
                    printf("%s", cur);
                    fprintf(ofp, "%s", cur);
                }
            }
        }
        // if nothing in alphabet is found, unknown word
        if (found == -1)
        {
            printf("unknown word");
            fprintf(ofp, "unknown word");
        }
        return;
        
    }
    else // go the next letter
    {
        int nextIndex = word[iter] - 'a';
        query(ofp, text->next[nextIndex], word, cur, iter + 1); 
    }
}

// create empty node
struct trie* init() 
{   
    struct trie* text = malloc(sizeof(struct trie));    
    text->word = 0;    
 
    int i;    
    for (i = 0; i < 26; i++)
        text->next[i] = NULL;
        
    // Return root.    
    return text;
    
}

// find max sum of the next node
int findMax(struct trie* text, int nextSum)
{
    if (text->max == '\0') 
        return nextSum;   // if nothing to compare it to, automatically the next node
    else if (text->max < nextSum)
        text->max = nextSum;
    
    return text->max;
}

// insert nodes of word
void insert(struct trie* text, char word[], int freq, int iter) 
{
    if (iter == strlen(word)) 
    {   
        text->word = 1;
        text->frequency = freq;
        return;    
    }   
    // See if the next letter
    int newWord = word[iter] - 'a';
    
    if (text->next[newWord] == NULL)
        text->next[newWord] = init();
  
    text->next[newWord]->sum += freq;
    
    insert(text->next[newWord], word, freq, iter + 1);
    
    text->max = findMax(text, text->next[newWord]->sum);
}

int main()
{
    atexit(report_mem_leak);
    
    FILE *ifp, *ofp;
    ifp = fopen("in.txt", "r");
    ofp =  fopen("out.txt", "w");
    
    struct trie* text = init();    
    char s[30000]; // word
    int i, j, n, f, command;  // n is number lines; f = frquency; 2 commands
    
    fscanf(ifp, "%d", &n);  
    for (i = 0; i < n; i++) 
    {        
        fscanf(ifp, "%d", &command); // 1 = insert; 2 = query
        fscanf(ifp, "%s", s); // word
        //printf("%d %s", command, s);
        // if command is 1, insert word
        if (command == 1)
        {
            fscanf(ifp, "%d", &f); 
            insert(text, s, f, 0);
        }
        // else query
        else 
        {
            char cur[100];
            cur[0] = '\0';
            query(ofp, text, s, cur, 0);
            printf("\n");
            fprintf(ofp, "\n");
        }
    }
    free(text);
    fclose(ifp);
    fclose(ofp);
    return 0;
}
