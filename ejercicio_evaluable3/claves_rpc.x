const MAX_STRING = 256;
const MAX_ARRAY  = 32;

struct Coord {
    int x;
    int y;
};

struct KeyArg {
    int key;
};

struct SetArgs {
    int key;
    string value1<MAX_STRING>;
    int N_value2;
    double V_value2<MAX_ARRAY>;
    Coord value3;
};

struct ModifyArgs {
    int key;
    string value1<MAX_STRING>;
    int N_value2;
    double V_value2<MAX_ARRAY>;
    Coord value3;
};

struct GetReply {
    int status;
    string value1<MAX_STRING>;
    int N_value2;
    double V_value2<MAX_ARRAY>;
    Coord value3;
};

program CLAVES_PROG {
    version CLAVES_VERS {
        int DESTROY(void)          = 0;
        int SET_VALUE(SetArgs)     = 1;
        GetReply GET_VALUE(KeyArg) = 2;
        int DELETE_KEY(KeyArg)     = 3;
        int MODIFY_VALUE(ModifyArgs) = 4;
        int EXIST(KeyArg)          = 5;
    } = 1;
} = 0x20000001;