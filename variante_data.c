#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include <string.h>

#define BASE 10

typedef enum TYPE_DATA_ENUM {
    INT_TYPE,
    FLOAT_TYPE,
    STRING_TYPE
} type_data;

typedef union VALUES_VARIANT{
    int i;
    float f;
    char *s;
} value;

typedef struct VARIABLE_VARAINT{
    type_data type;
    value val;
} variant;

typedef struct DATA_INFORAMTION_{
    type_data type;
    int float_point_index;
} data_info;

void print_variant(variant *v) {

    if (v == NULL) {
        return;
    }

    switch (v->type) {
        case FLOAT_TYPE:
            printf("%.02f\n", v->val.f);
            break;
        case INT_TYPE:
            printf("%d\n", v->val.i);
            break;
        case STRING_TYPE:
            printf("%s\n", v->val.s);
            break;
        default:
            printf("Unknown type\n");
            break;
    }
}

static int padd_to_left_space(char *buff,int size){

    int padd_to_left = 0;
    int write_index = 0;

    // Read and write at separate positions so characters are not overwritten.
    for (int read_index = 0; read_index < size; read_index++)
    {
        if (buff[read_index] == ' ')
        {
            padd_to_left++;
            continue;
        }

        buff[write_index++] = buff[read_index];
    }

    return padd_to_left;
}


data_info* type_detecetion(char *buff,int size){
    
    int count_point = 0;
    int index_point_float = -1;
    bool found_chars = false;

    for (int i = 0; i < size; i++)
    {
        if (buff[i] >= 'A' && buff[i] <= 'Z'){
            found_chars = true;
            break;
        }
        if (buff[i] >= 'a' && buff[i] <= 'z'){
            found_chars = true;
            break;
        }

        // Digits are valid numeric characters and need no special handling.
        if (buff[i] >= '0' && buff[i] <= '9')
        {
            continue;
        }

        if (buff[i] != '.')
        {
            found_chars = true;
            break;
        }

        if (buff[i] == '.' && !found_chars)
        {
            if (count_point >1)
            {
                found_chars = true;
                break;
            }
            index_point_float = i;
            count_point++;
        }
    }

    data_info *data = malloc(sizeof(data_info));

    if (data == NULL)
    {
        perror("null pointer: cannot allocate memory error\n");
        return NULL;
    }
    
    if (found_chars)
    {
        data->type = STRING_TYPE;
        data->float_point_index = -1;
        return data;
    }

    if (index_point_float != -1)
    {
        data->float_point_index = index_point_float;
        data->type = FLOAT_TYPE;
        return data;
    }

    data->type = INT_TYPE;
    data->float_point_index = -1;
    return data;
}

static int power_integer(int base,int exp){

    if (base == 0 && exp == 0)
    {
        fprintf(stderr,"math error: cannot zero power.");
        return -1;
    }
    
    int pow = 1;

    for (int i = 0; i < exp; i++)
    {
        pow *= base;
    }

    return pow;
}

/**
 * this inline functino is carrine the main logic of parsing 
 * i make it inline not just to save code ,but to name this step
 * and make it clear like you read pares char mean here 
 * the true prosse happen.
 */
static inline int pares_char(char a,int b,int c){
    return (a-b)*c;
}

float pares_to_float(char *buff,int size,data_info *a){
    
    if (a == NULL || buff == NULL)
    {
        fprintf(stderr,"invalid input: null pointer input ...\n");
        exit(1);
    }
    
    if (size < 1)
    {
        fprintf(stderr,"invalid input : size must be bigger then 1.\n");
        exit(1);
    }
    
    if (a->type != FLOAT_TYPE || a->float_point_index == -1)
    {
        fprintf(stderr,"wrong type input:input must be integer..\n");
        exit(1);
    }

    
    int ignored_space = padd_to_left_space(buff,size);
    // size is the number of input characters; the terminator is not counted.
    int digits_count = size-ignored_space;
    int integer_digits = a->float_point_index - ignored_space;
    int fraction_digits = digits_count - integer_digits - 1;

    if (integer_digits == 0 || fraction_digits == 0)
    {
        fprintf(stderr,"invalid input: invalid float point inedex.");
        exit(1);
    }

    char zero_base = '0';
    float number = 0;
    int integer_part = 0;
    int float_point_part = 0;

    for (int i = 0; i < a->float_point_index; i++)
    {
        int pw = power_integer(BASE,integer_digits-i-1);

        if (pw == -1)
        {
            fprintf(stderr,"output error: an error occur.");
            exit(1);
        }
        
        integer_part += pares_char(buff[i],zero_base,pw);
    }
   
    for (int i = a->float_point_index+1; i < size; i++)
    {
        int fraction_index = i - a->float_point_index - 1;
        int pw = power_integer(BASE,fraction_digits-fraction_index-1);
        if (pw == -1)
        {
            fprintf(stderr,"output error: an error occur.");
            exit(1);
        }
        
        float_point_part += pares_char(buff[i],zero_base,pw);
    }

    number = (float) integer_part;

    if (float_point_part == 0)
    { 
        return number;
    }

    int exposant = fraction_digits;
    float float_point_p = (float) float_point_part / (float) power_integer(10,exposant);

    number += float_point_p;

    return number; 

}

int pares_to_int(char *buff,int size,data_info *a){

    if (a == NULL || buff == NULL)
    {
        fprintf(stderr,"invalid input: null pointer input ..]\n");
        exit(1);
    }

    if (size < 1)
    {
        fprintf(stderr,"invalid input : size must be bigger then 1.\n");
        exit(1);
    }
    
    if (a->type != INT_TYPE)
    {
        fprintf(stderr,"wrong type input:input must be integer..\n");
        exit(1);
    }
    
    char zero_base = '0';
    int ignored_space = padd_to_left_space(buff,size);
    int number = 0;
    // size is the number of input characters; the terminator is not counted.
    int digits_count = size-ignored_space;

    for (int i = 0; i < digits_count; i++)
    {
        int pw = power_integer(BASE,digits_count-i-1);
        if (pw == -1)
        {
            fprintf(stderr,"output error: an error occur.");
            exit(1);
        }
        
        number += pares_char(buff[i],zero_base,pw);
    }

    return number;
}

variant* detecte_type_and_read_value(char *buff,int size){

    data_info *a = type_detecetion(buff,size);
    if (a == NULL)
    {
        return NULL;
    }

    variant *v = malloc(sizeof(variant));
    if (v == NULL)
    {
        free(a);
        return NULL;
    }

    switch (a->type)
    {
    case INT_TYPE: {
        int value_i =  pares_to_int(buff,size,a);
        v->type = a->type;
        v->val.i = value_i;
        break;
    }
    case FLOAT_TYPE: {
        float value_f =  pares_to_float(buff,size,a);
        v->type = a->type;
        v->val.f = value_f;
        break;
    }
    case STRING_TYPE:
        v->type = a->type;
        v->val.s =  buff;
        break;
    }

    free(a);
    return v;
}

int main(){

    char buffer[20];
    

    printf("write or give any type of data you want..(must be 19 char under or number)\n");

    if (scanf("%19s",buffer) != 1)
    {
        return 1;
    }

    // Pass the actual input length, excluding the existing terminator.
    int size = (int)strlen(buffer);


    variant *v = detecte_type_and_read_value(buffer,size);
    if (v == NULL)
    {
        return 1;
    }

    print_variant(v);

    free(v);
    return 0;
}