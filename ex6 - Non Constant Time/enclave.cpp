

#include <stdint.h>

// this is the secret key stored inside the protected enclave memory
uint64_t key[] = {0,-1llu,-1llu,0,0,-1llu, 2};

extern "C" int callback();

// stringify
#define STR(x) #x

// this is a macro wrapper to gernate code that behaves similar to
// sgx-step zero and single stepping
#define _SIM(_label, _out, _x)     \
"mov " _out ", %%rbx       \n" \
STR(_label)":            \n" \
"mov %%rbx," _out "      \n" \
_x                      "\n" \
"call callback           \n" \
"cmp $0, %%rax           \n" \
"jne " STR(_label)"b     \n" 

// a wrapper for the _SIM macro which automatically creates a unique label per line
#define SIM(_out, _x) _SIM(__LINE__, _out, _x)



uint64_t victim() {
  uint64_t result = 0;
  
  asm volatile(R"(
    lea key(%%rip), %%r12            /* base pointer for the key array */
    mov $0xdeadbeaf, %%r13           /* move in some constant X */
    mov $1, %%r15                    /* init Y */
loop:
  )"
  
  SIM("%%r14", "mov (%%r12), %%r14") /* read the key bit */

  "cmp $0, %%r14 \n"                 /* conditional UNSAFE branch! */
  "je .zero \n"

".one:"
  SIM("%%r15", "imul %%r15, %%r15")  /* square */
  SIM("%%r15", "imul %%r13, %%r15")  /* multiply */

  "jmp .end \n"                      /* end */

".zero:"
  SIM("%%r15", "imul %%r15, %%r15")  /* square */

".end:"

  SIM("%%r12", "add $8, %%r12")      /* increment key index */

  R"(
    cmpq $2, (%%r12)
    jne loop
    mov %%r15, %[result]             /* store result */
  )"
  :[result]"=r"(result)
  :
  : "rbx", "rax", "r12", "r13", "r14", "r15"
  );
  return result;
}