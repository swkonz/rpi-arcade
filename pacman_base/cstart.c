#define GPIO_FSEL4 ((unsigned int *)0x20200010)
#define GPIO_SET1 ((unsigned int *)0x20200020)
#define GPIO_CLR1 ((unsigned int *)0x2020002c)

// Green ACT LED is GPIO 47.
#define SUCCESS_OUTPUT (1 << (3 * 7))
#define SUCCESS_BIT (1 << (47 - 32))

extern int __bss_start__;
extern int __bss_end__;
extern int _vectors;
extern int _vectors_end;

extern void main();

#define RPI_VECTOR_START 0x0

void _cstart()
{
    int *bss = &__bss_start__;
    int *bss_end = &__bss_end__;

    while (bss < bss_end)
    {
        *bss++ = 0;
    }

    /*
     * Copy in interrupt vector vector and FIQ handler _vector and _vector_end are
     * symbols defined in the interrupt assembly file, at the beginning and end of
     * the vector and its embedded constants.
     */
    int *vectorsdst = (int *)RPI_VECTOR_START;
    int *vectors = &_vectors;
    int *vectors_end = &_vectors_end;
    while (vectors < vectors_end)
    {
        *vectorsdst++ = *vectors++;
    }

    main();

    // Turn the green ACT LED (GPIO 47).
    *GPIO_FSEL4 = SUCCESS_OUTPUT;
    *GPIO_SET1 = SUCCESS_BIT;
}
