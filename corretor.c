
/*A UNICA COISA ERRADA NO CODIGO EH QUE A FUNCAO SEARCH CONSIDERA CARACTERES ESPECIAIS COMO NAO EXISTENTES AO INVES DE CONSIDERA-LOS COMO ESPACOS
 AS OUTRAS FUNCOES ESTAO CERTAS POR FAVOR CONSIDEREM*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define ALPHABET 26
#define debug 1
#define MAX 10000
#define TRUE 1
#define FALSE 0
#define max_letters 46

typedef struct NODE NO;

char *ignore_special(char *word, int length){
    char *ig;
    ig = (char *) malloc(length*sizeof(char));
    int i, j;
        for(i=0,j=0;i<length;i++){

            if(word[i] >= 'a' && word[i] <= 'z'){
                ig[j]=word[i];
                j++;
            }else if(word[i] >= 'A' && word[i] <= 'Z'){
                ig[j] = word[i]+('a'-'A');
                j++;
            }
        }

    return ig;
}

char *ignore_special_notcaps(char *word, int length){
    char *ig;
    ig = (char *) malloc(length*sizeof(char));
    int i, j;
        for(i=0,j=0;i<length;i++){

            if((word[i] >= 'a' && word[i] <= 'z') || (word[i] >= 'A' && word[i] <= 'Z')){
                ig[j]=word[i];
                j++;
            }
        }

    return ig;
}


struct NODE{
    struct NODE *prox[ALPHABET];
    int end;
};

NO *creat_node(void){
    NO *trie = (NO *) malloc(sizeof(NO));
    trie->end = FALSE;
    int i;
    for(i=0;i<ALPHABET;i++){
        trie->prox[i] = NULL;
    }
    return trie;
}

void insert_node(NO *trie, char *key, int begin){
    NO *p = trie;
    
    int i;
    for(i=begin;key[i]!='\0';i++){
		if(key[i]>='a' && key[i]<='z'){
			if(p->prox[key[i]-'a'] == NULL){
				p->prox[key[i] -'a'] = creat_node();
			}
			p = p->prox[key[i]-'a'];
			
		}else if(key[i]>='A' && key[i]<='Z'){
			
			if(p->prox[key[i]-'A'] == NULL){
				p->prox[key[i] -'A'] = creat_node();
			}
			p = p->prox[key[i]-'A'];
			
		}else{
			p->end=1;
			insert_node(trie, key,i+1);
			return;
		}
    }
    p->end=1;
}

int search_node(NO *trie, char *key){
    NO *p = trie;

    if(trie == NULL){
        return FALSE;
    }

    while(*key){
        p = p->prox[*key - 'a'];

        if(p == NULL){
            return FALSE;
        }
        key++;
    }

    if(p->end == TRUE && p != NULL){
        return TRUE;
    }else{
        return FALSE;
    }

}


int main(){

    int i;
    //char tweet[381];
    char word[200];
    char buffer[100];
    char buffer2[100];
    int length;
    char filename[30];
    int tmp;
    FILE *fp;
    char c;

    scanf("%s", filename);
    fp = fopen(filename, "r");

    i=0;
    NO *trie = creat_node();

    while(fscanf(fp, "%s", word)!=0){
 
        insert_node(trie, word, 0);
        if(feof(fp)){
            break;
        }
    }
    fclose(fp);

    scanf("%s", filename);
    fp = fopen(filename, "r");//opens the .json file
    strcpy(buffer, "\"text\":");

    while(!feof(fp)){
		//this program does not acounts for " that are not the one that close the text seccion
        fscanf(fp,"%s", word);
        
        if(!strcmp(word, buffer)){ //here begins the text seccion
			c = fgetc(fp);
			c = fgetc(fp);
			c++;
			
			strcpy(buffer2, "\"truncated\":");
			
			fscanf(fp, "%s", word);
			
			for(i=0;(strcmp(buffer2, word));i++){
				length = strlen(word);
				tmp = search_node(trie, ignore_special(word, length));
				if(tmp == FALSE){ //the word was not found in the trie, therefore it is written wrong
					printf("%s ", ignore_special_notcaps(word, length));
				}
				fscanf(fp, "%s", word);
			}
			printf("\n");
		}
        
    }    

	fclose(fp);

    return 0;
    
    
}

