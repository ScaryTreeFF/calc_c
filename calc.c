#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR 255

//Интерфейс Стека
typedef struct{
	int *data;
	size_t counter, max_size;
} stack_t;

stack_t create_stack(size_t size);
void	delete_stack(stack_t* stack);

size_t	stack_size(stack_t* stack);

int		stack_top(stack_t* stack);
bool	stack_pop(stack_t* stack);
bool	stack_push(stack_t* stack, int value);


int prior(char c){
	switch(c){
		case '*':
		case '/':
			return 3;
		case '-':
		case '+':
			return 2;
		case '(':
			return 1;
	}
}

//Перевод в обратную польскую запись
void to_rpn(char* output){
	char input[MAX_STR];
	size_t i = 0, j = 0;
	int flag = 0, is_neg = 0;
	stack_t stack = create_stack(MAX_STR);

	fgets(input, sizeof(input), stdin);

	while(input[i] != '\0'){
		if(j >= MAX_STR){
			puts("Not enough space");
			break;
		}
		if(input[i] == ')'){
			while(stack_top(&stack) != '('){
				output[j++] = ' ';
				output[j++] = stack_top(&stack);
				stack_pop(&stack);
			}
			stack_pop(&stack);
		}
		if(input[i] >= '0' && input[i] <= '9'){
			if(flag){
				output[j++] = ' ';
				flag = 0;
			}
			
			if(input[i - 1] == '-' && is_neg == 1){
				is_neg = 0;
				output[j++] = '-';
			}
			
			output[j++] = input[i];
		}
		if(input[i] == '('){
			stack_push(&stack, input[i]);
		}
		if(input[i] == '+' || input[i] == '-' || input[i] == '/' || input[i] == '*'){
			if(input[i] == '-' && input[i - 1] == '('){
				is_neg = 1;
				continue;
			}
			flag = 1;
			if(stack_size(&stack) == 0){
				stack_push(&stack, (int)input[i]);
			}
			else{
				while((stack_top(&stack) != -1) && (prior(stack_top(&stack)) >= prior(input[i]))){
					output[j++] = ' ';
					output[j++] = stack_top(&stack);
					stack_pop(&stack);
				}
				stack_push(&stack, input[i]);
			}
		}
		i++;
	}

	while((stack_size(&stack) != 0)){
		char top = (char)stack_top(&stack);
		if(top == '+' || top == '-' || top == '*' || top == '/'){
			output[j++] = ' ';
			output[j++] = top;
			stack_pop(&stack);
		}
		else{
			puts("Wrong input");
			break;
		}	
	}
	output[j] = '\0';
}


//Считает выражение записанное в ОПН
int calculate(char* expr){
	int oper_1, oper_2, res, flag = 0;
	size_t i = 0, buff_i = 0;
	char buff[MAX_STR];
	stack_t stack = create_stack(MAX_STR);

	for(size_t j = 0; j < MAX_STR; j++)
		buff[j] = '\0';

	while(expr[i] != '\0'){
		if(expr[i] >= '0' && expr[i] <= '9'){
			if(expr[i - 1] == '-')
				buff[buff_i++] = '-';
			buff[buff_i++] = expr[i];
			flag = 1;
		}
		if(expr[i] == ' ' && flag == 1){
			stack_push(&stack, atoi(buff));
			for(size_t j = 0; j < MAX_STR; j++)
				buff[j] = '\0';
			buff_i = 0;
			flag = 0;
		}
		if(expr[i] == '-' || expr[i] == '+' || expr[i] == '*' || expr[i] == '/'){
			oper_2 = stack_top(&stack);
			stack_pop(&stack);
			oper_1 = stack_top(&stack);
			stack_pop(&stack);
			switch(expr[i]){
				case '+': res = oper_1 + oper_2; break;
				case '-': res = oper_1 - oper_2; break;
				case '*': res = oper_1 * oper_2; break;
				case '/': res = oper_1 / oper_2; break;
			}
			stack_push(&stack, res);
		}
		i++;
	}
	return stack_top(&stack);
}



int main(){
	char output[MAX_STR];
	to_rpn(output);
	printf("%i", calculate(output));
	return 0;
}



//Реализация Стека
stack_t create_stack(size_t size){
	stack_t stack;
	stack.data = (int*)malloc(size * sizeof(int));
	stack.counter = -1;
	stack.max_size = size;
	return stack;
}

void delete_stack(stack_t* stack){
	free(stack->data);
	stack->counter = -1;
	stack->max_size = 0;	
}

size_t stack_size(stack_t* stack){
	return (stack->counter + 1);
}

int stack_top(stack_t* stack){
	if(stack->counter == -1){
		return (-1);
	}
	else
		return stack->data[stack->counter];
}

bool stack_pop(stack_t* stack){
	if(stack->counter == -1)
		return 1;
	else{
		stack->counter--;
		return 0;
	}
}

bool stack_push(stack_t* stack, int value){
	if((stack->counter + 1) >= stack->max_size)
		return 1;
	else{
		stack->data[++stack->counter] = value;
		return 0;
	}
}
