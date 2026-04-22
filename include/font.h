struct fcamn
{
	unsigned short mg;
	char ch;
}__attribute__((packed));

struct lsctf
{
	unsigned short lsc;
	char ch;
	char sch;
}__attribute__((packed));

const struct fcamn fmcl[] = 
{
    {0xB300, ' '}, {0xB301, '!'}, {0xB302, '"'}, {0xB303, '#'},
    {0xB304, '$'}, {0xB305, '%'}, {0xB306, '&'}, {0xB307, '\''},
    {0xB308, '('}, {0xB309, ')'}, {0xB30A, '*'}, {0xB30B, '+'},
    {0xB30C, ','}, {0xB30D, '-'}, {0xB30E, '.'}, {0xB30F, '/'},
    {0xB310, '0'}, {0xB311, '1'}, {0xB312, '2'}, {0xB313, '3'},
    {0xB314, '4'}, {0xB315, '5'}, {0xB316, '6'}, {0xB317, '7'},
    {0xB318, '8'}, {0xB319, '9'}, {0xB31A, ':'}, {0xB31B, ';'},
    {0xB31C, '<'}, {0xB31D, '='}, {0xB31E, '>'}, {0xB31F, '?'},
    {0xB320, '@'}, {0xB321, 'A'}, {0xB322, 'B'}, {0xB323, 'C'},
    {0xB324, 'D'}, {0xB325, 'E'}, {0xB326, 'F'}, {0xB327, 'G'},
    {0xB328, 'H'}, {0xB329, 'I'}, {0xB32A, 'J'}, {0xB32B, 'K'},
    {0xB32C, 'L'}, {0xB32D, 'M'}, {0xB32E, 'N'}, {0xB32F, 'O'},
    {0xB330, 'P'}, {0xB331, 'Q'}, {0xB332, 'R'}, {0xB333, 'S'},
    {0xB334, 'T'}, {0xB335, 'U'}, {0xB336, 'V'}, {0xB337, 'W'},
    {0xB338, 'X'}, {0xB339, 'Y'}, {0xB33A, 'Z'}, {0xB33B, '['},
    {0xB33C, '\\'}, {0xB33D, ']'}, {0xB33E, '^'}, {0xB33F, '_'},
    {0xB340, '`'}, {0xB341, 'a'}, {0xB342, 'b'}, {0xB343, 'c'},
    {0xB344, 'd'}, {0xB345, 'e'}, {0xB346, 'f'}, {0xB347, 'g'},
    {0xB348, 'h'}, {0xB349, 'i'}, {0xB34A, 'j'}, {0xB34B, 'k'},
    {0xB34C, 'l'}, {0xB34D, 'm'}, {0xB34E, 'n'}, {0xB34F, 'o'},
    {0xB350, 'p'}, {0xB351, 'q'}, {0xB352, 'r'}, {0xB353, 's'},
    {0xB354, 't'}, {0xB355, 'u'}, {0xB356, 'v'}, {0xB357, 'w'},
    {0xB358, 'x'}, {0xB359, 'y'}, {0xB35A, 'z'}, {0xB35B, '{'},
    {0xB35C, '|'}, {0xB35D, '}'}, {0xB35E, '~'}
};

const struct lsctf lscfl[] = {
    {KEY_A, 'a', 'A'}, {KEY_B, 'b', 'B'}, {KEY_C, 'c', 'C'}, {KEY_D, 'd', 'D'},
    {KEY_E, 'e', 'E'}, {KEY_F, 'f', 'F'}, {KEY_G, 'g', 'G'}, {KEY_H, 'h', 'H'},
    {KEY_I, 'i', 'I'}, {KEY_J, 'j', 'J'}, {KEY_K, 'k', 'K'}, {KEY_L, 'l', 'L'},
    {KEY_M, 'm', 'M'}, {KEY_N, 'n', 'N'}, {KEY_O, 'o', 'O'}, {KEY_P, 'p', 'P'},
    {KEY_Q, 'q', 'Q'}, {KEY_R, 'r', 'R'}, {KEY_S, 's', 'S'}, {KEY_T, 't', 'T'},
    {KEY_U, 'u', 'U'}, {KEY_V, 'v', 'V'}, {KEY_W, 'w', 'W'}, {KEY_X, 'x', 'X'},
    {KEY_Y, 'y', 'Y'}, {KEY_Z, 'z', 'Z'},
    {KEY_1, '1', '!'}, {KEY_2, '2', '@'}, {KEY_3, '3', '#'}, {KEY_4, '4', '$'},
    {KEY_5, '5', '%'}, {KEY_6, '6', '^'}, {KEY_7, '7', '&'}, {KEY_8, '8', '*'},
    {KEY_9, '9', '('}, {KEY_0, '0', ')'},
    {KEY_SPACE, ' ', ' '},     {KEY_MINUS, '-', '_'}, {KEY_EQUAL, '=', '+'},
    {KEY_DOT, '.', '>'},       {KEY_COMMA, ',', '<'}, {KEY_SLASH, '/', '?'},
    {KEY_SEMICOLON, ';', ':'}, {KEY_APOSTROPHE, '\'', '"'}
};
