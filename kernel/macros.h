#pragma once

#define STR_(x) #x
#define STR(x)  STR_(x)

#define kstr(s)  (kstr){.u64=((uint64*)(s))[0]}
#define klstr(s) (kstr){.u128=((uint128*)(s))[0]}
