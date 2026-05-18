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

CTL *encode_props(char *props, int len){ //pas d'imbrications pour le moment
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
			else if(props[i]==')'){
				closed_brackets--;
			}
			else if(closed_brackets==1){
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
			//if()  // EF(p & EF(p &...))
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
	return ctl;
}

char check_EF(State *q,  CTL *ctl){
	/*
	if(ctl->op==11){
		return a_in_ap(q->ap,ctl->ap);
	}
	if(ctl->op==1){//AF<-
		for(int i=0;q->len_out;i++){
			if(!(q->leaf) || !(a_in_ap(q->ap,ctl->p->ap))){
				return 0;
			}
			check(q->out[i],ctl);
		}
	}
	if(ctl->op==2){//AG<-
		for(int i=0;q->len_out;i++){
			if(!(q->leaf) || !(a_in_ap(q->ap,ctl->p->ap))){
				return 0;
			}
			check(q->out[i],ctl);
			//ag_eg();
		}
	}
	if(ctl->op==3){//AX
		for(int i=0;q->len_out;i++){
			if(!(a_in_ap(q->out[i],ctl->p->ap))){
				return 0;
			}
		}
	}
	
	if(ctl->op==4){//EF<-
		for(int i=0;q->len_out;i++){
			if(q->leaf && a_in_ap(q->ap,ctl->p->ap))
			check(q->out[i],ctl);
		}
	}
	if(ctl->op==6){//EX<- 	
		for(int i=0;q->len_out;i++){
			if(a_in_ap(q->out[i],ctl->p->ap)){
				return 1;
			}
		}
	}
	*/
	return 0;
}


char model_checking(DAG *g, char * props, int len_props){
	CTL *ctl=encode_props(props,len_props);
	check_EF(g->q_0,ctl);
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

