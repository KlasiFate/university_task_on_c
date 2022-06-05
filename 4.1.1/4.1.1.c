#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct WordStruct
{
    char* string;
    int length;
    int space;
} Word;

const int DEFAULT_STRING_LENGTH = 64;

Word* Word_new(){
    Word* word = (Word*) malloc(sizeof(Word));
    word->string = (char*) malloc(sizeof(char) * DEFAULT_STRING_LENGTH);
    word->length = 0;
    word->space = DEFAULT_STRING_LENGTH;

    return word;
}

void Word_putc(Word* word, char symbol){
    if (word->space - word->length == 1){
        word->string = (char*) realloc(word->string, sizeof(char) * (word->space + DEFAULT_STRING_LENGTH));
        word->space += DEFAULT_STRING_LENGTH;
    }
    word->string[word->length] = symbol;
    word->length++;
}

void Word_end_word(Word* word){
    if(word->space - word->space > 1){
        word->string = (char*) realloc(word->string, sizeof(char) * (word->length + 1));
        word->space = word->length + 1;
    }
    word->string[word->length] = '\0';
}

void Word_destruct(Word* word){
    free(word->string);
    free(word);
}

typedef struct WordsStruct
{
    Word** array;
    int length;
    int space;
} Words;

const int DEFAULT_WORDS_LENGTH = 64;

Words* Words_new(){
    Words* words = (Words*) malloc(sizeof(Words));
    words->array = (Word**) malloc(sizeof(Word*) * DEFAULT_WORDS_LENGTH);
    words->length = 0;
    words->space = DEFAULT_WORDS_LENGTH;

    return words;
}

void Words_put_word(Words* words, Word* word){
    if(words->space - words->length < 1){
        words->array = (Word**) realloc(words->array, sizeof(Word*) * (words->space + DEFAULT_WORDS_LENGTH));
        words->space += DEFAULT_WORDS_LENGTH;
    }
    words->array[words->length] = word;
    words->length++;
}

void Words_end_words(Words* words){
    if(words->space - words->length > 0){
        words->array = (Word**) realloc(words->array, sizeof(Word*) * words->length);
        words->space = words->length;
    }
}

void Words_destruct(Words* words){
    free(words->array);
    free(words);
}

int Word_compare(Word* word1, Word* word2){
    for(int i = 0; i < word1->length && i < word2->length; i++){
        if(word1->string[i] > word2->string[i]){
            return -1;
        }
        if(word1->string[i] < word2->string[i]){
            return 1;
        }
    }
    if(word1->length == word2->length){
        return 0;
    }
    if(word1->length < word2->length){
        return 1;
    }else{
        return -1;
    }
}

typedef struct NodeStruct
{
    struct NodeStruct* left_child;
    struct NodeStruct* right_child;
    Word* key;
} Node;

Node* Node_new(Word* word){
    Node* node = (Node*) malloc(sizeof(Node));
    node->left_child = NULL;
    node->right_child = NULL;
    node->key = word;
    return node;
}

void Node_destruct(Node* node){
    Word_destruct(node->key);
    free(node);
}

int Node_compare(Node* node1, Node* node2){
    return Word_compare(node1->key, node2->key);
}

typedef struct NodesStruct {
    Node** array;
    int lenght;
    int space;
} Nodes;

const int DEFAULT_NODES_SPACE = 64;

Nodes* Nodes_new(){
    Nodes* nodes = (Nodes*) malloc(sizeof(Nodes));
    nodes->array = (Node**) malloc(sizeof(Node*) * DEFAULT_NODES_SPACE);
    nodes->space = DEFAULT_NODES_SPACE;
    nodes->lenght = 0;
    return nodes;
}

void Nodes_put_node(Nodes* nodes, Node* node){
    if(nodes->space - nodes->lenght == 0){
        nodes->array = (Node**) realloc(nodes->array, sizeof(Node*) * (nodes->space + DEFAULT_NODES_SPACE));
        nodes->space += DEFAULT_NODES_SPACE;
    }
    nodes->array[nodes->lenght] = node;
    nodes->lenght++;
}

int Nodes_insert(Nodes* nodes, int position, Node* node){
    if(position < 0 || nodes->lenght < position){
        return 1;
    }
    if(nodes->space - nodes->lenght - 1 == 0){
        nodes->array = (Node**) (nodes->array, sizeof(Node*) * (nodes->space + DEFAULT_NODES_SPACE));
        nodes->space += DEFAULT_NODES_SPACE;
    }
    if(position == nodes->lenght){
        nodes->array[nodes->lenght] = node;
        nodes->lenght++;
        return 0;
    }

    for(int i = nodes->lenght; i != position; i--){
        nodes->array[i] = nodes->array[i - 1];
    }
    nodes->array[position] = node;
    nodes->lenght++;
    return 0;
}


void Nodes_del_node(Nodes* nodes, int position){
    if(position < 0 || nodes->lenght <= position){
        return;
    }
    if(nodes->lenght - 1 == position){
        nodes->lenght--;
        return;
    }

    for(int i = position + 1; i < nodes->lenght; i++){
        nodes->array[i - 1] = nodes->array[i];
    }
    nodes->lenght--;
    return;
}


void Nodes_end_nodes(Nodes* nodes){
    if(nodes->space - nodes->lenght > 0){
        nodes->array = (Node**) realloc(nodes->array, sizeof(Node*) * (nodes->lenght));
        nodes->space = nodes->lenght;
    }
}

void Nodes_destruct(Nodes* nodes){
    free(nodes->array);
    free(nodes);
}

typedef struct BinaryTreeStruct
{
    Node* root;
    int node_count;
} BinaryTree;

BinaryTree* BinaryTree_new(Node* root){
    BinaryTree* binary_tree = (BinaryTree*) malloc(sizeof(BinaryTree));
    binary_tree->root = root;
    binary_tree->node_count = 1;
    return binary_tree;
}

int BinaryTree_insert(BinaryTree* binary_tree, Node* new_node){
    Node* current_node = binary_tree->root;
    Node* parent_node = NULL;

    int comparing_result;
    while (current_node != NULL){
        comparing_result = Node_compare(current_node, new_node);
        if(comparing_result == 0){
            return 1;
        }

        parent_node = current_node;
        if(comparing_result > 0){
            current_node = current_node->right_child;
        }else{
            current_node = current_node->left_child;
        }
    }
    if(comparing_result > 0){
        parent_node->right_child = new_node;
    }else{
        parent_node->left_child = new_node;
    }
    binary_tree->node_count++;
    return 0;
}

int BinaryTree_delete_node(BinaryTree* binary_tree, Word* key){
    Node* node_to_del = Node_new(key);
    Node* current_node = binary_tree->root;
    Node* parent_node = NULL;

    int comparing_result = Node_compare(current_node, node_to_del);
    int is_left;
    while(comparing_result != 0){
        parent_node = current_node;
        if(comparing_result > 0){
            current_node = current_node->right_child;
            is_left = 0;
        }else{
            current_node = current_node->left_child;
            is_left = 1;
        }
        if(current_node == NULL){
            return 1;
        }
        comparing_result = Node_compare(current_node, node_to_del);
    }

    binary_tree->node_count--;

    Node_destruct(node_to_del);
    node_to_del = current_node;
    
    if(node_to_del->right_child == NULL){
        if(parent_node != NULL){
            if(is_left == 1){
                parent_node->left_child = node_to_del->left_child;
            }else{
                parent_node->right_child = node_to_del->left_child;
            }
        }else{
            binary_tree->root = node_to_del->left_child;
        }
        return 0;
    }else{
        if(parent_node != NULL){
            if(is_left == 1){
                parent_node->left_child = node_to_del->right_child;
            }else{
                parent_node->right_child = node_to_del->right_child;
            }
        }else{
            binary_tree->root = node_to_del->right_child;
        }
    }
    

    if(node_to_del->left_child == NULL || node_to_del->right_child == NULL){
        return 0;
    }

    current_node = node_to_del->right_child;
    while (current_node->left_child != NULL){
        current_node = current_node->left_child;
    }

    current_node->left_child = node_to_del->left_child;

    Node_destruct(node_to_del);

    return 0;
}

Words* get_words_by_level(BinaryTree* binary_tree, int level){
    if(level < 0){
        return NULL;
    }

    if(binary_tree->root == NULL){
        return NULL;
    }

    int current_level = 0, nodes_i, lenght_of_last_level;
    Nodes* nodes = Nodes_new();
    Node* node;

    Nodes_put_node(nodes, binary_tree->root);

    while(current_level != level){
        lenght_of_last_level = nodes->lenght;
        if(lenght_of_last_level == 0){
            break;
        }
        for(nodes_i = 0; nodes_i < nodes->lenght; nodes_i++){
            node = nodes->array[nodes_i];

            if(node->left_child != NULL){
                nodes->array[nodes_i] = node->left_child;
                if(node->right_child != NULL){
                    Nodes_insert(nodes, nodes_i + 1, node->right_child);
                    nodes_i++;
                }
            }else{
                if(node->right_child != NULL){
                    nodes->array[nodes_i] = node->right_child;
                }else{
                    Nodes_del_node(nodes, nodes_i);
                    lenght_of_last_level--;
                    nodes_i--;
                }
            }
        }

        current_level++;
    }

    Nodes_end_nodes(nodes);

    Words* words = Words_new();
    Word* word;
    for(nodes_i = 0; nodes_i < nodes->lenght; nodes_i++){
        word = nodes->array[nodes_i]->key;

        Words_put_word(words, word);
    }

    Nodes_destruct(nodes);

    Words_end_words(words);
    
    return words;
}

Words* scan_sentence(FILE* file_in){
    char DELETE_word[] = "DELETE:";
    int DELETE_lenght = 7;

    Words* words = Words_new();
    Word* word = Word_new();

    char symbol;
    while((symbol = fgetc(file_in)) != '\n'){}

    int words_per_line;
    while(symbol != '\0'){
        words_per_line = 0;
        while ((symbol = fgetc(file_in)) != '\n' && symbol != '\0'){
            if(symbol == ' '){
                if(word->length != 0){
                    Word_end_word(word);
                    words_per_line++;
                    Words_put_word(words, word);
                    word = Word_new();
                }
            }else{
                Word_putc(word, symbol);
            }
        }
        if(word->length != 0){
            if(strncmp(word->string, DELETE_word, word->length) == 0 && words_per_line == 0 && word->length == DELETE_lenght){
                Word_destruct(word);
                break;
            }else{
                Word_end_word(word);
                words_per_line++;
                Words_put_word(words, word);
                word = Word_new();
            }
        }
    }
    Words_end_words(words);
    return words;
}

Words* scan_words_to_del(FILE* file_in){
    char LEVEL_word[] = "LEVEL:";
    int LEVEL_lenght = 6;

    char symbol = 'a';
    Word* word;
    Words* words = Words_new();

    while (symbol != '\0')
    {
        word = Word_new();
        while((symbol = fgetc(file_in)) != '\n'){
            Word_putc(word, symbol);
        }
        if(strncmp(word->string, LEVEL_word, word->length) == 0 && LEVEL_lenght == word->length){
            Word_destruct(word);
            break;
        }else{
            Word_end_word(word);
            Words_put_word(words, word);
        }
    }
    Words_end_words(words);
    return words;
}


BinaryTree* build_binary_tree(Words* words){
    Node* root = Node_new(words->array[0]);
    BinaryTree* binary_tree = BinaryTree_new(root);

    Node* node;
    int necessary_to_del;
    for(int i = 1; i < words->length; i++){
        node = Node_new(words->array[i]);
        necessary_to_del = BinaryTree_insert(binary_tree, node);
        if(necessary_to_del == 1){
            Node_destruct(node);
        }
    }

    return binary_tree;
}



int main(){
    FILE* file_in;
    FILE* file_out;
    if((file_in = fopen("input.txt", "r")) == NULL){
        // printf("Attemtp to open file, %s is fail", name_in);
        return 1;
    }
    if((file_out = fopen("output.txt", "w")) == NULL){
        // printf("Attemtp to open file, %s is fail", name_out);
        return 1;
    }

    fprintf(file_out, "");

    Words* words = scan_sentence(file_in);

    BinaryTree* binary_tree = build_binary_tree(words);
    Words_destruct(words);

    Words* words_to_del = scan_words_to_del(file_in);

    for(int i = 0; i < words_to_del->length; i++){
        BinaryTree_delete_node(binary_tree, words_to_del->array[i]);
    }

    Words_destruct(words_to_del);

    char* temp = (char*) malloc(sizeof(char) * 64);
    fgets(temp, 63, file_in);
    int level = atoi(temp);
    free(temp);

    Words* words_to_print = get_words_by_level(binary_tree, level);

    fprintf(file_out, "%d\n", binary_tree->node_count);
    if(words_to_print != NULL){
        for(int i = 0; i < words_to_print->length; i++){
            fprintf(file_out, "%s ", words_to_print->array[i]->string);
        }
        Words_destruct(words_to_print);
    }

    fclose(file_in);
    fclose(file_out);

    return 0;
}