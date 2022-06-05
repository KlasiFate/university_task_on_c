#include <stdlib.h>
#include <stdio.h>
#include <string.h>



typedef struct VariableStruct
{
    char* name;
    int value;
} Variable;

typedef struct TupleStruct
{
    int integer_number;
    void* pointer;
} Tuple;

typedef struct OperationStruct
{
    Tuple first_operation_or_identifier;
    Tuple second_operation_or_identifier;
    char action;
    
} Operation;

Tuple INCORRECT_EXPRESSION = (Tuple) {0, NULL};






Tuple scan_variables(FILE* fp_in){
    int variable_count = 0;
    fscanf(fp_in, "%d\n", &variable_count);

    Variable* variables = (Variable*) malloc(sizeof(Variable) * variable_count);
    
    char* string = (char*) malloc(257);
    int name_length;

    for(int i = 0; i < variable_count; i++){
        fgets(string, 257, fp_in);
        name_length = (int) (strchr(string, ' ') - string);
        variables[i] = (Variable) {
            strncpy((char*) malloc(sizeof(char) * (name_length + 1)), string, name_length),
            atoi(string + name_length + 1)
        };
        variables[i].name[name_length] = '\0';
    }

    free(string);

    return (Tuple) {
        variable_count,
        variables
    };

}




int is_unavailable_symbol(char symbol){
    if ((65 <= (int) symbol && (int) symbol <= 90) || (97 <= (int) symbol && (int) symbol <= 122) || (48 <= (int) symbol && (int) symbol <= 57) || (int) symbol == 95){
        return 0;
    }
    return 1;
}





Tuple scan_expression(char* string, int string_length){
    if(string[0] != '('){
        int string_i;
        for(string_i = 0; string_i < string_length; string_i++){
            if(is_unavailable_symbol(string[string_i]) == 1){
                return INCORRECT_EXPRESSION;
            }
        }
        if(string_i != string_length){
            return INCORRECT_EXPRESSION;
        }

        char* identifier = strncpy((char*) malloc(sizeof(char) * (string_length + 1)), string, string_length);
        identifier[string_length] = '\0';

        return (Tuple) {
            0,
            (void*) identifier
        };

    }else{
        if(string[string_length - 1] != ')'){
            return INCORRECT_EXPRESSION;
        }
        if(string_length < 3){
            return INCORRECT_EXPRESSION;
        }
        int count_bracket = 0, string_i, first_expression_length = -1;
        char action = ' ';
        for(string_i = 1; string_i < string_length - 1; string_i++){
            if(string[string_i] == '('){
                count_bracket++;
                continue;
            }
            if(string[string_i] == ')'){
                count_bracket--;
                continue;
            }
            if (string[string_i] != '*' && string[string_i] != '/' && string[string_i] != '+' && string[string_i] != '-'){
                if(is_unavailable_symbol(string[string_i]) == 1){
                    return INCORRECT_EXPRESSION;
                }
            }else{
                if(count_bracket == 0){
                    first_expression_length = string_i - 1;
                    action = string[string_i];
                    break;
                }
            }
        }
        if(action == ' '){
            return INCORRECT_EXPRESSION;
        }
        
        int second_expression_length = string_length - 1 - first_expression_length - 1 - 1;

        if(second_expression_length < 1){
            return INCORRECT_EXPRESSION;
        }

        Tuple first_operation = scan_expression(string + 1, first_expression_length);
        Tuple second_operation = scan_expression(string + 1 + first_expression_length + 1, second_expression_length);

        if(first_operation.pointer == NULL || second_operation.pointer == NULL){
            return INCORRECT_EXPRESSION;
        }
        Operation* operation = (Operation*) malloc(sizeof(Operation));
        operation[0] = (Operation) {
            first_operation,
            second_operation,
            action
        };

        return (Tuple) {
            1,
            (void*) operation
        };
    }
}

Variable* find_identifier(char* identifier, Variable* variables, int variable_count){
    for(int i = 0; i < variable_count; i++){
        if (strcoll(identifier, variables[i].name) == 0){
            return (variables + i);
        }
    }
    return NULL;
}

Tuple process_operation(Tuple operation_or_identifier, Variable* variables, int variable_count){
    int value;
    if(operation_or_identifier.integer_number == 0){
        char* identifier = (char*) operation_or_identifier.pointer;
        value = atoi(identifier);
        if(value == 0 && identifier[0] != '0'){
            Variable* variable = find_identifier(identifier, variables, variable_count);
            if(variable == NULL){
                free(operation_or_identifier.pointer);
                return (Tuple) {0, NULL};
            }
            value = variable -> value;
        }
        free(operation_or_identifier.pointer);
        return (Tuple) {value, (void*) 1};
    }

    Operation operation = *((Operation*) operation_or_identifier.pointer);
    Tuple first_result = (Tuple) process_operation(operation.first_operation_or_identifier, variables, variable_count);
    Tuple second_result = (Tuple) process_operation(operation.second_operation_or_identifier, variables, variable_count);
    char action = operation.action;

    if(first_result.pointer == NULL || second_result.pointer == NULL){
        return (Tuple) {0, NULL};
    }

    int first_value = first_result.integer_number;
    int second_value = second_result.integer_number;

    if(operation.action == '+'){
        value = first_value + second_value;
    }
    if(operation.action == '-'){
        value = first_value - second_value;
    }
    if(operation.action == '*'){
        value = first_value * second_value;
    }
    if(operation.action == '/'){
        value = first_value / second_value;
    }

    free(operation_or_identifier.pointer);

    return (Tuple) {
        value,
        (void*) 1
    };
}




char* replace(char* string, Variable* variables, int variable_count){
    char* var_name = (char*) malloc(sizeof(char) * 257);
    char* new_string = (char*) malloc(sizeof(char) * 257);
    int new_string_len = 257;
    char* new_substring = (char*) malloc(sizeof(char) * 257);
    int var_name_i = 0, has_letter = 0, new_string_i = 0;

    for(int string_i = 0; string[string_i] != '\0' && string[string_i] != '\n'; string_i++){
        if (new_string_i == 1023){
            new_string = (char*) realloc(new_string, sizeof(char) * (new_string_len + 255));
            new_string_len += 255;
        }
        if(is_unavailable_symbol(string[string_i]) == 1){
            if(var_name_i != 0){
                if(has_letter == 1){
                    var_name[var_name_i] = '\0';
                    Variable* variable_pointer = find_identifier(var_name, variables, variable_count);
                    if(variable_pointer != NULL){
                        Variable variable = *variable_pointer;
                        sprintf(new_substring, "%d", variable.value);
                        strcpy(new_string + new_string_i, new_substring);
                        new_string_i += strlen(new_substring);
                    }else{
                        strncpy(new_string + new_string_i, var_name, var_name_i);
                        new_string_i += var_name_i;
                    }
                }else{
                    strncpy(new_string + new_string_i, var_name, var_name_i);
                    new_string_i += var_name_i;
                }
            }
            new_string[new_string_i] = string[string_i];
            new_string_i++;
            var_name_i = 0;
            has_letter = 0;
            continue;
        }
        var_name[var_name_i] = string[string_i];
        var_name_i++;
        if (48 > (int) string[string_i] || (int) string[string_i] > 57){
            has_letter = 1;
        }
    }
    if(var_name_i != 0){
        if(has_letter == 1){
            Variable* variable_pointer = find_identifier(var_name, variables, variable_count);
            if(variable_pointer != NULL){
                Variable variable = *variable_pointer;
                sprintf(new_substring, "%d", variable.value);
                strcpy(new_string + new_string_i, new_substring);
                new_string_i += strlen(new_substring);
            }else{
                strncpy(new_string + new_string_i, var_name, var_name_i);
                new_string_i += var_name_i;
            }
        }else{
            strncpy(new_string + new_string_i, var_name, var_name_i);
            new_string_i += var_name_i;
        }
    }
    new_string[new_string_i] = '\0';

    free(var_name);
    free(new_substring);
    free(string);
    return new_string;
}






char name_in[] = "input.txt";
char name_out[] = "output.txt";

int main(){
    FILE* fp_in;
    FILE* fp_out;
    if((fp_in = fopen(name_in, "r")) == NULL){
        // printf("Attemtp to open file, %s is fail", name_in);
        return 1;
    }
    if((fp_out = fopen(name_out, "w")) == NULL){
        // printf("Attemtp to open file, %s is fail", name_out);
        return 1;
    }
    Tuple res = scan_variables(fp_in);
    int variable_count = res.integer_number;
    Variable* variables = (Variable*) res.pointer;


    char* string = (char*) malloc(257);
    fgets(string, 257, fp_in);
    fclose(fp_in);
    
    Tuple operation_or_identifier = scan_expression(string, strlen(string) - 1);
    if(operation_or_identifier.pointer == NULL){
        fprintf(fp_out, "INCORRECT");
        fclose(fp_out);
        return 0;
    }

    char* new_string = replace(string, variables, variable_count);
    Tuple result = process_operation(operation_or_identifier, variables, variable_count);
    if(result.pointer != NULL){
        // fprintf(fp_out, "%s → %d", new_string, result.integer_number);
        fprintf(fp_out, "%s", new_string);
    }else{
        fprintf(fp_out, "%s", new_string);
    }
    free(new_string);
    fclose(fp_out);
    return 0;



}