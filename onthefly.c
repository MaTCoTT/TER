#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "onthefly.h"


int a_in_ap(char *l, char a){
	for(int i=0;i<AP_SIZE;i++){
		if(l[i]==a){
			return 1;
		}
	}
	return 0;
}

CTL *encode_prop(char *props, int len){ //pas d'imbrications pour le moment
	CTL *ctl=malloc(sizeof(CTL)),*curr_ctl=ctl;
	int i=0;
	while(i<len){//EF:4,EG=5,EX=6,&:7,|:8,T=9,F=10,AP=11
		if(props[i]=='A'){
			if(props[i+1]=='F'){//¬EG(¬)
				curr_ctl->op=5;
			}
			else if(props[i+1]=='G'){//¬EF(¬)
				curr_ctl->op=4;
			}
			else{//¬EX(¬)
				curr_ctl->op=6;
			}
			curr_ctl->negation=1;
		}
		if(props[i]=='E'){
			if(props[i+1]=='F'){//EF
				curr_ctl->op=5;
			}
			else if(props[i+1]=='G'){//EG
				curr_ctl->op=4;
			}
			else{//EX
				curr_ctl->op=6;
			}
		}
		i=i+3;
		int closed_brackets=1;//hypothese: chaque sous formule CTL est enveloppée de paranthèses EF((p)|(EF(¬q)))
		while((closed_brackets>0) && (i<len)){
			if(props[i]=='('){
				closed_brackets++;
			}
			if(props[i]==')'){
				closed_brackets--;
			}
			if(closed_brackets==1){
				if(props[i]=='&'){
					curr_ctl->op_p=7;
				}
				if(props[i]=='|'){
					curr_ctl->op_p=8;
				}
				/*if(props[i]=='U'){
					curr_ctl->op_p=12;
				}*/
			}
		}
	}
	/*
	if(len)
	else if(props[0]=='A'){
		if(props[1]=='F'){
			
		}
		else if(props[1]=='G'){
		
		}
		else{//AX
		
		}
	}
	else if(props[0]=='E'){
		if(props[1]=='F'){
			
		}
		else if(props[1]=='G'){
		
		}
		else{//EX
		
		}
	}
	else{
		if(props[0]=='')//AP
		if(props[1]=='&'){
			
		}
		if(props[1]=='|'){
		
		}
		if(props[1]=='¬'){
		
		}
	}
	*/
}

char model_checking(DAG *g, char * props, int len_props){
	CTL *ctl=encode_prop(props,len_props);
	//check_EF(g->q_0,props);
	//for(int i=0;i<g->)
	return 0;
}

void free_State(DAG *dag){
	for(int i=0;i<dag->num_states;i++){
		free(dag->s[i]->out);
		free(dag->s[i]);
	}
	free(dag->s);
	return;
}

void free_DAG(DAG *dag){
	free_State(dag);
	free(dag);
	
	return;
}

char ag_eg(){
	
}



State* all_states[MAX_STATES] = {NULL};

State* charger_etat(int target_id, const char* filename) {
    if (all_states[target_id] != NULL) {
        return all_states[target_id];
    }

    FILE* file = fopen(filename, "r");
    if (!file) {
        printf("Erreur : Impossible d'ouvrir %s\n", filename);
        return NULL;
    }

    State* current = NULL;
    char line[256];
    int reading = 0;

    while (fgets(line, sizeof(line), file)) {
        int state_id;
        char ap_buf[64] = {0};

        if (sscanf(line, "state q%d (%63[^)])", &state_id, ap_buf) >= 1) {
            if (state_id == target_id) {
                reading = 1;
                current = calloc(1, sizeof(State));
                current->n = target_id;

                if (strcmp(ap_buf, "empty") != 0 && strlen(ap_buf) > 0) {
                    int ap_idx = 0;
                    for (int i = 0; ap_buf[i] != '\0' && ap_idx < AP_SIZE; i++) {
                        if (ap_buf[i] != ' ' && ap_buf[i] != ',') {
                            current->ap[ap_idx++] = ap_buf[i];
                        }
                    }
                    if (ap_idx < AP_SIZE) current->ap[ap_idx] = '\0';
                } else {
                    current->ap[0] = '\0';
                }
                continue;
            } else if (reading) {
                break;
            }
        }

        if (reading && strncmp(line, "transition", 10) == 0) {
            int src, dest;

            if (sscanf(line, "transition q%d -> q%d", &src, &dest) == 2) {
                if (src == dest) {
                    current->leaf = 1;
                } else {
                    current->len_out++;
                    current->out = realloc(current->out, current->len_out * sizeof(State*));
                    State* ref = calloc(1, sizeof(State));
                    ref->n = dest;
                    current->out[current->len_out - 1] = ref;
                }
            }
        }
    }

    fclose(file);

    if (current != NULL) {
        all_states[target_id] = current;
        printf("[PARSEUR] Etat q%d charge (AP: '%s', %d successeurs%s)\n",
               target_id, current->ap, current->len_out,
               current->leaf ? ", FEUILLE" : "");
    }

    return current;
}

int check_EX(int current_id, char target_ap, const char* filename) {
    State* s = charger_etat(current_id, filename);
    if (s == NULL) return 0;

    for (int i = 0; i < s->len_out; i++) {
        int next_id = s->out[i]->n;
        State* next = charger_etat(next_id, filename);
        if (next != NULL && a_in_ap(next->ap, target_ap)) {
            return 1;
        }
    }
    return 0;
}

int check_EF(int current_id, char target_ap, const char* filename) {
    State* s = charger_etat(current_id, filename);
    if (s == NULL) return 0;

    if (a_in_ap(s->ap, target_ap)) {
        printf(" -> TEMOIN en q%d\n", current_id);
        return 1;
    }

    for (int i = 0; i < s->len_out; i++) {
        int next_id = s->out[i]->n;
        if (check_EF(next_id, target_ap, filename)) {
            return 1;
        }
    }
    return 0;
}

int check_EG(int current_id, char target_ap, const char* filename) {
    State* s = charger_etat(current_id, filename);
    if (s == NULL) return 0;

    if (!a_in_ap(s->ap, target_ap)) {
        return 0;
    }

    if (s->leaf) {
        return 1;
    }

    for (int i = 0; i < s->len_out; i++) {
        int next_id = s->out[i]->n;
        if (check_EG(next_id, target_ap, filename)) {
            return 1;
        }
    }
    return 0;
}

int check_nested_EF_EF(int current_id, char p, char q, const char* filename) {
    State* s = charger_etat(current_id, filename);
    if (s == NULL) return 0;

    if (a_in_ap(s->ap, p)) {
        if (check_EF(current_id, q, filename) == 1) {
            printf(" -> TEMOIN IMBRIQUE : q%d a '%c' et mene a '%c'\n", current_id, p, q);
            return 1;
        }
    }
    for (int i = 0; i < s->len_out; i++) {
        int next_id = s->out[i]->n;
        if (check_nested_EF_EF(next_id, p, q, filename)) {
            return 1;
        }
    }
    
    return 0;
}

int check_EF_p_ET_feuille(int current_id, char target_ap, const char* filename) {
    State* s = charger_etat(current_id, filename);
    if (s == NULL) return 0;
 
    if (s->leaf && a_in_ap(s->ap, target_ap)) {
        printf(" -> FEUILLE TEMOIN en q%d\n", current_id);
        return 1;
    }
 
    for (int i = 0; i < s->len_out; i++) {
        int next_id = s->out[i]->n;
        if (check_EF_p_ET_feuille(next_id, target_ap, filename)) {
            return 1;
        }
    }
    return 0;
}

int check_AF_p_ET_feuille(int current_id, char target_ap, const char* filename) {
    State* s = charger_etat(current_id, filename);
    if (s == NULL) return 0;
 
    if (s->leaf) {
        return a_in_ap(s->ap, target_ap);
    }
 
    for (int i = 0; i < s->len_out; i++) {
        int next_id = s->out[i]->n;
        if (check_AF_p_ET_feuille(next_id, target_ap, filename) == 0) {
            return 0;
        }
    }
    return 1;
}

int check_AF_at_state(int state_id, char target_ap, const char* filename) {
    State* s = charger_etat(state_id, filename);
    if (s == NULL) return 0;

    if (a_in_ap(s->ap, target_ap)) {
        return 1;
    }

    if (s->leaf) {
        return 0;
    }

    for (int i = 0; i < s->len_out; i++) {
        int next_id = s->out[i]->n;
        if (check_AF_at_state(next_id, target_ap, filename) == 0) {
            return 0;
        }
    }
    return 1;
}
 
int check_EG_AF(int current_id, char target_ap, const char* filename) {
    State* s = charger_etat(current_id, filename);
    if (s == NULL) return 0;

    if (check_AF_at_state(current_id, target_ap, filename) == 0) {
        return 0;
    }

    if (s->leaf) {
        return 1;
    }

    for (int i = 0; i < s->len_out; i++) {
        int next_id = s->out[i]->n;
        if (check_EG_AF(next_id, target_ap, filename)) {
            return 1;
        }
    }
    return 0;
}
