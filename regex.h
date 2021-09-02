

#ifndef REGEX_H
#define REGEX_H

// definitions for modifier bits
#define REP_START   (0x1)
#define REP_END     (0x2)
#define WILD        (0x4)
#define NOT         (0x8)
#define ANY_NUM     (0x10)
#define ONE_PLUS    (0x20)
#define ESCAPE      (0x40)

// the intermediate formate to create n-ary Regex trees
struct IF_Regex{
    // pointers to adjacent nodes in the tree
    struct IF_Regex *up;
    struct IF_Regex *down;
    // the left/right relationship acts as an OR
    struct IF_Regex *left;
    struct IF_Regex *right;

    // flags to denote special nodes:
    // repeat start
    // repeat end
    // wildcard
    // not
    // 0+
    // 1+
    // escape
    int modifier;

    // the character to match, depending on the flag might not be used
    char symbol;
    
    // the token to be returned when this pattern is matched
    // only valid on leaf nodes
    int token;
};

struct IF_Regex *generate_regex_tree(char *file);
void free_regex_tree(struct IF_Regex **root);

#endif
