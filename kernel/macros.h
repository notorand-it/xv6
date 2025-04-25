#define STR_(x) #x
#define STR(x)  STR_(x)

#define kstr(s) ((kstr)(((uint64*)(s))[0]))
