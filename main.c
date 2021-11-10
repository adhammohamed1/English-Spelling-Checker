#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define FILENAME "EN-US-Dictionary.txt"

/**
       MOHAMED FARID ABDELAZIZ SALAMA    ID:6905
       YOUSSEF WALID MOHEYELDIN          ID:6967
       ADHAM MOHAMED ALI                 ID:6744
                                                     **/

typedef struct
{
    struct Node * leftChild;
    struct Node * rightChild;
    char data[64];

} Node;

/// Constructor for new node
Node *newNode(char word[])
{
    Node *n = malloc(sizeof(Node));
    strcpy( n->data, word );
    n->leftChild = NULL;
    n->rightChild = NULL;

    return n;
}

/// Function to insert node into the tree
Node * insertInTree( Node *root, char word[] )
{
    if( root == NULL ) return newNode( word );
    else if( strcasecmp( word, root->data ) > 0 )
        root->rightChild = (struct Node *)insertInTree( (Node *)root->rightChild, word );
    else if( strcasecmp( word, root->data ) < 0 )
        root->leftChild = (struct Node *)insertInTree( (Node *)root->leftChild, word );
    return root;

}

/// Function to find minimum leaf of a given root
Node* findMin(Node *root)
{
    if(root==NULL)
        return NULL;
    else if(root->leftChild!=NULL)
        return findMin((Node*)root->leftChild);
    else
        return root;
}

/// Function to find maximum leaf of a given root
Node* findMax(Node *root)
{
    if(root==NULL)
        return NULL;
    else if(root->rightChild!=NULL)
        return findMax((Node*)root->rightChild);
    else
        return root;
}

/// Function to find smaller Ancestor
Node *findSmallerAncestor(Node *root,Node* currentNode)
{
    Node *temp = root;
    Node *tempReturn;
    while(temp != currentNode)
    {
        if( strcasecmp( currentNode->data, temp->data ) > 0 )
        {
            tempReturn = temp;
            temp = (Node*)temp->rightChild;
        }
        else
        {
            temp=(Node*)temp->leftChild;
        }
    }
    return tempReturn;
}

/// Function to find the predecessor for a node
Node* findPredecessor(Node *root, Node *currentNode)
{

    if( currentNode == NULL )
    {
        return NULL;
    }
    else if( currentNode->leftChild != NULL )
    {
        return findMax( (Node*)currentNode->leftChild );
    }
    else if( currentNode->leftChild == NULL )
    {
        return findSmallerAncestor( root, currentNode );
    }
    return NULL;

}

/// Function to find the bigger ancestor of the node
Node * findBiggerAncestor(Node *root,Node *currentNode)
{
    Node*temp=root;
    Node*tempReturn;
    while(temp!=currentNode)
    {
        if( strcasecmp( currentNode->data, temp->data ) < 0 )
        {
            tempReturn=temp;
            temp=(Node*)temp->leftChild;
        }
        else
        {
            temp=(Node*)temp->rightChild;
        }
    }
    return tempReturn;
}

/// Function to find the in-order successor of the node
Node *findSuccessor(Node *root, Node *currentNode)
{
    if( currentNode == NULL ) return NULL;
    else if(currentNode->rightChild!=NULL)
    {
        return findMin( (Node*)currentNode->rightChild ) ;
    }
    else if( currentNode->rightChild == NULL )
    {
        return findBiggerAncestor(root,currentNode);
    }

    return NULL;

}

/// Function get number of lines in a file
int getNumberOfLines(char * filename)
{
    FILE *fp;
    int count_lines = 1;
    char chr;

    fp = fopen(filename, "r");

    chr = getc(fp);
    while (chr != EOF)
    {
        if (chr == '\n') count_lines++;
        chr = getc(fp);
    }
    fclose(fp);
    return count_lines;
}

///Function to get the greater of 2 given numbers
int max( int a, int b )
{
    return a > b ? a : b;
}

/// Function to get height of a tree from a given root
int getHeightofTree(Node * root)
{
    if(root == NULL) return -1;
    else return 1 + max( getHeightofTree( (Node*)root->rightChild), getHeightofTree((Node*)root->leftChild) );
}

/// Function to get data from file and insert it in a binary tree
Node * LoadDictionary()
{
    FILE * fp;
    int i = 0, numberOfLines = getNumberOfLines( FILENAME );
    char word[64];
    Node *tempRoot = NULL, *root = NULL;

    if(!(fp = fopen( FILENAME, "r" ) ))
    {
        printf("File \"%s\" could not be found.", FILENAME);
        return NULL;
    }
    printf("Loading dictionary ( 0 / %d )", numberOfLines);

    while( !feof( fp ) )
    {

        fscanf( fp, "%s\n", word );
        tempRoot = insertInTree( root, word );
        if( i == 0 ) root = tempRoot;
        printf("\r\x1b[19C\x1b[0d"); // Clear the progress numbers
        printf("( %d / %d )", ++i, numberOfLines);


    }
    system("cls");
    printf("Dictionary loaded successfully ( %d / %d )\n", i, numberOfLines);
    printf("Tree height = %d \n\n", getHeightofTree(root) );

    return root;
}

/// Function to search for a node containing the given word
Node * searchInTree( Node *root, char word[],Node *WordBefore,Node * constantRoot )
{
    if( root == NULL )
    {
        printf("\x1b[31m Incorrect.\t \x1b[0m");
        printf("SUGGESTIONS: ");
        printf("%s  //  ",WordBefore->data);
        printf("%s  //  ",findSuccessor(constantRoot,WordBefore)->data);
        printf("%s  ",findPredecessor(constantRoot,WordBefore)->data);
        return NULL;

    }
    else if( strcasecmp( word, root->data ) < 0 )
    {
        WordBefore=root;
        printf("\x1b[0m");
        return searchInTree( (Node*)root->leftChild, word,WordBefore,constantRoot);
    }

    else if( strcasecmp( word, root->data ) > 0 )
    {
        WordBefore=root;
        printf("\x1b[0m");
        return searchInTree( (Node*)root->rightChild, word,WordBefore,constantRoot);
    }
    else
        return root;
}

/// Function to check validity of the given word
void checkWord( Node *root, char word[] )
{
    Node *WordBefore=malloc(sizeof(Node));
    Node *r= searchInTree( root, word,WordBefore,root);
    if( r != NULL )
    {
        printf("\x1b[32m Correct.");
        printf("\x1b[0m");
    }

}

/// Function too analyze a given sentence
void analyseSentence( Node *root, char sentence[] )
{
    char word[64], tempStr[512], *token = "";
    strcpy( tempStr, sentence );
    token = strtok( tempStr, " " );
    while( token != NULL )
    {
        strcpy( word, token );
        printf("%s :", token);
        checkWord( root, word );
        printf("\n");

        token = strtok(NULL, " ");
    }


}


int main()
{
    char sentence[512];
    system("cls"); // Clear screen to enable use of ANSI escape codes
    Node *root = LoadDictionary(); // Load dictionary from " EN-US-Dictionary.txt "
    printf("Please enter a sentence\n");
    printf("===============================================================================|\n");
    printf(":=>                                                                            |\n");
    printf("===============================================================================|\r");
    printf("\x1b[1A\x1b[4C");
    gets( sentence );
    printf("\n\n");
    analyseSentence( root, sentence );


    return 0;
}
