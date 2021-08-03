#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <assert.h>

//#define DEBUG

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wunknown-pragmas"
#pragma ide diagnostic ignored "ConstantFunctionResult"

int input_len = 1728;
char *input = "#etouq,,fi,,!rdc-tes,,esrever,?erudecorp,enifed,,,!rac-tes,,,!tes,,,,adbmal,rddc,lobmys>-gnirts,cossa,fer-tsil,gnirts>-lobmys,,gnirts>-tsil,,tneitouq,qssa,?gnirts,,enilwen,ton,rebmun>-gnirts,lobmys-denretninu>-gnirts,lave,,rdddac,,,*,,?lobmys,,,,,,,,daer,,etirw,,,rahc-keep,,,?llun,htgnel,tsil>-gnirts,,+,,-,,,,,,?lauqe,,rddac,rdac,,<,,rahctup,=,rahc-daer,,?riap,snoc,rac,?qe,,rdc,,,,,,,;9']'9'@Z3@YLZ/YN@TvCvR3y]/94]4Z8^z]88Gi&>GjOai&kkz]O>kw(k!C(_.YCaA_D^~F^{!;(^8;YBlbA`^(`~C_D_~F_|!J8HAaRk_D`0YJdAbEai$D`^~F_|]:'`ko89^~i$'`ko89^~i$'`ko89^~Cw)M^~ClK^~YA^z]$'Z:a_m{!H'a_l'k_k~CjO_{!0'b`o8JEdDbAa_'YHewS#'d~YFbYGi&>GjOOeYCEEfi$i$akYE_oN`~Cx@^0>GgZ-ecGfOdbpNa_~CxP^0Z$dRlbNbOa_~CxJ^8JEdDbAa_'YHewS#'d~YFbYGi&>GjOOeYCEEfi$i$akYE_oN`~Cx@^0>GgZ-ecGfOdbpNa_~CxP^0Z$dRlbNbOa_~i%~CxD^'cNao~CwS$^D_~F_'bRk``n~Z(_|!=3_@J^{]33uy]%3^3^@Z%_~L`kYBWZ*u``vR%Z7u^z!M(i$8MA^@JD^~F^z]#(i$(i$9#A^@YLD^~F^@JvC~F^z!L9%^8MYDZ;^~Z(^3vE@YMYD^@JvE~Z5^3vL@Z#A^@YLD^@JvK~F^8=vLvK~YF^8=vS;vF~Ci%^8=vS-vF~Z2^z])9)8>~Iu^(^~Lk^Hy!>8>@H(^9)~IvR0^~L_vC(^~Lk^YIy!<.S^@H(i&~LvD^.S^@H(i&~i%~IvL^.S^@H(i&~i%~IvK^YIy]&.Z&^YN(i&@H~IvL^Uy!N9>_(^~^Z1^Z9ES^@H.Ei&YNwS$@H~IvJ^(i%@H(i$@H~IvS-^YI@H~IvF^9&@H~IvK^(^~Lk^Uy!I(^(^@YK_jFYO~IjA^KjFy!1(^@YKjAjF8O~IjA^KjFy]F>kkjA]AWmk]+(_9+EaD_A^~F^{]L9+i&^z]=(i$9=Aa_(^~QD__D_~F_{]6(i$96Aa_(^~CD__D_~F_{!E(k8BYEA_l~F^z]<-^9<Wl`A^~L`k{!:(i$(i$(i$(i$8:P`P^~QM`M^~QK`K^~YA_~YA^(i%~C`^{].(^!#>ki#^Z0^9.Ma_(^~Q`M^K_~F_{]>9.i#^z!P(_(i$(i$8PYBWvR%`Z*buA_~LvR/^~L_vR$D^~F^{]18PkYD^z!2,`^{!F,i&^z]I9,`^{]B4^z];6^z]0'n_kz](8?n^z!D4^z]9'mk^z]58?m^z]C6^z]M4^z!G'l`^{]K8?l^z]2,i$^z]-88A^z!887A^z]?*A^z!7-A^z]N9,`^{]G8K`^{!*6^z!-4^z!.'k`^{!/8?k^z!?(i$,`P^~YA^{!3Bv6!OBv5]7Bv4]*Bv3!@Bv2!BBv1!5Bv0!,Bv/]EBv.],Bu!KBt!9Bs!6Br!4Bq!ABp!S#Bo]HBn!)Bm!(Bl!+'lk^zy";


// an unsigned byte value for the REPL's code
typedef unsigned char byte;

// a primitive function
typedef bool (*primitive)();

// a tagged value
typedef unsigned long obj;

// a number
typedef long num;

// a clump obj
typedef struct {
    obj car;
    obj cdr;
    obj tag;
} clump;

#define VM_HALT 6

#define PRIM_C 20
#define UNTAG(x) ((x) >> 1)
#define CLUMP_OF(x) ((clump*)(UNTAG(x)))
#define NUM_OF(x) ((num)(UNTAG((num)(x))))
#define TAG_CLUMP(c_ptr) (((obj)(c_ptr)) << 1)
#define TAG_NUM(num) ((((obj)(num)) << 1) | 1)
#define PRINTLN() do { printf("\n"); } while(0)

#define PRIM1() obj x = pop()
#define PRIM2() obj y = pop(); PRIM1()
#define PRIM3() obj z = pop(); PRIM2()

clump *stack = NULL;
clump *pc = NULL;
size_t pos = 0;

clump *create_clump() {
    return (clump *) malloc(sizeof(clump));
}

clump *new_clump(obj car, obj cdr, obj tag) {
    clump *c = create_clump();
    c->car = car;
    c->cdr = cdr;
    c->tag = tag;
    return c;
}

obj pop() {
    obj x = stack->car;
    stack = CLUMP_OF(stack->cdr);
    return x;
}

bool push(obj x) {
    stack = new_clump(x, TAG_CLUMP(stack), TAG_NUM(0));
    return true;
}

char get_byte() {
    return input[pos++];
}

num get_code() {
    num x = get_byte() - 35;
    return x < 0 ? 57 : x;
}

num get_int(num n) {
    num x = get_code();
    n *= 46;
    return x < 46 ? n + x : get_int(n + x - 46);
}

bool is_clump(obj x) {
    return !(x & 1);
}

bool is_num(obj x) {
    return x & 1;
}

clump *list_tail(clump *lst, num i) {
    return (i == 0) ? lst : list_tail(CLUMP_OF(lst->cdr), i - 1);
}

obj list_ref(clump *lst, num i) {
    return list_tail(lst, i)->car;
}

obj get_opnd(obj o) {
    return (is_num(o) ? list_tail(stack, NUM_OF(o)) : CLUMP_OF(o))->car;
}

clump *get_cont() {
    clump *s = stack;

    while (!NUM_OF(s->tag)) {
        s = CLUMP_OF(s->cdr);
    }

    return s;
}

clump FALSE = {TAG_NUM(0), TAG_NUM(0), TAG_NUM(4)};
clump TRUE = {TAG_NUM(0), TAG_NUM(0), TAG_NUM(5)};
clump NIL = {TAG_NUM(0), TAG_NUM(0), TAG_NUM(6)};

#ifdef DEBUG

void chars2str(obj o) {
    if (o != TAG_CLUMP(&NIL)) {
        printf("%c", (char) (NUM_OF(CLUMP_OF(o)->car) % 256));
        chars2str(CLUMP_OF(o)->cdr);
    }
}

void sym2str(clump *c) {
    chars2str(CLUMP_OF(c->cdr)->car);
}

void show_operand(obj o) {
    if (is_num(o)) {
        printf("int %ld", NUM_OF(o));
    } else {
        printf("sym ");
        sym2str(CLUMP_OF(o));
    }
}

#endif

obj boolean(bool x) {
    return TAG_CLUMP(x ? &TRUE : &FALSE);
}

bool prim_clump() {
    PRIM3();
    push(TAG_CLUMP(new_clump(x, y, z)));
    return true;
}

bool prim_id() {
    PRIM1();
    push(x);
    return true;
}

bool prim_pop() {
    pop();
    return true;
}

bool prim_skip() {
    obj x = pop();
    pop();
    return push(x);
}

bool prim_unk() {
    obj x = CLUMP_OF(stack->car)->car;
    obj y = stack->cdr;
    obj z = TAG_NUM(1);
    stack->car = TAG_CLUMP(new_clump(x, y, z));
    return true;
}

bool prim_is_clump() {
    PRIM1();
    push(boolean(is_clump(x)));
    return true;
}

bool prim_field0() {
    PRIM1();
    push(CLUMP_OF(x)->car);
    return true;
}

bool prim_field1() {
    PRIM1();
    push(CLUMP_OF(x)->cdr);
    return true;
}

bool prim_field2() {
    PRIM1();
    push(CLUMP_OF(x)->tag);
    return true;
}

bool prim_set_field0() {
    PRIM2();
    push(CLUMP_OF(x)->car = y);
    return true;
}

bool prim_set_field1() {
    PRIM2();
    push(CLUMP_OF(x)->cdr = y);
    return true;
}

bool prim_set_field2() {
    PRIM2();
    push(CLUMP_OF(x)->tag = y);
    return true;
}

bool prim_eq() {
    PRIM2();
    push(boolean(x == y));
    return true;
}

bool prim_lt() {
    PRIM2();
    num _x = NUM_OF(x);
    num _y = NUM_OF(y);
    return push(boolean(_x < _y));
}

bool prim_add() {
    PRIM2();
    return push(TAG_NUM((NUM_OF(x) + NUM_OF(y))));
}

bool prim_sub() {
    PRIM2();
    return push(TAG_NUM((NUM_OF(x) - NUM_OF(y))));
}

bool prim_mul() {
    PRIM2();
    return push(TAG_NUM((NUM_OF(x) * NUM_OF(y))));
}

bool prim_div() {
    PRIM2();
    return push(TAG_NUM((NUM_OF(x) / NUM_OF(y))));
}

bool prim_getc() {
    int c = pos < input_len ? get_byte() : getchar();
    return push(TAG_NUM(c));
}

bool prim_putc() {
    PRIM1();
    putchar((int) NUM_OF(x));
    fflush(stdout);
    return push(x);
}

primitive primitives[PRIM_C] = {
        prim_clump,
        prim_id,
        prim_pop,
        prim_skip,
        prim_unk,
        prim_is_clump,
        prim_field0,
        prim_field1,
        prim_field2,
        prim_set_field0,
        prim_set_field1,
        prim_set_field2,
        prim_eq,
        prim_lt,
        prim_add,
        prim_sub,
        prim_mul,
        prim_div,
        prim_getc,
        prim_putc
};


void run() {
#define ADVANCE_PC() do {pc = CLUMP_OF(pc->tag); } while(0)
    while (1) {
        obj o = pc->cdr;
        num instr = NUM_OF(pc->car);

        switch (instr) {
            default:
            case VM_HALT: { // halt
                return;
            }
            case 0: // jump
            case 1: // call
            {
#ifdef DEBUG
                printf(instr ? "--- jump " : "--- call ");
                show_operand(o);
                PRINTLN();
#endif
                o = get_opnd(o);
                obj c = (CLUMP_OF(o))->car;

                if (is_num(c)) {
                    if (!primitives[NUM_OF(c)]()) {
                        return;
                    }

                    if (instr) {
                        // call
                        c = TAG_CLUMP(pc);
                    } else {
                        // jump
                        clump *cont = get_cont();
                        c = TAG_CLUMP(cont);
                        stack->cdr = get_cont()->car;
                    }
                } else {
                    clump *c2 = new_clump(TAG_NUM(0), o, TAG_NUM(0));
                    clump *s2 = c2;
                    num nargs = NUM_OF(CLUMP_OF(c)->car);

                    while (nargs--) {
                        s2 = new_clump(pop(), TAG_CLUMP(s2), TAG_NUM(0));
                    }

                    if (is_num(pc->car) && NUM_OF(pc->car)) {
                        c2->car = TAG_CLUMP(stack);
                        c2->tag = pc->tag;
                    } else {
                        clump *k = get_cont();
                        c2->car = k->car;
                        c2->tag = k->tag;
                    }

                    stack = s2;
                }
                pc = CLUMP_OF(CLUMP_OF(c)->tag);
                break;
            }
            case 2: { // set
#ifdef DEBUG
                printf("--- set ");
                show_operand(o);
                PRINTLN();
#endif
                if (is_num(o)) {
                    list_tail(stack, NUM_OF(o))->car = pop();
                } else {
                    CLUMP_OF(o)->car = pop();
                }
                ADVANCE_PC();
                break;
            }
            case 3: { // get
#ifdef DEBUG
                printf("--- get ");
                show_operand(o);
                PRINTLN();
#endif
                push(get_opnd(o));
                ADVANCE_PC();
                break;
            }
            case 4: { // const
#ifdef DEBUG
                printf("--- const ");
                PRINTLN();
#endif
                push(o);
                ADVANCE_PC();
                break;
            }
            case 5: { // if
#ifdef DEBUG
                printf("--- if");
                PRINTLN();
#endif


                obj p = pop();
                if (p != TAG_CLUMP(&FALSE)) {
                    pc = CLUMP_OF(pc->cdr);
                } else {
                    pc = CLUMP_OF(pc->tag);
                }
                break;
            }
        }
    }
#undef ADVANCE_PC
}


clump *symbol_table = &NIL;

clump *symbol_ref(num n) {
    return CLUMP_OF(list_ref(symbol_table, n));
}

void build_sym_table() {
    num n = get_int(0);

    while (n > 0) {
        n--;

        clump *inner = new_clump(TAG_CLUMP(&NIL), TAG_NUM(0), TAG_NUM(2));
        clump *outer = new_clump(TAG_NUM(0), TAG_CLUMP(inner), TAG_NUM(3));
        clump *root = new_clump(TAG_CLUMP(outer), TAG_CLUMP(symbol_table), TAG_NUM(0));

        symbol_table = root;
    }

    clump *accum = &NIL;

    int iter = 0;
    while (1) {
        iter++;
        byte c = get_byte();

        if (c == 44) {
            clump *inner = new_clump(TAG_CLUMP(accum), TAG_NUM(0), TAG_NUM(2));
            clump *outer = new_clump(TAG_NUM(0), TAG_CLUMP(inner), TAG_NUM(3));
            clump *root = new_clump(TAG_CLUMP(outer), TAG_CLUMP(symbol_table), TAG_NUM(0));

            symbol_table = root;
            accum = &NIL;
            continue;
        }

        if (c == 59) break;

        accum = new_clump(TAG_NUM(c), TAG_CLUMP(accum), TAG_NUM(0));
    }

    clump *inner = new_clump(TAG_CLUMP(accum), TAG_NUM(0), TAG_NUM(2));
    clump *outer = new_clump(TAG_NUM(0), TAG_CLUMP(inner), TAG_NUM(3));
    clump *root = new_clump(TAG_CLUMP(outer), TAG_CLUMP(symbol_table), TAG_NUM(0));

    symbol_table = root;
}

void set_global(clump *c) {
    CLUMP_OF(symbol_table->car)->car = TAG_CLUMP(c);
    symbol_table = CLUMP_OF(symbol_table->cdr);
}


void decode() {
    int weights[6] = {20, 30, 0, 10, 11, 4};

    obj n;
    int d;
    int op;

    int iter = 0;
    while (1) {
        num x = get_code();
        n = x;
        op = -1;

        while (n > 2 + (d = weights[++op])) {
            n -= d + 3;
        }

        if (x > 90) {
            n = pop();
        } else {
            if (!op) {
                stack = new_clump(TAG_NUM(0), TAG_CLUMP(stack), TAG_NUM(0));
            }

            // not very readable, see generic.vm.js

            if (n >= d) {
                n = (n == d) ? TAG_NUM(get_int(0)) : TAG_CLUMP(symbol_ref(get_int(n - d - 1)));
            } else {
                n = op < 3 ? TAG_CLUMP(symbol_ref(n)) : TAG_NUM(n);
            }

            if (op > 4) {
                clump *inner = new_clump(n, TAG_NUM(0), pop());
                n = TAG_CLUMP(new_clump(TAG_CLUMP(inner), TAG_CLUMP(&NIL), TAG_NUM(1)));
                if (stack == NULL) {
                    break;
                }
                op = 4;
            }
        }

        assert(n != 0);
        stack->car = TAG_CLUMP(new_clump(TAG_NUM(op), n, stack->car));
        iter++;
    }

    pc = CLUMP_OF(CLUMP_OF(CLUMP_OF(n)->car)->tag);
}

void setup_stack() {
    stack = new_clump(TAG_NUM(0),
                      TAG_NUM(0),
                      TAG_CLUMP(new_clump(
                              TAG_NUM(VM_HALT),
                              TAG_NUM(0),
                              TAG_NUM(0))));
}

void init() {
    build_sym_table();
    decode();

    set_global(symbol_table);
    set_global(&FALSE);
    set_global(&TRUE);
    set_global(&NIL);
    set_global(new_clump(TAG_NUM(0), TAG_CLUMP(&NIL), TAG_NUM(1))); /* primitive 0 */

    setup_stack();

    run();
}

int main() {
    init();
}

#pragma clang diagnostic pop
