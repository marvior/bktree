/*  
 *   Copyright (C) 2025 Walter Rosario Molino
 *
 *   This file is part of BKTREE.
 *   BKTREE is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU Lesser General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   BKTREE is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU Lesser General Public License for more details.
 *
 *   You should have received a copy of the GNU Lesser General Public License
 *   along with QSS (Quantized Similarity Search).  If not, see <http://www.gnu.org/licenses/>.
*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOLLERANCE 3
#define MAX_SON 10

/*
 * base structure with tree N-ary
 * each index is the distance with the value of the father node
 */
struct bknode{
    struct bknode * next[MAX_SON];
    char * value;
};


struct bkfunctions{
    int (*bk_distance)(struct bknode*,char*);
    void (*bk_insert_node)(struct bknode*, struct bknode*);
};

/*
 *Hamming distance with variant, adding the length difference for unequal strings 
 *
 */
int calculate_distance(struct bknode * node, char *key){
    int i=0;
    int distance=0;
    while(node->value[i]!='\0'){
        if (key[i]=='\0') break;
        if (node->value[i]!=key[i]) distance++;
        i++;
    }

    while(key[i]!='\0'){
        distance++;
        i++;
    }

    while(node->value[i]!='\0'){
        distance++;
        i++;
    }
    return distance;
}



/*
* insert the new node starting from a specific node
*
*/

void insert_node(struct bknode * start, struct bknode * new_node){
    int distance;
    printf("insert %s %s \n",start->value,new_node->value);
    int level=0;
    while(1){
        distance = calculate_distance(start, new_node->value);
        if (start->next[distance]==NULL) {
            start->next[distance] = new_node;
            printf("level insert as son of %s for distance %d at level  %d\n",start->value,distance,level);
            break;
        }else{
            start = start->next[distance];
            level++;
        }


    }

}


/*
 * Search all elements with distance <= TOLLERANCE 
 *
 */
struct bknode ** search_node(struct bknode * start, char * key){
    struct bknode ** candidate_list = malloc(MAX_SON* sizeof(struct bknode *));  //candidate list with distance <= TOLLERANCE
    int count = 0; 
    
    struct bknode * stack[100];  //stack used to navigate in the node with range [distance_father-TOLLERANCE, distance_father+TOLLERANCE]
    int top = 0;

    struct bknode * curr = NULL;

    int distance,distance_father;
    
    stack[top] = start;

    distance_father = calculate_distance(stack[top],key);

    if (distance_father <= TOLLERANCE){
        candidate_list[count++] = start;
        printf("element found %s \n",start->value);

    }
    while(1){
        if (top<0) break;

        curr = stack[top];
        distance_father = calculate_distance(curr,key);
        printf("*** father %s key %s current distance %d\n",curr->value,key,distance_father); 
        --top;

        for (int i=0; i <MAX_SON; i++){
            if (curr->next[i]!=NULL){
                distance = calculate_distance(curr->next[i],key);
                if (distance<=TOLLERANCE) {
                         candidate_list[count++] = curr->next[i];
                         printf("word %s distance %i elemnt found in the level %i %s \n",key,distance,i,curr->next[i]->value);
                     }
                if (distance>= distance_father-TOLLERANCE && distance <= distance_father+TOLLERANCE){
                    printf("*** name, distance %s,%d \n",curr->next[i]->value,distance);
                        stack[++top] = curr->next[i];
                    
                }    
            }   
         }

    }
    return candidate_list;

}


/*
 * modificare sulla base della struttura da  creare potrebbe anche essere non utilizzata
 */
struct bknode * create_node(char * value){
    struct bknode * new_node = malloc(sizeof(struct bknode));
    size_t len = strlen(value);
    char *new_value = malloc(len+1);

    strcpy(new_value,value);

    new_node->value=new_value;
    for (int i = 0 ; i<9; i++) new_node->next[i]=NULL;
   
    return new_node;

}




int main(void){
    char * lista[] ={"hell","help","shell","smell",
                           "fell","felt","oops","pop","oouch","halt"
    };
    
    struct bknode * start_node = NULL;
    struct bknode * tmp_node = NULL;
    struct bknode ** risultati = NULL; 
    for(int i=0;i<10;i++){
    
        printf("%s\n",lista[i]);

        if (start_node == NULL) start_node = create_node(lista[i]);
        else{

            tmp_node= create_node(lista[i]);
            insert_node(start_node,tmp_node);
        }

    
    } 
    // start_node = create_node(lista[0]);
    printf("first word is  %s\n",start_node->value);

    printf("\n\n Result:\n");
    risultati = search_node(start_node,"helt");
    for(int i=0; i<10; i++){
        if (risultati[i]!= NULL){
            printf("%s \n",risultati[i]->value);
        }
    }
    return 0;

}


